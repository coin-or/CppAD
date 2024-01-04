#! /usr/bin/env bash
set -e -u
# ---------------------------------------------------------------------------
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ---------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ $# != 0 ]
then
   echo 'usage: bin/git_commit.sh: does not expect arugments'
   exit 1
fi
if [ "$0" != 'bin/git_commit.sh' ]
then
   echo 'bin/git_commit.sh: must execute this script from its parent directory'
   exit 1
fi
if [ ! -e './.git' ]
then
   echo 'bin/git_commit.sh: cannot find ./.git'
   exit 1
fi
# -----------------------------------------------------------------------------
# EDITOR
set +u
if [ "$EDITOR" == '' ]
then
   echo 'bin/git_commit.sh: EDITOR is not defined.'
   exit 1
fi
set -u
# -----------------------------------------------------------------------------
# new files
list=$(git status --porcelain | sed -n -e '/^?? /p' | sed -e 's|^?? ||')
for file in $list
do
   res=''
   while [ "$res" != 'delete' ] && [ "$res" != 'add' ] && [ "$res" != 'abort' ]
   do
      read -p "$file is uknown to git, [delete/add/abort] ?" res
   done
   if [ "$res" == 'delete' ]
   then
      echo_eval rm $file
   elif [ "$res" == 'abort' ]
   then
      echo 'bin/git_commit.sh: aborting'
      exit 1
   fi
done
# -----------------------------------------------------------------------------
# git_commit.log
branch=$(git branch --show-current)
cat << EOF > git_commit.log
# Please enter the commit message for your changes. Lines starting with '#'
# will be ignored. This commit will abort if the first line does not begin with
# $branch: because $branch is the branch for this commit. 
# Below is a list of the files for this commit:
EOF
git status --porcelain | sed -e 's|^|# |' >> git_commit.log
$EDITOR git_commit.log
sed -i git_commit.log -e '/^#/d'
if ! head -1 git_commit.log | grep "^$branch:" > /dev/null  
then
   echo 'The first line in this commit message is:'
   head -1 git_commit.log
   echo "Aborting because it does not start with '$branch:'"
   exit 1
fi
# -----------------------------------------------------------------------------
# git add
echo_eval git add --all
#
#
# git commit
git commit --file=git_commit.log
#
echo 'bin/git_commit.sh: OK'
exit 0
