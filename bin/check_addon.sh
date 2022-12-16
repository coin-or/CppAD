#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [  "$0" != 'bin/check_addon.sh' ]
then
   echo "bin/check_addon.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
addon_list='
   CG
   PY
   TMB
   MIXED
'
grep_cmd=''
for addon in $addon_list
do
   if [ "$grep_cmd" == '' ]
   then
      grep_cmd="CPPAD_${addon}_"
   else
      grep_cmd="$grep_cmd|CPPAD_${addon}_"
   fi
done
#
echo "Checking soruce code for names reserved for addon packages"
echo "-------------------------------------------------------"
ok="yes"
file_list=`git ls-files`
for file in $file_list
do
   if grep -E $grep_cmd $file > /dev/null
   then
      echo "$file containts $grep_cmd"
      ok="no"
   fi
done
echo "-------------------------------------------------------"
if [ "$ok" = "no" ]
then
   echo 'bin/check_if.sh: Error'
   exit 1
else
   echo 'bin/check_if.sh: OK'
   exit 0
fi
