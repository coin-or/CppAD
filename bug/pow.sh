#! /bin/bash -e
# vim: set expandtab:
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
name=`echo $0 | sed -e 's|^bug/||' -e 's|\.sh$||'`
if [ "$0" != "bug/$name.sh" ]
then
    echo 'usage: bug/alloc_global.sh'
    exit 1
fi
# -----------------------------------------------------------------------------
if [ -e build/bug ]
then
    rm -r build/bug
fi
mkdir -p build/bug
cd build/bug
# cmake ../..
# -----------------------------------------------------------------------------
cat << EOF
Issue 43:
f(x) = x_0^0.5 and differentiate at x = (0,0).
Expect some NaN or Inf in the Jacobian or some error,
but get a jac = {0, 0}.
EOF
cat << EOF > $name.cpp
# include <cstdio>
# include "cppad/cppad.hpp"


int main(int argc, char** argv)
{   bool ok = true;

    using std::cout;
    using CppAD::AD;
    using CppAD::vector;
    //
    vector< double> x(2);
    vector< AD<double> > ax(2), ay(1);
    //
    ax[0] = x[0] = 0.0;
    ax[1] = x[1] = 0.0;
    //
    CppAD::Independent(ax);
    ay[0] = pow(ax[0], 0.5);
    CppAD::ADFun<double> f(ax, ay);
    //
    vector<double> jac = f.Jacobian(x);
    cout << "jac[0] = " << jac[0] << "\n";
    //
    ok &= Value(ay[0]) == 0.0;
    ok &= ! std::isfinite( jac[0] );
    //
    if( ! ok )
        return 1;
    return 0;
}
EOF
cxx_flags='-Wall -pedantic-errors -std=c++11 -Wshadow -Wconversion -g -O0'
eigen_dir="$HOME/prefix/eigen/include"
echo "g++ -I../../include -isystem $eigen_dir $cxx_flags $name.cpp -o $name"
g++ -I../../include -isystem $eigen_dir $cxx_flags $name.cpp -o $name
#
echo "build/bug/$name"
if ! ./$name
then
    echo
    echo "build/bug/$name: Error"
    exit 1
fi
echo
# -----------------------------------------------------------------------------
echo "bug/$name.sh: OK"
exit 0
