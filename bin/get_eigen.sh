#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# $begin get_eigen.sh$$ $newlinech #$$
# $spell
#	gz
#	Eigen
#	CppAD
# $$
#
# $section Download and Install Eigen in Build Directory$$
#
# $head Syntax$$
# $code bin/get_eigen.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install
# $href%http://eigen.tuxfamily.org%Eigen%$$ in the
# CppAD $code build$$ directory.
#
# $head Distribution Directory$$
# This command must be executed in the
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head External Directory$$
# The Eigen source code is downloaded into the sub-directory
# $code build/external$$ below the distribution directory.
#
# $head Prefix Directory$$
# The Eigen include files are installed in the sub-directory
# $code build/prefix/include/Eigen$$ below the distribution directory.
#
# $head Reuse$$
# The file $codei%build/external/eigen-%version%.tar.gz%$$
# and the directory $codei%build/external/eigen-%version%$$
# will be reused if they exist. Delete this file and directory
# to get a complete rebuild.
#
# $end
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_eigen.sh" ]
then
	echo "bin/get_eigen.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo 'Download eigen to build/external and install it to build/prefix'
version='3.3.3'
web_page='https://bitbucket.org/eigen/eigen/get'
cppad_dir=`pwd`
prefix="$cppad_dir/build/prefix"
installed_flag="build/external/eigen-${version}.installed"
if [ -e "$installed_flag" ]
then
	echo "$installed_flag exists: Skipping get_eigen.sh"
	exit 0
fi
# -----------------------------------------------------------------------------
# determine which version of cmake to use
cmake --version |  sed -n \
		-e 's|[^0-9]*|.|g ' \
		-e 's|\.\([0-9]*\)\.\([0-9]*\).*|\1 * 10 + \2|' \
		-e '1,1p' \
	| bc > get_ipopt.$$
cmake_version=`cat get_ipopt.$$`
rm get_ipopt.$$
echo "cmake_version=$cmake_version"
#
cmake_program=''
if [ "$cmake_version" -ge '28' ]
then
	cmake_program='cmake'
else
	for cmake_version in 28 29
	do
		if which cmake$cmake_version >& /dev/null
		then
			cmake_program="cmake$cmake_version"
		fi
	done
fi
if [ "$cmake_program" == '' ]
then
	echo 'cannot find a verison of cmake that is 2.8 or higher'
	exit 1
fi
# -----------------------------------------------------------------------------
if [ ! -d build/external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "eigen-$version.tar.gz" ]
then
	echo_eval wget --no-check-certificate $web_page/$version.tar.gz
	echo_eval mv $version.tar.gz eigen-$version.tar.gz
fi
# -----------------------------------------------------------------------------
if [ -e eigen-eigen-* ]
then
	echo_eval rm -r eigen-eigen-*
fi
if [ -e "$prefix/include/Eigen" ]
then
	echo_eval rm "$prefix/include/Eigen"
fi
if [ ! -e eigen-$version ]
then
	echo_eval tar -xzf eigen-$version.tar.gz
	git_name=`ls | grep eigen-eigen`
	echo_eval mv $git_name eigen-$version
fi
# -----------------------------------------------------------------------------
echo_eval cd eigen-$version
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
echo_eval $cmake_program .. -DCMAKE_INSTALL_PREFIX=$prefix
echo_eval make install
echo_eval ln -s $prefix/include/eigen3/Eigen $prefix/include/Eigen
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$installed_flag
echo "get_eigen.sh: OK"
