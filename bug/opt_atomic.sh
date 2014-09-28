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
Assert during optimization of an atomic function.
EOF
cat << EOF > bug.$$
// Another bug (reported by Kasper Kristensen) optimizing combination of 
// conditional expressions and atomic functions.
# include <cppad/cppad.hpp>

using CppAD::vector;
using CppAD::AD;
using CppAD::ADFun;

class atomic_reciprocal : public CppAD::atomic_base<double> {
public:
	atomic_reciprocal(const std::string& name) :
	CppAD::atomic_base<double>(name)
	{ }
private:
	virtual bool forward(
		size_t                    p ,
		size_t                    q ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<double>&    tx ,
		      vector<double>&    ty
	)
	{	size_t n = tx.size() / (q + 1);
		size_t m = ty.size() / (q + 1);
		assert( n == 1 );
		assert( m == 1 );
		assert( p <= q );

		// return flag
		bool ok = true;

		// check for defining variable information
		// This case must always be implemented
		if( vx.size() > 0 )
			vy[0] = vx[0];

		// Order zero forward mode.
		// This case must always be implemented
		// y^0 = f( x^0 ) = 1 / x^0
		double f = 1. / tx[0];
		if( p <= 0 )
			ty[0] = f;
		if( q <= 0 )
			return ok;
		//
		assert( vx.size() == 0 );
		ok = false;
		return ok;
	}
	// reverse Jacobian set sparsity routine called by CppAD
	virtual bool rev_sparse_jac(
		size_t                                p  ,
		const vector< std::set<size_t> >&     rt ,
		      vector< std::set<size_t> >&     st )
	{	// This function needed if using RevSparseJac or optimize
		// with afun.option( CppAD::atomic_base<double>::set_sparsity_enum )
		size_t n = st.size();
		size_t m = rt.size();
		assert( n == 1 );
		assert( m == 1 );

		// sparsity for S(x)^T = f'(x)^T * R^T is same as sparsity for R^T
		st[0] = rt[0];

		return true; 
	}
};


int main()
{	bool ok = true;
	atomic_reciprocal afun("reciprocal");

	vector< AD<double> > av(1), aw(1), ax(2), ay(1);
	AD<double> zero = 0.0;  
	ax[0] = 1.0;
	ax[1] = 1.0;
	Independent(ax);
	av[0] = ax[0] + ax[1];
	afun(av, aw);
	ay[0] = CondExpGt(aw[0], zero, zero, aw[0]);
	ADFun<double> f;
	f.Dependent(ax, ay);

	// run case that skips the second call to afun
	// (but not for order zero)
	vector<double> x(2), y_before(1), y_after(1);
	x[0]      = 1.0;
	x[1]      = 1.0;
	y_before  = f.Forward(0, x);
	f.optimize();
	y_after   = f.Forward(0, x);
	
	ok &= y_before[0] == y_after[0];

	return( ! ok );
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
./$name
