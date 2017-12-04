/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin general.cpp$$
$spell
	Cpp
$$

$section CppAD Examples and Tests$$

$head Running Tests$$
To build this program and run its correctness tests see $cref cmake_check$$.

$code
$srcfile%example/general/general.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// standard string
# include <string>

// memory utility
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// prototype external compiled tests (this line expected by bin/new_test.sh)
extern bool abort_recording(void);
extern bool fabs(void);
extern bool acosh(void);
extern bool acos(void);
extern bool ad_assign(void);
extern bool ad_ctor(void);
extern bool AddEq(void);
extern bool Add(void);
extern bool ad_fun(void);
extern bool ad_in_c(void);
extern bool ad_input(void);
extern bool ad_output(void);
extern bool asinh(void);
extern bool asin(void);
extern bool atan2(void);
extern bool atanh(void);
extern bool atan(void);
extern bool azmul(void);
extern bool base_require(void);
extern bool BenderQuad(void);
extern bool BoolFun(void);
extern bool capacity_order(void);
extern bool change_param(void);
extern bool check_for_nan(void);
extern bool compare_change(void);
extern bool Compare(void);
extern bool complex_poly(void);
extern bool CondExp(void);
extern bool Cosh(void);
extern bool Cos(void);
extern bool DivEq(void);
extern bool Div(void);
extern bool eigen_array(void);
extern bool eigen_det(void);
extern bool EqualOpSeq(void);
extern bool Erf(void);
extern bool expm1(void);
extern bool exp(void);
extern bool ForOne(void);
extern bool ForTwo(void);
extern bool forward_dir(void);
extern bool forward_order(void);
extern bool Forward(void);
extern bool fun_assign(void);
extern bool FunCheck(void);
extern bool HesLagrangian(void);
extern bool HesLuDet(void);
extern bool HesMinorDet(void);
extern bool Hessian(void);
extern bool HesTimesDir(void);
extern bool Independent(void);
extern bool Integer(void);
extern bool Interface2C(void);
extern bool interp_onetape(void);
extern bool interp_retape(void);
extern bool JacLuDet(void);
extern bool JacMinorDet(void);
extern bool Jacobian(void);
extern bool log10(void);
extern bool log1p(void);
extern bool log(void);
extern bool LuRatio(void);
extern bool LuVecADOk(void);
extern bool MulEq(void);
extern bool mul_level_adolc_ode(void);
extern bool mul_level_adolc(void);
extern bool mul_level_ode(void);
extern bool mul_level(void);
extern bool Mul(void);
extern bool NearEqualExt(void);
extern bool number_skip(void);
extern bool NumericType(void);
extern bool num_limits(void);
extern bool OdeStiff(void);
extern bool ode_taylor(void);
extern bool opt_val_hes(void);
extern bool ParVar(void);
extern bool Poly(void);
extern bool pow_int(void);
extern bool pow(void);
extern bool print_for(void);
extern bool reverse_any(void);
extern bool reverse_one(void);
extern bool reverse_three(void);
extern bool reverse_two(void);
extern bool RevOne(void);
extern bool RevTwo(void);
extern bool Rosen34(void);
extern bool runge_45_2(void);
extern bool seq_property(void);
extern bool sign(void);
extern bool Sinh(void);
extern bool Sin(void);
extern bool Sqrt(void);
extern bool StackMachine(void);
extern bool SubEq(void);
extern bool Sub(void);
extern bool Tanh(void);
extern bool Tan(void);
extern bool TapeIndex(void);
extern bool UnaryMinus(void);
extern bool UnaryPlus(void);
extern bool Value(void);
extern bool Var2Par(void);
extern bool vec_ad(void);

// main program that runs all the tests
int main(void)
{	std::string group = "example/general";
	size_t      width = 20;
	CppAD::test_boolofvoid Run(group, width);

	// This line is used by test_one.sh

	// run external compiled tests (this line expected by bin/new_test.sh)
	Run( abort_recording,   "abort_recording"  );
	Run( fabs,              "fabs"             );
	Run( acos,              "acos"             );
	Run( acosh,             "acosh"            );
	Run( ad_assign,         "ad_assign"        );
	Run( ad_ctor,           "ad_ctor"          );
	Run( Add,               "Add"              );
	Run( AddEq,             "AddEq"            );
	Run( ad_fun,            "ad_fun"           );
	Run( ad_in_c,           "ad_in_c"          );
	Run( ad_input,          "ad_input"         );
	Run( ad_output,         "ad_output"        );
	Run( asin,              "asin"             );
	Run( asinh,             "asinh"            );
	Run( atan2,             "atan2"            );
	Run( atan,              "atan"             );
	Run( atanh,             "atanh"            );
	Run( azmul,             "azmul"            );
	Run( BenderQuad,        "BenderQuad"       );
	Run( BoolFun,           "BoolFun"          );
	Run( capacity_order,    "capacity_order"   );
	Run( change_param,      "change_param"     );
	Run( compare_change,    "compare_change"   );
	Run( Compare,           "Compare"          );
	Run( complex_poly,      "complex_poly"     );
	Run( CondExp,           "CondExp"          );
	Run( Cos,               "Cos"              );
	Run( Cosh,              "Cosh"             );
	Run( Div,               "Div"              );
	Run( DivEq,             "DivEq"            );
	Run( EqualOpSeq,        "EqualOpSeq"       );
	Run( Erf,               "Erf"              );
	Run( exp,               "exp"              );
	Run( expm1,             "expm1"            );
	Run( ForOne,            "ForOne"           );
	Run( ForTwo,            "ForTwo"           );
	Run( forward_dir,       "forward_dir"      );
	Run( Forward,           "Forward"          );
	Run( forward_order,     "forward_order"    );
	Run( fun_assign,        "fun_assign"       );
	Run( FunCheck,          "FunCheck"         );
	Run( HesLagrangian,     "HesLagrangian"    );
	Run( HesLuDet,          "HesLuDet"         );
	Run( HesMinorDet,       "HesMinorDet"      );
	Run( Hessian,           "Hessian"          );
	Run( HesTimesDir,       "HesTimesDir"      );
	Run( Independent,       "Independent"      );
	Run( Integer,           "Integer"          );
	Run( Interface2C,       "Interface2C"      );
	Run( interp_onetape,    "interp_onetape"   );
	Run( interp_retape,     "interp_retape"    );
	Run( JacLuDet,          "JacLuDet"         );
	Run( JacMinorDet,       "JacMinorDet"      );
	Run( Jacobian,          "Jacobian"         );
	Run( log10,             "log10"            );
	Run( log1p,             "log1p"            );
	Run( log,               "log"              );
	Run( LuRatio,           "LuRatio"          );
	Run( LuVecADOk,         "LuVecADOk"        );
	Run( MulEq,             "MulEq"            );
	Run( mul_level,         "mul_level"        );
	Run( mul_level_ode,     "mul_level_ode"    );
	Run( Mul,               "Mul"              );
	Run( NearEqualExt,      "NearEqualExt"     );
	Run( number_skip,       "number_skip"      );
	Run( NumericType,       "NumericType"      );
	Run( num_limits,        "num_limits"       );
	Run( OdeStiff,          "OdeStiff"         );
	Run( ode_taylor,        "ode_taylor"       );
	Run( opt_val_hes,       "opt_val_hes"      );
	Run( ParVar,            "ParVar"           );
	Run( Poly,              "Poly"             );
	Run( pow_int,           "pow_int"          );
	Run( pow,               "pow"              );
	Run( reverse_any,       "reverse_any"      );
	Run( reverse_one,       "reverse_one"      );
	Run( reverse_three,     "reverse_three"    );
	Run( reverse_two,       "reverse_two"      );
	Run( RevOne,            "RevOne"           );
	Run( RevTwo,            "RevTwo"           );
	Run( Rosen34,           "Rosen34"          );
	Run( runge_45_2,        "runge_45_2"       );
	Run( seq_property,      "seq_property"     );
	Run( sign,              "sign"             );
	Run( Sinh,              "Sinh"             );
	Run( Sin,               "Sin"              );
	Run( Sqrt,              "Sqrt"             );
	Run( StackMachine,      "StackMachine"     );
	Run( SubEq,             "SubEq"            );
	Run( Sub,               "Sub"              );
	Run( Tanh,              "Tanh"             );
	Run( Tan,               "Tan"              );
	Run( TapeIndex,         "TapeIndex"        );
	Run( UnaryMinus,        "UnaryMinus"       );
	Run( UnaryPlus,         "UnaryPlus"        );
	Run( Value,             "Value"            );
	Run( Var2Par,           "Var2Par"          );
	Run( vec_ad,            "vec_ad"           );
# ifndef CPPAD_DEBUG_AND_RELEASE
	Run( check_for_nan,     "check_for_nan"    );
# endif
# if CPPAD_HAS_ADOLC
	Run( mul_level_adolc,      "mul_level_adolc"     );
	Run( mul_level_adolc_ode,  "mul_level_adolc_ode" );
# endif
# if CPPAD_HAS_EIGEN
	Run( eigen_array,       "eigen_array"      );
	Run( eigen_det,         "eigen_det"        );
# endif
	//
	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
// END C++
