/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
$verbatim%Example/Example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

// system include files used for I/O
# include <iostream>

// CppAD include file
# include <CppAD/CppAD.h>

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
extern bool DetByLu(void);
extern bool DetByMinor(void);
extern bool DetOfMinor(void);
extern bool Discrete(void);
extern bool Div(void);
extern bool DivEq(void);
extern bool Eq(void);
extern bool Erf(void);
extern bool ErrorHandler(void);
extern bool Exp(void);
extern bool ForOne(void);
extern bool ForTwo(void);
extern bool ForSparseJac(void);
extern bool Forward(void);
extern bool FunCheck(void);
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
extern bool MulTape(void);
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
extern bool PowInt(void);
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
extern bool TrackNewDel(void);
extern bool UnaryMinus(void);
extern bool UnaryPlus(void);
extern bool Value(void);
extern bool VecAD(void);

namespace {
	// function that runs one test
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		using namespace std;
	
		ok &= TestOk();
	
		if( ok )
			std::cout << "Ok:    " << name << std::endl;
		else	std::cout << "Error: " << name << std::endl;
	
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	// This comment is used by OneTest 

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
	ok &= Run( DetByLu,           "DetByLu"          );
	ok &= Run( DetByMinor,        "DetByMinor"       );
	ok &= Run( DetOfMinor,        "DetOfMinor"       );
	ok &= Run( Discrete,          "Discrete"         );
	ok &= Run( Div,               "Div"              );
	ok &= Run( DivEq,             "DivEq"            );
	ok &= Run( Eq,                "Eq"               );
	ok &= Run( Erf,               "Erf"              );
	ok &= Run( ErrorHandler,      "ErrorHandler"     );
	ok &= Run( Exp,               "Exp"              );
	ok &= Run( ForOne,            "ForOne"           );
	ok &= Run( ForTwo,            "ForTwo"           );
	ok &= Run( Forward,           "Forward"          ); 
	ok &= Run( ForSparseJac,      "ForSparseJac"     );
	ok &= Run( FunCheck,          "FunCheck"         );
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
	ok &= Run( MulTape,           "MulTape"          );
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
	ok &= Run( PowInt,            "PowInt"           );
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
	ok &= Run( TrackNewDel,       "TrackNewDel"      );
	ok &= Run( UnaryMinus,        "UnaryMinus"       );
	ok &= Run( UnaryPlus,         "UnaryPlus"        );
	ok &= Run( Value,             "Value"            );
	ok &= Run( VecAD,             "VecAD"            );

	// check for memory leak in previous calculations
	if( CppADTrackCount() != 0 )
	{	ok = false;
		cout << "Error: memroy leak detected" << endl;
	}
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
