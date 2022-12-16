#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [  "$0" != 'bin/check_if.sh' ]
then
   echo "bin/check_if.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
# CppAD uses preprocessor '# if 0' comment blocks for temporary changes
# that will to be removed before testing for check in.
echo "Checking for '# if 0' and '# if NDEBUG' commands in source code"
echo "-------------------------------------------------------"
ok="yes"
list=`git ls-files | sed -n \
   -e '/\.cpp$/p' \
   -e '/\.hpp$/p' \
   -e '/\.hpp.in$/p'`
for file in $list
do
   if grep '^# *if *0 *$' $file > /dev/null
   then
      # CppAD uses /* comment */ for all its block commnets
      echo "$file has an '# if 0' preprocessor command"
      ok="no"
   fi
   if grep '^# *if *NDEBUG *$' $file > /dev/null
   then
      # This should probably be # ifndef NDEBUG ?
      echo "$file has an '# if NDEBUG' preprocessor command"
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
