/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin CheckSimpleVector.cpp$$

$section The CheckSimpleVector Function: Example and Test$$
$index CheckSimpleVector$$
$index example, check SimpleVector$$
$index test, check SimpleVector$$

$code
$verbatim%example/check_simple_vector.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/vector.hpp>
# include <cppad/check_simple_vector.hpp>
# include <iostream>


// Chosing a value between 1 and 9 selects a simple vector properity to be 
// omitted and result in an error message being generated 
# define CppADMyVectorOmit 0

// -------------------------------------------------------------------------

// example class used for non-constant elements (different from Scalar)
template <class Scalar>
class MyElement {
private:
	Scalar *element;
public:
	// element constructor
	MyElement(Scalar *e)
	{	element = e; }
	// an example element assignment that returns void
	void operator = (const Scalar &s)
	{	*element = s; }
	// conversion to Scalar
	operator Scalar() const
	{	return *element; }
}; 
	 

// example simple vector class 
template <class Scalar>
class MyVector {
private:
	size_t length;
	Scalar * data;
public:

# if CppADMyVectorOmit != 1
	// type of the elements in the vector
	typedef Scalar value_type;
# endif
# if CppADMyVectorOmit != 2
	// default constructor
	inline MyVector(void) : length(0) , data(0)
	{ }
# endif
# if CppADMyVectorOmit != 3
	// constructor with a specified size
	inline MyVector(size_t n) : length(n)
	{	if( length == 0 )
			data = 0;
		else	data = new Scalar[length]; 
	}
# endif
# if CppADMyVectorOmit != 4
	// copy constructor
	inline MyVector(const MyVector &x) : length(x.length)
	{	size_t i;
		if( length == 0 )
			data = 0;
		else	data = new Scalar[length]; 

		for(i = 0; i < length; i++)
			data[i] = x.data[i];
	}
# endif
# if CppADMyVectorOmit != 4 
# if CppADMyVectorOmit != 7
	// destructor (it is not safe to delete the pointer in cases 4 and 7)
	~MyVector(void)
	{	delete [] data; }
# endif
# endif
# if CppADMyVectorOmit != 5
	// size function
	inline size_t size(void) const
	{	return length; }
# endif
# if CppADMyVectorOmit != 6
	// resize function
	inline void resize(size_t n)
	{	if( length > 0 )
			delete [] data;
		length = n;
		if( length > 0 )
			data = new Scalar[length];
		else	data = 0;
	}
# endif
# if CppADMyVectorOmit != 7
	// assignment operator
	inline MyVector & operator=(const MyVector &x)
	{	size_t i;
		for(i = 0; i < length; i++)
			data[i] = x.data[i];
		return *this;
	}
# endif
# if CppADMyVectorOmit != 8
	// non-constant element access
	MyElement<Scalar> operator[](size_t i)
	{	return data + i; }
# endif
# if CppADMyVectorOmit != 9
	// constant element access
	const Scalar & operator[](size_t i) const
	{	return data[i]; }
# endif
};
// -------------------------------------------------------------------------

/*
Compute r = a * v, where a is a scalar with same type as the elements of 
the Simple Vector v. This routine uses the CheckSimpleVector function to ensure that 
the types agree.
*/ 
namespace { // Empty namespace
	template <class Scalar, class Vector>
	Vector Sscal(const Scalar &a, const Vector &v)
	{
		// invoke CheckSimpleVector function 
		CppAD::CheckSimpleVector<Scalar, Vector>();
	
		size_t n = v.size();
		Vector r(n);
	
		size_t i;
		for(i = 0; i < n; i++)
			r[i] = a * v[i];
	
		return r;
	}
}

bool CheckSimpleVector(void)
{	bool ok  = true;
	using CppAD::vector;

	// --------------------------------------------------------
	// If you change double to float in the next statement,
	// CheckSimpleVector will generate an error message at compile time.
	double a = 3.;
	// --------------------------------------------------------

	size_t n = 2;
	MyVector<double> v(n);
	v[0]     = 1.;
	v[1]     = 2.;
	MyVector<double> r = Sscal(a, v);
	ok      &= (r[0] == 3.);
	ok      &= (r[1] == 6.);

	return ok;
}

// END PROGRAM
