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
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------
cat << EOF
Function:
f(x)   = x^T * Q * x / 2

Note that the derivative of f(x) is given by:
f'(x)  = [ x^T * Q +  (Q * x)^T ] / 2
       = x^T * (Q * Q^T) / 2
EOF
# -----------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
#
cat << EOF > $name.cpp
# include <cppad/example/cppad_eigen.hpp>
# include <Eigen/Dense>
# include <iostream>

int main()
{	bool ok = true;

	using CppAD::AD;
	using CppAD::ADFun;
	using CppAD::Independent;
	using Eigen::Matrix;
	using Eigen::Dynamic;
	using Eigen::VectorXd;

	typedef Matrix< AD<double> , Dynamic, Dynamic > a_MatrixXd;
	typedef Matrix< AD<double> , Dynamic , 1>       a_VectorXd;

	int size = 2, i , j;

	a_VectorXd a_x(size);
	VectorXd x(size);

	x << 1, 2;
	for(i = 0; i < size; i++) {
		a_x[i] = x[i];
	}

	a_MatrixXd a_Q(size, size);
	a_Q << 1, 2, 3, 4;

	Independent(a_x);
	AD<double> a_two = 2.0;
	a_VectorXd a_y = (a_x.transpose() * a_Q * a_x) / a_two;

	// create f: x -> y and stop tape recording
	ADFun<double> f(a_x, a_y);

	VectorXd jac           = f.Jacobian(x);
	a_MatrixXd a_check_jac = a_x.transpose() * (a_Q + a_Q.transpose()) / a_two;


	std::cout << "True f'(x)  = " << a_check_jac << std::endl;
	std::cout << "CppAD f'(x) = " << jac.transpose() << std::endl;

	return 0;
}
EOF
if [ -e "$name" ]
then
	echo_eval rm $name
fi
echo_eval g++ \
	$name.cpp \
	-I../.. \
	-I$HOME/prefix/eigen/include \
	-g \
	-O0 \
	-std=c++11 \
	-Wno-shadow \
	-Wno-deprecated-declarations \
	-o $name
echo_eval ./$name
