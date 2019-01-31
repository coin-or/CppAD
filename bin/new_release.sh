#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
stable_version="20190200" # start each stable_version at yyyymm00
release='0'               # first release for each stable version is 0
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/new_release.sh' ]
then
    echo "bin/new_release.sh: must be executed from its parent directory"
    exit 1
fi
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
# master
# -----------------------------------------------------------------------------
#
branch=`git branch | grep '^\*'`
if [ "$branch" != '* master' ]
then
    echo 'new_release.sh: cannot checkout master branch'
    exit 1
fi
# Make sure version is same in source code
version.sh date > /dev/null
version.sh copy > /dev/null
#
# Make sure no uncommitted changes
list=`git status -s`
if [ "$list" != '' ]
then
    echo "new_release.sh: 'git status -s' is not empty (for master branch)"
    echo 'You must commit or abort changes before proceeding.'
    exit 1
fi
# local hash code for master
local_hash=`git show-ref master | \
    grep "refs/heads/$stable_branch" | \
    sed -e "s| *refs/heads/$stable_branch||"`
#
# remote hash code
remote_hash=`git show-ref master | \
    grep "refs/remotes/origin/$stable_branch" | \
    sed -e "s| *refs/remotes/origin/$stable_branch||"`
#
if [ "$local_hash" != "$remote_hash" ]
then
    echo 'new_release.sh: local and remote for master differ'
    echo "local  $stable_branch: $local_hash"
    echo "remote $stable_branch: $remote_hash"
    echo 'try:   git push'
    exit 1
fi
# =============================================================================
# stable branch
# =============================================================================
# Make sure local, remote and svn hash codes agree for this stable branch
stable_branch=stable/$stable_version
#
# local hash code
local_hash=`git show-ref $stable_branch | \
    grep "refs/heads/$stable_branch" | \
    sed -e "s| *refs/heads/$stable_branch||"`
#
# remote hash code
remote_hash=`git show-ref $stable_branch | \
    grep "refs/remotes/origin/$stable_branch" | \
    sed -e "s| *refs/remotes/origin/$stable_branch||"`
#
if [ "$local_hash" == '' ] && [ "$remote_hash" == '' ]
then
    echo "new_release.sh: $stable_branch does not exist"
    echo "Use following command to create it ?"
    echo "    git checkout -b $stable_branch master"
	echo "    version.sh set $stable_version"
	echo 'Then run tests. Then commit changes.'
    exit 1
fi
if [ "$local_hash" == '' ] && [ "$remote_hash" != '' ]
then
    echo "new_release.sh: local $stable_branch does not exist."
    echo "    git checkout -b $stable_branch origin/$stable_branch"
    exit 1
fi
#
if [ "$remote_hash" == '' ]
then
    echo "new_release.sh: remote $stable_branch does not exist ?"
    echo "    git push origin $stable_branch"
    exit 1
fi
#
# check local == remote
if [ "$local_hash" != "$remote_hash" ]
then
    echo 'new_release.sh: local and remote differ for this branch'
    echo "local  $stable_branch: $local_hash"
    echo "remote $stable_branch: $remote_hash"
    echo "try: git checkout $stable_branch"
    echo '     git push'
    exit 1
fi
# -----------------------------------------------------------------------------
# Check that release version does not already exist
#
if git tag --list | grep "$stable_version.$release"
then
    echo 'This git reference tag already exists. Delete old version ?'
    echo "    git tag -d $stable_version.$release"
    echo "    git push --delete origin $stable_version.$release"
    exit 1
fi
# =============================================================================
# stable branch
# =============================================================================
# checkout the stable branch
branch=`git branch | grep '^\*' | sed -e 's|^\* *||'`
if [ "$branch" != "$stable_branch" ]
then
    echo_eval git checkout $stable_branch
fi
# -----------------------------------------------------------------------------
check_one=`version.sh get`
check_two=`grep "cppad-$stable_version\.$release" doc.omh \
    | sed -e 's|cppad-\([0-9.]*\):.*|\1|'`
if [ "$check_one" != "$check_two" ]
then
    echo 'bin/new_release.sh: version number is not correct ?'
    echo "    version.sh set $stable_version.$release"
    echo '    version.sh copy'
    echo '    version.sh check'
    echo 'Then commit the changes.'
    exit 1
fi
# -----------------------------------------------------------------------------
# Make sure that autotools version of makfiles is up to current version.
bin/autotools.sh automake
list=`git status -s`
if [ "$list" != '' ]
then
    echo "new_release.sh: 'git status -s' is not empty"
    echo 'stable branch autotools install not up to current version'
    echo 'commit the local changes.'
    exit 1
fi
# -----------------------------------------------------------------------------
read -p 'All checks have passed. More testing or commit release [t/c] ?' \
    response
if [ "$response" != 'c' ]
then
    echo 'Exiting for more testing of stable branch'
    exit 1
fi
# -----------------------------------------------------------------------------
# tag this release
#
echo_eval git tag -a -m "created by bin/new_release.sh" \
    $stable_version.$release
#
echo_eval git push origin $stable_version.$release
# =============================================================================
# master branch
# =============================================================================
git checkout master
echo "$0: OK"
exit 0
