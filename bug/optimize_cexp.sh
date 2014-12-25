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
Running this script causes an assert connected to conditional expressions.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
using namespace CppAD;

int main(){
	bool ok = true;
	vector< AD<double> > ax(1), ay(1);
	ax[0] = 1.0;
	Independent(ax);
	ay[0] = CondExpGe(ax[0], ax[0], exp(ax[0]), exp(ax[0]));
	ADFun<double> f(ax, ay);

	f.optimize();

	vector<double> x0(1), y0(1);
	x0[0] = Value(ax[0]);
	y0    = f.Forward(0, x0);
	ok   &= y0[0] == Value(ay[0]);
	if( ok )
		return 0;
	return 1;
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
	exit 0
else
	echo "Error"
	exit 1
fi
