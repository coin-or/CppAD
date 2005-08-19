// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
$begin Example.cpp$$
$spell
	Cpp
$$

$section Program That Runs the CppAD Example Suite$$
$mindex program run all example test$$

$code
$verbatim%Example/Example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

// system include files used for I/O
# include <cstddef>
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
extern bool BoolFun(void);
extern bool CheckSimpleVector(void);
extern bool Compare(void);
extern bool CompareChange(void);
extern bool ComplexPoly(void);
extern bool CondExp(void);
extern bool Copy(void);
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
extern bool Exp(void);
extern bool FromBase(void);
extern bool HesLuDet(void);
extern bool HesMinorDet(void);
extern bool HesTimesDir(void);
extern bool Integer(void);
extern bool JacLuDet(void);
extern bool JacMinorDet(void);
extern bool Log(void);
extern bool Log10(void);
extern bool LuFactor(void);
extern bool LuInvert(void);
extern bool LuSolve(void);
extern bool LuVecADOk(void);
extern bool Mul(void);
extern bool MulEq(void);
extern bool MulTape(void);
extern bool Near_Equal(void);
extern bool Neg(void);
extern bool OdeErrControl(void);
extern bool OdeTaylor(void);
extern bool Output(void);
extern bool Poly(void);
extern bool Pow(void);
extern bool PowInt(void);
extern bool Romberg(void);
extern bool Rosen34(void);
extern bool Runge45(void);
extern bool SimpleVector(void);
extern bool Sin(void);
extern bool Sinh(void);
extern bool Sqrt(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool Value(void);
extern bool Vec(void);

// templated tests in example directory
# include "Fun.h"      // bool      Fun<VectorADdouble>(void)
# include "ForOne.h"   // bool   ForOne<Vectordouble>  (void)
# include "ForTwo.h"   // bool   ForTwo<Vectordouble>  (void)
# include "Forward.h"  // bool  Forward<Vectordouble>  (void)
# include "Jacobian.h" // bool Jacobian<Vectordouble>  (void)
# include "Hessian.h"  // bool  Hessian<Vectordouble>  (void)
# include "Reverse.h"  // bool  Reverse<Vectordouble>  (void)
# include "RevOne.h"   // bool   RevOne<Vectordouble>  (void)
# include "RevTwo.h"   // bool   RevTwo<Vectordouble>  (void)

// standard array template classes used by tests
# include <vector>
# include <valarray>

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

// macro that runs template cases with double elements
# define RunDouble(fun, name)                          \
	ok &= Run(                                     \
		 fun   < CppAD::vector  <double> >,    \
	         name " with CppAD::vector"            \
	);                                             \
	ok &= Run(                                     \
		 fun   <   std::vector  <double> >,    \
		name  " with std::vector "             \
	);                                             \
	ok &= Run(                                     \
		 fun   <   std::valarray<double> >,    \
		name  " with std::valarray "           \
	)

// macro that runs template cases with AD<double> elements
# define RunADdouble(fun, name)                                    \
	ok &= Run(                                                 \
		 fun   < CppAD::vector  < CppAD::AD<double> > >,   \
	         name " with CppAD::vector"                        \
	);                                                         \
	ok &= Run(                                                 \
		 fun   <   std::vector  < CppAD::AD<double> > >,   \
		name  " with std::vector "                         \
	);                                                         \
	ok &= Run(                                                 \
		 fun   <   std::valarray< CppAD::AD<double> > >,   \
		name  " with std::valarray "                       \
	)

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
	ok &= Run( BoolFun,           "BoolFun"          );
	ok &= Run( CheckSimpleVector, "CheckSimpleVector");
	ok &= Run( Compare,           "Compare"          );
	ok &= Run( CompareChange,     "CompareChange"    );
	ok &= Run( ComplexPoly,       "ComplexPoly"      );
	ok &= Run( CondExp,           "CondExp"          );
	ok &= Run( Copy,              "Copy"             );
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
	ok &= Run( Exp,               "Exp"              );
	ok &= Run( FromBase,          "FromBase"         );
	RunDouble( ForOne,            "ForOne"           );
	RunDouble( ForTwo,            "ForTwo"           );
	RunDouble( Forward,           "Forward"          ); 
	RunADdouble(Fun,              "Fun"              ); 
	ok &= Run( HesLuDet,          "HesLuDet"         );
	ok &= Run( HesMinorDet,       "HesMinorDet"      );
	RunDouble( Hessian,           "Hessian"          );
	ok &= Run( HesTimesDir,       "HesTimesDir"      );
	ok &= Run( Integer,           "Integer"          );
	ok &= Run( JacLuDet,          "JacLuDet"         );
	ok &= Run( JacMinorDet,       "JacMinorDet"      );
	RunDouble( Jacobian,          "Jacobian"         );
	ok &= Run( Log,               "Log"              );
	ok &= Run( Log10,             "Log10"            );
	ok &= Run( LuFactor,          "LuFactor"         );
	ok &= Run( LuInvert,          "LuInvert"         );
	ok &= Run( LuSolve,           "LuSolve"          );
	ok &= Run( LuVecADOk,         "LuVecADOk"        );
	ok &= Run( Mul,               "Mul"              );
	ok &= Run( MulEq,             "MulEq"            );
	ok &= Run( MulTape,           "MulTape"          );
	ok &= Run( Near_Equal,        "Near_Equal"       );
	ok &= Run( Neg,               "Neg"              );
	ok &= Run( OdeErrControl,     "OdeErrControl"    );
	ok &= Run( OdeTaylor,         "OdeTaylor"        );
	ok &= Run( Output,            "Output"           );
	ok &= Run( Pow,               "Poly"             );
	ok &= Run( Pow,               "Pow"              );
	ok &= Run( PowInt,            "PowInt"           );
	RunDouble( Reverse,           "Reverse"          );
	RunDouble( RevOne,            "RevOne"           );
	RunDouble( RevTwo,            "RevTwo"           );
	ok &= Run( Romberg,           "Romberg"          );
	ok &= Run( Rosen34,           "Rosen34"          );
	ok &= Run( Runge45,           "Runge45"          );
	ok &= Run( SimpleVector,      "SimpleVector"     );
	ok &= Run( Sin,               "Sin"              );
	ok &= Run( Sinh,              "Sinh"             );
	ok &= Run( Sqrt,              "Sqrt"             );
	ok &= Run( Sub,               "Sub"              );
	ok &= Run( SubEq,             "SubEq"            );
	ok &= Run( Value,             "Value"            );
	ok &= Run( Vec,               "Vec"              );

	cout << endl << endl;
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return 0;
}
// END PROGRAM
