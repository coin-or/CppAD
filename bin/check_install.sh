#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
if [ $0 != 'bin/check_install.sh' ]
then
    echo 'bin/check_install.sh: must be executed from its parent directory'
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
# dir=$(pkg-config cppad --variable pcfiledir)
# cat $dir/cppad.pc
# -----------------------------------------------------------------------------
if [ ! -e build/check_install ]
then
    mkdir build/check_install
fi
cp example/get_started/get_started.cpp build/check_install/get_started.cpp
cd build/check_install
# -----------------------------------------------------------------------------
cflags=$(pkg-config cppad --cflags)
libs=$(pkg-config cppad --libs)
echo_eval g++ $cflags $libs get_started.cpp -o get_started
echo './get_started'
if ! ./get_started
then
    echo 'check_install.sh: ./get_started test failed.'
    exit 1
fi
# -----------------------------------------------------------------------------
echo 'check_install.sh: OK'
exit 0
