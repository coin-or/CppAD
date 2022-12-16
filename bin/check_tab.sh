#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [  "$0" != 'bin/check_tab.sh' ]
then
   echo "bin/check_tab.sh: must be executed from its parent directory"
   exit 1
fi
echo "-------------------------------------------------------"
ok='yes'
file_list=`git ls-files | sed \
   -e '/makefile[.]am$/d' \
   -e '/makefile[.]in$/d' \
   -e '/^batch_edit.sed$/d' \
   -e '/^coin.png$/d' \
   -e '/^ar-lib$/d' \
   -e '/^configure$/d' \
   -e '/^compile$/d' \
   -e '/^config.guess$/d' \
   -e '/^config.sub$/d' \
   -e '/^depcomp$/d' \
   -e '/^install-sh$/d' \
   -e '/^uw_copy_040507.html/d' \
`
tab=$'\t'
for file in $file_list
do
   if grep $'\t' $file > /dev/null
   then
      echo $file
      ok='no'
   fi
done
echo "-------------------------------------------------------"
if [ "$ok" = 'no' ]
then
   echo 'bin/check_tab.sh: The files above contain tabs'
   exit 1
fi
echo 'bin/check_if.sh: OK'
exit 0
