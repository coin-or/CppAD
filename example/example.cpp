/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Example.cpp$$
$spell
	Cpp
$$

$section Program That Runs the CppAD Examples$$
$index example, run all$$

$code
$verbatim%example/example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// CppAD include file
# include <cppad/cppad.hpp>

// external complied tests
extern bool abort_recording(void);
extern bool Abs(void);
extern bool Acos(void);
extern bool Add(void);
extern bool AddEq(void);
extern bool Asin(void);
extern bool Atan(void);
extern bool Atan2(void);
extern bool BenderQuad(void);
extern bool BoolFun(void);
extern bool vectorBool(void);
extern bool CheckNumericType(void);
extern bool CheckSimpleVector(void);
extern bool Compare(void);
extern bool CompareChange(void);
extern bool complex_poly(void);
extern bool CondExp(void);
extern bool CopyAD(void);
extern bool CopyBase(void);
extern bool Cos(void);
extern bool Cosh(void);
extern bool CppAD_vector(void);
extern bool Default(void);
extern bool Div(void);
extern bool DivEq(void);
extern bool Eq(void);
extern bool EqualOpSeq(void);
extern bool Erf(void);
extern bool ErrorHandler(void);
extern bool Exp(void);
extern bool ForOne(void);
extern bool ForTwo(void);
extern bool ForSparseJac(void);
extern bool Forward(void);
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
extern bool Log(void);
extern bool Log10(void);
extern bool LuFactor(void);
extern bool LuInvert(void);
extern bool LuRatio(void);
extern bool LuSolve(void);
extern bool LuVecADOk(void);
extern bool Mul(void);
extern bool MulEq(void);
extern bool mul_level(void);
extern bool mul_level_adolc(void);
extern bool nan(void);
extern bool Near_Equal(void);
extern bool NearEqualExt(void);
extern bool not_complex_ad(void);
extern bool NumericType(void);
extern bool OdeErrControl(void);
extern bool OdeErrMaxabs(void);
extern bool OdeGear(void);
extern bool OdeGearControl(void);
extern bool OdeStiff(void);
extern bool ode_taylor(void);
extern bool ode_taylor_adolc(void);
extern bool Output(void);
extern bool ParVar(void);
extern bool Poly(void);
extern bool Pow(void);
extern bool pow_int(void);
extern bool reverse_any(void);
extern bool reverse_one(void);
extern bool reverse_two(void);
extern bool RevOne(void);
extern bool RevSparseHes(void);
extern bool RevSparseJac(void);
extern bool RevTwo(void);
extern bool RombergMul(void);
extern bool RombergOne(void);
extern bool Rosen34(void);
extern bool runge_45_1(void);
extern bool runge_45_2(void);
extern bool SeqProperty(void);
extern bool SimpleVector(void);
extern bool Sin(void);
extern bool Sinh(void);
extern bool sparse_hessian(void);
extern bool sparse_jacobian(void);
extern bool Sqrt(void);
extern bool StackMachine(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool Tan(void);
extern bool Tanh(void);
extern bool TapeIndex(void);
extern bool TrackNewDel(void);
extern bool UnaryMinus(void);
extern bool UnaryPlus(void);
extern bool Value(void);
extern bool Var2Par(void);
extern bool VecAD(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		ok &= TestOk();
		if( ok )
		{	std::cout << "Ok:    " << name << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error: " << name << std::endl;
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
	ok &= Run( abort_recording,   "abort_recording"  );
	ok &= Run( Abs,               "Abs"              );
	ok &= Run( Acos,              "Acos"             );
	ok &= Run( Add,               "Add"              );
	ok &= Run( AddEq,             "AddEq"            );
	ok &= Run( Asin,              "Asin"             );
	ok &= Run( Atan,              "Atan"             );
	ok &= Run( Atan2,             "Atan2"            );
	ok &= Run( BenderQuad,        "BenderQuad"       );
	ok &= Run( BoolFun,           "BoolFun"          );
	ok &= Run( vectorBool,        "vectorBool"       );
	ok &= Run( CheckNumericType,  "CheckNumericType" );
	ok &= Run( CheckSimpleVector, "CheckSimpleVector");
	ok &= Run( Compare,           "Compare"          );
	ok &= Run( CompareChange,     "CompareChange"    );
	ok &= Run( complex_poly,      "complex_poly"     );
	ok &= Run( CondExp,           "CondExp"          );
	ok &= Run( CopyAD,            "CopyAD"           );
	ok &= Run( CopyBase,          "CopyBase"         );
	ok &= Run( Cos,               "Cos"              );
	ok &= Run( Cosh,              "Cosh"             );
	ok &= Run( CppAD_vector,      "CppAD_vector"     );
	ok &= Run( Default,           "Default"          );
	ok &= Run( Div,               "Div"              );
	ok &= Run( DivEq,             "DivEq"            );
	ok &= Run( Eq,                "Eq"               );
	ok &= Run( EqualOpSeq,        "EqualOpSeq"       );
	ok &= Run( Erf,               "Erf"              );
	ok &= Run( ErrorHandler,      "ErrorHandler"     );
	ok &= Run( Exp,               "Exp"              );
	ok &= Run( ForOne,            "ForOne"           );
	ok &= Run( ForTwo,            "ForTwo"           );
	ok &= Run( Forward,           "Forward"          ); 
	ok &= Run( ForSparseJac,      "ForSparseJac"     );
	ok &= Run( FunCheck,          "FunCheck"         );
	ok &= Run( HesLagrangian,     "HesLagrangian"    );
	ok &= Run( HesLuDet,          "HesLuDet"         );
	ok &= Run( HesMinorDet,       "HesMinorDet"      );
	ok &= Run( Hessian,           "Hessian"          );
	ok &= Run( HesTimesDir,       "HesTimesDir"      );
	ok &= Run( Independent,       "Independent"      );
	ok &= Run( Integer,           "Integer"          );
	ok &= Run( Interface2C,       "Interface2C"      );
	ok &= Run( interp_onetape,    "interp_onetape"   );
	ok &= Run( interp_retape,     "interp_retape"    );
	ok &= Run( JacLuDet,          "JacLuDet"         );
	ok &= Run( JacMinorDet,       "JacMinorDet"      );
	ok &= Run( Jacobian,          "Jacobian"         );
	ok &= Run( Log,               "Log"              );
	ok &= Run( Log10,             "Log10"            );
	ok &= Run( LuFactor,          "LuFactor"         );
	ok &= Run( LuInvert,          "LuInvert"         );
	ok &= Run( LuRatio,           "LuRatio"          );
	ok &= Run( LuSolve,           "LuSolve"          );
	ok &= Run( LuVecADOk,         "LuVecADOk"        );
	ok &= Run( Mul,               "Mul"              );
	ok &= Run( MulEq,             "MulEq"            );
	ok &= Run( mul_level,         "mul_level"        );
	ok &= Run( nan,               "nan"              );
	ok &= Run( Near_Equal,        "Near_Equal"       );
	ok &= Run( NearEqualExt,      "NearEqualExt"     );
	ok &= Run( not_complex_ad,    "not_complex_ad"   );
	ok &= Run( NumericType,       "NumericType"      );
	ok &= Run( OdeErrControl,     "OdeErrControl"    );
	ok &= Run( OdeErrMaxabs,      "OdeErrMaxabs"     );
	ok &= Run( OdeGear,           "OdeGear"          );
	ok &= Run( OdeGearControl,    "OdeGearControl"   );
	ok &= Run( OdeStiff,          "OdeStiff"         );
	ok &= Run( ode_taylor,        "ode_taylor"       );
	ok &= Run( Output,            "Output"           );
	ok &= Run( ParVar,            "ParVar"           );
	ok &= Run( Pow,               "Poly"             );
	ok &= Run( Pow,               "Pow"              );
	ok &= Run( pow_int,           "pow_int"          );
	ok &= Run( reverse_any,       "reverse_any"      );
	ok &= Run( reverse_one,       "reverse_one"      );
	ok &= Run( reverse_two,       "reverse_two"      );
	ok &= Run( RevOne,            "RevOne"           );
	ok &= Run( RevSparseHes,      "RevSparseHes"     );
	ok &= Run( RevSparseJac,      "RevSparseJac"     );
	ok &= Run( RevTwo,            "RevTwo"           );
	ok &= Run( RombergMul,        "RombergMul"       );
	ok &= Run( RombergOne,        "RombergOne"       );
	ok &= Run( Rosen34,           "Rosen34"          );
	ok &= Run( runge_45_1,        "runge_45_1"       );
	ok &= Run( runge_45_2,        "runge_45_2"       );
	ok &= Run( SeqProperty,       "SeqProperty"      );
	ok &= Run( SimpleVector,      "SimpleVector"     );
	ok &= Run( Sin,               "Sin"              );
	ok &= Run( Sinh,              "Sinh"             );
	ok &= Run( sparse_hessian,    "sparse_hessian"   );
	ok &= Run( sparse_jacobian,   "sparse_jacobian"  );
	ok &= Run( Sqrt,              "Sqrt"             );
	ok &= Run( StackMachine,      "StackMachine"     );
	ok &= Run( Sub,               "Sub"              );
	ok &= Run( SubEq,             "SubEq"            );
	ok &= Run( Tan,               "Tan"              );
	ok &= Run( Tanh,              "Tanh"             );
	ok &= Run( TapeIndex,         "TapeIndex"        );
	ok &= Run( TrackNewDel,       "TrackNewDel"      );
	ok &= Run( UnaryMinus,        "UnaryMinus"       );
	ok &= Run( UnaryPlus,         "UnaryPlus"        );
	ok &= Run( Value,             "Value"            );
	ok &= Run( Var2Par,           "Var2Par"          );
	ok &= Run( VecAD,             "VecAD"            );

# ifdef CPPAD_ADOLC_EXAMPLES
	ok &= Run( mul_level_adolc,   "mul_level_adolc"  );
	ok &= Run( ode_taylor_adolc,  "ode_taylor_adolc" );
# endif
	

	// check for errors
	using std::cout;
	using std::endl;
	assert( ok || (Run_error_count > 0) );
	if( CPPAD_TRACK_COUNT() == 0 )
	{	Run_ok_count++;
		cout << "Ok:    " << "No memory leak detected" << endl;
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
// END PROGRAM
