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
# Using g++ 4.8.1 results in the following error message:
#
# cppad-20130918 error from unknown source
# Error detected by false result for
#     0
# at line 1898 in the file 
#     ../../cppad/local/optimize.hpp
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
cat << EOF > $name.cpp
#include <cppad/cppad.hpp>

int main()  {
	using std::cout;
	using CppAD::vector;
	using CppAD::AD;

	vector< AD<double> > ax(1), ay(1);
	ax[0] = 3.;

	CppAD::Independent(ax);

	ay[0] = CppAD::sign(ax[0]); 

	CppAD::ADFun<double> tape;
	tape.Dependent(ax,ay);

	tape.optimize();

	return(0);

}
EOF
echo "g++ -std=c++11 -g -I../.. -I$HOME/prefix/eigen/include $name.cpp -o $name"
g++ -std=c++11 -g -I../.. -I$HOME/prefix/eigen/include $name.cpp -o $name
#
echo "./$name"
./$name
#
echo "rm $name $name.cpp"
rm $name $name.cpp
