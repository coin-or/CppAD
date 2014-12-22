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
Running this script causes an assert connected to optimizing
the recording of a Jacobian calculation.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
using namespace CppAD;

int main(){
	// F(x) = sin(x[0]) if x[0] >= 3
	//      = sin(x[1]) otherwise
	vector<AD<AD<double> > > a2x(2), a2y(1);
	a2x[0]=1; a2x[1]=2;
	Independent(a2x);
	AD<AD<double> > three(3);
	// note if you change sin to 1.0/ you get a different assertion
	a2y[0] = sin( CppAD::CondExpGe(a2x[0], three, a2x[0], a2x[1]) );
	ADFun<AD<double> > F(a2x, a2y);

	// G_0(x) = partial F(x) w.r.t x[0]
	//        = cos( x[0] ) if x[0] >= 3
	//        = 0           otherwise
	// G_1(x) = cos( x[1] ) if x[0] <  3
	//        = 0           otherwise
	vector<AD<double> > ax(2), ay(2);
	ax[0]=1; ax[1]=2;
	Independent(ax);
	ay = F.Jacobian(ax);
	ADFun<double> G(ax, ay);

	// causes an assertion
	G.optimize();
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
