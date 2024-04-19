#! /usr/bin/env bash
set -e -u
# ---------------------------------------------------------------------------
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
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
# convert spaces in file names to @@
list=$(
   git status --porcelain | sed -n -e '/^?? /p' |  \
      sed -e 's|^?? ||' -e 's|"||g' -e 's| |@@|g'
)
for file in $list
do
   # convert @@ in file names back to spaces
   file=$(echo $file | sed -e 's|@@| |g')
   res=''
   while [ "$res" != 'delete' ] && [ "$res" != 'add' ] && [ "$res" != 'abort' ]
   do
      read -p "'$file' is unknown to git, [delete/add/abort] ?" res
   done
   if [ "$res" == 'delete' ]
   then
      # may be spaces in file name so do not use echo_eval
      echo "rm '$file'"
      rm "$file"
   elif [ "$res" == 'abort' ]
   then
      echo 'bin/git_commit.sh: aborting'
      exit 1
   else
      git add "$file"
   fi
done
# -----------------------------------------------------------------------------
# temp.log
branch=$(git branch --show-current)
cat << EOF > temp.log
$branch:
# 1. Enter message for this commit above this line.
# 2. The message for the previous commit is in git_commit.log (if it exists).
# 3. This commit will abort if the first line does not begin with "$branch:"
#    because $branch is the branch for this commit.
# 4. Lines starting with '#' are not included in the message.
# 5. Below is a list of the files for this commit:
EOF
git status --porcelain | sed -e 's|^|# |' >> temp.log
$EDITOR temp.log
sed -i temp.log -e '/^#/d'
if ! head -1 temp.log | grep "^$branch:" > /dev/null
then
   echo "Aborting because first line does not begin with $branch:"
   echo 'See ./temp.log'
   exit 1
fi
if ! head -1 temp.log | grep "^$branch:.*[^ \t]" > /dev/null
then
   echo "Aborting because only white space follow $branch: in first line"
   echo 'See ./temp.log'
   exit 1
fi
#
# git_commit.log
mv temp.log git_commit.log
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
