#! /usr/bin/env bash
set -e -u
# !! EDITS TO THIS FILE ARE LOST DURING UPDATES BY xrst.git/bin/dev_tools.sh !!
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-25 Bradley M. Bell
# -----------------------------------------------------------------------------
# bin/check_sort.sh
# Checks that for all files, all the sections between
#  BEGIN_SORT_THIS_LINE_PLUS_#
#  END_SORT_THIS_LINE_MINUS_#
# are sorted. If not, it is corrected and an error is returned.
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
   file_list=$(\
      git status --porcelain | \
         $sed -e '/^D/d' -e 's|^...||' -e 's|^.*-> *||' \
   )
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
   if [ -d "$file_name" ]
   then
      check='no'
   else
      if ! $grep BEGIN_SORT_THIS_LINE $file_name > /dev/null
      then
         check='no'
      fi
   fi
   if [ "$check" == 'yes' ]
   then
      if ! bin/sort.sh $file_name >& temp.$$
      then
         cat temp.$$
         echo 'check_sort.sh: Error'
         rm temp.$$
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
if [ -e "temp.$$" ]
then
   rm temp.$$
fi
if [ "$ok" == 'no' ]
then
   echo 'check_sort.sh: Some files have been sorted (run again to get OK).'
   exit 1
fi
echo 'check_sort.sh: OK'
exit 0
