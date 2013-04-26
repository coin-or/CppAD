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
trunk_dir=`pwd`
# Running bin/get_fadbad.sh will install include files in
#	trunk_dir/build/prefix/include/FADBAD++
echo_eval bin/get_fadbad.sh
#
# Running bin/get_ipopt.sh will
# build ipopt in: trunk_dir/build/external
# install it in:  trunk_dir/build/prefix
# It will re-use the currently compiled verison if available.
echo_eval bin/get_ipopt.sh
#
#
# Use trunk_dir/../build to build and test CppAD (no reuse)
echo_eval cd ..
echo_eval rm -rf build
echo_eval mkdir build
echo_eval cd build
echo_eval $trunk_dir/configure \
	IPOPT_DIR="$trunk_dir/build/prefix" \
	FADBAD_DIR="$trunk_dir/build/prefix" 
echo_eval make test 
