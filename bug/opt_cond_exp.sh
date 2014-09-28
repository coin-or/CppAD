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
Description
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>

namespace {
	using CppAD::vector;
	using CppAD::AD;
	using CppAD::ADFun;

	template <class Type>
	Type max(const vector<Type> &arg)
	{	Type res = arg[0];
		for(size_t i = 0;i < arg.size(); i++)
    		res = CondExpGt(res, arg[i], res, arg[i]);
		return res;
  	}
}

int main()
{
	// n >= 3 required to trigger bug
	size_t n = 2;

	vector< AD<double> > ax(n), ay(1);
	for(size_t i = 0; i < n; i++)
		ax[i] = 1.0;
	Independent(ax);
	ay[0] = max(ax) + max(ax);
	ADFun<double> f(ax, ay);

	f.optimize();

	vector<double> x(n);
	for(size_t i = 0;i < n; i++)
		x[i] = 1.0;
	std::cout << f.Jacobian(x) << "\n";
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
mv ../bug.$$ $name.cpp
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ./$name
then
	echo "OK"
else
	echo "Error"
fi
