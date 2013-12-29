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
# eigen_scalar.cpp:57:21: 
# warning: ISO C++ says that these are ambiguous, 
# even though the worst conversion for the first is better 
# than the worst conversion for the second: [enabled by default]
#  D      = A * (B * C);
#                     ^
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
# include <iostream>
# include <Eigen/Core>

// following is 0 or 1. If 0, there is no warning
# define DEFINE_TEMPLATED_CONSTRUCTOR 1

// ------------------------------------------------------------------------
class myscalar {
public:
	// data
	double value_;
	// constructors
	myscalar(void)
	{	value_ = 0.0; }	
	myscalar(const double& value)
	{	value_ = value; }

# if DEFINE_TEMPLATED_CONSTRUCTOR
	template <class T> explicit myscalar(const T& value)
	{	value_ = double(value); }	
# endif

	// binary operators
	myscalar operator+(const myscalar& right) const
	{	myscalar result = value_ + right.value_;
		return result;
	}
	myscalar operator*(const myscalar& right) const
	{	myscalar result = value_ * right.value_;
		return result;
	}
	bool operator!=(const myscalar& right) const
	{	bool result = value_ != right.value_;
		return result;
	}

	// assignments
	myscalar& operator=(const double& value)
	{	value_ = value; 
		return *this;
	}

	// computed assignment operators
	myscalar& operator +=(const myscalar& right)
	{	value_ += right.value_;
		return *this;
	}

};
// ------------------------------------------------------------------------
int main() {
	using Eigen::Matrix;
	using Eigen::Dynamic;

	Matrix<myscalar, Dynamic, Dynamic> A(1,1);
	Matrix<myscalar, Dynamic, Dynamic> B(1,1);
	Matrix<myscalar, Dynamic, Dynamic> C(1,1);
	Matrix<myscalar, Dynamic, Dynamic> D(1,1);

	A(0,0) = 1.0;
	B(0,0) = 2.0;
	C(0,0) = 3.0;
	D      = A * (B * C); 

	if( D(0,0) != myscalar(6.0) )
	{	std::cout << "$name: Error" << std::endl;
		return 1;
	}
	std::cout << "$name: OK" << std::endl;
	return 0;
}
EOF
if [ -e "$name" ]
then
	echo_eval rm $name
fi
echo_eval g++ \
	$name.cpp \
	-I$HOME/prefix/eigen/include \
	-g \
	-O0 \
	-std=c++11 \
	-o $name
echo_eval ./$name
