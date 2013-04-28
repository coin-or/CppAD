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
cat << EOF > eigen_io.cpp
# include <cppad/cppad.hpp>
# include <cppad/example/cppad_eigen.hpp>

int main()
{	
	typedef Eigen::Matrix< CppAD::AD<double>, 2, 2> MatrixAD;
	MatrixAD X;
	X << 1, 2, 3, 4;
	std::cout << X << std::endl;
	std::cout << "output above should be" << std::endl;
	std::cout << "1 2" << std::endl;
	std::cout << "3 4" << std::endl;
	return 0;
}
EOF
echo_eval g++ \
	eigen_io.cpp \
	-I$HOME/cppad/trunk \
	-I$HOME/prefix/eigen/include \
	-g \
	-O0 \
	-std=c++11 \
	-o eigen_io
echo_eval ./eigen_io
echo_eval rm eigen_io.cpp eigen_io
