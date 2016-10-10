#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# $begin get_ipopt.sh$$ $newlinech #$$
# $spell
#	tgz
#	Ipopt
#	CppAD
# $$
#
# $section Download and Install Ipopt in Build Directory$$
# $index ipopt, download and install$$
# $index download, install ipopt$$
# $index install, ipopt$$
#
# $head Syntax$$
# $code bin/get_ipopt.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install
# $href%http://www.coin-or.org/projects/Ipopt.xml%Ipopt%$$ in the
# CppAD $code build$$ directory.
#
# $head Distribution Directory$$
# This command must be executed in the
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head External Directory$$
# The Ipopt source code is downloaded and compiled in the sub-directory
# $code build/external$$ below the distribution directory.
#
# $head Prefix Directory$$
# The Ipopt libraries and include files are installed in the sub-directory
# $code build/prefix$$ below the distribution directory.
#
# $head Reuse$$
# The file $codei%build/external/Ipopt-%version%.tgz%$$
# and the directory $codei%build/external/Ipopt-%version%$$
# will be reused if they exist. Delete this file and directory
# to get a complete rebuild.
#
# $end
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_ipopt.sh" ]
then
	echo "bin/get_ipopt.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo 'Download ipopt to build/external and install it to build/prefix'
version='3.11.9'
web_page='http://www.coin-or.org/download/source/Ipopt'
cppad_dir=`pwd`
prefix="$cppad_dir/build/prefix"
installed_flag="build/external/ipopt-${version}.installed"
if [ -e "$installed_flag" ]
then
	echo "$installed_flag exists: Skipping get_ipopt.sh"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ ! -d build/external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "Ipopt-$version.tgz" ]
then
	echo_eval wget --no-check-certificate "$web_page/Ipopt-$version.tgz"
fi
if [ ! -e "Ipopt-$version" ]
then
	echo_eval tar -xzf Ipopt-$version.tgz
fi
echo_eval cd "Ipopt-$version"
# -----------------------------------------------------------------------------
for file in ThirdParty/Blas/get.Blas ThirdParty/Lapack/get.Lapack
do
	sed -e 's|ftp:\(\/\/www.netlib.org\/\)|http:\1|' \
		-i $file
done
# -----------------------------------------------------------------------------
if [ -e ThirdParty/HSL ]
then
	echo_eval rm -rf ThirdParty/HSL
fi
for package in Blas Lapack Metis Mumps
do
	if [ ! -e "ThirdParty/$package/get.$package.done" ]
	then
		echo_eval cd ThirdParty/$package
		echo_eval "./get.$package"
		echo_eval touch "get.$package.done"
		echo_eval cd ../..
	fi
done
# -----------------------------------------------------------------------------
if [ -e /usr/lib64 ]
then
	libdir='lib64'
else
	libdir='lib'
fi
export PKG_CONFIG_PATH="$prefix/$libdir/pkgconfig"
echo_eval ./configure \
	--enable-debug \
	--prefix="$prefix" \
	--libdir="$prefix/$libdir" \
	coin_skip_warn_cxxflags='yes'
echo_eval make install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$installed_flag
echo "get_ipopt.sh: OK"
