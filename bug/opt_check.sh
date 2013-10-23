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
# Optimizing a check point function does not handel conditional expressions
#	CondExpRel(left, right, if_true, if_false)
# properly because the sparsity pattern indicates that the atomic 
# function does not depend on the value of left and right. 
# The simpliest way to fix this is to change the sparsity pattern.
# A better way to fix this is to have an optional argument to the sparsity
# pattern so that the more efficient pattern can be obtained when needed.
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>
void g_algo( 
	const CppAD::vector< CppAD::AD<double> >& ax ,
	      CppAD::vector< CppAD::AD<double> >& ay )
{	ay[0] = CondExpGt(ax[0], ax[1], ax[2], ax[3]); }

int main(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;

	// Create a checkpoint version of the function g
	vector< AD<double> > au(4), av(1);
	for(size_t i = 0; i < 4; i++)
		au[i] = AD<double>(i);
	CppAD::checkpoint<double> g_check("g_check", g_algo, au, av);

	// independent variable vector 
	vector< AD<double> > ax(2), ay(1);
	ax[0] = 1.;
	ax[1] = 1.;
	Independent(ax);

	// call atomic function that does not get used
	for(size_t i = 0; i < 4; i++) 
		au[i] = ax[0] + AD<double>(i + 1) * ax[1];
	g_check(au, ay);

	// create function object f : ax -> ay
	CppAD::ADFun<double> f(ax, ay);

	// now optimize the operation sequence
	f.optimize();

	// check result where true case is used; i.e., au[0] > au[1]
	vector<double> x(2), y(1);
	x[0] = 1.;
	x[1] = -1;
	y    = f.Forward(0, x);
	ok  &= y[0] == x[0] + double(3) * x[1];
	

	// check result where false case is used; i.e., au[0] <= au[1]
	x[0] = 1.;
	x[1] = 1;
	y    = f.Forward(0, x);
	ok  &= y[0] == x[0] + double(4) * x[1];
	
	if( ! ok )
		return 1;
	return 0;
}
EOF
#
echo_eval g++ -g -I../.. -std=c++11 $name.cpp -o $name
if ! ./$name
then
	echo "./$name Error"
	exit 1
fi
echo "./$name OK"
exit 0

