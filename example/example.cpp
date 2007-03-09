/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

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

$section Program That Runs all the CppAD Example$$
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
extern bool ComplexPoly(void);
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
extern bool Near_Equal(void);
extern bool NearEqualExt(void);
extern bool NumericType(void);
extern bool OdeErrControl(void);
extern bool OdeErrMaxabs(void);
extern bool OdeGear(void);
extern bool OdeGearControl(void);
extern bool OdeStiff(void);
extern bool OdeTaylor(void);
extern bool Output(void);
extern bool ParVar(void);
extern bool Piecewise(void);
extern bool Poly(void);
extern bool Pow(void);
extern bool pow_int(void);
extern bool ReverseAny(void);
extern bool ReverseOne(void);
extern bool RevOne(void);
extern bool RevSparseHes(void);
extern bool RevSparseJac(void);
extern bool RevTwo(void);
extern bool RombergMul(void);
extern bool RombergOne(void);
extern bool Rosen34(void);
extern bool Runge45(void);
extern bool SeqProperty(void);
extern bool SimpleVector(void);
extern bool Sin(void);
extern bool Sinh(void);
extern bool Sqrt(void);
extern bool StackMachine(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool Tan(void);
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
	ok &= Run( ComplexPoly,       "ComplexPoly"      );
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
	ok &= Run( Near_Equal,        "Near_Equal"       );
	ok &= Run( NearEqualExt,      "NearEqualExt"     );
	ok &= Run( NumericType,       "NumericType"      );
	ok &= Run( OdeErrControl,     "OdeErrControl"    );
	ok &= Run( OdeErrMaxabs,      "OdeErrMaxabs"     );
	ok &= Run( OdeGear,           "OdeGear"          );
	ok &= Run( OdeGearControl,    "OdeGearControl"   );
	ok &= Run( OdeStiff,          "OdeStiff"         );
	ok &= Run( OdeTaylor,         "OdeTaylor"        );
	ok &= Run( Output,            "Output"           );
	ok &= Run( ParVar,            "ParVar"           );
	ok &= Run( Piecewise,         "Piecewise"        );
	ok &= Run( Pow,               "Poly"             );
	ok &= Run( Pow,               "Pow"              );
	ok &= Run( pow_int,           "pow_int"          );
	ok &= Run( ReverseAny,        "ReverseAny"       );
	ok &= Run( ReverseOne,        "ReverseOne"       );
	ok &= Run( RevOne,            "RevOne"           );
	ok &= Run( RevSparseHes,      "RevSparseHes"     );
	ok &= Run( RevSparseJac,      "RevSparseJac"     );
	ok &= Run( RevTwo,            "RevTwo"           );
	ok &= Run( RombergMul,        "RombergMul"       );
	ok &= Run( RombergOne,        "RombergOne"       );
	ok &= Run( Rosen34,           "Rosen34"          );
	ok &= Run( Runge45,           "Runge45"          );
	ok &= Run( SeqProperty,       "SeqProperty"      );
	ok &= Run( SimpleVector,      "SimpleVector"     );
	ok &= Run( Sin,               "Sin"              );
	ok &= Run( Sinh,              "Sinh"             );
	ok &= Run( Sqrt,              "Sqrt"             );
	ok &= Run( StackMachine,      "StackMachine"     );
	ok &= Run( Sub,               "Sub"              );
	ok &= Run( SubEq,             "SubEq"            );
	ok &= Run( Tan,               "Tan"              );
	ok &= Run( TapeIndex,         "TapeIndex"        );
	ok &= Run( TrackNewDel,       "TrackNewDel"      );
	ok &= Run( UnaryMinus,        "UnaryMinus"       );
	ok &= Run( UnaryPlus,         "UnaryPlus"        );
	ok &= Run( Value,             "Value"            );
	ok &= Run( Var2Par,           "Var2Par"          );
	ok &= Run( VecAD,             "VecAD"            );

	// check for errors
	using std::cout;
	using std::endl;
	assert( ok || (Run_error_count > 0) );
	if( CppADTrackCount() == 0 )
	{	Run_ok_count++;
		cout << "Ok:    " << "No memory leak detected" << endl;
	}
	else
	{	ok = false;
		Run_error_count++;
		cout << "Error: " << "memory leak detected" << endl;
	}
	if( ok )
		cout << "All " << Run_ok_count << " tests passed." << endl;
	else	cout << Run_error_count << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
