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
if [ $0 != "bin/github2coin.sh" ]
then
	echo "bin/github2coin.sh: must be executed from its parent directory"
	exit 1
fi
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
echo_eval cd $HOME/cppad/git/repo
echo_eval git push
pause
echo_eval cd $HOME/cppad/git_svn
echo_eval git checkout master
echo_eval git fetch github
pause
echo_eval git merge --squash github/master
pause
echo_eval git commit
pause
echo_eval git svn dcommit
