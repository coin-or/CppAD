#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ $# != 0 ]
then
   echo 'bin/check_version.sh: does not expect any arguments'
   exit 1
fi
if [ "$0" != 'bin/check_version.sh' ]
then
   echo 'bin/check_version.sh: must be executed from its parent directory'
   exit 1
fi
if [ ! -e './.git' ]
then
   echo 'bin/check_version.sh: cannot find ./.git'
   exit 1
fi
#
# grep, sed
source bin/grep_and_sed.sh
# -----------------------------------------------------------------------------
#
# check_version("$1", temp.sed, version_ok)
check_version() {
   $sed "$1" -f temp.sed > temp.out
   if ! diff "$1" temp.out > /dev/null
   then
      version_ok='no'
      #
      if [ -x "$1" ]
      then
         mv temp.out "$1"
         chmod +x "$1"
      else
         mv temp.out "$1"
      fi
      echo_eval git diff "$1"
   fi
}
# -----------------------------------------------------------------------------
#
# this_version
this_version=$(
   $sed -n -e '/^SET( *cppad_version *"[0-9.]*")/p' CMakeLists.txt | \
      $sed -e 's|.*"\([^"]*\)".*|\1|'
)
#
# branch
branch=$(git branch | $sed -n -e '/^[*]/p' | $sed -e 's|^[*] *||')
#
# this_version
if [ "$branch" == 'master' ]
then
   this_version=$( date +%Y%m%d )
fi
#
# stable_version, release_version
if echo $branch | $grep '^stable/' > /dev/null
then
   if ! echo $this_version | $grep '^[0-9]\{4\}0000[.]' > /dev/null
   then
      echo 'check_version.sh: Stable version does not begin with yyyy0000.'
      exit 1
   fi
   release_version="$this_version"
   stable_version=$(echo $release_version | $sed -e 's|[.][0-9]*$||')
else
   eval $($grep '^stable_version=' bin/new_release.sh)
   eval $($grep '^release=' bin/new_release.sh)
   release_version="$stable_version.$release"
fi
#
# version_ok
version_ok='yes'
#
# version_files
version_files='
   CMakeLists.txt
   user_guide.xrst
'
#
# temp.sed
cat << EOF > temp.sed
# CMakeLists.txt
s|^SET( *cppad_version *"[0-9.]*")|SET(cppad_version "$this_version")|
#
# user_guide.xrst
s|cppad-[0-9]\\{8\\}[.]*[.0-9]*|cppad-$this_version|
#
s|release-[0-9]\\{8\\}\.[.0-9]*|release-$release_version|
s|archive/[0-9]\\{8\\}\.[0-9]*|archive/$release_version|
#
s|documentation-[0-9]\\{8\\}|documentation-$stable_version|
s|stable-[0-9]\\{8\\}|stable-$stable_version|
EOF
#
for file in $version_files
do
   # check_version(file, temp.sed, version_ok)
   check_version $file
done
# ----------------------------------------------------------------------------
if [ "$version_ok" == 'no' ]
then
   echo 'bin/check_version.sh: version numbers were fixed (see above).'
   echo "Re-execute bin/check_version.sh ?"
   exit 1
fi
echo 'check_version.sh: OK'
exit 0
