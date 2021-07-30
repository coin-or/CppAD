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
# -----------------------------------------------------------------------------
# libdir
if ls $prefix/lib/libcppad_lib.* >& /dev/null
then
    libdir='lib'
else
    libdir='lib64'
fi
# -----------------------------------------------------------------------------
list="
    include/cppad
    share/pkgconfig/cppad.pc
    $libdir/pkgconfig/cppad.pc
"
for name in $list
do
    if [ ! -e $prefix/$name ]
    then
        echo "check_install.sh: cannot find $prefix/$name"
        exit 1
    fi
done
# -----------------------------------------------------------------------------
# check pkgconfig file
pkg-config cppad --libs > check_install.$$
if ! grep ' -lcppad_lib \| -lcppad_lib$' check_install.$$ > /dev/null
then
    echo 'check_install.sh: cannot find -lcppad_lib in Libs section of'
    echo "$PKG_CONFIG_PATH/cppad.pc"
fi
rm check_install.$$
# -----------------------------------------------------------------------------
echo 'check_install.sh: OK'
exit 0
