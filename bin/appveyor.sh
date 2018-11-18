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
if [ $0 != "bin/appveyor.sh" ]
then
	echo 'bin/appveyor.sh: must be executed from its parent directory'
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
echo_eval cmake \
	-G '"Unix Makefiles"' \
	-D CMAKE_C_COMPILER=gcc \
	-D CMAKE_CXX_COMPILER=g++ \
	..
# -----------------------------------------------------------------------------
echo 'bin/appveyor.sh: OK'
exit 0
