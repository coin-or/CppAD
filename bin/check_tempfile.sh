#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ ! -e "bin/check_tempfile.sh" ]
then
   echo "bin/check_tempfile.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
list=`ls | sed -n \
   -e '/^new.[0-9]*$/d' \
   -e '/^temp.[0-9]*$/d' \
   -e '/\/temp.[0-9]*$/d' \
   -e '/\.[0-9]*$/p'`
if [ "$list" != '' ]
then
   echo 'Use following command to remove temporary files:'
   cmd='rm '
   for file in $list
   do
      cmd="$cmd $file"
   done
   echo "    $cmd"
   echo 'check_tempfile.sh: Error'
   exit 1
fi
echo 'check_tempfile.sh: OK'
exit 0
