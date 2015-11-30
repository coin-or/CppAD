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
if [ $0 != "bin/jenkins.sh" ]
then
	echo "bin/jenkins.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" != '' ]
then
	echo 'bin/junk.sh no longer has any arguments'
	exit 1
fi
# -----------------------------------------------------------------------------
# distribution directory corresponding to this version of CppAD
trunk_dir=`pwd`
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# bash function that logs stdout, stderr, and executes a command
log_eval() {
	echo "------------------------------------------------" >> ../jenkins.log
	echo "------------------------------------------------" >> ../jenkins.err
	echo $*  >> $trunk_dir/jenkins.log
	echo $*  >> $trunk_dir/jenkins.err
	echo $* "1>> $trunk_dir/jenkins.log 2>> $trunk_dir/jenkins.err"
	if ! eval $*  1>> $trunk_dir/jenkins.log 2>> $trunk_dir/jenkins.err
	then
		echo_eval cat $trunk_dir/jenkins.log
		echo_eval cat $trunk_dir/jenkins.err
		exit 1
	fi
}
for ext in log err
do
	if [ -e "$trunk_dir/jenkins.$ext" ]
	then
		echo_eval rm $trunk_dir/jenkins.$ext
	fi
done
# --------------------------------------------------------------------------
if [ -e /usr/lib64 ]
then
	libdir='lib64'
else
	libdir='lib'
fi
# -----------------------------------------------------------------------
g++ --version
# -----------------------------------------------------------------------
# Running bin/get_fadbad.sh ensures its include files are in
#	$trunk_dir/build/prefix/include/FADBAD++
log_eval bin/get_fadbad.sh
# -------------------------------------------------------------------
# Running bin/get_eigen.sh ensures its include files are in
#	$trunk_dir/build/prefix/include/Eigen
log_eval bin/get_eigen.sh
# -------------------------------------------------------------------
# Running bin/get_ipopt.sh ensures its include files are in
#	$trunk_dir/build/prefix/include/coin
# and library files in
#	$trunk_dir/build/prefix/$libdir
# where $libdir is 'lib64' if /usr/lib64 exists and 'lib' otherwise.
log_eval bin/get_ipopt.sh
# -------------------------------------------------------------------
# Running bin/get_sacado.sh ensures its include files are in
#	$trunk_dir/build/prefix/include
# and library files in
#	$trunk_dir/build/prefix/$libdir
log_eval bin/get_sacado.sh
# -------------------------------------------------------------------
# Running bin/get_colpack.sh ensures its library files are in
#	$trunk_dir/build/prefix/$libdir
log_eval bin/get_colpack.sh
# -------------------------------------------------------------------
# Running bin/get_acolc.sh ensures its include files are in
#	$trunk_dir/build/prefix/include/adolc
# and library files in
#	$trunk_dir/build/prefix/$libdir
log_eval bin/get_adolc.sh
# -------------------------------------------------------------------
system_name=`uname | sed -e 's|\(......\).*|\1|'`
if [ "$system_name" == 'CYGWIN' ]
then
	export PATH="$trunk_dir/build/prefix/bin:$PATH"
else
	export LD_LIBRARY_PATH="$trunk_dir/build/prefix/$libdir"
fi
# -----------------------------------------------------------------------
# Use trunk_dir/build/auto_tools to build and test CppAD (no reuse)
echo_eval cd build
echo_eval rm -rf auto_tools
echo_eval mkdir auto_tools
echo_eval cd auto_tools
#
# configure cppad to use all the packages above
if which rpm >& /dev/null
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
	--disable-silent-rules \\
	--with-implicit_ctor \\
	ADOLC_DIR="$trunk_dir/build/prefix" \\
	SACADO_DIR="$trunk_dir/build/prefix" \\
	EIGEN_DIR="$trunk_dir/build/prefix" \\
	IPOPT_DIR="$trunk_dir/build/prefix" \\
	FADBAD_DIR="$trunk_dir/build/prefix"  \\
	OPENMP_FLAGS=-fopenmp
EOF
if ! $trunk_dir/configure $build_type \
	--disable-silent-rules \
	--with-implicit_ctor \
	ADOLC_DIR="$trunk_dir/build/prefix" \
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
sed -n -e '/^make test$/,$p' $trunk_dir/jenkins.log
#
# make it here without an error exit
echo "jenkins.sh: OK"
