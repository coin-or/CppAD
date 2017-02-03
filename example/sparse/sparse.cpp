/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// external compiled tests
extern bool sub_sparse_hes(void);
extern bool sparsity_sub(void);
extern bool sparse_sub_hes(void);
extern bool sparse_jacobian(void);
extern bool sparse_hessian(void);
extern bool rev_hes_sparsity(void);
extern bool rev_jac_sparsity(void);
extern bool RevSparseJac(void);
extern bool rev_sparse_hes(void);
extern bool for_jac_sparsity(void);
extern bool ForSparseJac(void);
extern bool for_sparse_hes(void);
extern bool dependency(void);
extern bool conj_grad(void);
extern bool colpack_jac(void);
extern bool colpack_hes(void);
extern bool bool_sparsity(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		ok &= TestOk();
		if( ok )
		{	std::cout << "OK:    " << "sparse: " << name << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error: " << "sparse: " << name << std::endl;
			Run_error_count++;
		}
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;

	// This line is used by test_one.sh

	// external compiled tests
	ok &= Run( sub_sparse_hes,            "sub_sparse_hes" );
	ok &= Run( sparsity_sub,              "sparsity_sub" );
	ok &= Run( sparse_sub_hes,            "sparse_sub_hes" );
	ok &= Run( sparse_jacobian,           "sparse_jacobian" );
	ok &= Run( sparse_hessian,            "sparse_hessian" );
	ok &= Run( rev_hes_sparsity,          "rev_hes_sparsity" );
	ok &= Run( rev_jac_sparsity,          "rev_jac_sparsity" );
	ok &= Run( RevSparseJac,              "RevSparseJac" );
	ok &= Run( rev_sparse_hes,            "rev_sparse_hes" );
	ok &= Run( for_jac_sparsity,          "for_jac_sparsity" );
	ok &= Run( ForSparseJac,              "ForSparseJac" );
	ok &= Run( for_sparse_hes,            "for_sparse_hes" );
	ok &= Run( dependency,                "dependency" );
	ok &= Run( conj_grad,                 "conj_grad" );
	ok &= Run( colpack_jac,               "colpack_jac" );
	ok &= Run( colpack_hes,               "colpack_hes" );
	ok &= Run( bool_sparsity,             "bool_sparsity" );


	// check for errors
	using std::cout;
	using std::endl;
	assert( ok || (Run_error_count > 0) );
	if( CppAD::thread_alloc::free_all() )
	{	Run_ok_count++;
		cout << "OK:    " << "No memory leak detected" << endl;
	}
	else
	{	ok = false;
		Run_error_count++;
		cout << "Error: " << "memory leak detected" << endl;
	}
	// convert int(size_t) to avoid warning on _MSC_VER systems
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
