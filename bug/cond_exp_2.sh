#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
This program corresponds to:
	https://github.com/coin-or/CppAD/issues/8
it exits with the following assertion
	dw = f.Reverse(q, w): has a nan,
	but none of its Taylor coefficents are nan.
	Error detected by false result for
		! ( hasnan(value) && check_for_nan_ )
	at line 202 in the file
		../../cppad/local/reverse.hpp
which sould not be the case
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>
using namespace CppAD;

int main(void) {
    std::vector< AD<double> > ax(2);
    ax[0] = 1.;
    ax[1] = 1.;
    Independent(ax);

    std::vector< AD<double> > ay(1);
    // y_0 = x_0 / x_1 if x_1 > 1.0
	//       0.0       otherwise
    ay[0] = CondExpGt(ax[1], AD<double>(1.0), ax[0] / ax[1], AD<double>(0.0));

    ADFun<double> f(ax, ay);
    std::vector<double> x(2);
    x[0] = 1.;
    x[1] = 0.;
    std::vector<double> J(2);
    J = f.Jacobian(x);
    assert(J[0] == 0.0);
    assert(J[1] == 0.0);

    return 0;
}
EOF
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
mv ../bug.$$ $name.cpp
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ! ./$name
then
	echo
	echo "$name.sh: Error"
	exit 1
fi
echo
echo "$name.sh: OK"
exit 0
