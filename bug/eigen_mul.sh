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
# eigen_mul.cpp:15:12: warning: ISO C++ says that these are ambiguous, 
# even though the worst conversion for the first is better than the worst 
# conversion for the second: [enabled by default]
#  A2 * B2 * A2;
#            ^
# ------------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
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
#include <Eigen/Core>
#include <cppad/cppad.hpp>

// set this flag to 1 to get a version that works without warning
# define USE_EVAL 0

int main() {
	using Eigen::Matrix;
	using Eigen::Dynamic;
	typedef CppAD::AD<double> AScalar;

	Matrix<AScalar, Dynamic, Dynamic> A(1,1);
	Matrix<AScalar, Dynamic, Dynamic> B(1,1);
	Matrix<AScalar, Dynamic, Dynamic> C(1,1);
	Matrix<AScalar, Dynamic, Dynamic> D(1,1);

	A(0,0) = 1.0;
	B(0,0) = 2.0;
	C(0,0) = 3.0;
# if USE_EVAL
	D      = A * (B * C).eval(); 
# else
	D      = A * (B * C); 
# endif

	if( D(0,0) != 6.0 )
	{	std::cout << "eigen_mul: Error" << std::endl;
		return 1;
	}
	std::cout << "eigen_mul: OK" << std::endl;
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
	-o $name
echo_eval ./$name
