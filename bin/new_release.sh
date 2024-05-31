#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
stable_version='20240000' # date at which this stable branch started
release='5'               # first release for each stable version is 0
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
   echo "   git push --delete origin $tag"
   exit 1
fi
#
# .coin-or/projDesc.xml
key='stableVersionNumber'
sed -i .coin-or/projDesc.xml \
   -e "s|<$key>[0-9]*</$key>|<$key>$stable_version</$key>|"
#
key='releaseNumber'
sed -i .coin-or/projDesc.xml \
   -e "s|<$key>[0-9.]*</$key>|<$key>$tag</$key>|"
#
# user_guide.xrst
sed -i user_guide.xrst \
   -e "/\/archive\//s|[0-9]\{8\}\.[0-9]*|$tag|g" \
   -e "s|release-[0-9]\{8\}\.[0-9]*|release-$tag|g" \
   -e "s|documentation-[0-9]\{8\}|documentation-$stable_version|g" \
   -e "s|stable-[0-9]\{8\}|stable-$stable_version|g" \
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
   echo "bin/new_release: local $stable_branch does not exist."
   echo 'Use the following to create it ?'
   echo "   git checkout -b $stable_branch master"
   echo '   git checkout master'
   exit 1
fi
if [ "$stable_local_hash" == '' ] && [ "$stable_remote_hash" != '' ]
then
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
# CMakeLists.txt
cat << EOF > temp.sed
s|^SET( *cppad_version *"[0-9.]*")|SET(cppad_version "$tag")|
EOF
sed -i CMakeLists.txt -f temp.sed
#
# check_version.sh
git show master:bin/check_version.sh > bin/check_version.sh
chmod +x bin/check_version.sh
if ! bin/check_version.sh
then
   echo 'bin/check_version.sh changed version numbers'
   echo "commit changes to $stable_branch branch ?"
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
   echo "bin/new_release: remote $stable_branch does not exist."
   echo 'Use the following to create it ?'
   echo "   git push origin $stable_branch"
   exit 1
fi
if [ "$stable_local_hash" != "$stable_remote_hash" ]
then
   echo "bin/new_release: local and remote $stable_branch differ."
   echo "local  $stable_local_hash"
   echo "remote $stable_remote_hash"
   echo 'Use "git push" to fix this ?'
   echo 'If so, check the result for the corresponding actions on'
   echo 'https://github.com/coin-or/CppAD/actions'
   exit 1
fi
#
# master_local_hash
pattern=$(echo " *refs/heads/master" | sed -e 's|/|[/]|g')
master_local_hash=$(
   git show-ref master | \
      sed -n -e "/$pattern/p" | \
         sed -e "s|$pattern||"
)
#
# master_remote_hash
pattern=$(echo " *refs/remotes/origin/master" | sed -e 's|/|[/]|g')
master_remote_hash=$(
   git show-ref master | \
      sed -n -e "/$pattern/p" | \
         sed -e "s|$pattern||"
)
#
if [ "$master_local_hash" != "$master_remote_hash" ]
then
   echo 'bin/new_release.sh: local and remote master differ'
   echo "local  $master_local_hash"
   echo "remote $master_remote_hash"
   echo 'Use "git checkout master; git push" to fix this ?'
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
   bin/check_all.sh mixed
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
