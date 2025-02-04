#! /usr/bin/env bash
set -e -u
# !! EDITS TO THIS FILE ARE LOST DURING UPDATES BY xrst.git/bin/dev_tools.sh !!
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-25 Bradley M. Bell
# -----------------------------------------------------------------------------
# bin/check_verison.sh
# Checks that the version number in the version_file_list are correct;
# see bin/dev_settings.sh for more discussion.
# -----------------------------------------------------------------------------
#
# echo_eval
echo_eval() {
   echo $*
   eval $*
}
#
# sed
source bin/grep_and_sed.sh
#
# package_name, version_file_list
source bin/dev_settings.sh
#
if [ "$0" != "bin/check_version.sh" ]
then
   echo "bin/check_version.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$#" != 0 ]
then
   echo 'check_version does not expect any arguments'
   exit 1
fi
if [ ! -e './.git' ]
then
   echo 'bin/check_version.sh: cannot find ./.git'
   exit 1
fi
# -----------------------------------------------------------------------------
#
# branch
branch=$(git branch --show-current)
#
# first_version_file
first_version_file=$(echo $version_file_list | $sed -e 's|^ *||' -e 's| .*||')
#
# version
cat << EOF > temp.sed
/["'][0-9]{8}["']/b one
/["'][0-9]{8}[.][0-9]{1,2}["']/b one
/["'][0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2}["']/b one
b end
#
: one
s|.*["']([0-9]{8})["'].*|\\1|
s|.*["']([0-9]{8}[.][0-9]{1,2})["'].*|\\1|
s|.*["']([0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2})["'].*|\\1|
p
#
: end
EOF
version=$($sed -n -r -f temp.sed $first_version_file)
#
# version_type
if [[ "$version" =~ ^[0-9]{8}$ ]]
then
   version_type=1
elif [[ "$version" =~ ^[0-9]{8}[.][0-9]{1,2}$ ]]
then
   version_type=2
elif [[ "$version" =~ ^[0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2}$ ]]
then
   version_type=3
else
   echo "check_version.sh: can't find version number in $first_version_file"
   exit 1
fi
if [[ "$branch" =~ ^stable/.* ]]
then
   if [ "$version_type" == 1 ]
   then
      echo "check_version.sh: version in $first_version_file"
      echo "is not for a release but this is the $branch branch"
      exit 1
   elif [ "$version_type" == 3 ]
   then
      if [[ "$version" =~  ^[0-9]{4}[.][^0].*$ ]]
      then
         echo "check_version.sh: version in $first_version_file"
         echo "is not for a release but this is the $branch branch"
         exit 1
      fi
   fi
fi
#
# version
if [ "$branch" == 'master' ] || [ "$branch" == 'main' ]
then
   if [ "$version_type" == 1 ]
   then
      version=$(date +%Y%m%d)
   elif [ "$version_type" == 2 ]
   then
      echo "check_version.sh: version in $first_version_file"
      echo "is for a release but this is the $branch branch"
      exit 1
   else
      if [[ "$version" =~  ^[0-9]{4}[.]0[.][0-9]{1,2}$ ]]
      then
         echo "check_version.sh: version in $first_version_file"
         echo "is for a release but this is the $branch branch"
         exit 1
      fi
      version=$(date +%Y.%-m.%-d)
   fi
fi
#
# temp.sed
cat << EOF > temp.sed
s|(["'])[0-9]{8}(["'])|\\1$version\\2|
s|(["'])[0-9]{8}[.][0-9]{1,2}(["'])|\\1$version\\2|
s|(["'])[0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2}(["'])|\\1$version\\2|
s|$package_name-[0-9]{8}|$package_name-$version|
s|$package_name-[0-9]{8}[.][0-9]{1,2}|$package_name-$version|
s|$package_name-[0-9]{4}[.][0-9]{1,2}[.][0-9]{1,2}|$package_name-$version|
EOF
#
# check_version
check_version() {
   $sed -r "$1" -f temp.sed > temp.out
   if ! diff "$1" temp.out > /dev/null
   then
      version_ok='no'
      echo "check_version.sh: changes to $1"
      set +e
      diff "$1" temp.out
      set -e
      #
      if [ -x "$1" ]
      then
         mv temp.out "$1"
         chmod +x "$1"
      else
         mv temp.out "$1"
      fi
   fi
}
#
# version_ok
version_ok='yes'
#
# check_version
for file in $version_file_list
do
   check_version $file
done
#
# ----------------------------------------------------------------------------
if [ "$version_ok" == 'no' ]
then
   echo 'bin/check_version.sh: version numbers were fixed (see above).'
   echo 'Re-execute bin/check_version.sh ?'
   exit 1
fi
echo 'check_version.sh OK'
exit 0
