#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [  "$0" != 'bin/check_automake.sh' ]
then
   echo "bin/check_automake: must be executed from its parent directory"
   exit 1
fi
dir=$(ls /usr/share | grep automake)
dir="/usr/share/$dir"
ok='yes'
for name in $(ls)
do
   if [ -e $dir/$name ]
   then
      if [ -L $name ]
      then
         echo "$name should be a file not a link"
         exit 1
      fi
      if [ $name != 'COPYING' ]
      then
         if ! diff $dir/$name $name > /dev/null
         then
            ok='no'
            echo "cp $dir/$name $name"
         fi
      fi
   fi
done
if [ "$ok" == 'no' ]
then
   echo 'check_automake.sh: commands above will update automake files'
   exit 1
fi
echo 'check_sutomake.sh: OK'
exit 0
