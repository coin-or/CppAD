#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
set -e -u
# ----------------------------------------------------------------------------
# This sript name is 'test_*' instead of 'check_*' because it requires
# that cmake has already cerated build/bukld.ninja or Makefile.
program='bin/test_install.sh'
if [ "$0" != "$program" ]
then
   echo "$program: must be executed from its parent directory"
   exit 1
fi
if [ "$#" != 1 ]
then
   echo "usage: $program builder"
   echo 'where builder is ninja or make'
   exit 1
fi
builder="$1"
if [ "$1" != 'ninja' ] && [ "$1" != 'make' ]
then
   echo "usage: $program builder"
   echo 'where builder is ninja or make'
   exit 1
fi
if [ "$builder" == 'ninja' ] && [ ! -e build/build.ninja ]
then
   echo "$program: builder is ninja and connot find build/build.ninja"
   exit 1
fi
if [ "$builder" == 'make' ] && [ ! -e build/Makefile ]
then
   echo "$program: builder is make and connot file build/Makefile"
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
# -----------------------------------------------------------------------------
PKG_CONFIG_PATH="$prefix/share/pkgconfig"
LD_LIBRARY_PATH=""
for dir in lib lib64
do
   PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$prefix/$dir/pkgconfig"
   LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$prefix/$dir"
done
export PKG_CONFIG_PATH
export LD_LIBRARY_PATH
# -----------------------------------------------------------------------------
# make install
cd build
$builder install
# -----------------------------------------------------------------------------
#
# cflags
# libs
cflags=$(pkg-config cppad --cflags)
libs=$(pkg-config cppad --libs)
#
# test_install
if [ ! -e test_install ]
then
   mkdir test_install
fi
cd test_install
#
# CppAD get_started
cp ../../example/get_started/get_started.cpp get_started.cpp
echo_eval g++ $cflags $libs get_started.cpp -o get_started
echo 'CppAD: ./get_started'
if ! ./get_started
then
   echo "$program: $(pwd)/get_started test failed."
   exit 1
fi
#
# ipopt_solve get_started
cp ../../example/ipopt_solve/get_started.cpp get_started.cpp
cat << EOF >> get_started.cpp
int main(void)
{  if( ! get_started() )
      return 1;
   return 0;
}
EOF
echo_eval g++ $cflags $libs get_started.cpp -o get_started
echo 'ipopt_solve: ./get_started'
if ! ./get_started
then
   echo "$program: $(pwd)/get_started test failed."
   exit 1
fi
# -----------------------------------------------------------------------------
echo "$program: OK"
exit 0
