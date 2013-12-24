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

int main() {
	using Eigen::Matrix;
	using Eigen::Dynamic;
	typedef CppAD::AD<double> AScalar;

	Matrix<AScalar, Dynamic, Dynamic> A2(1,1);
	Matrix<AScalar, Dynamic, Dynamic> B2(1,1);

	A2(0,0) = 1.0;
	B2(0,0) = 2.0;

	A2 * B2 * A2; 

	return 0;
}
EOF
echo_eval g++ \
	$name.cpp \
	-I../.. \
	-I$HOME/prefix/eigen/include \
	-g \
	-O0 \
	-std=c++11 \
	-o $name
echo_eval ./$name
