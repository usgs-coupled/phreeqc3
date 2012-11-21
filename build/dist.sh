#!/bin/sh
# $Id: dist.sh 7092 2012-11-21 01:19:33Z charlton $

#
# USAGE: ./dist.sh -v VERSION -r REVISION -d RELEASE_DATE 
#                  [-rs REVISION-SVN] [-pr REPOS-PATH]
#                  [-zip] [-alpha ALPHA_NUM|-beta BETA_NUM|-rc RC_NUM]
#
#   Create a distribution tarball, labelling it with the given VERSION.
#   The REVISION or REVISION-SVN will be used in the version string.
#   The tarball will be constructed from the root located at REPOS-PATH.
#   If REPOS-PATH is not specified then the default is "branches/VERSION".
#   For example, the command line:
#
#      ./dist.sh -v 0.24.2 -r 6284
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
#   To build a Windows zip file package pass -zip.

# echo everything
# set -x

# A quick and dirty usage message
USAGE="USAGE: ./dist.sh -v VERSION -r REVISION -d RELEASE_DATE \
[-rs REVISION-SVN ] [-pr REPOS-PATH] \
[-alpha ALPHA_NUM|-beta BETA_NUM|-rc RC_NUM] \
[-zip]
 EXAMPLES: ./dist.sh -v 0.36.0 -r 8278 -d 2/7/05
           ./dist.sh -v 0.36.0 -r 8278 -d 2/7/05 -pr trunk
           ./dist.sh -v 0.36.0 -r 8282 -d 2/7/05 -rs 8278 -pr tags/0.36.0
           ./dist.sh -v 0.36.0 -r 8282 -d 2/7/05 -rs 8278 -pr tags/0.36.0 -alpha
           ./dist.sh -v 0.36.0 -r 8282 -d 2/7/05 -rs 8278 -pr tags/0.36.0 -beta 1"




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
      -zip)
        ZIP=1
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
  
if [ -n "$ZIP" ] ; then
  EXTRA_EXPORT_OPTIONS="--native-eol CRLF"
fi

if [ -z "$VERSION" ] || [ -z "$REVISION" ] || [ -z "$RDATE" ]; then
  echo " $USAGE"
  exit 1
fi

VER="$VERSION"
REL="$REVISION"
LOWER='abcdefghijklmnopqrstuvwxyz'
UPPER='ABCDEFGHIJKLMNOPQRSTUVWXYZ'
VER_UC=`echo $VER | sed -e "y/$LOWER/$UPPER/"`

RELEASE_DATE="`date -d "$RDATE" "+%B %e, %G"`"
V_FIXDATE="`date -d "$RDATE" "+%d-%b-%G"`"

if [ -z "$REPOS_PATH" ]; then
  REPOS_PATH="branches/$VERSION"
else
  REPOS_PATH="`echo $REPOS_PATH | sed 's/^\/*//'`"
fi

DISTNAME="phreeqc-${VERSION}${VER_NUMTAG}"
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

echo "Exporting revision $REVISION of PHREEQC into sandbox..."
(cd "$DIST_SANDBOX" && \
 	${SVN:-svn} export -q $EXTRA_EXPORT_OPTIONS --ignore-externals -r "$REVISION" \
	     "http://internalbrr/svn_GW/phreeqc3/$REPOS_PATH" \
	     "$DISTNAME")
if [ $? != 0 ] ; then
  echo "svn checkout error"
  exit $?;
fi
	     
ver_major=`echo $VERSION | cut -d '.' -f 1`
ver_minor=`echo $VERSION | cut -d '.' -f 2`
ver_patch=`echo $VERSION | cut -d '.' -f 3`

if [ -z "$ver_patch" ]; then
  ver_patch="0"
fi

SED_FILES="$DISTPATH/build/phreeqc_version.h \
           $DISTPATH/src/main.c \
           $DISTPATH/src/revisions \
           $DISTPATH/win/README.TXT \
           $DISTPATH/doc/README.TXT"

for vsn_file in $SED_FILES
do
  sed \
   -e "/#define *PHREEQC_VER_MAJOR/s/[0-9]\+/$ver_major/" \
   -e "/#define *PHREEQC_VER_MINOR/s/[0-9]\+/$ver_minor/" \
   -e "/#define *PHREEQC_VER_PATCH/s/[0-9]\+/$ver_patch/" \
   -e "/#define *PHREEQC_VER_TAG/s/\".*\"/\" ($VER_TAG)\"/" \
   -e "/#define *PHREEQC_VER_NUMTAG/s/\".*\"/\"$VER_NUMTAG\"/" \
   -e "/#define *PHREEQC_VER_REVISION/s/[0-9]\+/$REVISION_SVN/" \
   -e "s/@RELEASE_DATE@/$RELEASE_DATE/g" \
   -e "s/@VER_DATE@/$RELEASE_DATE/g" \
   -e "s/@VER@/$VER/g" \
   -e "s/@VERSION@/$VER/g" \
   -e "s/@V_FIXDATE@/$V_FIXDATE/g" \
   -e "s/@VER_UC@/$VER_UC/g" \
   -e "s/@REVISION@/$REL/g" \
   -e "s/@REL@/$REL/g" \
   -e "s/@PHREEQC_VER@/$VER/g" \
   -e "s/@PHREEQC_DATE@/$RELEASE_DATE/g" \
    < "$vsn_file" > "$vsn_file.tmp"
  unix2dos "$vsn_file.tmp" 2> /dev/null
  mv -f "$vsn_file.tmp" "$vsn_file"
  cp "$vsn_file" "$vsn_file.dist"
done

cp $DISTPATH/src/revisions $DISTPATH/RELEASE.TXT
cp $DISTPATH/win/README.TXT $DISTPATH/README.TXT
cp $DISTPATH/doc/NOTICE.TXT $DISTPATH/NOTICE.TXT

if [ -z "$ZIP" ]; then
  echo "Rolling $DISTNAME.tar ..."
  (cd "$DIST_SANDBOX" > /dev/null && tar c "$DISTNAME") > \
    "$DISTNAME.tar"
  echo "Compressing to $DISTNAME.tar.gz ..."
  gzip -9f "$DISTNAME.tar"
else
  echo "Rolling $DISTNAME.zip ..."
  (cd "$DIST_SANDBOX" > /dev/null && zip -q -r - "$DISTNAME") > \
    "$DISTNAME.zip"
fi
echo "Removing sandbox..."
rm -rf "$DIST_SANDBOX"

echo ""
echo "Done:"
if [ -z "$ZIP" ]; then
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
else
  ls -l "$DISTNAME.zip"
  echo ""
  echo "md5sum:"
  md5sum "$DISTNAME.zip"
  type sha1sum > /dev/null 2>&1
  if [ $? -eq 0 ]; then
    echo ""
    echo "sha1sum:"
    sha1sum "$DISTNAME.zip"
  fi
fi