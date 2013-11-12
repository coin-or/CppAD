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
# Missing error message when result of an atomic fucntion is used as 
# argument to another atomic function (and the tape is optimized)
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>
# define DEFINE_REVERSE_SPARSE_JACOBIAN 0 // change to 1 and program will work

namespace {
	using CppAD::vector;
	using CppAD::AD;

	class atomic_user : public CppAD::atomic_base<double> {
	public:
		atomic_user(const std::string& name) :
			CppAD::atomic_base<double>(name) {}
	private:
		virtual bool forward(
			size_t                     q , // lowest order Taylor coeff
			size_t                     p , // highest order Taylor coeff
			const vector<bool>&       vx , // which x elements are variables
			vector<bool>&             vy , // which y elements are variables
			const vector<double>&     tx , // input vector of x
			vector<double>&           ty ) // output vector of y
		{
			if( vx.size() > 0 )
				vy[0] = vx[0];
	
			if ((q <= 0) && (p == 0))
			{	ty[0] = tx[0];      
				return true;
			}
			return false;
		}

# if DEFINE_REVERSE_SPARSE_JACOBIAN
		// reverse Jacobian set sparsity routine called by CppAD
		// This function needed because using optimize.
		virtual bool rev_sparse_jac(
			size_t                                q  ,
			const vector< std::set<size_t> >&     rt ,
			      vector< std::set<size_t> >&     st )
		{	// This function needed if using RevSparseJac (or optimize)
			size_t n = st.size();
			size_t m = rt.size();
			assert( n == 1 );
			assert( m == 1 );
	
			// sparsity for S(x)^T = f'(x)^T * R^T is same as for R^T
			st[0] = rt[0];
	
			return true; 
		}
# endif

	}; // end class
}
int main()  {
	atomic_user func("atomic_user");
	size_t n = 1;
	vector< AD<double> > ax(n), ay(1), au(1), av(1);
	vector<double> x(n), y_before(1), y_after(1);

	ax[0] = x[0] = 3.0;
	CppAD::Independent(ax);
	//
	au[0] = ax[0];
	func(au, av);
	//
	au[0] = av[0];
	func(au, av);
	//
	ay[0] = av[0];

	CppAD::ADFun<double> f;
	f.Dependent(ax,ay);

	y_before = f.Forward(0,x);
	f.optimize();
	y_after = f.Forward(0,x);

	if( fabs( y_before[0] - y_after[0] ) <= 1e-5 )
		std::cout << "opt_atomic: OK" << std::endl;
	else	std::cout << "opt_atomic: Error" << std::endl;
	return(0);
}
EOF
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
./$name
#
echo "rm $name $name.cpp"
rm $name $name.cpp
