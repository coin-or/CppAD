#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
TMB is having trouble using subgraphs to compute the derivative of w.r.t mu
for the following:
	# include <TMB.hpp>
	template<class Type>
	Type objective_function<Type>::operator() ()
	{
	  DATA_VECTOR(z);
	  PARAMETER(x);
	  Type f = ( (z - x) * (z - x) ) .sum();
	  return f;
	}
where z = seq(-1, 1, length=1e5) .
This is a test of the same computation entirely in CppAD.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
int main(void)
{	bool ok = true;
	using std::cout;
	using CppAD::AD;
	using CppAD::vector;
	//
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
	//
	// length of the data vector z (1e5)
	size_t nz = 100000;
	//
	// z starts at -1.0 and ends at 1.0
	vector<double> z(nz);
	for(size_t i = 0; i < nz; i++)
		z[i] = -1.0 + i * 2.0 / double(nz - 1);
	//
	// f(x) = sum from i=0 to nz-1 of (x - z[i])^2
	vector< AD<double> > ax(1), ay(1);
	ax[0] = 0.0;
	CppAD::Independent(ax);
	AD<double> sum = 0.0;
	for(size_t i = 0; i < nz; i++)
		sum += (z[i] - ax[0]) * (z[i] - ax[0]);
	ay[0] = sum;
	CppAD::ADFun<double> f(ax, ay);
	//
	// value of x where we are computing derivative
	vector<double> x(1);
	x[0] = .1;
	f.Forward(0, x);
	//
	// compute gradient of f using subgraphs
	vector<bool> select_domain(1);
	select_domain[0] = true;
	f.subgraph_reverse(select_domain);
	vector<size_t> col;
	vector<double> dw;
	size_t q  = 1;
	size_t ell = 0;
	f.subgraph_reverse(q, ell, col, dw);
	//
	// compute check
	double check = 0.0;
	for(size_t i = 0; i < nz; i++)
		check -= 2.0 * (z[i] - x[0]);
	//
	ok &= col.size() == 1;
	ok &= col[0] == 0;
	ok &= CppAD::NearEqual(dw[0], check, eps99, eps99);
	//
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
if ! ./$name
then
	echo
	echo "$name.sh: Error"
	exit 1
fi
echo
echo "$name.sh: OK"
exit 0
