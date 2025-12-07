#! /usr/bin/env bash
set -e -u
# !! EDITS TO THIS FILE ARE LOST DURING UPDATES BY xrst.git/bin/dev_tools.sh !!
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2023-25 Bradley M. Bell
# ----------------------------------------------------------------------------
# bin/check_copy.sh
# Checks that the copyright message, in all the source files,
# is correct and up to date. If there were any errors, a message is printed,
# it is automatically corrected, and this script exits with an error.
# Files that are not checked can be specified in bin/dev_setting.sh
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
#
# yy
yy=$(date +%y)
#
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
# fullname
fullname=''
if [ "${USER+x}" != '' ]
then
   for contributor in $contributor_list
   do
      if [[ $contributor == ${USER}* ]]
      then
         fullname=$(echo $contributor | sed -e 's|^.*:||' -e 's|_| |g')
      fi
   done
   if [ "$fullname" == '' ] && [ "${USER+x}" != '' ]
   then
      echo "Cannot user name = $USER in bin/dev_settings.sh contributor_list"
      exit 1
   fi
fi
# ---------------------------------------------------------------------------
# copyright_all, copyright_changed
echo "#" > temp.sed
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
copyright_all=$(git ls-files | $sed -f temp.sed)
copyright_changed=$(
   git status --porcelain | $sed -e 's|^...||' | $sed -f temp.sed
)
# ---------------------------------------------------------------------------
missing='no'
changed='no'
for file_name in $copyright_all
do
   # if file has not been deleted
   if [ -e $file_name ]
   then
      # if file does not have expected license identifier
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
   fi
done
# ---------------------------------------------------------------------------
cat << EOF > temp.sed
/SPDX-FileContributor:[ 0-9.-]*$fullname/! b end
s|\\([0-9]\\{4\\}\\)[-0-9]* |\\1-$yy |
s|20$yy-$yy |20$yy |
#
: end
EOF
list=''
if [ "$fullname" != '' ]
then
   list="$copyright_changed"
fi
for file_name in $list
do
   if [ -e $file_name ] && [ -f $file_name ]
   then
      if ! $grep "SPDX-FileContributor:[ 0-9.-]*$fullname" $file_name \
         > /dev/null
      then
         echo "username = $USER, fullname = $fullname"
         echo "The following pattern does not appear in $file_name"
         echo 'SPDX-FileContributor:[ 0-9.-]*'$fullname:
         exit 1
      fi
      $sed -f temp.sed $file_name > temp.$$
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
   echo 'check_copy.sh: The copyright messages above were updated.'
   echo 'Re-execute bin/check_copy.sh ?'
   exit 1
fi
echo 'bin/check_copy.sh: OK'
exit 0
