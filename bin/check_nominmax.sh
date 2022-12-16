#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [  "$0" != 'bin/check_nominmax.sh' ]
then
   echo "bin/check_nominmax.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
list=$(git grep -n '^# *include *<windows.h>'  | sed -e 's|\(:[0-9]*:\).*|\1|')
for match in $list
do
   file=$(echo $match | sed -e 's|\([^:]*\):\([0-9]*\):|\1|')
   line=$(echo $match | sed -e 's|\([^:]*\):\([0-9]*\):|\2|')
   let "line = $line - 2"
   if ! sed -n "${line}p" $file | grep '^# *define *NOMINMAX' > /dev/null
   then
cat << EOF
In $file
   # ifndef NOMINMAX
   # define NOMINMAX
   # endif
Must be the three lines before:
   # include <windows.h>
EOF
   fi
done
#
echo 'check_nominmax.sh: OK'
exit 0
