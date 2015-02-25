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
The two programs below corresponding to:
	https://github.com/coin-or/CppAD/issues/7
	https://github.com/coin-or/CppAD/issues/8

The first program exits with the following assertion:
	Error detected by false result for
		IdenticalPar(left)
	at line 126 in the file
		../../cppad/local/cskip_op.hpp

The second program exists with the following assertion:
	cppad-20141230 error from a known source:
	dw = f.Reverse(q, w): has a nan,
	but none of its Taylor coefficents are nan.
	Error detected by false result for
		! ( hasnan(value) && check_for_nan_ )
	at line 202 in the file
		../../cppad/local/reverse.hpp
which sould not be the case

EOF
cat << EOF > bug_1.cpp
#include <iostream>
#include <cppad/cppad.hpp>
int main(void) {
	using namespace CppAD;

	typedef AD<double> adouble;
	typedef AD<adouble> a2double;

	std::vector<double> x{0, 1};

	/**
	* First tape
	* (using AD<AD<double> >)
	*/
	std::vector<a2double> a2x(x.size());
	for (size_t i = 0; i < a2x.size(); i++) {
	a2x[i] = adouble(x[i]);
	}
	Independent(a2x);

	std::vector<a2double> a2y(1);
	a2double a = a2x[0] * a2x[1];
	a2y[0] = CondExpEq(a2x[0], a2double(1.0), a, a2double(0.0));

	// af(x) = x_0 * x_1 if x[0] == 1
	//         0.0       otherwise
	ADFun<adouble> af(a2x, a2y);
	af.optimize();

	/**
	* Second tape
	* (using AD<double>)
	*/
	std::vector<adouble> ax{adouble(1), adouble(0)};
	Independent(ax);

	std::vector<adouble> ay = af.Forward(0, ax);

	// f(x) = x_0 * x_1 if x[0] == 1
	//        0.0       otherwise
	CppAD::ADFun<double> f(ax, ay);

	/**
	* Use second tape
	*/
	x = {1, 0.5};

	std::vector<double> y = f.Forward(0, x);

	//std::cout << y[0] << std::endl;
	assert(std::abs(y[0] - x[0] * x[1]) < 1e-10);

}
EOF
cat << EOF > bug_2.cpp
#include <cppad/cppad.hpp>
using namespace CppAD;

int main(void) {
    std::vector<AD<double>> X(2);
    X[0] = 1.;
    X[1] = 1.;
    Independent(X);

    std::vector<AD<double>> Y(1);
    // Y[0] = X[1] > 1.0 ? X[0] / X[1] : 0;
    Y[0] = CondExpGt(X[1], AD<double>(1.0), X[0] / X[1], AD<double>(0.0));

    ADFun<double> fun(X, Y);
    std::vector<double> u(2);
    u[0] = 1.;
    u[1] = 0.;
    std::vector<double> J(2);
    J = fun.Jacobian(u);
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
ok='ture'
for number in 1 2
do
	mv ../bug_$number.cpp ${name}_$number.cpp
	#
	echo "g++ -I../.. --std=c++11 -g ${name}_$number.cpp -o ${name}_$number"
	g++ -I../.. --std=c++11 -g ${name}_$number.cpp -o ${name}_$number
	#
	echo "./${name}_$number"
	if ! ./${name}_$number
	then
		ok='false'
	fi
	echo
done
if [ "$ok" == 'true' ]
then
	echo "OK"
else
	echo "Error"
fi
