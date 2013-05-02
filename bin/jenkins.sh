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
# distribution directory corresponding to this version of CppAD
trunk_dir=`pwd`
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# bash function that echos and executes a command
log_eval() {
	echo "------------------------------------------------" >> ../jenkins.log
	echo "------------------------------------------------" >> ../jenkins.err
	echo $*  >> ../jenkins.log
	echo $*  >> ../jenkins.err
	echo $* "1>> ../jenkins.log 2>> ../jenkins.err"
	if ! eval $*  1>> ../jenkins.log 2>> ../jenkins.err
	then
		echo_eval cat ../jenkins.log
		echo_eval cat ../jenkins.err
		exit 1
	fi
}
for ext in log err
do
	if [ -e "../jenkins.$ext" ]
	then
		echo_eval rm ../jenkins.$ext
	fi
done
# -----------------------------------------------
if [ $0 != "bin/jenkins.sh" ]
then
	echo "bin/jenkins.sh: must be executed from its parent directory"
	exit 1
fi
#
# Running bin/get_fadbad.sh will install include files in
#	trunk_dir/build/prefix/include/FADBAD++
log_eval bin/get_fadbad.sh
#
# Running bin/get_eigen.sh will install include files in
#	trunk_dir/build/prefix/include/Eigen
log_eval bin/get_eigen.sh
#
# Running bin/get_sacado.sh will
# build sacado in: trunk_dir/build/external
# install it in:   trunk_dir/build/prefix
# It will re-use the currently compiled verison if available.
### log_eval bin/get_sacado.sh
#
# Running bin/get_ipopt.sh will
# build ipopt in: trunk_dir/build/external
# install it in:  trunk_dir/build/prefix
# It will re-use the currently compiled verison if available.
log_eval bin/get_ipopt.sh
#
# Use trunk_dir/../build to build and test CppAD (no reuse)
echo_eval cd ..
echo_eval rm -rf build
echo_eval mkdir build
echo_eval cd build
#
# configure cppad to use all the packages above
#### SACADO_DIR="$trunk_dir/build/prefix" \
log_eval $trunk_dir/configure \
	EIGEN_DIR="$trunk_dir/build/prefix" \
	IPOPT_DIR="$trunk_dir/build/prefix" \
	FADBAD_DIR="$trunk_dir/build/prefix" 
#
# compile the tests
log_eval make check 
#
# run the tests
log_eval make test
#
# print the test results on the console
sed -n -e '/^make test$/,$p' ../jenkins.log
#
# print final get_ipopt.sh result on the console 
grep 'get_ipopt.sh:' ../jenkins.log
#
# make it here without an error exit
echo "jenkins.sh: OK"
