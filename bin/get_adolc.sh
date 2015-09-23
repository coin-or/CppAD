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
# $begin get_adolc.sh$$ $newlinech #$$
# $spell
#	tgz
#	Adolc
#	gz
#	CppAD
# $$
#
# $section Download and Install Adolc in Build Directory$$
#
# $head Syntax$$
# $code bin/get_adolc.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install
# $href%https://projects.coin-or.org/ADOL-C%ADOL-C%$$ in the
# CppAD $code build$$ directory.
#
# $head Requirements$$
# You must first use $cref get_colpack.sh$$ to download and install
# $code ColPack$$ (coloring algorithms used for sparse matrix derivatives).
#
# $head Distribution Directory$$
# This command must be executed in the
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head External Directory$$
# The Adolc source code is downloaded into the sub-directory
# $code build/external$$ below the distribution directory.
#
# $head Prefix Directory$$
# The Adolc include files are installed in the sub-directory
# $code build/prefix/include/adolc$$ below the distribution directory.
#
# $head Reuse$$
# The files $codei%build/external/ADOL-C-%version%.tgz%$$
# and the directory $codei%build/external/ADOL-C-%version%$$
# will be reused if they exist. Delete this file and directory
# to get a complete rebuild.
#
# $end
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_adolc.sh" ]
then
	echo "bin/get_adolc.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo 'Download adolc to build/external and install it to build/prefix'
# ADOL-C verison 2.5.2 results in:
#	adouble.h:74: error: 'badouble::badouble()' is protected
# It apprears that newer versions of adolc assume some c++11 features; see
#	http://list.coin-or.org/pipermail/adol-c/2014-December/001023.html
version='2.4.1'
web_page="http://www.coin-or.org/download/source/ADOL-C"
cppad_dir=`pwd`
prefix="$cppad_dir/build/prefix"
installed_flag="build/external/adolc-${version}.installed"
if [ -e "$installed_flag" ]
then
	echo "$installed_flag exists: Skipping get_adolc.sh"
	exit 0
fi
# --------------------------------------------------------------------------
if [ -e /usr/lib64 ]
then
	libdir='lib64'
else
	libdir='lib'
fi
# -----------------------------------------------------------------------------
if [ ! -d build/external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "ADOL-C-$version.tgz" ]
then
	echo_eval wget --no-check-certificate $web_page/ADOL-C-$version.tgz
fi
# -----------------------------------------------------------------------------
if [ -e "$prefix/include/adolc" ]
then
	echo_eval rm -r "$prefix/include/adolc"
fi
# -----------------------------------------------------------------------------
if [ ! -e ADOL-C-$version ]
then
	echo_eval tar -xzf ADOL-C-$version.tgz
fi
echo_eval cd ADOL-C-$version
# -----------------------------------------------------------------------------
system=`uname | tr [A-Z] [a-z] | sed -e 's|\([a-z][a-z]*\).*|\1|'`
# -----------------------------------------------------------------------------
if which autoconf >& /dev/null
then
	echo_eval autoreconf --install --force
fi
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
# -----------------------------------------------------------------------------
flags="--prefix=$prefix --with-colpack=$prefix --libdir=$prefix/$libdir"
if [ "$system" == 'cygwin' ]
then
	flags="$flags --enable-static --disable-shared"
else
	flags="$flags --enable-static --enable-shared"
fi
#
echo_eval ../configure $flags
echo_eval make install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$installed_flag
echo "get_adolc: OK"
