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
# $begin get_sacado.sh$$ $newlinech #$$
# $spell
#	trilinos
#	gz
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
# $head Reuse$$
# The file $codei%build/external/trilinos-%version%-Source.tar.gz%$$
# and the directory $codei%build/external/trilinos-%version%-Source%$$
# will be reused if they exist. Delete this file and directory
# to get a complete rebuild.
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
version="11.12.1" # using old version because new version requires c++11
trilinos_dir="trilinos-$version-Source"
web_page="http://trilinos.org/oldsite/download/files"
cppad_dir=`pwd`
prefix="$cppad_dir/build/prefix"
installed_flag="build/external/trilinos-${version}.installed"
if [ -e "$installed_flag" ]
then
	echo "$installed_flag exists: Skipping get_sacado.sh"
	exit 0
fi
# -----------------------------------------------------------------------------
# determine which version of cmake to use
cmake --version |  sed -n \
		-e 's|[^0-9]*|.|g ' \
		-e 's|\.\([0-9]*\)\.\([0-9]*\).*|\1 * 10 + \2|' \
		-e '1,1p' \
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
# libdir
if [ -e /usr/lib64 ]
then
	libdir='lib64'
else
	libdir='lib'
fi
# -----------------------------------------------------------------------------
# check if we have a local copy of coin lapack and blas libraries installed
coin_lapack_blas='no'
if [ -e build/prefix/$libdir/libcoinlapack.a ]
then
	if [ -e build/prefix/$libdir/libcoinblas.a ]
	then
		coin_lapack_blas='yes'
	fi
fi
echo "coin_lapack_blas=$coin_lapack_blas"
# -----------------------------------------------------------------------------
# change into build/external directory
if [ ! -d build/external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
# create the trilions source directory and change into it
if [ ! -e "$trilinos_dir.tar.gz" ]
then
	echo_eval wget --no-check-certificate $web_page/$trilinos_dir.tar.gz
fi
for package in Sacado Teuchos Trilinois
do
	echo_eval rm -rf $prefix/include/$package*
done
if [ ! -e "$trilinos_dir" ]
then
	echo_eval tar -xzf $trilinos_dir.tar.gz
	# ------------------------------------------------------------------------
	# patch the cmake/tribits/modules/FindPythonInterp.cmake file
	file="$trilinos_dir/cmake/tribits/modules/FindPythonInterp.cmake"
	line='[HKEY_LOCAL_MACHINE\\\\SOFTWARE\\\\Python\\\\PythonCore\\\\2.8\\\\'
	line="${line}InstallPath]"
	if [ -e "$file" ]
	then
		echo "patch $file"
		sed \
			-e 's|NAMES \(python2.7 python2.6\)|NAMES python2.8 \1|' \
			-e "s|^\( *\)\[HKEY_LOCAL_MACHINE.*2\.7.*|\1$line\n&|" \
			-i $file
	fi
	# ------------------------------------------------------------------------
fi
echo_eval cd $trilinos_dir
# -----------------------------------------------------------------------------
# change into build sub-directory
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
# -----------------------------------------------------------------------------
# cmake command and install
if [ "$coin_lapack_blas" == 'yes' ]
then
	echo_eval $cmake_program \
		-D CMAKE_BUILD_TYPE:STRING=RELEASE \
		-D Trilinos_ENABLE_Sacado:BOOL=ON \
		-D Sacado_ENABLE_TESTS:BOOL=OFF \
		-D CMAKE_INSTALL_PREFIX:PATH=$prefix \
		-D Trilinos_INSTALL_LIB_DIR=$prefix/$libdir \
		-D LAPACK_LIBRARY_NAMES:STRING=coinlapack \
		-D BLAS_LIBRARY_NAMES:STRING=coinblas \
		-D LAPACK_LIBRARY_DIRS:STRING=../prefix/$libdir \
		-D BLAS_LIBRARY_DIRS:STRING=../prefix/$libdir \
		../
else
	echo_eval $cmake_program \
		-D CMAKE_BUILD_TYPE:STRING=RELEASE \
		-D Trilinos_ENABLE_Sacado:BOOL=ON \
		-D Sacado_ENABLE_TESTS:BOOL=OFF \
		-D CMAKE_INSTALL_PREFIX:PATH=$prefix \
		-D Trilinos_INSTALL_LIB_DIR=$prefix/$libdir \
		../
fi
echo_eval make install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$installed_flag
echo "get_sacado.sh: OK"
