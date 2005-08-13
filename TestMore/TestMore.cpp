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

// system include files used for I/O
# include <cstddef>
# include <iostream>

// AD<Type> template class definition
# include <CppAD/CppAD.h>

# include <complex>

typedef CppAD::AD<double>     ADdouble;
typedef CppAD::AD<ADdouble>   ADDdouble;

// various examples / tests
extern bool Add(void);
extern bool AddEq(void);
extern bool AddZero(void);
extern bool Atan(void);
extern bool Compare(void);
extern bool CompareChange(void);
extern bool CondExp(void);
extern bool CondExpAD(void);
extern bool Div(void);
extern bool DivEq(void);
extern bool DivZeroOne(void);
extern bool Exp(void);
extern bool ForHess(void);
extern bool Forward(void);
extern bool Log(void);
extern bool Mul(void);
extern bool MulEq(void);
extern bool MulZeroOne(void);
extern bool Reverse(void);
extern bool Rosen34(void);
extern bool Runge45(void);
extern bool SimpleVector(void);
extern bool SinCos(void);
extern bool Sqrt(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool SubZero(void);
extern bool VecADPar(void);
extern bool VecUnary(void);

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

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	// This comment is used by OneTest 

	ok &= Run( Add,             "Add"            );
	ok &= Run( AddEq,           "AddEq"          );
	ok &= Run( AddZero,         "AddZero"        );
	ok &= Run( Atan,            "Atan"           );
	ok &= Run( Compare,         "Compare"        );
	ok &= Run( CompareChange,   "CompareChange"  );
	ok &= Run( CondExp,         "CondExp"        );
	ok &= Run( CondExpAD,       "CondExpAD"      );
	ok &= Run( Div,             "Div"            );
	ok &= Run( DivEq,           "DivEq"          );
	ok &= Run( DivZeroOne,      "DivZeroOne"     );
	ok &= Run( Exp,             "Exp"            );
	ok &= Run( ForHess,         "ForHess"        );
	ok &= Run( Forward,         "Forward"        );
	ok &= Run( Log,             "Log"            );
	ok &= Run( Mul,             "Mul"            );
	ok &= Run( MulEq,           "MulEq"          );
	ok &= Run( MulZeroOne,      "MulZeroOne"     );
	ok &= Run( Reverse,         "Reverse"        );
	ok &= Run( Rosen34,         "Rosen34"        );
	ok &= Run( Runge45,         "Runge45"        );
	ok &= Run( SimpleVector,    "SimpleVector"   );
	ok &= Run( SinCos,          "SinCos"         );
	ok &= Run( Sqrt,            "Sqrt"           );
	ok &= Run( Sub,             "Sub"            );
	ok &= Run( SubEq,           "SubEq"          );
	ok &= Run( SubZero,         "SubZero"        );
	ok &= Run( VecADPar,        "VecADPar"       );
	ok &= Run( VecUnary,        "VecUnary"       );

	cout << endl << endl;
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return 0;
}
// END PROGRAM
