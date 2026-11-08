#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
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
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <new>
#include <utility>

int main() {
    using Fun = CppAD::ADFun<double>;
    Fun source;
    alignas(Fun) unsigned char storage[sizeof(Fun)];
    std::memset(storage, 0xBE, sizeof(storage));
    Fun* destination = new (storage) Fun(std::move(source));
    const std::size_t n_var = destination->size_var();
    destination->~Fun();
    std::printf("https://github.com/coin-or/CppAD/issues/263\n");
    std::printf("moved ADFun size_var=%zu\n", n_var);
    return n_var == 0 ? 0 : 1;
}
EOF
#
# $name
cmd="
    clang++
    -fsanitize=undefined
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
$cmd
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
