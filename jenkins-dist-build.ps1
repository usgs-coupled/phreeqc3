#
# To get the Invoke-WebRequest to work under the 'nt authority\system' account, the DOIRootCA2.cer
# CA needs to be installed by running internet explorer as system using 'psexec -sid cmd' from
# Sysinternals.  The -UseBasicParsing flag may also be required.
#

#
# set DATE
#
if ([string]::IsNullOrEmpty($Env:DATE)) {
  $Env:DATE = date +%x
}
$Env:DATE = date -d $Env:DATE +%x
$Env:RELEASE_DATE = date -d $Env:DATE "+%B %e, %G"

#
# set VER
#
if ([string]::IsNullOrEmpty($Env:VER)) {
  $request = Invoke-WebRequest https://raw.githubusercontent.com/usgs-coupled/phreeqc-version/main/phreeqc-version.txt -UseBasicParsing
  $v = ($request.Content) -split "\."
  if ([string]::IsNullOrEmpty($v[2])) {
    $v[2] = 0
  }
  $v[2] = 1 + $v[2]
  $Env:ver_major = $v[0]
  $Env:ver_minor = $v[1]
  $Env:ver_patch = $v[2]
  $Env:VER = $v -join "."
}
else {
  $v = ($Env:VER) -split "\."  
  $Env:ver_major = $v[0]
  $Env:ver_minor = $v[1]
  $Env:ver_patch = $v[2]
}
if ([string]::IsNullOrEmpty($v[0]) -or [string]::IsNullOrEmpty($v[1]) -or [string]::IsNullOrEmpty($v[2])) {
  throw "Bad VER"
}

$Env:VER_TAG="r$Env:REL"
$Env:VER_NUMTAG="-$Env:REL"
$Env:VERSION_LONG="$Env:ver_major.$Env:ver_minor.$Env:ver_patch.$Env:REL"
$Env:VER_UC="$Env:ver_major.$Env:ver_minor.$Env:ver_patch.$Env:REL"
$Env:MSI_VERSION="$Env:ver_major.$Env:ver_minor.$Env:REL"
$Env:FULLPKG="$Env:NAME-$Env:VER-$Env:REL"

Write-Output "HEAD=$HEAD"
Write-Output "Env:DATE=$Env:DATE"
Write-Output "Env:RELEASE_DATE=$Env:RELEASE_DATE"
Write-Output "Env:ver_major=$Env:ver_major"
Write-Output "Env:ver_major=$Env:ver_minor"
Write-Output "Env:ver_major=$Env:ver_patch"
Write-Output "Env:VER=$Env:VER"
Write-Output "Env:REL=$Env:REL"
Write-Output "Env:VER_TAG=$Env:VER_TAG"
Write-Output "Env:VER_NUMTAG=$Env:VER_NUMTAG"
Write-Output "Env:VERSION_LONG=$Env:VERSION_LONG"
Write-Output "Env:MSI_VERSION=$Env:MSI_VERSION"
Write-Output "Env:FULLPKG=$Env:FULLPKG"
###}}


###$Env:VERSION="$Env:ver_major.$Env:ver_minor.$Env:ver_patch"
###$Env:VER=$Env:VERSION
###{{
$Env:VERSION=$Env:VER
###}}
$Env:VER_UC=$Env:VERSION.ToUpper()
###$Env:REL=$Env:SVN_REVISION
###$Env:REVISION=$Env:SVN_REVISION
###$Env:REVISION_SVN=$Env:SVN_REVISION
###$Env:REL=$Env:SVN_REVISION
$Env:REVISION=$Env:REL
$Env:REVISION_SVN=$Env:REL
###$Env:REVISION_PHAST=$Env:SVN_REVISION
###$Env:VER_TAG="r$Env:SVN_REVISION"
###$Env:VER_NUMTAG="-$Env:SVN_REVISION"

###$Env:REPOS_PATH="trunk"
###{{
$Env:REPOS_PATH="$Env:TAG"
###}}

###$Env:RELEASE_DATE=Get-Date -format "MMMM d, yyyy"

###$Env:DISTNAME="phast4windows-$Env:VERSION$Env:VER_NUMTAG"
###{{
$Env:DISTNAME="$Env:NAME-$Env:VERSION$Env:VER_NUMTAG"
###}}

Write-Output "Distribution will be named: $Env:DISTNAME"
Write-Output " release branch's revision: $Env:REVISION"
Write-Output "     executable's revision: $Env:REVISION_SVN"
###Write-Output "          phast's revision: $Env:REVISION_PHAST"
Write-Output "     constructed from path: /$Env:REPOS_PATH"
Write-Output "              release date: $Env:RELEASE_DATE"

#
$sed_files=@('phreeqc_version.h', `
             'src\class_main.cpp', `
             'doc\RELEASE.TXT', `
             'doc\README.Phreeqc.TXT')
foreach ($file in $sed_files) {
  (Get-Content $file) | Foreach-Object {
    $_ -replace "(#define PHREEQC_VER_MAJOR\s*)[0-9]*",    "`${1}$Env:ver_major" `
       -replace "(#define PHREEQC_VER_MINOR\s*)[0-9]*",    "`${1}$Env:ver_minor" `
       -replace "(#define PHREEQC_VER_PATCH\s*)[0-9]*",    "`${1}$Env:ver_patch" `
       -replace "(#define PHREEQC_VER_TAG\s*)[0-9]*",      "`${1} ($Env:VER_TAG)" `
       -replace "(#define PHREEQC_VER_NUMTAG\s*)[0-9]*",   "`${1}$Env:VER_NUMTAG" `
       -replace "(#define PHREEQC_VER_REVISION\s*)[0-9]*", "`${1}$Env:REVISION" `
       -replace "(define *Major=\s*)[0-9]+",               "`${1}$Env:ver_major" `
       -replace "(define *Minor=\s*)[0-9]+",               "`${1}$Env:ver_minor" `
       -replace "(define *Build=\s*)[0-9]+",               "`${1}$Env:REVISION" `
       -replace "@RELEASE_DATE@",                          "$Env:RELEASE_DATE" `
       -replace "@VER_DATE@",                              "$Env:RELEASE_DATE" `
       -replace "@VER@",                                   "$Env:VER" `
       -replace "@VERSION@",                               "$Env:VER" `
       -replace "@V_FIXDATE@",                             "$Env:V_FIXDATE" `
       -replace "@VER_UC@",                                "$Env:VER_UC" `
       -replace "@REVISION@",                              "$Env:REL" `
       -replace "@MAJOR@",                                 "$Env:ver_major" `
       -replace "@MINOR@",                                 "$Env:ver_minor" `
       -replace "@REL@",                                   "$Env:REL" `
       -replace "@VERSION_LONG@",                          "$Env:VERSION_LONG" `
       -replace "@PHREEQC_VER@",                           "$Env:VER" `
       -replace "@PHREEQC_DATE@",                          "$Env:RELEASE_DATE"
  } | Set-Content $file
}
#
Get-ChildItem Env:

[string]${MsBuild}="c:\WINDOWS\Microsoft.NET\Framework\v2.0.50727\MsBuild.exe"

# build exe
[string]$opts="phreeqcpp.2005.sln /p:Configuration=Release /p:Platform=x64"
Invoke-Expression "$MsBuild $opts"

# build CLR exe
[string]$opts_clr="phreeqcpp.2005.sln /p:Configuration=ClrRelease /p:Platform=x64"
Invoke-Expression "$MsBuild $opts_clr"

# build msi
[string]$opts_msi="msi\msi.sln /p:Configuration=Release /p:Platform=x64 /p:TargetName=$Env:FULLPKG-x64 /p:Major=$Env:ver_major /p:Minor=$Env:ver_minor /p:Patch=$Env:ver_patch /p:Build=$Env:REL"
Invoke-Expression "$MsBuild $opts_msi"

# copy docs
New-Item ".\dist" -ItemType directory
Copy-Item doc\README.Phreeqc.TXT dist\README.Phreeqc.${Env:REL}.TXT
Copy-Item doc\RELEASE.TXT dist\RELEASE.${Env:REL}.TXT