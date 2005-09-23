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
$begin ExampleAdolc.cpp$$
$spell
	Adolc
	Cpp
$$

$index Adolc, compare$$
$section Compare Adolc and CppAD Results$$

$comment This file is in the Adolc subdirectory$$ 
$code
$verbatim%Adolc/Example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

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

// various test routines
extern bool AllocMatTest(void);
extern bool AllocVecTest(void);
extern bool DetMinor(void);
extern bool DetLu(void);

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	ok &= Run(AllocMatTest,      "AllocMat"   );
	ok &= Run(AllocVecTest,      "AllocVec"   );
	ok &= Run(DetMinor,          "DetMinor"   );
	ok &= Run(DetLu,             "DetLu"      );
	

	cout << endl << endl;
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return 0;
}

// END PROGRAM
