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
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------
if [ $0 != "bin/jenkins.sh" ]
then
	echo "bin/jenkins.sh: must be executed from its parent directory"
	exit 1
fi
# distribution direcotry corresponding to this version of CppAD
dist_dir=`pwd`
# Running bin/get_ipopt.sh will
# build ipopt in: dist_dir/trunk/build/external
# install it in:  dist_dir/trunk/build/prefix
# It will re-use the currently compiled verison if available.
echo_eval bin/get_ipopt.sh
echo_eval cd ..
#
# Use dist_dir/../build to build and test CppAD (no reuse)
echo_eval rm -rf build
echo_eval mkdir build
echo_eval cd build
echo_eval ../trunk/configure IPOPT_DIR="$dist_dir/build/prefix"
echo_eval make test | tee test.out
