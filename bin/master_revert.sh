#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ "$0" != 'bin/master_revert.sh' ]
then
   echo "bin/master_revert.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
branch=$(git branch | sed -n '/^[*]/p' | sed -e 's/[*] *//')
list=$(
   git diff master $branch | \
   sed -n -e '/^diff --git/p' | \
   sed -e 's|^diff --git a/||' -e 's| b/|@|'
)
for pair in $list
do
   master_file=$(echo $pair | sed -e 's|@.*||')
   branch_file=$(echo $pair | sed -e 's|[^@]*@||')
   if [ "$master_file" == "$branch_file" ]
   then
      echo "reverting $master_file"
      git show master:$master_file > $branch_file
   else
      echo 'skipping move of'
      echo "$master_file -> $branch_file"
   fi
done
echo 'master_revert.sh: OK'
exit 0
