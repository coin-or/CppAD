#! /bin/bash -e
# $Id: check_jenkins.sh 3768 2015-12-28 18:58:35Z bradbell $
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#            GNU General Public License, Version 3.0.
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
echo_eval bin/jenkins.sh
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
