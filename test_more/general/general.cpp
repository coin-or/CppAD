/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// memory leak checker
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// prototype external compiled tests (this line expected by bin/new_test.sh)
extern bool fabs(void);
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
extern bool atomic_sparsity(void);
extern bool azmul(void);
extern bool base_adolc(void);
extern bool base_alloc_test(void);
extern bool bool_sparsity(void);
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
extern bool dependency(void);
extern bool Div(void);
extern bool DivEq(void);
extern bool DivZeroOne(void);
extern bool eigen_mat_inv(void);
extern bool erf(void);
extern bool Exp(void);
extern bool expm1(void);
extern bool ForHess(void);
extern bool for_sparse_hes(void);
extern bool for_sparse_jac(void);
extern bool Forward(void);
extern bool forward_dir(void);
extern bool forward_order(void);
extern bool FromBase(void);
extern bool FunCheck(void);
extern bool hes_sparsity(void);
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
extern bool optimize(void);
extern bool parameter(void);
extern bool Poly(void);
extern bool Pow(void);
extern bool PowInt(void);
extern bool print_for(void);
extern bool reverse(void);
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
extern bool sparse_sub_hes(void);
extern bool sparse_vec_ad(void);
extern bool Sqrt(void);
extern bool std_math(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool subgraph(void);
extern bool SubZero(void);
extern bool tan(void);
extern bool to_string(void);
extern bool test_vector(void);
extern bool Value(void);
extern bool VecAD(void);
extern bool VecADPar(void);
extern bool VecUnary(void);

// tests in local subdirectory
extern bool vector_set(void);

// main program that runs all the tests
int main(void)
{	std::string group = "test_more/general";
	size_t      width = 20;
	CppAD::test_boolofvoid Run(group, width);

	// This line is used by test_one.sh

	// run external compiled tests (this line expected by bin/new_test.sh)
	Run( fabs,            "fabs"           );
	Run( acos,            "acos"           );
	Run( acosh,           "acosh"          );
	Run( adfun_copy,      "adfun_copy"     );
	Run( Add,             "Add"            );
	Run( AddEq,           "AddEq"          );
	Run( AddZero,         "AddZero"        );
	Run( asin,            "asin"           );
	Run( asinh,           "asinh"          );
	Run( assign,          "assign"         );
	Run( atan,            "atan"           );
	Run( atanh,           "atanh"          );
	Run( atan2,           "atan2"          );
	Run( atomic_sparsity, "atomic_sparsity");
	Run( azmul,           "azmul"          );
	Run( bool_sparsity,   "bool_sparsity"  );
	Run( check_simple_vector, "check_simple_vector" );
	Run( checkpoint,      "checkpoint"     );
	Run( Compare,         "Compare"        );
	Run( compare_change,  "compare_change" );
	Run( CondExp,         "CondExp"        );
	Run( CondExpAD,       "CondExpAD"      );
	Run( cond_exp_rev,    "cond_exp_rev"   );
	Run( copy,            "copy"           );
	Run( Cos,             "Cos"            );
	Run( Cosh,            "Cosh"           );
	Run( dbl_epsilon,     "dbl_epsilon"    );
	Run( dependency,      "dependency"     );
	Run( Div,             "Div"            );
	Run( DivEq,           "DivEq"          );
	Run( DivZeroOne,      "DivZeroOne"     );
	Run( erf,             "erf"            );
	Run( Exp,             "Exp"            );
	Run( expm1,           "expm1"          );
	Run( ForHess,         "ForHess"        );
	Run( for_sparse_hes,  "for_sparse_hes" );
	Run( for_sparse_jac,  "for_sparse_jac" );
	Run( Forward,         "Forward"        );
	Run( forward_dir,     "forward_dir"    );
	Run( forward_order,   "forward_order"  );
	Run( FromBase,        "FromBase"       );
	Run( FunCheck,        "FunCheck"       );
	Run( hes_sparsity,    "hes_sparsity"   );
	Run( jacobian,        "jacobian"       );
	Run( log,             "log"            );
	Run( log10,           "log10"          );
	Run( log1p,           "log1p"          );
	Run( Mul,             "Mul"            );
	Run( mul_level,       "mul_level"      );
	Run( mul_cond_rev,    "mul_cond_rev"   );
	Run( mul_cskip,       "Mul_cskip"      );
	Run( MulEq,           "MulEq"          );
	Run( mul_zdouble,     "mul_zdouble"    );
	Run( MulZeroOne,      "MulZeroOne"     );
	Run( NearEqualExt,    "NearEqualExt"   );
	Run( Neg,             "Neg"            );
	Run( num_limits,      "num_limits"     );
	Run( ode_err_control, "ode_err_control");
	Run( optimize,        "optimize"       );
	Run( parameter,       "parameter"      );
	Run( Poly,            "Poly"           );
	Run( Pow,             "Pow"            );
	Run( PowInt,          "PowInt"         );
	Run( print_for,       "print_for"      );
	Run( reverse,         "reverse"        );
	Run( rev_sparse_jac,  "rev_sparse_jac" );
	Run( RevTwo,          "RevTwo"         );
	Run( RombergOne,      "RombergOne"     );
	Run( Rosen34,         "Rosen34"        );
	Run( Runge45,         "Runge45"        );
	Run( SimpleVector,    "SimpleVector"   );
	Run( Sin,             "Sin"            );
	Run( SinCos,          "SinCos"         );
	Run( Sinh,            "Sinh"           );
	Run( sparse_hessian,  "sparse_hessian" );
	Run( sparse_jacobian, "sparse_jacobian");
	Run( sparse_sub_hes,  "sparse_sub_hes" );
	Run( sparse_vec_ad,   "sparse_vec_ad"  );
	Run( Sqrt,            "Sqrt"           );
	Run( std_math,        "std_math"       );
	Run( Sub,             "Sub"            );
	Run( SubEq,           "SubEq"          );
	Run( subgraph,        "subgraph"       );
	Run( SubZero,         "SubZero"        );
	Run( tan,             "tan"            );
	Run( to_string,       "to_string"      );
	Run( Value,           "Value"          );
	Run( VecAD,           "VecAD"          );
	Run( VecADPar,        "VecADPar"       );
	Run( VecUnary,        "VecUnary"       );
#if CPPAD_HAS_ADOLC
	Run( base_adolc,      "base_adolc"     );
# endif
#if CPPAD_HAS_IPOPT
	Run( ipopt_solve,     "ipopt_solve"    );
# endif
# ifdef CPPAD_OPENMP_TEST
	Run( alloc_openmp,    "alloc_openmp"   );
# endif
# if CPPAD_HAS_EIGEN
	Run( cppad_eigen,     "cppad_eigen"    );
	Run( eigen_mat_inv,   "eigen_mat_inv"  );
# endif
# if ! CPPAD_EIGENVECTOR
	Run( test_vector, "test_vector" );
# endif
	// local sub-directory
	Run( test_vector,      "test_vector"   );
	//
	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	//
	// Run base_alloc after memory leak check because base_alloc.hpp uses
	// thread_alloc to allocate memory for static copies of nan.
	Run( base_alloc_test,  "base_alloc"    );
	//
	// print summary at end
	bool ok = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
// END PROGRAM
