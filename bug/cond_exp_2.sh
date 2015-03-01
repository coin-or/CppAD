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
The skip during reverse mode should check that the multiplier is identically
zero. Here is an example that demonstrates why.
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>
int main(void) {
	bool ok = true;
    using namespace CppAD;

	double eps = 10. * std::numeric_limits<double>::epsilon();

    typedef AD<double> adouble;
    typedef AD<adouble> a2double;

    // This works (does not generate a nan)
    // std::vector<double> x{-1.0, -1.0};

    // This fails
    std::vector<double> x{0.0, 0.0};

    std::vector<a2double> a2x(x.size());
    for (size_t i = 0; i < a2x.size(); i++) {
        a2x[i] = adouble(x[i]);
    }
    Independent(a2x);

    std::vector<a2double> a2y(1);
    a2y[0] = CondExpGt(a2x[0], a2double(1.0), a2x[0] / a2x[1], a2double(0.0));

    ADFun<adouble> f1;
    f1.Dependent(a2x, a2y);

    std::vector<adouble> ax{adouble(x[0]), adouble(x[1])};
    Independent(ax);

    std::vector<adouble> ay = f1.Jacobian(ax);

    CppAD::ADFun<double> f2;
    f2.Dependent(ax, ay);

    x = {2, 1};

    std::vector<double> y = f2.Forward(0, x);
    ok &= CppAD::NearEqual(y[0], 1.0/ x[1], eps, eps);

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
