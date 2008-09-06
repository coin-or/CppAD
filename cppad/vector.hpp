# ifndef CPPAD_VECTOR_INCLUDED
# define CPPAD_VECTOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin CppAD_vector$$
$spell
	cppad.hpp
	Bool
	resize
	cout
	endl
	std
	Cpp
	const
	vec
	ostream
	elem
$$

$index vector, CppAD template class$$
$index class, template CppAD vector$$
$index template, CppAD vector class$$

$section The CppAD::vector Template Class$$

$head Syntax$$
$code # include <cppad/vector.hpp>$$



$head Description$$
The include file $code cppad/vector.hpp$$ defines the
vector template class $code CppAD::vector$$.
This is a $xref/SimpleVector/$$ template class and in addition
it has the features listed below:

$head Include$$
The file $code cppad/vector.hpp$$ is included by $code cppad/cppad.hpp$$
but it can also be included separately with out the rest of the 
CppAD include files.

$head Assignment$$
$index assignment, CppAD vector$$
If $icode x$$ and $icode y$$ are 
$codei%CppAD::vector<%Scalar%>%$$ objects,
$codei%
	%y% = %x%
%$$
has all the properties listed for a
$xref/SimpleVector/Assignment/simple vector assignment/$$
plus the following:
$pre

$$
The $code CppAD::vector$$ template class will check that
the size of $icode x$$ is equal to the size of $icode y$$
before doing the assignment.
If the sizes are not equal, $code CppAD::vector$$ will use
$xref/ErrorHandler/$$
to generate an appropriate error report.
$pre

$$
A reference to the vector $icode y$$ is returned.
An example use of this reference is in multiple assignments of the form
$codei%
	%z% = %y% = %x%
%$$

$head Element Access$$
$index [], CppAD vector$$
$index vector, [] CppAD$$
If $icode x$$ is a $codei%CppAD::vector<%Scalar%>%$$ object
and $code i$$ has type $code size_t$$,
$codei%
	%x%[%i%]
%$$
has all the properties listed for a
$xref/SimpleVector/Element Access/simple vector element access/$$
plus the following:
$pre

$$
The object $codei%%x%[%i%]%$$ has type $icode Scalar$$
(is not possibly a different type that can be converted to $icode Scalar$$).
$pre

$$
If $icode i$$ is not less than the size of the $icode x$$,
$code CppAD::vector$$ will use
$xref/ErrorHandler/$$
to generate an appropriate error report.

$head push_back$$
$index push_back, CppAD vector$$
$index vector, CppAD push_back$$
If $icode x$$ is a $codei%CppAD::vector<%Scalar%>%$$ object
with size equal to $icode n$$ and
$icode s$$ has type $icode Scalar$$,
$codei%
	%x%.push_back(%s%)
%$$
extends the vector $icode x$$ so that its new size is $icode n$$ plus one
and $codei%%x%[%n%]%$$ is equal to $icode s$$
(equal in the sense of the $icode Scalar$$ assignment operator).


$head push_vector$$
$index push_vector, CppAD$$
$index vector, CppAD push$$
If $icode x$$ is a $codei%CppAD::vector<%Scalar%>%$$ object
with size equal to $icode n$$ and
$icode v$$ is a $cref/simple vector/SimpleVector/$$
with elements of type $icode Scalar$$ and size $icode m$$,
$codei%
	%x%.push_vector(%v%)
%$$
extends the vector $icode x$$ so that its new size is $icode%n%+%m%$$
and $icode%x%[%n% + %i%]%$$ is equal to $icode%v%[%i%]%$$
for $icode%i = 1 , ... , m-1%$$
(equal in the sense of the $icode Scalar$$ assignment operator).

$head Output$$
If $icode x$$ is a $codei%CppAD::vector<%Scalar%>%$$ object
and $icode os$$ is an $code std::ostream$$,
and the operation
$codei%
	%os% << %x%
%$$
will output the vector $icode x$$ to the standard
output stream $icode os$$.
The elements of $icode x$$ are enclosed at the beginning by a
$code {$$ character,
they are separated by $code ,$$ characters,
and they are enclosed at the end by $code }$$ character.
It is assumed by this operation that if $icode e$$
is an object with type $icode Scalar$$,
$codei%
	%os% << %e%
%$$
will output the value $icode e$$ to the standard
output stream $icode os$$.

$head resize$$
If the $code resize$$ member function is called with argument
value zero, all memory allocated for the vector will be freed.
The can be useful when using very large vectors
and when checking for memory leaks (and there are global vectors).

$head vectorBool$$
$index vectorBool$$
The file $code <cppad/vector.hpp>$$ also defines the class
$code CppAD::vectorBool$$.
This has the same specifications as $code CppAD::vector<bool>$$ 
with the following exceptions

$list number $$
The class $code vectorBool$$ conserves on memory
(on the other hand, $code CppAD::vector<bool>$$ is expected to be faster
than $code vectorBool$$).

$lnext
The $code CppAD::vectorBool$$ output operator
prints each boolean value as 
a $code 0$$ for false,
a $code 1$$ for true, 
and does not print any other output; i.e.,
the vector is written a long sequence of zeros and ones with no
surrounding $code {$$, $code }$$ and with no separating commas or spaces. 

$lnext
If $icode x$$ has type $code vectorBool$$
and $icode i$$ has type $code size_t$$,
the element access value $codei%%x%[%i%]%$$ has an unspecified type
(referred to here as $icode elementType$$)
that can be implicitly converted to $code bool$$.
The return value of the assignment operator
$codei%
	%x%[%i%] = %y%
%$$
also has type $icode elementType$$. Thus, if $icode z$$
has type $code bool$$, the syntax
$codei%
	%z% = %x%[%i%] = %y%
%$$
is valid.

$lend

$head Example$$
$children%
	example/cppad_vector.cpp%
	example/vector_bool.cpp
%$$
The files
$xref/CppAD_vector.cpp/$$ and
$xref/vectorBool.cpp/$$ each
contain an example and test of this template class.
They return true if they succeed and false otherwise.

$head Exercise$$
$index exercise, CppAD::vector$$
Create and run a program that contains the following code:
$codep
	CppAD::vector<double> x(3);
	size_t i;
	for(i = 0; i < 3; i++)
		x[i] = 4. - i;
	std::cout << "x = " << x << std::endl;
$$

$end


$end

------------------------------------------------------------------------ 
*/

# include <cstddef>
# include <iostream>
# include <limits>
# include <cppad/local/cppad_assert.hpp>
# include <cppad/track_new_del.hpp>
# include <cppad/check_simple_vector.hpp>

# ifndef CPPAD_NULL
# define CPPAD_NULL 0
# endif

namespace CppAD { //  BEGIN CppAD namespace

// ------------------ CppAD::vector<Type> ----------------------------------

template <class Type>
class vector {
private:
	size_t capacity;
	size_t length;
	Type   * data;
public:
	// type of the elements in the vector
	typedef Type value_type;

	// default constructor
	inline vector(void) : capacity(0), length(0) , data(CPPAD_NULL)
	{ }
	// constructor with a specified size
	inline vector(size_t n) : capacity(n), length(n)
	{
		data = CPPAD_NULL;
		if( length > 0 )
			data = CPPAD_TRACK_NEW_VEC(capacity, data);
	}
	// copy constructor
	inline vector(const vector &x) : capacity(x.length), length(x.length)
	{	size_t i;
		data = CPPAD_NULL;
		if( length > 0 )
			data = CPPAD_TRACK_NEW_VEC(capacity, data);

		for(i = 0; i < length; i++)
			data[i] = x.data[i];
	}
	// destructor
	~vector(void)
	{	if( data != CPPAD_NULL )
			CPPAD_TRACK_DEL_VEC(data); 
	}

	// size function
	inline size_t size(void) const
	{	return length; }

	// resize function
	inline void resize(size_t n)
	{	length = n;
		if( (capacity >= n) & (n > 0)  )
			return;
		if( data != CPPAD_NULL  )
			CPPAD_TRACK_DEL_VEC(data);
		capacity = n;
		if( capacity == 0 )
			data = CPPAD_NULL;
		else	data = CPPAD_TRACK_NEW_VEC(capacity, data);
	}
	// assignment operator
	inline vector & operator=(const vector &x)
	{	size_t i;
		CPPAD_ASSERT_KNOWN(
			length == x.length ,
			"size miss match in assignment operation"
		);
		for(i = 0; i < length; i++)
			data[i] = x.data[i];
		return *this;
	}
	// non-constant element access
	Type & operator[](size_t i)
	{	CPPAD_ASSERT_KNOWN(
			i < length,
			"vector index greater than or equal vector size"
		);
		return data[i]; 
	}
	// constant element access
	const Type & operator[](size_t i) const
	{	CPPAD_ASSERT_KNOWN(
			i < length,
			"vector index greater than or equal vector size"
		);
		return data[i]; 
	}
	// add scalar to the back of the array
	void push_back(const Type &s)
	{	CPPAD_ASSERT_UNKNOWN( length <= capacity );
		if( length + 1 > capacity )
		{	// allocate more capacity
			if( capacity == 0 )
				capacity = 2;
			else	capacity = 2 * length;
			data = CPPAD_TRACK_EXTEND(capacity, length, data);
		}
		data[length++] = s;
		CPPAD_ASSERT_UNKNOWN( length <= capacity );
	}

	// add vector to the back of the array
	// (Cannot use push_back becasue MS V++ 7.1 does not resolve
	// to non-template member function when scalar is used.)
	template <class Vector>
	void push_vector(const Vector &v)
	{	CheckSimpleVector<Type, Vector>();
		size_t m = v.size();
		CPPAD_ASSERT_UNKNOWN( length <= capacity );
		if( length + m > capacity )
		{	// allocate more capacity
			capacity = length + m;
			data     = CPPAD_TRACK_EXTEND(capacity, length, data);
		}
		size_t i;
		for(i = 0; i < m; i++)
			data[length++] = v[i];
		CPPAD_ASSERT_UNKNOWN( length <= capacity );
	}
};

// output operator
template <class Type>
inline std::ostream& operator << (
	std::ostream              &os  , 
	const CppAD::vector<Type> &vec )
{	size_t i = 0;
	size_t n = vec.size();

	os << "{ ";
	while(i < n)
	{	os << vec[i++]; 
		if( i < n )
			os << ", ";
	}
	os << " }";
	return os;
}

/*
--------------------------- vectorBool -------------------------------------
*/
class vectorBoolElement {
	typedef size_t UnitType;
private:
	UnitType *unit;
	UnitType mask;
public:
	vectorBoolElement(UnitType *unit_, UnitType mask_)
	: unit(unit_) , mask(mask_)
	{ }
	vectorBoolElement(const vectorBoolElement &e)
	: unit(e.unit) , mask(e.mask)
	{ }
	operator bool() const
	{	return (*unit & mask) != 0; }
	vectorBoolElement& operator=(bool bit)
	{	if(bit)
			*unit |= mask;
		else	*unit &= ~mask;
		return *this;
	} 
	vectorBoolElement& operator=(const vectorBoolElement &e)
	{	if( *(e.unit) & e.mask )
			*unit |= mask;
		else	*unit &= ~mask;
		return *this;
	} 
};

class vectorBool {
	typedef size_t UnitType;
private:
	static const  size_t BitPerUnit 
		= std::numeric_limits<UnitType>::digits;
	size_t    nunit;
	size_t    length;
	UnitType *data;
public:
	// type of the elements in the vector
	typedef bool value_type;

	// default constructor
	inline vectorBool(void) : nunit(0), length(0), data(CPPAD_NULL)
	{ }
	// constructor with a specified size
	inline vectorBool(size_t n) : nunit(0), length(0), data(CPPAD_NULL)
	{	if( n == 0 )
			data = CPPAD_NULL;
		else 
		{	nunit    = (n - 1) / BitPerUnit + 1;
			length   = n;
			data     = CPPAD_TRACK_NEW_VEC(nunit, data);
		}
	}
	// copy constructor
	inline vectorBool(const vectorBool &v) 
	: nunit(v.nunit), length(v.length)
	{	size_t i;
		data = CPPAD_NULL;
		if( nunit > 0 )
			data = CPPAD_TRACK_NEW_VEC(nunit, data);

		for(i = 0; i < nunit; i++)
			data[i] = v.data[i];
	}
	// destructor
	~vectorBool(void)
	{	if( data != CPPAD_NULL )
			CPPAD_TRACK_DEL_VEC(data);
	}

	// size function
	inline size_t size(void) const
	{	return length; }

	// resize function
	inline void resize(size_t n)
	{	length = n;
		if( (nunit * BitPerUnit >= n) & (n > 0) )
			return;
		if( data != CPPAD_NULL )
			CPPAD_TRACK_DEL_VEC(data);
		if( n == 0 )
		{	nunit = 0;
			data = CPPAD_NULL;
		}
		else
		{	nunit    = (n - 1) / BitPerUnit + 1;
			data     = CPPAD_TRACK_NEW_VEC(nunit, data);
		}
	}
	// assignment operator
	inline vectorBool & operator=(const vectorBool &v)
	{	size_t i;
		CPPAD_ASSERT_KNOWN(
			length == v.length ,
			"size miss match in assignment operation"
		);
		CPPAD_ASSERT_UNKNOWN( nunit == v.nunit );
		for(i = 0; i < nunit; i++)
			data[i] = v.data[i];
		return *this;
	}
	// non-constant element access
	vectorBoolElement operator[](size_t k)
	{	size_t i, j;
		CPPAD_ASSERT_KNOWN(
			k < length,
			"vector index greater than or equal vector size"
		);
		i    = k / BitPerUnit;
		j    = k - i * BitPerUnit;
		return vectorBoolElement(data + i , UnitType(1) << j );
	}
	// constant element access
	bool operator[](size_t k) const
	{	size_t i, j;
		UnitType unit;
		UnitType mask;
		CPPAD_ASSERT_KNOWN(
			k < length,
			"vector index greater than or equal vector size"
		);
		i    = k / BitPerUnit;
		j    = k - i * BitPerUnit;
		unit = data[i];
		mask = UnitType(1) << j;
		return (unit & mask) != 0;
	}
	// add to the back of the array
	void push_back(bool bit)
	{	size_t i, j;
		UnitType mask;
		CPPAD_ASSERT_UNKNOWN( length <= nunit * BitPerUnit );
		if( length == nunit * BitPerUnit )
		{	// allocate another unit
			data = CPPAD_TRACK_EXTEND(nunit+1, nunit, data);
			nunit++;
		}
		i    = length / BitPerUnit;
		j    = length - i * BitPerUnit;
		mask = UnitType(1) << j;
		if( bit )
			data[i] |= mask;
		else	data[i] &= ~mask;
		length++;
	}
	// add vector to back of array
	void push_vector(const vectorBool &v)
	{	size_t i, j, k;
		UnitType mask;
		bool bit;
		CPPAD_ASSERT_UNKNOWN( length <= nunit * BitPerUnit );
		CPPAD_ASSERT_UNKNOWN( v.length <= v.nunit * BitPerUnit );
		if( length + v.length >= nunit * BitPerUnit )
		{	// allocate enough space
			data = CPPAD_TRACK_EXTEND(nunit+v.nunit, nunit, data);
			nunit += v.nunit;
		}
		for(k = 0; k < v.size(); k++)
		{	i    = length / BitPerUnit;
			j    = length - i * BitPerUnit;
			bit  = v[k];
			mask = UnitType(1) << j;
			if( bit )
				data[i] |= mask;
			else	data[i] &= ~mask;
			length++;
		}
		CPPAD_ASSERT_UNKNOWN( length <= nunit * BitPerUnit );
	}
};

// output operator
inline std::ostream& operator << (
	std::ostream     &os  , 
	const vectorBool &v   )
{	size_t i = 0;
	size_t n = v.size();

	while(i < n)
		os << v[i++]; 
	return os;
}

} // END CppAD namespace


# endif
