#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
f(x) = x[0] / x[1] if (x[0] > 0 and x[1] >= x[0]) else 1.0
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>
int main(void) {
	bool ok = true;
    using CppAD::vector;
	double eps = 10. * std::numeric_limits<double>::epsilon();

    typedef CppAD::AD<double>   a1double;
    typedef CppAD::AD<a1double> a2double;

    // value during taping
	vector<double> x(2);
	x[0] = 0.0;
	x[1] = 0.0;
	// works for this case
	// x[1] = 5.0;

    vector<a2double> a2x(x.size());
    for (size_t i = 0; i < a2x.size(); i++)
        a2x[i] = a2double(x[i]);
    Independent(a2x);

	a2double a2zero = a2double(0.0);
	a2double a2one  = a2double(1.0);
	a2double temp_1 = CondExpGt(a2x[1], a2x[0], a2x[0] / a2x[1], a2one);
	a2double temp_2 = CondExpGt(a2x[0], a2zero, temp_1, a2one);

    vector<a2double> a2y(1);
    a2y[0] = temp_2;

    CppAD::ADFun<a1double> f1;
    f1.Dependent(a2x, a2y);

    vector<a1double> a1x(x.size());
    for (size_t i = 0; i < a1x.size(); i++)
        a1x[i] = a1double(x[i]);
    Independent(a1x);

    vector<a1double> a1z = f1.Jacobian(a1x);

    CppAD::ADFun<double> f;
    f.Dependent(a1x, a1z);

    // now check result using doubles
	// for a case where f(x) = x[0] / x[1];
	x[0] = 1.0;
	x[1] = 2.0;

    vector<double> z = f.Forward(0, x);
	std::cout << "z = " << z << std::endl;
    ok &= CppAD::NearEqual(z[0], 1.0/x[1], eps, eps);
    ok &= CppAD::NearEqual(z[1], - x[0]/(x[1]*x[1]), eps, eps);

	if( ! ok )
		return 1;

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
