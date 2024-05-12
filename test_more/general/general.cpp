// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// memory leak checker
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_1
extern bool Add(void);
extern bool AddEq(void);
extern bool AddZero(void);
extern bool Compare(void);
extern bool CondExp(void);
extern bool CondExpAD(void);
extern bool Cos(void);
extern bool Cosh(void);
extern bool Div(void);
extern bool DivEq(void);
extern bool DivZeroOne(void);
extern bool Exp(void);
extern bool ForHess(void);
extern bool Forward(void);
extern bool FromBase(void);
extern bool FunCheck(void);
extern bool Mul(void);
extern bool MulEq(void);
extern bool MulZeroOne(void);
extern bool Neg(void);
extern bool Poly(void);
extern bool Pow(void);
extern bool PowInt(void);
extern bool RevTwo(void);
extern bool RombergOne(void);
extern bool Rosen34(void);
extern bool Runge45(void);
extern bool SimpleVector(void);
extern bool Sin(void);
extern bool SinCos(void);
extern bool Sinh(void);
extern bool Sqrt(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool SubZero(void);
extern bool Value(void);
extern bool VecAD(void);
extern bool VecADPar(void);
extern bool VecUnary(void);
extern bool abs_normal(void);
extern bool acos(void);
extern bool acosh(void);
extern bool adfun(void);
extern bool alloc_openmp(void);
extern bool asin(void);
extern bool asinh(void);
extern bool assign(void);
extern bool atan(void);
extern bool atan2(void);
extern bool atanh(void);
extern bool atomic_three(void);
extern bool azmul(void);
extern bool base2ad(void);
extern bool base_adolc(void);
extern bool base_alloc_test(void);
extern bool base_complex(void);
extern bool bool_sparsity(void);
extern bool check_simple_vector(void);
extern bool chkpoint_one(void);
extern bool chkpoint_two(void);
extern bool compare_change(void);
extern bool cond_exp_rev(void);
extern bool copy(void);
extern bool cpp_graph(void);
extern bool cppad_eigen(void);
extern bool cppad_vector(void);
extern bool dbl_epsilon(void);
extern bool dependency(void);
extern bool eigen_mat_inv(void);
extern bool erf(void);
extern bool expm1(void);
extern bool fabs(void);
extern bool for_sparse_hes(void);
extern bool for_sparse_jac(void);
extern bool forward_dir(void);
extern bool forward_order(void);
extern bool hes_sparsity(void);
extern bool ipopt_solve(void);
extern bool jacobian(void);
extern bool json_graph(void);
extern bool log(void);
extern bool log10(void);
extern bool log1p(void);
extern bool mul_cond_rev(void);
extern bool mul_cskip(void);
extern bool mul_level(void);
extern bool mul_zdouble(void);
extern bool near_equal_ext(void);
extern bool new_dynamic(void);
extern bool num_limits(void);
extern bool ode_err_control(void);
extern bool optimize(void);
extern bool parameter(void);
extern bool print_for(void);
extern bool rev_sparse_jac(void);
extern bool reverse(void);
extern bool sparse_hessian(void);
extern bool sparse_jac_work(void);
extern bool sparse_jacobian(void);
extern bool sparse_sub_hes(void);
extern bool sparse_vec_ad(void);
extern bool std_math(void);
extern bool subgraph_1(void);
extern bool subgraph_2(void);
extern bool subgraph_hes2jac(void);
extern bool tan(void);
extern bool to_csrc(void);
extern bool to_string(void);
// END_SORT_THIS_LINE_MINUS_1

// tests in local subdirectory
extern bool is_pod(void);
extern bool json_lexer(void);
extern bool json_parser(void);
extern bool temp_file(void);
extern bool vector_set(void);

// main program that runs all the tests
int main(void)
{  std::string group = "test_more/general";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // BEGIN_SORT_THIS_LINE_PLUS_1
   Run( Add,             "Add"            );
   Run( AddEq,           "AddEq"          );
   Run( AddZero,         "AddZero"        );
   Run( Compare,         "Compare"        );
   Run( CondExp,         "CondExp"        );
   Run( CondExpAD,       "CondExpAD"      );
   Run( Cos,             "Cos"            );
   Run( Cosh,            "Cosh"           );
   Run( Div,             "Div"            );
   Run( DivEq,           "DivEq"          );
   Run( DivZeroOne,      "DivZeroOne"     );
   Run( Exp,             "Exp"            );
   Run( ForHess,         "ForHess"        );
   Run( Forward,         "Forward"        );
   Run( FromBase,        "FromBase"       );
   Run( FunCheck,        "FunCheck"       );
   Run( Mul,             "Mul"            );
   Run( MulEq,           "MulEq"          );
   Run( MulZeroOne,      "MulZeroOne"     );
   Run( Neg,             "Neg"            );
   Run( Poly,            "Poly"           );
   Run( Pow,             "Pow"            );
   Run( PowInt,          "PowInt"         );
   Run( RevTwo,          "RevTwo"         );
   Run( RombergOne,      "RombergOne"     );
   Run( Rosen34,         "Rosen34"        );
   Run( Runge45,         "Runge45"        );
   Run( SimpleVector,    "SimpleVector"   );
   Run( Sin,             "Sin"            );
   Run( SinCos,          "SinCos"         );
   Run( Sinh,            "Sinh"           );
   Run( Sqrt,            "Sqrt"           );
   Run( Sub,             "Sub"            );
   Run( SubEq,           "SubEq"          );
   Run( SubZero,         "SubZero"        );
   Run( Value,           "Value"          );
   Run( VecAD,           "VecAD"          );
   Run( VecADPar,        "VecADPar"       );
   Run( VecUnary,        "VecUnary"       );
   Run( abs_normal,      "abs_normal"     );
   Run( acos,            "acos"           );
   Run( acosh,           "acosh"          );
   Run( adfun,           "adfun"          );
   Run( asin,            "asin"           );
   Run( asinh,           "asinh"          );
   Run( assign,          "assign"         );
   Run( atan,            "atan"           );
   Run( atan2,           "atan2"          );
   Run( atanh,           "atanh"          );
   Run( atomic_three,    "atomic_three"   );
   Run( azmul,           "azmul"          );
   Run( base2ad,         "base2ad"        );
   Run( base_complex,    "base_complex"   );
   Run( bool_sparsity,   "bool_sparsity"  );
   Run( check_simple_vector, "check_simple_vector" );
   Run( chkpoint_one,    "chkpoint_one"   );
   Run( chkpoint_two,    "chkpoint_two"   );
   Run( compare_change,  "compare_change" );
   Run( cond_exp_rev,    "cond_exp_rev"   );
   Run( copy,            "copy"           );
   Run( cpp_graph,       "cpp_graph"      );
   Run( cppad_vector,    "cppad_vector"   );
   Run( dbl_epsilon,     "dbl_epsilon"    );
   Run( dependency,      "dependency"     );
   Run( erf,             "erf"            );
   Run( expm1,           "expm1"          );
   Run( fabs,            "fabs"           );
   Run( for_sparse_hes,  "for_sparse_hes" );
   Run( for_sparse_jac,  "for_sparse_jac" );
   Run( forward_dir,     "forward_dir"    );
   Run( forward_order,   "forward_order"  );
   Run( hes_sparsity,    "hes_sparsity"   );
   Run( jacobian,        "jacobian"       );
   Run( json_graph,      "json_graph"     );
   Run( log,             "log"            );
   Run( log10,           "log10"          );
   Run( log1p,           "log1p"          );
   Run( mul_cond_rev,    "mul_cond_rev"   );
   Run( mul_cskip,       "Mul_cskip"      );
   Run( mul_level,       "mul_level"      );
   Run( mul_zdouble,     "mul_zdouble"    );
   Run( near_equal_ext,  "near_equal_ext" );
   Run( new_dynamic,     "new_dynamic"    );
   Run( num_limits,      "num_limits"     );
   Run( ode_err_control, "ode_err_control");
   Run( optimize,        "optimize"       );
   Run( parameter,       "parameter"      );
   Run( print_for,       "print_for"      );
   Run( rev_sparse_jac,  "rev_sparse_jac" );
   Run( reverse,         "reverse"        );
   Run( sparse_hessian,  "sparse_hessian" );
   Run( sparse_jac_work, "sparse_jac_work");
   Run( sparse_jacobian, "sparse_jacobian");
   Run( sparse_sub_hes,  "sparse_sub_hes" );
   Run( sparse_vec_ad,   "sparse_vec_ad"  );
   Run( std_math,        "std_math"       );
   Run( subgraph_1,      "subgraph_1"     );
   Run( subgraph_2,      "subgraph_2"     );
   Run( subgraph_hes2jac, "subgraph_hes2jac" );
   Run( tan,             "tan"            );
   Run( to_string,       "to_string"      );
   // END_SORT_THIS_LINE_MINUS_1
# if CPPAD_C_COMPILER_GNU_FLAGS || CPPAD_C_COMPILER_MSVC_FLAGS
   Run( to_csrc,         "to_csrc"        );
# endif
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
   // local sub-directory
   Run( is_pod,         "is_pod"          );
   Run( json_lexer,     "json_lexer"      );
   Run( json_parser,    "json_parser"     );
   Run( temp_file,       "temp_file"      );
   Run( vector_set,      "vector_set"     );
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
