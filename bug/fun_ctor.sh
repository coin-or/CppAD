#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-26 Bradley M. Bell
# -----------------------------------------------------------------------------
set -e -u
#
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
#
# name
name=`echo $0 | sed -e 's|^bug/||' -e 's|\.sh$||'`
if [ "$0" != "bug/$name.sh" ]
then
    echo "usage: bug/$name.sh"
    exit 1
fi
#
# PKG_CONFIG_PATH, LD_LIBRARY_PATH
prefix="$(pwd)/build/prefix"
export PKG_CONFIG_PATH="$prefix/share/pkgconfig"
export LD_LIBRARY_PATH=""
for subdir in  'lib' 'lib64'
do
    PKG_CONFIG_PATH+=":$prefix/$subdir/pkgconfig"
    LD_LIBRARY_PATH+=";$prefix/$subdir"
done
#
# eigen_flags
eigen_flags=$(pkg-config -cflags eigen3 | sed -e 's|-I|-isystem |')
#
# ipopt_flags
ipopt_flags=$(
    pkg-config -cflags ipopt | sed -e 's|-I|-isystem |' -e 's|/coin-or | |'
)
#
# link_flags
link_flags=$(pkg-config -libs cppad)
#
# extra_flags
extra_flags='-Wall -pedantic-errors -Wshadow -Wconversion -g -O0'
#
# build/bug
if [ -e build/bug ]
then
    rm -r build/bug
fi
mkdir -p build/bug
cd build/bug
#
# $name.cpp
cat << EOF > $name.cpp
#include <cppad/cppad.hpp>

int main() {
    std::printf("https://github.com/coin-or/CppAD/issues/263\n");
    CppAD::ADFun<double> source;
    CppAD::ADFun<double> destination( std::move(source) );
    return 0;
}
EOF
#
# $name
cmd="
    g++
    -fsanitize=undefined -fno-sanitize-recover=undefined
    $name.cpp
    -std=c++14
    -I../../include
    $extra_flags
    $eigen_flags
    $ipopt_flags
    -o $name
    $link_flags
"
#
echo $cmd
if ! $cmd
then
    echo "The compile command above failed"
    exit 1
fi
#
echo "build/bug/$name"
if ! ./$name
then
    echo
    echo "build/bug/$name: Error"
    echo "export LD_LIBRARY_PATH='$LD_LIBRARY_PATH'"
    exit 1
fi
echo
# -----------------------------------------------------------------------------
echo "bug/$name.sh: OK"
exit 0
