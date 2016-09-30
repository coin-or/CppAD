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
# $begin get_colpack.sh$$ $newlinech #$$
# $spell
#	colpack
#	gz
#	Col
#	CppAD
# $$
#
# $section Download and Install ColPack in Build Directory$$
#
# $head Syntax$$
# $code bin/get_colpack.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install
# $href%http://cscapes.cs.purdue.edu/dox/ColPack/html/%ColPack%$$ in the
# CppAD $code build$$ directory.
#
# $head Distribution Directory$$
# This command must be executed in the
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head External Directory$$
# The ColPack source code is downloaded into the sub-directory
# $code build/external$$ below the distribution directory.
#
# $head Prefix Directory$$
# The ColPack include files are installed in the sub-directory
# $code build/prefix/include/ColPack$$ below the distribution directory.
#
# $head Reuse$$
# The file $codei%build/external/ColPack-%version%.tar.gz%$$
# and the directory $codei%build/external/ColPack-%version%$$
# will be reused if they exist. Delete this file and directory
# to get a complete rebuild.
#
# $end
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_colpack.sh" ]
then
	echo "bin/get_colpack.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo 'Download colpack to build/external and install it to build/prefix'
version='1.0.10'
web_page='https://codeload.github.com/CSCsw/ColPack/tar.gz'
cppad_dir=`pwd`
prefix="$cppad_dir/build/prefix"
installed_flag="build/external/colpack-${version}.installed"
if [ -e "$installed_flag" ]
then
	echo "$installed_flag exists: Skipping get_colpack.sh"
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
if [ ! -e "ColPack-$version.tar.gz" ]
then
	echo_eval curl $web_page/v$version -o ColPack-$version.tar.gz
fi
# -----------------------------------------------------------------------------
if [ ! -e ColPack-$version ]
then
	echo_eval tar -xzf ColPack-$version.tar.gz
fi
echo_eval cd ColPack-$version
# -----------------------------------------------------------------------------
if which autoconf >& /dev/null
then
	echo_eval libtoolize
	echo_eval autoreconf --install --force
fi
# -----------------------------------------------------------------------------
#
system_name=`uname | sed -e 's|\(......\).*|\1|'`
if [ "$system_name" == 'CYGWIN' ]
then
	lib_type='--enable-static --disable-shared'
	echo_eval ls -l ./configure
	echo_eval chmod +x ./configure
else
	lib_type='--enable-static --enable-shared'
fi
echo_eval ./configure \
	--prefix=$prefix \
	--libdir=$prefix/$libdir \
	$lib_type
#
echo_eval make install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$installed_flag
echo "get_colpack: OK"
