#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-24 Bradley M. Bell
# -----------------------------------------------------------------------------
if [ "$0" != "bin/check_invisible.sh" ]
then
   echo "bin/check_invisible.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$#" == 0 ]
then
   all='false'
elif [ "$#" == 1 ] && [ "$1" == 'all' ]
then
   all='true'
else
   echo 'usage: bin/check_invisible [all]'
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
for name in $no_copyright_list
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
      $sed -e '/^D/d' -e 's|^...||' | $sed -f sed.$$
   )
fi
# ----------------------------------------------------------------------------
#
# sed.$$
cat << EOF > sed.$$
s|[ \\t][ \\t]*\$||
s| *\t|\t|g
1{/^[ \\t]*\$/d}
\${/^[ \\t]*\$/d}
EOF
#
# file
for file in $file_list
do
   if [ -f "$file" ]
   then
      $sed -f sed.$$ $file > copy.$$
      if ! diff $file copy.$$ > diff.$$
      then
         echo "original (<) invisible white space removed (>)"
         cat diff.$$
         res=''
         while [ "$res" != 'yes' ] && [ "$res" != 'no' ]
         do
            read -p "Remove invisible white space in $file [yes/no] ?" res
         done
         if [ "$res" == 'yes' ]
         then
            if [ -x $file ]
            then
               chmod +x copy.$$
            fi
            mv copy.$$ $file
         else
            rm copy.$$
         fi
      else
         rm copy.$$
      fi
      rm diff.$$
   fi
done
rm sed.$$
echo 'check_invisible.sh: OK'
exit 0
