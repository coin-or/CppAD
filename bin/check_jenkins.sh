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
if [ ! -e "bin/check_jenkins.sh" ]
then
	echo "bin/check_jenkins.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
repository="https://projects.coin-or.org/svn/CppAD"
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
# ----------------------------------------------------------------------------
if [ ! -e 'jenkins' ]
then
	echo_eval svn checkout $repository/trunk jenkins
fi
echo_eval cd jenkins
echo_eval svn update
# ----------------------------------------------------------------------------
redo_external='install'
echo_eval bin/jenkins.sh $redo_external
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
