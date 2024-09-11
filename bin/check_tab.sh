#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-24 Bradley M. Bell
# -----------------------------------------------------------------------------
if [ "$0" != "bin/check_tab.sh" ]
then
   echo "bin/check_tab.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$#" == 0 ]
then
   all='false'
elif [ "$#" == 1 ] && [ "$1" == 'all' ]
then
   all='true'
else
   echo 'usage: bin/check_tab [all]'
   exit 1
fi
#
# sed
source bin/grep_and_sed.sh
#
# invisible_and_tab_ok
source bin/dev_settings.sh
# ----------------------------------------------------------------------------
#
# sed.$$
echo '#' > sed.$$
for name in $invisible_and_tab_ok
do
   if [ -f $name ]
   then
      echo "^$name\$" | $sed -e 's|/|[/]|g' -e 's|.*|/&/d|' >> sed.$$
   elif [ -d $name ]
   then
      echo "^$name/" | $sed -e 's|/|[/]|g' -e 's|.*|/&/d|' >> sed.$$
   else
      echo "$name in no_copyright_list is not a file or directory"
      exit 1
   fi
done
#
# file_list
if [ "$all" == 'true' ]
then
   file_list=$(git ls-files | $sed -f sed.$$)
else
   file_list=$(git status --porcelain | \
      $sed -e '/^D/d' -e 's|^...||' | $sed -f sed.$$)
fi
#
# ok
ok='yes'
for file in $file_list
do
   if $grep -P '\t' $file > /dev/null
   then
      echo "$file has a tab"
      ok='no'
   fi
done
if [ "$ok" != 'yes' ]
then
   echo 'check_tab: Error'
   rm sed.$$
   exit 1
fi
# -----------------------------------------------------------------------------
rm sed.$$
echo 'check_tab.sh: OK'
exit 0
