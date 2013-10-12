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
for ext in log err
do
	if [ -e "../jenkins.$ext" ]
	then
		echo_eval rm ../jenkins.$ext
	fi
done
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
# -----------------------------------------------------------------------
# Running bin/get_fadbad.sh will install include files in
#	trunk_dir/build/prefix/include/FADBAD++
log_eval bin/get_fadbad.sh
# -----------------------------------------------------------------------
# Running bin/get_eigen.sh will install include files in
#	trunk_dir/build/prefix/include/Eigen
log_eval bin/get_eigen.sh
# -----------------------------------------------------------------------
# Running bin/get_ipopt.sh will install include files in
#	trunk_dir/build/prefix/include/coin
# and library files in
#	trunk_dir/build/prefix/$libdir
# where $libdir is 'lib64' if /usr/lib64 exists and just 'lib' otherwise.
log_eval bin/get_ipopt.sh
# -----------------------------------------------------------------------
# Running bin/get_sacado.sh will install include files in
#	trunk_dir/build/prefix/include
# and library files in
#	trunk_dir/build/prefix/$libdir
log_eval bin/get_sacado.sh
# -----------------------------------------------------------------------
# Use trunk_dir/../build to build and test CppAD (no reuse)
echo_eval cd ..
echo_eval rm -rf build
echo_eval mkdir build
echo_eval cd build
#
# configure cppad to use all the packages above
if which rmp >& /dev/null 
then
	build_type=`rpm --eval %{_host}`
	build_type="--build=$build_type"
else
	build_type=''
fi
#
cat << EOF
$trunk_dir/configure \\
	$build_type \\
	SACADO_DIR="$trunk_dir/build/prefix" \\
	EIGEN_DIR="$trunk_dir/build/prefix" \\
	IPOPT_DIR="$trunk_dir/build/prefix" \\
	FADBAD_DIR="$trunk_dir/build/prefix"  \\
	OPENMP_FLAGS=-fopenmp
EOF
if ! $trunk_dir/configure $build_type \
	SACADO_DIR="$trunk_dir/build/prefix" \
	EIGEN_DIR="$trunk_dir/build/prefix" \
	IPOPT_DIR="$trunk_dir/build/prefix" \
	FADBAD_DIR="$trunk_dir/build/prefix" \
	OPENMP_FLAGS=-fopenmp
then
	echo "Error during configure command. Here is config.log file:"
	echo "--------------------------------------------------------"
	cat config.log
	exit 1
fi
#
# compile the tests
log_eval make check 
#
# run the tests
log_eval make test
#
# print the test results on the console
echo 'copy make test output to console'
sed -n -e '/^make test$/,$p' ../jenkins.log
#
# make it here without an error exit
echo "jenkins.sh: OK"
