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
$begin DetLuSpeed.cpp$$
$spell
	Lu
	Cpp
$$

$index speed, Lu determinant$$
$index determinant, Lu speed$$
$index Lu, determinant speed$$
$section Determinant by Lu Factor and Solve: Speed Test$$

$comment This file is in the Speed subdirectory$$ 
$code
$verbatim%Speed/DetLu.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "../Example/DetByLu.h"

# include <sstream>
# include <string>

std::string doubleDetLu(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double> a( size * size );

	for( i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			a[i + j * size] = double ( i / (double) (j + 1.) );

	DetByLu<double> Det(size);

	while(repeat--)
		Det(a);

	return "double: Determinant by Lu Factorization";
}

std::string ADdoubleDetLu(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector< AD<double> > A( size * size );

	for( i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			A[i + j * size] =  AD<double>( i / (double) (j + 1.) );

	DetByLu< AD<double> > Det(size);

	while(repeat--)
		Det(A);

	return "AD<double>: Determinant by Lu Factorization";
}


std::string TapeDetLu(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double>       a( size * size );
	CppADvector<double>       J( size * size );

	CppADvector< AD<double> > A( size * size );
	CppADvector< AD<double> > D(1);

	for( i = 0; i < size; i++)
	{	for(j = 0; j < size; j++)
		{	A[i + j * size] = a[i + j * size] 
			                = double( i / (double) (j + 1.));
		}
	}

	DetByLu< AD<double> > Det(size);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare the independent variables
		Independent(A);

		// tape the evaluation
		D[0] = Det(A);

		// create f : A -> D
		ADFun<double> f(A, D);

		// save so can return
		length = f.Size();
		memory = f.Memory();
	}

	ostringstream buf;
	buf << "Tape of Determinate by Lu Factorization: Length = "
	    << length << ", Memory = " << memory;

	return buf.str();
}

std::string JacDetLu(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double>       a( size * size );
	CppADvector<double>       J( size * size );

	CppADvector< AD<double> > A( size * size );
	CppADvector< AD<double> > D(1);

	for( i = 0; i < size; i++)
	{	for(j = 0; j < size; j++)
		{	A[i + j * size] = a[i + j * size] 
			                = double( i / (double) (j + 1.));
		}
	}

	DetByLu< AD<double> > Det(size);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare the independent variables
		Independent(A);

		// tape the evaluation
		D[0] = Det(A);

		// create f : A -> D
		ADFun<double> f(A, D);

		// evaluate the Jacobian of f
		J = f.Jacobian(a);

		// save so can return
		length = f.Size();
		memory = f.Memory();
	}

	ostringstream buf;
	buf << "Jacobian of Determinate by Lu Factorization: Length = "
	    << length << ", Memory = " << memory;

	return buf.str();
}

std::string HesDetLu(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double>       a( size * size );
	CppADvector<double>       H( size * size * size * size );

	CppADvector< AD<double> > A( size * size );
	CppADvector< AD<double> > D(1);

	for( i = 0; i < size; i++)
	{	for(j = 0; j < size; j++)
		{	A[i + j * size] = a[i + j * size] 
			                = double( i / (double) (j + 1.));
		}
	}

	DetByLu< AD<double> > Det(size);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare the independent variables
		Independent(A);

		// tape the evaluation
		D[0] = Det(A);

		// create f : A -> D
		ADFun<double> f(A, D);

		// evaluate the Hessian of f
		H = f.Hessian(a, 0);

		// save so can return
		length = f.Size();
		memory = f.Memory();
	}

	ostringstream buf;
	buf << "Hessian of Determinate by Lu Factorization: Length = "
	    << length << ", Memory = " << memory;

	return buf.str();
}

// END PROGRAM
