#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/travis.sh" ]
then
	echo "bin/travis.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" != '' ]
then
	echo 'bin/travis.sh does not have arguments'
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
if [ -e 'build' ]
then
	echo_eval rm -r build
fi
echo_eval mkdir build
echo_eval cd build
echo_eval cmake ..
echo_eval make check
# -----------------------------------------------------------------------------
echo 'bin/travis.sh: OK'
exit 0
