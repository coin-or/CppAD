#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/new_release.sh" ]
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
svn_repository="https://projects.coin-or.org/svn/CppAD"
stable_version="20150000"
release='4'
release_version="$stable_version.$release"
# -----------------------------------------------------------------------------
# Check release version
if svn list $svn_repository/releases | grep "$release_version" > /dev/null
then
	echo bin/"new_release.sh: Release number $release_version already exists."
	echo "You must first change the assigment"
	echo "	release=$release"
	echo "in file bin/new_release.sh to a higher release number."
	exit 1
fi
# -----------------------------------------------------------------------------
echo_eval git checkout stable/$stable_version
# -----------------------------------------------------------------------------
# check that local branch is up to date
list=`git status -s`
if [ "$list" != '' ]
then
	echo "new_release.sh: 'git status -s' is not empty"
	exit 1
fi
# -----------------------------------------------------------------------------
# check that remote branch agrees with local branch
local_hash=`git show-ref stable/$stable_version | \
	grep "refs/heads/stable/$stable_version" | \
	sed -e "s| *refs/heads/stable/$stable_version||"`
remote_hash=`git show-ref stable/$stable_version | \
	grep "refs/remotes/origin/stable/$stable_version" | \
	sed -e "s| *refs/remotes/origin/stable/$stable_version||"`
if [ "$local_hash" != "$remote_hash" ]
then
	echo_eval git show-ref stable/$stable_version
	echo 'new_release.sh: exiting because local and remote branch differ'
	exit 1
fi
# -----------------------------------------------------------------------------
check_one=`bin/version.sh get`
echo_eval git checkout doc.omh
check_two=`grep "cppad-$stable_version" doc.omh \
	| sed -e 's|cppad-\([0-9.]*\):.*|\1|'`
if [ "$check_one" != "$release_version" ] || [ "$check_one" != "$check_two" ]
then
	echo 'bin/new_release.sh:'
	echo 'Set new version number with following commands:'
	echo "	bin/version.sh set $release_version"
	echo '	bin/version.sh copy'
	echo 'Then test and then commit the changes.'
fi
# -----------------------------------------------------------------------------
svn_hash=`svn log $svn_repository/stable/$stable_version --stop-on-copy | \
	grep 'end *hash *code:' | head -1 | sed -e 's|end *hash *code: *||'`
if [ "$svn_hash" != "$remote_hash" ]
then
	echo "svn hash code = $svn_hash"
	echo "git hash code = $remote_hash"
	echo 'Then execute the following command:'
	echo "	bin/push_git2svn.py stable/$stable_version"
	exit 1
fi
# -----------------------------------------------------------------------------
# tag this version of the repository
if git tag --list | grep "$release_version"
then
	git tag -d $release_version
	git push --delete origin $release_version
fi
#
git tag -a \
	-m "corresponds $svn_repository/releases/$release_version" \
	$release_version
#
git push origin $release_version
# -----------------------------------------------------------------------------
msg="Creating releases/$release_version"
rep_stable="$svn_repository/stable/$stable_version"
rep_release="$svn_repository/releases/$release_version"
echo_eval svn copy $rep_stable $rep_release -m \"$msg\"
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	echo_eval mkdir -p build
fi
echo_eval cd build
echo_eval svn checkout $svn_repository/conf conf
#
echo_eval cd conf
#
msg="Update stable and release numbers in projDesc.xml"
echo 'Settting stable and advance release in build/conf/projDesc.xml.'
sed -i projDesc.xml \
	-e "/^ *<stable/,/^ *<\/stable/s/[0-9]\{8\}/$stable_version/" \
	-e "/^ *<release/,/^ *<\/release/s/[0-9]\{8\}\.[0-9]*/$release_version/"
#
echo "Use the command the following command to finish the process"
echo "	svn commit -m \"$msg\" build/conf/projDesc.xml"
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
