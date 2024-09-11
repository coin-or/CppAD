#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-24 Bradley M. Bell
# -----------------------------------------------------------------------------
if [ "$0" != "bin/check_sort.sh" ]
then
   echo "bin/check_sort.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$#" == 0 ]
then
   all='false'
elif [ "$#" == 1 ] && [ "$1" == 'all' ]
then
   all='true'
else
   echo 'usage: bin/check_sort [all]'
   echo 'see usage message forbin/sort.sh'
   exit 1
fi
#
# grep, sed
source bin/grep_and_sed.sh
# ----------------------------------------------------------------------------
# file_list
if [ "$all" == 'true' ]
then
   file_list=$(git grep -l 'BEGIN_SORT_THIS_LINE_PLUS_')
else
   file_list=$(git status --porcelain | $sed -e '/^D/d' -e 's|^...||')
fi
#
# ok
ok='yes'
for file_name in $file_list
do
   check='yes'
   if [ "$file_name" == 'bin/sort.sh' ]
   then
      check='no'
   fi
   if [ "$file_name" == 'bin/check_sort.sh' ]
   then
      check='no'
   fi
   if ! $grep BEGIN_SORT_THIS_LINE $file_name > /dev/null
   then
      check='no'
   fi
   if [ "$check" == 'yes' ]
   then
      if ! bin/sort.sh $file_name >& temp.$$
      then
         cat temp.$$
         echo 'check_sort.sh: Error'
         exit 1
      fi
      last_line=$(tail -1 temp.$$)
      if [ "$last_line" == 'sort.sh: Done' ]
      then
         cat temp.$$
         ok='no'
      fi
   fi
done
#
if [ "$ok" == 'no' ]
then
   echo 'check_sort.sh: Some files have been sorted (run again to get OK).'
   exit 1
fi
echo 'check_sort.sh: OK'
exit 0
