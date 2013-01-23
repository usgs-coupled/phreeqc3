#!/bin/sh
# $Id: dist.sh 4710 2010-08-09 22:22:55Z charlton $

#
# USAGE: ./dist.sh -v VERSION -r REVISION -d RELEASE_DATE
#                  [-rs REVISION-SVN] [-pr REPOS-PATH]
#                  [-win] [-alpha ALPHA_NUM|-beta BETA_NUM|-rc RC_NUM]
#
#   Create a distribution tarball, labelling it with the given VERSION.
#   The REVISION or REVISION-SVN will be used in the version string.
#   The tarball will be constructed from the root located at REPOS-PATH.
#   If REPOS-PATH is not specified then the default is "branches/VERSION".
#   For example, the command line:
#
#      ./dist.sh -v 0.24.2 -r 6284 -d 2/7/05
#
#   from the top-level of a branches/0.24.2 working copy will create
#   the 0.24.2 release tarball.
#
#   When building a alpha, beta or rc tarballs pass the apppropriate flag
#   followed by the number for that release.  For example you'd do
#   the following for a Beta 1 release:
#      ./dist.sh -v 1.1.0 -r 10277 -pr branches/1.1.x -beta 1
# 
#   If neither an -alpha, -beta or -rc option with a number is
#   specified, it will build a release tarball.
#  
#   To build a Windows package pass -win.

# echo everything
# set -x

# A quick and dirty usage message
USAGE="USAGE: ./dist.sh -v VERSION -r REVISION -d RELEASE_DATE \
[-rs REVISION-SVN ] [-pr REPOS-PATH] \
[-alpha ALPHA_NUM|-beta BETA_NUM|-rc RC_NUM] \
[-win]
 EXAMPLES: ./dist.sh -v 1.1 -r 150 -d 2/7/05
           ./dist.sh -v 1.1 -r 150 -d 2/7/05 -pr trunk"


# Let's check and set all the arguments
ARG_PREV=""

for ARG in $@
do
  if [ "$ARG_PREV" ]; then

    case $ARG_PREV in
         -v)  VERSION="$ARG" ;;
         -r)  REVISION="$ARG" ;;
        -rs)  REVISION_SVN="$ARG" ;;
        -pr)  REPOS_PATH="$ARG" ;;
        -rc)  RC="$ARG" ;;
      -beta)  BETA="$ARG" ;;
     -alpha)  ALPHA="$ARG" ;;
         -d)  RDATE="$ARG" ;;
          *)  ARG_PREV=$ARG ;;
    esac

    ARG_PREV=""

  else

    case $ARG in
      -v|-r|-rs|-pr|-beta|-rc|-alpha|-d)
        ARG_PREV=$ARG
        ;;
      -win)
        WIN=1
        ARG_PREV=""
	;;
      *)
        echo " $USAGE"
        exit 1
        ;;
    esac
  fi
done

if [ -z "$REVISION_SVN" ]; then
  REVISION_SVN=$REVISION
fi

if [ -n "$ALPHA" ] && [ -n "$BETA" ] ||
   [ -n "$ALPHA" ] && [ -n "$RC" ] ||
   [ -n "$BETA" ] && [ -n "$RC" ] ; then
  echo " $USAGE"
  exit 1
elif [ -n "$ALPHA" ] ; then
  VER_TAG="Alpha $ALPHA"
  VER_NUMTAG="-alpha$ALPHA" 
elif [ -n "$BETA" ] ; then
  VER_TAG="Beta $BETA"
  VER_NUMTAG="-beta$BETA"
elif [ -n "$RC" ] ; then
  VER_TAG="Release Candidate $RC"
  VER_NUMTAG="-rc$RC"
else
  VER_TAG="r$REVISION_SVN"
  VER_NUMTAG="-$REVISION"
fi

case `uname` in
  CYGWIN*)  WIN=1;;
esac

if [ -z "$VERSION" ] || [ -z "$REVISION" ] || [ -z "$RDATE" ]; then
  echo " $USAGE"
  exit 1
fi

LOWER='abcdefghijklmnopqrstuvwxyz'
UPPER='ABCDEFGHIJKLMNOPQRSTUVWXYZ'
VER_UC=`echo $VERSION | sed -e "y/$LOWER/$UPPER/"`

# format date string
RELEASE_DATE="`date -d $RDATE  "+%B %e, %G"`"
GCC_VER_64="`ssh -q lobo2 'gcc -v 2>&1 | egrep ^gcc | sed "s/version //"'`"
GCC_VER="`ssh -q lobo7 'gcc -v 2>&1 | egrep ^gcc | sed "s/version //"'`"
KERNEL_VER_64="`ssh -q lobo2 'uname -r'`"
KERNEL_VER="`ssh -q lobo7 'uname -r'`"
M32="`ssh -q lobo7 'uname -m'`"
M64="`ssh -q lobo2 'uname -m'`"

if [ -z "$REPOS_PATH" ]; then
  REPOS_PATH="branches/$VERSION"
else
  REPOS_PATH="`echo $REPOS_PATH | sed 's/^\/*//'`"
fi

DISTNAME="phreeqc-src-${VERSION}${VER_NUMTAG}"
DIST_SANDBOX=.dist_sandbox
DISTPATH="$DIST_SANDBOX/$DISTNAME"

echo "Distribution will be named: $DISTNAME"
echo " release branch's revision: $REVISION"
echo "     executable's revision: $REVISION_SVN"
echo "     constructed from path: /$REPOS_PATH"
echo "              release date: $RELEASE_DATE"

rm -rf "$DIST_SANDBOX"
mkdir "$DIST_SANDBOX"
echo "Removed and recreated $DIST_SANDBOX"

echo "Exporting revision $REVISION of phreeqc into sandbox..."
(cd "$DIST_SANDBOX" && \
 	${SVN:-svn} export -q $EXTRA_EXPORT_OPTIONS --ignore-externals -r "$REVISION" \
	     "http://internalbrr.cr.usgs.gov/svn_GW/phreeqc3/$REPOS_PATH" \
	     "$DISTNAME")

ver_major=`echo $VERSION | cut -d '.' -f 1`
ver_minor=`echo $VERSION | cut -d '.' -f 2`
ver_patch=`echo $VERSION | cut -d '.' -f 3`

if [ -z "$ver_patch" ]; then
  ver_patch="0"
fi

VERSION_LONG="$ver_major.$ver_minor.$ver_patch.$REVISION_SVN"

SED_FILES="$DISTPATH/configure.ac \
           $DISTPATH/doc/README.Linux.txt \
           $DISTPATH/doc/RELEASE.TXT"

for vsn_file in $SED_FILES
do
  sed \
   -e "s/AC_INIT(.*)/AC_INIT([phreeqc], [$VERSION-$REVISION], [dlpark@usgs.gov])/g" \
   -e "s/@VERSION@/$VERSION/g" \
   -e "s/@PHREEQC_VER@/$VERSION/g" \
   -e "s/@VER_DATE@/$RELEASE_DATE/g" \
   -e "s/@PHREEQC_DATE@/$RELEASE_DATE/g" \
   -e "s/@REVISION@/$REVISION/g" \
   -e "s/@GCC_VER@/$GCC_VER/g" \
   -e "s/@KERNEL_VER@/$KERNEL_VER/g" \
   -e "s/@GCC_VER_64@/$GCC_VER_64/g" \
   -e "s/@KERNEL_VER_64@/$KERNEL_VER_64/g" \
   -e "s/@M32@/$M32/g" \
   -e "s/@M64@/$M64/g" \
    < "$vsn_file" > "$vsn_file.tmp"
  mv -f "$vsn_file.tmp" "$vsn_file"
  if [ -n "$WIN" ]; then
    unix2dos "$vsn_file"
  fi  
  cp "$vsn_file" "$vsn_file.dist"
done

mv $DISTPATH/doc/RELEASE.TXT           $DISTPATH/doc/RELEASE
mv $DISTPATH/doc/README.Linux.txt      $DISTPATH/doc/README

echo "Rolling $DISTNAME.tar ..."
(cd "$DIST_SANDBOX" > /dev/null && tar c "$DISTNAME") > \
"$DISTNAME.tar"

echo "Compressing to $DISTNAME.tar.gz ..."
gzip -9f "$DISTNAME.tar"
echo "Removing sandbox..."
rm -rf "$DIST_SANDBOX"

echo ""
echo "Done:"
ls -l "$DISTNAME.tar.gz"
echo ""
echo "md5sums:"
md5sum "$DISTNAME.tar.gz"
type sha1sum > /dev/null 2>&1
if [ $? -eq 0 ]; then
  echo ""
  echo "sha1sums:"
  sha1sum "$DISTNAME.tar.gz"
fi
