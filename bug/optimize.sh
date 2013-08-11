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
#! /bin/bash -e
cat << EOF > optimize.cpp
# include <cppad/cppad.hpp>
# include <iostream>

namespace {
	using CppAD::AD;
	typedef CppAD::vector< AD<double> > ADvector;
	typedef CppAD::vector<double>       Dvector;

	void myfun(const ADvector& x, ADvector& y)
	{	y[0] = x[0]; }	
}

int main(void)
{	ADvector au(2), ax(2), ay(1);
	ax[0] = 1.0;
	CppAD::checkpoint<double> myfun_atomic("myfun_check", myfun, ax, ay);

	au[0] = 2.0;
	au[1] = 3.0;
	CppAD::Independent(au);
	ax[0] = au[0];
	ax[1] = au[0] + au[1];
	myfun_atomic(ax, ay);
	CppAD::ADFun<double> my_adfun(au, ay);

	my_adfun.optimize();

	Dvector u(2), y(1);
	u[0] = 0.0;
	u[1] = 1.0;
	y    = my_adfun.Forward(0, u);

	std::cout << "y = " << y << std::endl;
	
	return 0;
}
EOF
echo_eval g++ \
	optimize.cpp \
	-I$HOME/cppad/trunk \
	-I$HOME/prefix/eigen/include \
	-g \
	-O0 \
	-std=c++11 \
	-o optimize
echo_eval ./optimize
echo_eval rm optimize.cpp optimize
