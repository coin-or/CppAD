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
# $begin get_sacado.sh$$ $newlinech #$$
# $spell
#	Sacado
#	CppAD
# $$
#
# $section Download and Install Sacado in Build Directory$$
# $index sacado, download and install$$
# $index download, install sacado$$
# $index install, sacado$$ 
#
# $head Syntax$$
# $code bin/get_sacado.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install 
# $href%http://trilinos.sandia.gov/packages/sacado%Sacado%$$ in the
# CppAD $code build$$ directory.
#
# $head Distribution Directory$$
# This command must be executed in the 
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head External Directory$$
# The Sacado source code is downloaded into the sub-directory
# $code build/external$$ below the distribution directory.
#
# $head Prefix Directory$$
# The Sacado libraries and include files are installed in the sub-directory
# $code build/prefix$$ below the distribution directory.
#
# $end
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_sacado.sh" ]
then
	echo "bin/get_sacado.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo 'Download sacado to build/external and install it to build/prefix'
version="trilinos-10.8.3-Source"
web_page="http://trilinos.sandia.gov/download/files"
prefix=`pwd`'/build/prefix'
# -----------------------------------------------------------------------------
# determine which version of cmake to use
cmake --version |  sed \
		-e 's|[^0-9.]*||g' \
		-e 's|\([0-9]*\)\.\([0-9]*\)\..*|\1 * 10 + \2|' \
	| bc > get_sacado.$$
cmake_version=`cat get_sacado.$$`
rm get_sacado.$$
echo "cmake_version=$cmake_version"
#
cmake_program=''
if [ "$cmake_version" -ge '27' ]
then
	cmake_program='cmake'
else
	for cmake_version in 27 28 29
	do
		if which cmake$cmake_version >& /dev/null
		then
			cmake_program="cmake$cmake_version"
		fi
	done
fi
if [ "$cmake_program" == '' ]
then
	echo 'cannot find a verison of cmake that is 2.7 or higher'
	exit 1
fi
# -----------------------------------------------------------------------------
if [ -e /usr/lib64 ]
then
	libdir='lib64'
else
	libdir='lib'
fi
# -----------------------------------------------------------------------------
if [ ! -d external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "$version.tar.gz" ]
then
	echo_eval wget $web_page/$version.tar.gz
fi
for package in Sacado Teuchos Trilinois 
do
	echo_eval rm -rf $prefix/include/$package*
done 
if [ ! -e "$version" ]
then
	echo_eval tar -xzf $version.tar.gz
fi
#
echo_eval cd $version
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
if [ -e CMakeCache.txt ]
then
	echo_eval rm CMakeCache.txt
fi
echo_eval $cmake_program \
	-D CMAKE_BUILD_TYPE:STRING=RELEASE \
	-D Trilinos_ENABLE_Sacado:BOOL=ON \
	-D Sacado_ENABLE_TESTS:BOOL=OFF \
	-D CMAKE_INSTALL_PREFIX:PATH=$prefix \
	-D Trilinos_INSTALL_LIB_DIR=$prefix/$libdir \
	../
echo_eval make install
#
echo "get_sacado.sh: OK"
