// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// system include files used for I/O
# include <iostream>

// memory leak checker
# include <cppad/utility/thread_alloc.hpp>

// prototype external compiled tests (this line expected by bin/new_test.sh)
extern bool abs(void);
extern bool acos(void);
extern bool acosh(void);
extern bool adfun_copy(void);
extern bool Add(void);
extern bool AddEq(void);
extern bool AddZero(void);
extern bool alloc_openmp(void);
extern bool asin(void);
extern bool asinh(void);
extern bool assign(void);
extern bool atan(void);
extern bool atanh(void);
extern bool atan2(void);
extern bool azmul(void);
extern bool base_adolc(void);
extern bool base_alloc_test(void);
extern bool check_simple_vector(void);
extern bool checkpoint(void);
extern bool Compare(void);
extern bool compare_change(void);
extern bool CondExp(void);
extern bool CondExpAD(void);
extern bool cond_exp_rev(void);
extern bool copy(void);
extern bool Cos(void);
extern bool Cosh(void);
extern bool cppad_eigen(void);
extern bool dbl_epsilon(void);
extern bool Div(void);
extern bool DivEq(void);
extern bool DivZeroOne(void);
extern bool erf(void);
extern bool Exp(void);
extern bool expm1(void);
extern bool ForHess(void);
extern bool for_sparse_jac(void);
extern bool Forward(void);
extern bool forward_dir(void);
extern bool forward_order(void);
extern bool FromBase(void);
extern bool FunCheck(void);
extern bool ipopt_solve(void);
extern bool jacobian(void);
extern bool log(void);
extern bool log10(void);
extern bool log1p(void);
extern bool Mul(void);
extern bool mul_level(void);
extern bool mul_cond_rev(void);
extern bool mul_cskip(void);
extern bool MulEq(void);
extern bool mul_zdouble(void);
extern bool MulZeroOne(void);
extern bool NearEqualExt(void);
extern bool Neg(void);
extern bool num_limits(void);
extern bool ode_err_control(void);
extern bool old_mat_mul(void);
extern bool old_reciprocal(void);
extern bool old_tan(void);
extern bool old_usead_1(void);
extern bool old_usead_2(void);
extern bool omp_alloc(void);
extern bool optimize(void);
extern bool parameter(void);
extern bool Poly(void);
extern bool Pow(void);
extern bool PowInt(void);
extern bool print_for(void);
extern bool reverse(void);
extern bool rev_sparse_hes(void);
extern bool rev_sparse_jac(void);
extern bool RevTwo(void);
extern bool RombergOne(void);
extern bool Rosen34(void);
extern bool Runge45(void);
extern bool SimpleVector(void);
extern bool Sin(void);
extern bool SinCos(void);
extern bool Sinh(void);
extern bool sparse_hessian(void);
extern bool sparse_jacobian(void);
extern bool sparse_vec_ad(void);
extern bool Sqrt(void);
extern bool std_math(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool SubZero(void);
extern bool tan(void);
extern bool to_string(void);
extern bool test_vector(void);
extern bool track_new_del(void);
extern bool Value(void);
extern bool VecAD(void);
extern bool VecADPar(void);
extern bool VecUnary(void);
extern bool zdouble(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), std::string name)
	{	bool ok               = true;
		std::streamsize width =  20;
		std::cout.width( width );
		std::cout.setf( std::ios_base::left );
		std::cout << name;
		//
		ok &= name.size() < size_t(width);
		ok &= TestOk();
		if( ok )
		{	std::cout << "OK" << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error" << std::endl;
			Run_error_count++;
		}
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	// This line is used by test_one.sh

	// run external compiled tests (this line expected by bin/new_test.sh)
	ok &= Run( abs,             "abs"            );
	ok &= Run( acos,            "acos"           );
	ok &= Run( acosh,           "acosh"          );
	ok &= Run( adfun_copy,      "adfun_copy"     );
	ok &= Run( Add,             "Add"            );
	ok &= Run( AddEq,           "AddEq"          );
	ok &= Run( AddZero,         "AddZero"        );
	ok &= Run( asin,            "asin"           );
	ok &= Run( asinh,           "asinh"          );
	ok &= Run( assign,          "assign"         );
	ok &= Run( atan,            "atan"           );
	ok &= Run( atanh,           "atanh"          );
	ok &= Run( atan2,           "atan2"          );
	ok &= Run( azmul,           "azmul"          );
	ok &= Run( check_simple_vector, "check_simple_vector" );
	ok &= Run( checkpoint,      "checkpoint"     );
	ok &= Run( Compare,         "Compare"        );
	ok &= Run( compare_change,  "compare_change" );
	ok &= Run( CondExp,         "CondExp"        );
	ok &= Run( CondExpAD,       "CondExpAD"      );
	ok &= Run( cond_exp_rev,    "cond_exp_rev"   );
	ok &= Run( copy,            "copy"           );
	ok &= Run( Cos,             "Cos"            );
	ok &= Run( Cosh,            "Cosh"           );
	ok &= Run( dbl_epsilon,     "dbl_epsilon"    );
	ok &= Run( Div,             "Div"            );
	ok &= Run( DivEq,           "DivEq"          );
	ok &= Run( DivZeroOne,      "DivZeroOne"     );
	ok &= Run( erf,             "erf"            );
	ok &= Run( Exp,             "Exp"            );
	ok &= Run( expm1,           "expm1"          );
	ok &= Run( ForHess,         "ForHess"        );
	ok &= Run( for_sparse_jac,  "for_sparse_jac" );
	ok &= Run( Forward,         "Forward"        );
	ok &= Run( forward_dir,     "forward_dir"    );
	ok &= Run( forward_order,   "forward_order"  );
	ok &= Run( FromBase,        "FromBase"       );
	ok &= Run( FunCheck,        "FunCheck"       );
	ok &= Run( jacobian,        "jacobian"       );
	ok &= Run( log,             "log"            );
	ok &= Run( log10,           "log10"          );
	ok &= Run( log1p,           "log1p"          );
	ok &= Run( Mul,             "Mul"            );
	ok &= Run( mul_level,       "mul_level"      );
	ok &= Run( mul_cond_rev,    "mul_cond_rev"   );
	ok &= Run( mul_cskip,       "Mul_cskip"      );
	ok &= Run( MulEq,           "MulEq"          );
	ok &= Run( mul_zdouble,     "mul_zdouble"    );
	ok &= Run( MulZeroOne,      "MulZeroOne"     );
	ok &= Run( NearEqualExt,    "NearEqualExt"   );
	ok &= Run( Neg,             "Neg"            );
	ok &= Run( num_limits,      "num_limits"     );
	ok &= Run( ode_err_control, "ode_err_control");
	ok &= Run( old_mat_mul,     "old_mat_mul"    );
	ok &= Run( old_reciprocal,  "old_reciprocal" );
	ok &= Run( old_tan,         "old_tan"        );
	ok &= Run( old_usead_1,     "old_usead_1"    );
	ok &= Run( old_usead_2,     "old_usead_2"    );
	ok &= Run( omp_alloc,       "omp_alloc"      );
	ok &= Run( optimize,        "optimize"       );
	ok &= Run( parameter,       "parameter"      );
	ok &= Run( Poly,            "Poly"           );
	ok &= Run( Pow,             "Pow"            );
	ok &= Run( PowInt,          "PowInt"         );
	ok &= Run( print_for,       "print_for"      );
	ok &= Run( reverse,         "reverse"        );
	ok &= Run( rev_sparse_hes,  "rev_sparse_hes" );
	ok &= Run( rev_sparse_jac,  "rev_sparse_jac" );
	ok &= Run( RevTwo,          "RevTwo"         );
	ok &= Run( RombergOne,      "RombergOne"     );
	ok &= Run( Rosen34,         "Rosen34"        );
	ok &= Run( Runge45,         "Runge45"        );
	ok &= Run( SimpleVector,    "SimpleVector"   );
	ok &= Run( Sin,             "Sin"            );
	ok &= Run( SinCos,          "SinCos"         );
	ok &= Run( Sinh,            "Sinh"           );
	ok &= Run( sparse_hessian,  "sparse_hessian" );
	ok &= Run( sparse_jacobian, "sparse_jacobian");
	ok &= Run( sparse_vec_ad,   "sparse_vec_ad"  );
	ok &= Run( Sqrt,            "Sqrt"           );
	ok &= Run( std_math,        "std_math"       );
	ok &= Run( Sub,             "Sub"            );
	ok &= Run( SubEq,           "SubEq"          );
	ok &= Run( SubZero,         "SubZero"        );
	ok &= Run( tan,             "tan"            );
	ok &= Run( to_string,       "to_string"      );
	ok &= Run( track_new_del,   "track_new_del"  );
	ok &= Run( Value,           "Value"          );
	ok &= Run( VecAD,           "VecAD"          );
	ok &= Run( VecADPar,        "VecADPar"       );
	ok &= Run( VecUnary,        "VecUnary"       );
	ok &= Run( zdouble,         "zdouble"        );
# ifdef CPPAD_ADOLC_TEST
	ok &= Run( base_adolc,      "base_adolc"     );
# endif
# ifdef CPPAD_IPOPT_TEST
	ok &= Run( ipopt_solve,     "ipopt_solve"    );
# endif
# ifdef CPPAD_OPENMP_TEST
	ok &= Run( alloc_openmp,    "alloc_openmp"   );
# endif
# ifdef CPPAD_EIGEN_TEST
	ok &= Run( cppad_eigen, "cppad_eigen" );
# endif
# if ! CPPAD_EIGENVECTOR
	ok &= Run( test_vector, "test_vector" );
# endif

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
	// Run base_require after memory leak check because base_alloc.hpp uses
	// thread_alloc to allocate memory for static copies of nan.
	ok &= Run( base_alloc_test, "base_alloc"     );
	// convert int(size_t) to avoid warning on _MSC_VER systems
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );

}
// END PROGRAM
