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
version='3.10'
repository='https://projects.coin-or.org/svn/Ipopt'
prefix=`pwd`'/build/prefix'
# -----------------------------------------------------------------------------
if [ ! -d external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "Ipopt-$version" ]
then
	echo_eval svn checkout "$repository/stable/$version Ipopt-$version"
fi
echo_eval cd "Ipopt-$version"
# -----------------------------------------------------------------------------
file='Ipopt/src/Algorithm/IpIpoptAlg.cpp'
comment='// Suppress printing startup message'
if ! grep "$comment" $file > /dev/null
then
	match='static bool copyright_message_printed'
	echo "Patch $file"
	echo "so it does not print message with every run."
	sed -e "s|^  $match = false;|$comment\n//&\n  $match = true;|" -i $file
fi
# -----------------------------------------------------------------------------
if [ -e ThirdParty/HSL ]
then
	echo_eval rm -rf ThirdParty/HSL
fi
for package in Mumps Metis
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
	--with-blas-lib=-lblas \
	--with-lapack-lib=-llapack
# -----------------------------------------------------------------------------
echo_eval make install 
