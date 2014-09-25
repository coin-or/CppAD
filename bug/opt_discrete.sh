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
	double my_abs(const double& x)
	{	return std::fabs(x); }

	CPPAD_DISCRETE_FUNCTION(double,my_abs);
}

int main()
{
  CppAD::vector< CppAD::AD<double> > x(2), y(1);
  x[0]=0;
  x[1]=0;
  CppAD::Independent(x);
  y[0]=my_abs(x[0]);
  CppAD::ADFun<double> F(x,y);
  
  F.optimize(); 
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
