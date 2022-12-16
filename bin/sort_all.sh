#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ $0 != "bin/sort_all.sh" ]
then
   echo "bin/sort_all.sh: must be executed from its parent directory"
   exit 1
fi
list=`git grep -l 'BEGIN_SORT_THIS_LINE_PLUS_' | sed \
   -e '/\/makefile.in$/d' \
   -e '\/^makefile.in$/d' \
   -e '/\/sort_all.sh$/d' `
#
for file in $list
do
   sort.sh $file
done
#
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
