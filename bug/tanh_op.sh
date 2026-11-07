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

int main() {
    using AD = CppAD::AD<double>;
    const double S = 1e16;
    std::vector<AD> x(1), y(1);
    x[0] = 2.0;
    CppAD::Independent(x);
    AD u = S * x[0] * x[0];
    AD v = S * x[0] * x[0] * x[0];
    y[0] = tanh(u) * v + u;
    CppAD::ADFun<double> f(x, y);

    std::vector<double> xv{2.0}, seed{1.0}, zero{0.0}, weight{1.0};
    f.Forward(0, xv);
    auto gradient = f.Forward(1, seed);
    auto second = f.Forward(2, zero);
    auto reverse = f.Reverse(2, weight);
    auto hessian = f.Hessian(xv, 0);

    std::printf("See https://github.com/coin-or/CppAD/issues/261\n");
    std::printf("gradient = %g\n", gradient[0]);
    std::printf("Forward(2) * 2 = %g\n", 2.0 * second[0]);
    std::printf("Reverse(2) = %g\n", reverse[1]);
    std::printf("Hessian = %g\n", hessian[0]);
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
