#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ "$0" != "bin/new_branch.sh" ]
then
	echo 'bin/new_branch.sh: must be executed from its parent directory'
	exit 1
fi
if [ ! -e '.git/svn' ]
then
	echo 'bin/new_branch.sh: must be executed in the git_svn repository'
	exit 1
fi
if [ "$2" == '' ]
then
	echo 'usage:  bin/new_branch.sh branch_path svn_revision'
	exit 1
fi
branch_path="$1"
svn_revision="$2"
repository='https://projects.coin-or.org/svn/CppAD'
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
# make sure repsitories are in sync
echo_eval git svn rebase
echo_eval git fetch github 
# -----------------------------------------------------------------------------
msg="Creating $branch_path from trunk at revision $svn_revision."
cmd="svn copy -r $svn_revision $repository/trunk $repository/$branch_path"
echo "$cmd -m \"$msg\""
$cmd -m "$msg"
# ----------------------------------------------------------------------------
echo 'Use git-svn to fetch the new stable version'
fetch="$branch_path:refs/remotes/svn/$branch_path"
if ! grep "fetch *= *$fetch" .git/config > /dev/null
then
	# backup current version of .git/config
	index='1'
	while [ -e .git/config.$index ]
	do
		index=`expr $index + 1`
	done
	echo_eval cp .git/config .git/config.$index
	#
	sed -e "s|^\turl *= *$repository|&\n\tfetch = $fetch|" -i .git/config
fi
# fetch the branch
echo_eval git svn fetch
# -----------------------------------------------------------------------------
# checkout the new stable version
branch_name=`echo $branch_path | sed -e 's|branches/||'`
echo_eval git checkout -b $branch_name svn/$branch_path
# -----------------------------------------------------------------------------
# push it to github
git push github $branch_name
# -----------------------------------------------------------------------------
