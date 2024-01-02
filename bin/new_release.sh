#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
stable_version='20240000' # date at which this stable branch started
release='0'               # first release for each stable version is 0
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ $# != 0 ]
then
   echo 'bin/new_release.sh does not expect any arguments'
   exit 1
fi
if [ "$0" != 'bin/new_release.sh' ]
then
   echo 'bin/new_release.sh: must be executed from its parent directory'
   exit 1
fi
if [ ! -e './.git' ]
then
   echo 'bin/new_release.sh: cannot find ./.git'
   exit 1
fi
# -----------------------------------------------------------------------------
# master
# -----------------------------------------------------------------------------
#
# branch
branch=$(git branch --show-current)
if [ "$branch" != 'master' ]
then
   echo 'bin/new_release.sh: must start execution using master branch'
   exit 1
fi
#
# tag
tag=$stable_version.$release
if git tag --list | grep "$tag" > /dev/null
then
   echo "The tag $tag already exists"
   echo 'Use the follow commands to delete it ?'
   echo "   git tag -d $tag"
   echo "   git push -delete origin $tag"
   exit 1
fi
#
# stable_branch
stable_branch=stable/$stable_version
#
# stable_local_hash
pattern=$(echo " *refs/heads/$stable_branch" | sed -e 's|/|[/]|g')
stable_local_hash=$(
   git show-ref $stable_branch | \
      sed -n -e "/$pattern/p" | \
         sed -e "s|$pattern||"
)
#
# stable_remote_hash
pattern=$(echo " *refs/remotes/origin/$stable_branch" | sed -e 's|/|[/]|g')
stable_remote_hash=$(
   git show-ref $stable_branch | \
      sed -n -e "/$pattern/p" | \
         sed -e "s|$pattern||"
)
#
if [ "$stable_local_hash" == '' ] && [ "$stable_remote_hash" == '' ]
then
   empty_hash='yes'
   echo "bin/new_release: local $stable_branch does not exist."
   echo 'Use the following to create it ?'
   echo "   git checkout -b $stable_branch master"
   echo '   git checkout master'
   exit 1
fi
if [ "$stable_local_hash" == '' ] && [ "$stable_remote_hash" != '' ]
then
   empty_hash='yes'
   echo "bin/new_release: local $stable_branch does not exist."
   echo 'Use the following to create it ?'
   echo "   git checkout -b $stable_branch origin/$stable_branch"
   echo '   git checkout master'
   exit 1
fi
#
# git_status
git_status=$(git status --porcelain)
if [ "$git_status" != '' ]
then
   echo 'bin/new_release: git staus --porcelean is not empty for master branch'
   echo 'commit changes to master branch ?'
   exit 1
fi
# ----------------------------------------------------------------------------
# stable branch
# ----------------------------------------------------------------------------
if ! git checkout $stable_branch
then
   echo 'bin/new_release: Program error.'
   echo "branch $stable_branch does not exist"
   exit 1
fi
#
# check version number
ok='yes'
check_one=`version.sh get`
if [ "$check_one" != "$stable_version.$release" ]
then
   ok='no'
fi
if ! version.sh check > /dev/null
then
   ok='no'
fi
if [ "$ok" != 'yes' ]
then
cat << EOF
bin/new_release.sh: version number is not correct in $stable_branch.
Currently in $stable_branch branch, use following to fix it ?
   git fetch
   version.sh set $stable_version.$release
   version.sh copy
   version.sh check
   Then check the chages to the $stable_branch branch and commit
EOF
   exit 1
fi
#
# git_status
git_status=$(git status --porcelain)
if [ "$git_status" != '' ]
then
   echo "bin/new_release: git staus --porcelean not empty for $stable_branch"
   echo "commit changes to $stable_branch branch ?"
   exit 1
fi
#
# stable_remote_hash
if [ "$stable_remote_hash" == '' ]
then
   empty_hash='yes'
   echo "bin/new_release: remote $stable_branch does not exist."
   echo 'Use the following to create it ?'
   echo "   git push origin $stable_branch"
   exit 1
fi
if [ "$stable_local_hash" != "$stable_remote_hash" ]
then
   empty_hash='yes'
   echo "bin/new_release: local and remote $stable_branch differ."
   echo "local  $stable_local_hash"
   echo "remote $stable_remote_hash"
   echo 'try git push ?'
   exit 1
fi
#
# test more or create tag
response=''
while [ "$response" != 'check' ] && [ "$response" != 'release' ]
do
   read -p 'Run check_all or commit release [check/release] ?' response
done
if [ "$response" == 'check' ]
then
   bin/check_all.sh
else
   echo "git tag -a -m 'created by new_release.sh' $tag $stable_remote_hash"
   git tag -a -m 'created by new_release.sh' $tag $stable_remote_hash
   #
   echo "git push origin $tag"
   git push origin $tag
fi
# -----------------------------------------------------------------------------
# master
# -----------------------------------------------------------------------------
git checkout master
echo 'bin/new_release.sh: OK'
exit 0
