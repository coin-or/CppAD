#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2024 Bradley M. Bell
set -e -u
# ----------------------------------------------------------------------------
if [ $0 != "bin/check_trace.sh" ]
then
   echo "bin/check_trace.sh: must be executed from its parent directory"
   exit 1
fi
change='no'
file_list=$(git ls-files include/cppad/local/sweep/*.hpp)
for file in $file_list
do
   sed -e 's|\(# *define *[A-Z0-9_]*\)_TRACE 1|\1_TRACE 0|' $file \
      > check_trace.$$
   if diff $file check_trace.$$ > /dev/null
   then
      rm check_trace.$$
   else
      change='yes'
      echo "$file changed as follows:"
      set +e
      diff $file check_trace.$$
      set -e
      mv check_trace.$$ $file
   fi
done
if [ "$change" == 'yes' ]
then
   echo "$0: Error"
   exit 1
fi
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
