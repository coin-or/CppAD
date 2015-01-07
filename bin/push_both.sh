#! /bin/bash -e
# $Id: push_both.sh 3569 2015-01-07 18:50:40Z bradbell $
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
if [ $0 != "bin/push_both.sh" ]
then
	echo "bin/push_both.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == '' ]
then
	echo "usage: bin/push_both.sh branch"
	exit 1
fi
branch="$1"
# -----------------------------------------------------------------------------
#! /bin/bash -e
pause() {
	response=''
	while [ "$response" != 'y' ] && [ "$response" != 'n' ]
	do
		read -p 'Continue [y/n] ' response
	done
	if [ "$response" != 'y' ]
	then
		exit 1
	fi
}
# -----------------------------------------------------------------------------
# git remote add github https://github.com/bradbell/cppad
# 
# check that things are OK to proceed
echo_eval cd $HOME/cppad/git_svn
echo_eval git fetch github
github_hash=`git show-ref --hash github/$branch`
gitsvn_hash=`git show-ref --hash $branch | head -1`
echo "github_hash=$github_hash"
echo "gitsvn_hash=$gitsvn_hash"
if [ "$github_hash" != "$gitsvn_hash" ]
then
	echo "should be equal"
	exit 1
fi
pause
#
# push git/repo to git repository
echo_eval cd $HOME/cppad/git/repo
echo_eval git checkout $branch
echo_eval git push
echo "Should fast forward from $branch to github"
pause
#
# pull from git repository to git_svn
echo_eval cd $HOME/cppad/git_svn
echo_eval git checkout $branch
echo_eval git pull github master
echo "Should fast forward from github to $branch"
pause
#
# push from git_svn repository to svn repositroy
echo_eval git svn dcommit
#
# check that everyting OK at end
echo_eval git svn rebase 
