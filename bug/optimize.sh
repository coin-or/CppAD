#! /bin/bash -e
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
cmake ../..
# -----------------------------------------------------------------------------
cat << EOF
Bug in CppAD optimizer.
Test passes when nz = 9998 and fails when nz = 9999
EOF
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>
int main(void)
{   bool ok = true;
    using std::cout;
    using CppAD::AD;
    using CppAD::vector;
    //
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // length of the data vector z
    size_t nz = 9999;
    //
    // factor for last term
    double factor = 1e+5;
    //
    // z starts at -1.0 and ends at 1.0
    vector<double> z(nz);
    for(size_t i = 0; i < nz; i++)
        z[i] = -1.0 + i * 2.0 / double(nz - 1);
    //
    // f(x) = sum from i=0 to nz-1 of (x - z[i])^2
    vector< AD<double> > ax(1), ay(1);
    ax[0] = 0.0;
    CppAD::Independent(ax);
    AD<double> asum = 0.0;
    for(size_t i = 0; i < nz; i++)
    {   AD<double> aterm = z[i] - ax[0];
        if( i == nz - 1 )
            asum += factor * aterm;
        else
            asum += aterm / factor;
    }
    ay[0] = asum;
    CppAD::ADFun<double> f(ax, ay);
    //
    // value of x where we are computing derivative
    vector<double> x(1), y_before(1), y_after(1);
    x[0]     = .1;
    y_before = f.Forward(0, x);
    f.optimize();
    y_after  = f.Forward(0, x);
    //
    ok &= CppAD::NearEqual(y_before[0], y_after[0], eps99, eps99);
    //
    if( ok )
        return 0;
    return 1;
}
EOF
# -----------------------------------------------------------------------------
echo "g++ -I../../include --std=c++11 -g $name.cpp -o $name"
g++ -I../../include --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ! ./$name
then
    echo
    echo "$name.sh: Error"
    exit 1
fi
echo
# ------------------------------------------------------------------------------
echo "$name.sh: OK"
exit 0
