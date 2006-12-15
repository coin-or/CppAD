/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
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

$code
$verbatim%speed_cppad/det_lu.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <speed/det_by_lu.hpp>

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

	det_by_lu<double> Det(size);

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

	det_by_lu< AD<double> > Det(size);

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

	det_by_lu< AD<double> > Det(size);

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
	return "Tape of Determinate by Lu Factorization";
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

	det_by_lu< AD<double> > Det(size);

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
	return "Jacobian of Determinate by Lu Factorization";
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

	det_by_lu< AD<double> > Det(size);

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
	return "Hessian of Determinate by Lu Factorization";
}

// END PROGRAM
