#! /bin/bash -e
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
Bug in CppAD optimizer.
Test passes when nz = 9998 and fails when nz = 9999
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
	// length of the data vector z
	size_t nz = 9999;
	//
	// factor for last term
	double factor = 1e+5;
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
	AD<double> asum = 0.0;
	for(size_t i = 0; i < nz; i++)
	{	AD<double> aterm = z[i] - ax[0];
		if( i == nz - 1 )
			asum += factor * aterm;
		else
			asum += aterm / factor;
	}
	ay[0] = asum;
	CppAD::ADFun<double> f(ax, ay);
	//
	// value of x where we are computing derivative
	vector<double> x(1), y_before(1), y_after(1);
	x[0]     = .1;
	y_before = f.Forward(0, x);
	f.optimize();
	y_after  = f.Forward(0, x);
	//
	ok &= CppAD::NearEqual(y_before[0], y_after[0], eps99, eps99);
	//
	if( ok )
		return 0;
	return 1;
}
EOF
# -----------------------------------------------------------------------------
if [ ! -e ../cppad/configure.hpp ]
then
	echo
	echo 'Cannot find the file cppad/configure.hpp in directory ..'
	echo 'Must change into .. directory and run bin/run_cmake.sh'
	rm bug.$$
	exit 1
fi
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
