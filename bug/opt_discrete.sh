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
#!/bin/bash -e
#
if [ ! -e build ]
then
	mkdir build
fi
cd build
if [ ! -e ../../cppad/configure.hpp ]
then
	cmake ../..
fi
#
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
echo "create $name.cpp"
cat << EOF > $name.cpp
// Bug reported by Kasper Kristensen on 2014-09-25

# include <cppad/cppad.hpp>

namespace {
	double floor(const double& x)
	{	return std::floor(x); }

	CPPAD_DISCRETE_FUNCTION(double, floor);
}

int main()
{	using CppAD::vector;

	vector< CppAD::AD<double> > ax(1), ay(1);
	ax[0] = 0.0; 
	CppAD::Independent(ax);
	ay[0] =  floor(ax[0]) + floor(ax[0]);  
	CppAD::ADFun<double> f(ax, ay);

	size_t size_before = f.size_var();
	f.optimize(); 
	size_t size_after = f.size_var();
	assert( size_after + 1 == size_before );

	vector<double> x(1), y(1);
	x[0] = -2.2;
	y    = f.Forward(0, x);
	assert( y[0] == -6.0 );

}
EOF
echo "g++ -g $name.cpp -I../.. -fopenmp -std=c++11 -o $name"
g++ -g $name.cpp -I../.. -fopenmp -std=c++11 -o $name
#
echo "./$name"
./$name
#
echo "rm $name $name.cpp"
rm $name $name.cpp
