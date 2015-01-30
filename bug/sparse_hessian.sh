#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
cat << EOF
Changing CPPAD_EXTRA_RUN_BEFORE_TIMING dramatically changes timing results.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>
namespace {
	using CppAD::vector;
	typedef vector< std::set<size_t> >  SetVector;

	void choose_row_col(
		size_t          n   ,
		vector<size_t>& row ,
		vector<size_t>& col )
	{	size_t i, j, k, ell;
		size_t max_per_row = 5;

		// generate the row and column indices
		row.resize(0);
		col.resize(0);
		for(i = 0; i < n; i++)
		{	// generate max_per_row random column indices between 0 and i
			for(j = i; j < i + max_per_row; j++)
			{	if( j < n )
				{	row.push_back(i);
					col.push_back(j);
				}
			}
		}
	}
	void calc_sparsity(SetVector& set_sparsity, CppAD::ADFun<double>& f)
	{	size_t n = f.Domain();
		size_t m = f.Range();
		CPPAD_ASSERT_UNKNOWN( m == 1 );
		SetVector r_set(n);
		for(size_t i = 0; i < n; i++)
			r_set[i].insert(i);
		f.ForSparseJac(n, r_set);
		//
		SetVector s_set(m);
		s_set[0].insert(0);
		//
		set_sparsity = f.RevSparseHes(n, s_set);
	}
	bool link_sparse_hessian(
		size_t                           size     ,
		size_t                           repeat   ,
		const CppAD::vector<size_t>&     row      ,
		const CppAD::vector<size_t>&     col      ,
		CppAD::vector<double>&           x        )
	{
		// -----------------------------------------------------
		// setup
		typedef CppAD::AD<double>           ADScalar;
		typedef vector<ADScalar>            ADVector;

		size_t j;
		size_t order = 0;         // derivative order corresponding to function
		size_t m = 1;             // number of dependent variables
		size_t n = size;          // number of independent variables
		ADVector   a_x(n);        // AD domain space vector
		ADVector   a_y(m);        // AD range space vector

		// declare sparsity pattern
		SetVector  set_sparsity(n);

		// ------------------------------------------------------
		while(repeat--)
		{	// choose a value for x
			for(j = 0; j < n; j++)
				a_x[j] = 0.0;

			// declare independent variables
			Independent(a_x);

			// AD computation of f(x)
			CppAD::sparse_hes_fun<ADScalar>(n, a_x, row, col, order, a_y);

			// create function object f : X -> Y
			CppAD::ADFun<double> f;
			f.Dependent(a_x, a_y);

		}
		return true;
	}
	void speed_sparse_hessian(size_t size, size_t repeat)
	{	CPPAD_ASSERT_UNKNOWN( size > 0 );
		static size_t previous_size = 0;
		vector<size_t> row, col;

		size_t n = size;
		vector<double> x(n);
		if( size != previous_size )
		{	choose_row_col(n, row, col);
			previous_size = size;
		}
		size_t K = row.size();

		// note that cppad/sparse_hessian.cpp assumes that x.size() == size
		link_sparse_hessian(n, repeat, row, col, x);
		return;
	}

}
int main(void)
{
	size_t size  = 3000;
	double time_min = 1.0;
	double time = CppAD::time_test(speed_sparse_hessian, time_min, size);
	std::cout << "time = " << time << std::endl;
	return 0;
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
for flag in 1 0
do
	echo_eval \
		sed -e "'s|\\(CPPAD_EXTRA_RUN_BEFORE_TIMING\\) *[01]|\\1 $flag|'" \
		-i ../../cppad/time_test.hpp
	echo_eval  g++ -I../.. --std=c++11 -DNDEBUG -pg -g $name.cpp -o $name
	echo_eval ./$name
done
