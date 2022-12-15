#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ ! -e "bin/check_include_xrst.sh" ]
then
   echo "bin/check_include_xrst.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
count=`git ls-files include/cppad | grep '/xrst/.*\.hpp' | wc -l`
if [ "$count" != '0' ]
then
   git ls-files include/cppad | grep '/xrst/.*\.hpp'
   echo 'Cannot put *.hpp files below xrst in include directory'
   echo 'because install of include directory will exclude them.'
   exit 1
fi
echo 'check_include_xrst: OK'
exit 0
