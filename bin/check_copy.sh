#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2023-24 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ "$0" != "bin/check_copy.sh" ]
then
   echo "bin/check_copy.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$#" != 0 ]
then
   echo 'check_copy does not expect any arguments'
   exit 1
fi
#
# grep, sed
source bin/grep_and_sed.sh
#
# spdx_license_id, no_copyright_list
source bin/dev_settings.sh
# ----------------------------------------------------------------------------
if [ $# != 0 ]
then
   echo 'bin/check_copy.sh does not expect any arguments'
   exit 1
fi
if [ "$0" != 'bin/check_copy.sh' ]
then
   echo 'bin/check_copy.sh: must be executed from its parent directory'
   exit 1
fi
if [ ! -e './.git' ]
then
   echo 'bin/check_copy.sh: cannot find ./.git'
   exit 1
fi
# ---------------------------------------------------------------------------
username='bradbell'
fullname='Bradley M. Bell'
# ---------------------------------------------------------------------------
if [ -e temp.sed ]
then
   rm temp.sed
fi
for name in $no_copyright_list
do
   if [ -f $name ]
   then
      echo "^$name\$" | $sed -e 's|/|[/]|g' -e 's|.*|/&/d|' >> temp.sed
   elif [ -d $name ]
   then
      echo "^$name/" | $sed -e 's|/|[/]|g' -e 's|.*|/&/d|' >> temp.sed
   else
      echo "$name in no_copyright_list is not a file or directory"
      exit 1
   fi
done
missing='no'
changed='no'
for file_name in $(git ls-files | $sed -f temp.sed)
do
   if ! $grep "$spdx_license_id\$" $file_name > /dev/null
   then
      if [ "$missing" == 'no' ]
      then
         echo "Cannot find line that ends with:"
         echo "   $spdx_license_id"
         echo "In the following files:"
      fi
      echo "$file_name"
      missing='yes'
   fi
done
# ---------------------------------------------------------------------------
cat << EOF > temp.sed
s|\\(SPDX-FileContributor: *[0-9]\\{4\\}\\)[-0-9]* $fullname|\\1-24 $fullname|
s|\\(SPDX-FileContributor\\): 2024-24 |\\1: 2024 |
EOF
list=''
if [ "${USER+x}" != '' ]
then
   if [ "$USER" == 'bradbell' ]
   then
      list=$(git status --porcelain | $sed -e 's|^...||' )
   fi
fi
for file_name in $list
do
   if [ -e $file_name ]
   then
      $sed \
      -e 's|\(SPDX-FileContributor\): *\([0-9]\{4\}\)[-0-9]* |\1: \2-24 |' \
      -e 's|\(SPDX-FileContributor\): 2024-24 |\1: 2024 |' \
      $file_name > temp.$$
      if diff $file_name temp.$$ > /dev/null
      then
         rm temp.$$
      else
         if [ "$changed" == 'no' ]
         then
            echo 'The following file contributor dates have been updated'
         fi
         echo $file_name
         if diff $file_name temp.$$
         then
            echo 'check_version.sh: program error'
            exit 1
         fi
         changed='yes'
         if [ -x $file_name ]
         then
            mv temp.$$ $file_name
            chmod +x $file_name
         else
            mv temp.$$ $file_name
         fi
      fi
   fi
done
#
if [ "$missing" = 'yes' ] || [ "$changed" == 'yes' ]
then
   echo 'bin/check_copy.sh: See copyright errors above'
   echo 'Re-execute bin/check_copy.sh ?'
   exit 1
fi
echo 'bin/check_copy.sh: OK'
exit 0
