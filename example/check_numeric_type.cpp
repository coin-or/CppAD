/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin CheckNumericType.cpp$$

$section The CheckNumericType Function: Example and Test$$
$index CheckNumericType$$
$index example, check NumericType$$
$index test, check NumericType$$

$code
$verbatim%example/check_numeric_type.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/check_numeric_type.hpp>
# include <cppad/near_equal.hpp>


// Chosing a value between 1 and 10 selects a numeric class properity to be 
// omitted and result in an error message being generated 
# define CppADMyTypeOmit 0

namespace { // Empty namespace

	// -------------------------------------------------------------------
	class MyType {
	private:
		double d;
	public:
		// constructor from void 
		MyType(void) : d(0.)
		{ }
		// constructor from an int 
		MyType(int d_) : d(d_)
		{ }
		// copy constuctor
		MyType(const MyType &x)
		{	d = x.d; }
		// assignment operator
		void operator = (const MyType &x)
		{	d = x.d; }
		// member function that converts to double
		double Double(void) const
		{	return d; }
# if CppADMyTypeOmit != 1
		// unary plus
		MyType operator + (void) const
		{	MyType x;
			x.d =  d;
			return x; 
		}
# endif
# if CppADMyTypeOmit != 2
		// unary plus
		MyType operator - (void) const
		{	MyType x;
			x.d = - d;
			return x; 
		}
# endif
# if CppADMyTypeOmit != 3
		// binary addition
		MyType operator + (const MyType &x) const
		{	MyType y;
			y.d = d + x.d ;
			return y; 
		}
# endif
# if CppADMyTypeOmit != 4
		// binary subtraction
		MyType operator - (const MyType &x) const
		{	MyType y;
			y.d = d - x.d ;
			return y; 
		}
# endif
# if CppADMyTypeOmit != 5
		// binary multiplication
		MyType operator * (const MyType &x) const
		{	MyType y;
			y.d = d * x.d ;
			return y; 
		}
# endif
# if CppADMyTypeOmit != 6
		// binary division
		MyType operator / (const MyType &x) const
		{	MyType y;
			y.d = d / x.d ;
			return y; 
		}
# endif
# if CppADMyTypeOmit != 7
		// computed assignment addition
		void operator += (const MyType &x)
		{	d += x.d; }
# endif
# if CppADMyTypeOmit != 8
		// computed assignment subtraction
		void operator -= (const MyType &x)
		{	d -= x.d; }
# endif
# if CppADMyTypeOmit != 9
		// computed assignment multiplication
		void operator *= (const MyType &x)
		{	d *= x.d; }
# endif
# if CppADMyTypeOmit != 10
		// computed assignment division
		void operator /= (const MyType &x)
		{	d /= x.d; }
# endif
	};
	// -------------------------------------------------------------------
	/*
	Solve: A[0] * x[0] + A[1] * x[1] = b[0] 
	       A[2] * x[0] + A[3] * x[1] = b[1] 
	*/ 
	template <class NumericType>
	void Solve(NumericType *A, NumericType *x, NumericType *b)
	{
		// make sure NumericType satisfies its conditions
		CppAD::CheckNumericType<NumericType>();

		// copy b to x
		x[0] = b[0];
		x[1] = b[1];

		// copy A to work space
		NumericType W[4];
		W[0] = A[0];
		W[1] = A[1];
		W[2] = A[2];
		W[3] = A[3];

		// divide first row by W(1,1)
		W[1] /= W[0];
		x[0] /= W[0];
		W[0] = NumericType(1);

		// subtract W(2,1) times first row from second row
		W[3] -= W[2] * W[1];
		x[1] -= W[2] * x[0];
		W[2] = NumericType(0);

		// divide second row by W(2, 2)
		x[1] /= W[3];
		W[3]  = NumericType(1);

		// use first row to solve for x[0]
		x[0] -= W[1] * x[1];
	}
} // End Empty namespace

bool CheckNumericType(void)
{	bool ok  = true;

	MyType A[4];
	A[0] = MyType(1); A[1] = MyType(2);
	A[2] = MyType(3); A[3] = MyType(4);

	MyType b[2]; 
	b[0] = MyType(1);
	b[1] = MyType(2);

	MyType x[2];
	Solve(A, x, b);

	MyType sum;
	sum = A[0] * x[0] + A[1] * x[1];
	ok &= CppAD::NearEqual(sum.Double(), b[0].Double(), 1e-10, 1e-10);

	sum = A[2] * x[0] + A[3] * x[1];
	ok &= CppAD::NearEqual(sum.Double(), b[1].Double(), 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
