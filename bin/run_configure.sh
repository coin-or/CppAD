#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ ! -e "bin/run_configure.sh" ]
then
   echo "bin/run_configure.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$1" == 'clang=yes' ]
then
   with_clang='--with-clang'
elif [ "$1" == 'clang=no' ]
then
   with_clang=''
else
   echo 'usage: bin/run_configure (clang=yes|clang=no)'
   exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
# prefix
eval `grep '^prefix=' bin/get_optional.sh`
if [[ "$prefix" =~ ^[^/] ]]
then
   prefix="$(pwd)/$prefix"
fi
echo "prefix=$prefix"
# -----------------------------------------------------------------------------
#
# PKG_CONFIG_PATH
PKG_CONFIG_PATH="$prefix/lib64/pkgconfig:$prefix/lib/pkgconfig"
PKG_CONFIG_PATH="$prefix/share/pkgconfig:$PKG_CONFIG_PATH"
export PKG_CONFIG_PATH
#
# testvector
testvector='cppad'
#
# cppad_cxx_flags
cppad_cxx_flags="-Wall -pedantic-errors -std=c++17 -Wshadow"
cppad_cxx_flags="$cppad_cxx_flags -Wfloat-conversion -Wconversion"
#
# ---------------------------------------------------------------------------
if [ ! -e build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
if [ -e CMakeCache.txt ]
then
   echo_eval rm CMakeCache.txt
fi
if [ -e CMakeFiles ]
then
   echo_eval rm -r CMakeFiles
fi
# -----------------------------------------------------------------------------
../configure.sh \
   --prefix=$prefix \
   $with_clang \
   --with-stdvector \
   MAX_NUM_THREADS=32 \
   CXX_FLAGS="'$cppad_cxx_flags'" \
   ADOLC_DIR=$prefix \
   BOOST_DIR=$prefix \
   EIGEN_DIR=$prefix \
   FADBAD_DIR=$prefix \
   SACADO_DIR=$prefix \
   IPOPT_DIR=$prefix \
   TAPE_ADDR_TYPE=size_t \
   TAPE_ID_TYPE=size_t
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
