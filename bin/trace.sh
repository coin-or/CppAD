#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ $0 != "bin/trace.sh" ]
then
   echo "bin/trace.sh: must be executed from its parent directory"
   exit 1
fi
file="include/cppad/local/sweep/$1"
option="$2"
#
ok='yes'
if [ "$option" != '0' ] && [ "$option" != '1' ]
then
   ok='no'
fi
if [ "$ok" == 'yes' ]
then
   if ! grep '_TRACE [01]' $file > /dev/null
   then
      ok='no'
   fi
fi
if [ "$ok" == 'no' ]
then
   echo 'usage: bin/trace.sh file (0|1)'
   echo 'Sets trace in file to off (0) or on (1) where the file is one of:'
   grep -l '_TRACE [01]' include/cppad/local/sweep/*.hpp | \
      sed -e 's|^include/cppad/local/sweep/||'
   exit 1
fi
old=`grep '_TRACE [01]' $file`
sed -e "s|TRACE [01]|TRACE $option|" -i $file
new=`grep '_TRACE [01]' $file`
#
echo "old: $old"
echo "new: $new"
#
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
