#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
# -----------------------------------------------------------------------------
set -e -u
#
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
eigen_flags=$(pkg-config -cflags eigen3)
#
# ipopt_flags
ipopt_flags=$(pkg-config -cflags ipopt | sed -e 's|/coin-or | |' )
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
#include <cstdio>
#include <vector>
//
// test_precision
template <class Float> 
void test_precision(void) {
    //
    using CppAD::AD;
    std::vector< AD<Float> > ax(1), ay(1);
    ax[0] = 1.0;
    CppAD::Independent(ax);
    AD<Float>  au = ax[0] * ax[0];
    ay[0] = tanh(au) * au;
    CppAD::ADFun<Float> f(ax, ay);

    std::vector<Float> x{1e5}, dx{1.0}, zero{0.0}, weight{1.0};
    f.Forward(0, x);
    f.Forward(1, dx);
    auto forward_2 = f.Forward(2, zero);
    auto reverse_2 = f.Reverse(2, weight);

    std::printf("forward_2 * 2 = %.5e\n", 2.0 * forward_2[0]);
    std::printf("reverse_2     = %.5e\n", reverse_2[1]);
}
//
// main
int main(void) {
    std::printf("See https://github.com/coin-or/CppAD/issues/261\n");
    std::printf("Test using float\n");
    test_precision<float>();
    std::printf("Test using double\n");
    test_precision<double>();
}
EOF
#
# $name
cmd="
    g++
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
