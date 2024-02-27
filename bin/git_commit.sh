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
list=$(
   git status --porcelain | sed -n -e '/^?? /p' |  \
      sed -e 's|^?? ||' -e 's|"||g' -e 's| |@@|g' 
)
for file in $list
do
   file=$(echo $file | sed -e 's|@@| |g')
   res=''
   while [ "$res" != 'delete' ] && [ "$res" != 'add' ] && [ "$res" != 'abort' ]
   do
      read -p "'$file' is uknown to git, [delete/add/abort] ?" res
   done
   if [ "$res" == 'delete' ]
   then
      echo "rm '$file'"
      rm "$file"
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
$branch:
# Enter the commit message for your changes above.
# This commit will abort if the first line does not begin with "$branch:"
# because $branch is the branch for this commit.
# Lines starting with '#' are not included in the message.
# Below is a list of the files for this commit:
EOF
git status --porcelain | sed -e 's|^|# |' >> git_commit.log
$EDITOR git_commit.log
sed -i git_commit.log -e '/^#/d'
if ! head -1 git_commit.log | grep "^$branch:" > /dev/null
then
   echo "Aborting because first line does not begin with $branch:"
   echo 'See ./git_commit.log'
   exit 1
fi
if ! head -1 git_commit.log | grep "^$branch:.*[^ \t]" > /dev/null
then
   echo "Aborting because only white space follow $branch: in first line"
   echo 'See ./git_commit.log'
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
