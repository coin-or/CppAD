# ifndef CPPAD_VEC_AD_INCLUDED
# define CPPAD_VEC_AD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin VecAD$$
$spell
	CondExpGt
	grep
	Ld
	vp
	Lu
	wc
	op
	Ldp
	Ldv
	Taylor
	VecAD
	const
	Cpp
$$

$index VecAD$$
$index vector, AD index$$
$index record, AD index$$
$index tape, AD index$$
$index index, AD record$$

$section AD Vectors that Record Index Operations$$


$head Syntax$$
$syntax%VecAD<%Base%> %v%(%n%)%$$
$pre
$$
$syntax%%v%.size()%$$
$pre
$$
$syntax%%b% = %v%[%i%]%$$
$pre
$$
$syntax%%y% = %v%[%x%]%$$


$head Purpose$$
If the tape for $xref/glossary/AD of Base/AD of/$$ $italic Base$$ operations is in the
$xref/glossary/Tape State/Recording State/$$,
the indexing operation
$syntax%
	%y% = %v%[%x%]
%$$
is included in the
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ and 
transferred to the corresponding $xref/ADFun/$$ object $italic f$$.
Such an index can change each time
zero order $xref/Forward//f.Forward/$$ is used; i.e.,
$italic f$$ is evaluated with new value for the 
$xref/glossary/Independent Variable/independent variables/$$. 
Note that the value of $italic y$$ depends on the value of $italic x$$
in a discrete fashion and CppAD computes its partial derivative with 
respect to $italic x$$ as zero.

$head Alternatives$$
If only the values in the vector, 
and not the indices, 
depend on the independent variables,
the class $syntax%%Vector%< AD<%Base%> >%$$ is much more efficient for
storing AD values where $italic Vector$$ is any
$xref/SimpleVector/$$ template class, 
If only the indices, 
and not the values in the vector,
depend on the independent variables,
The $xref/Discrete/$$ functions are a much more efficient
way to represent these vectors.

$head VecAD<Base>::reference$$
$index VecAD<Base>::reference$$
$index reference, VecAD<Base>$$
The result $italic y$$ has type
$syntax%
	VecAD<%Base%>::reference
%$$ 
which is very much like the $syntax%AD<%Base%>%$$ type 
with some notable exceptions:

$subhead Exceptions$$
$list number$$
The object $italic y$$ cannot be used with the
$xref/Value/$$ function to compute the corresponding $italic Base$$ value.
This function can only be use while the corresponding tape is in the 
$xref/glossary/Tape State/Empty state/$$.  
In this case, the syntax
$syntax%
	v[%i%]
%$$
should be used to compute the corresponding $italic Base$$ value.

$lnext
The object $italic y$$ cannot be used
with the $xref/Arithmetic//computed assignments operators/$$ 
$code +=$$, 
$code -=$$, 
$code *=$$, or
$code /=$$.
For example, the following syntax is not valid:
$syntax%
	%v%[%x%] += %z%;
%$$
no matter what the types of $italic z$$.

$lnext
$xref/Eq//Assignment/$$ to $italic y$$ returns a $code void$$.
For example, the following syntax is not valid:
$syntax%
	%z% = %v%[%x%] = %u%;
%$$
no matter what the types of $italic z$$, and $italic u$$.

$lnext
The $xref/CondExp/$$ functions do not accept 
$syntax%VecAD<%Base%>::reference%$$ arguments.
For example, the following syntax is not valid:
$syntax%
	CondExpGt(%y%, %z%, %u%, %v%)
%$$
no matter what the types of $italic z$$, $italic u$$, and $italic v$$.

$lnext
The $xref/ParVar//Parameter and Variable/$$ functions cannot be used with
$syntax%VecAD<%Base%>::reference%$$ arguments.

$lnext
The vectors passed to $xref/Independent/$$ must have elements
of type $syntax%AD<%Base%>%$$; i.e., $xref/VecAD/$$ vectors
cannot be passed to $code Independent$$.

$lnext
If one uses this type in a 
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$,
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ calculations
($xref/Sparse/$$)
are only valid for the current independent variable values,
instead of valid for all independent variables.

$lend

$head n$$
The argument $italic n$$ has prototype
$syntax%
	size_t %n%
%$$

$head v$$
The syntax 
$syntax%
	VecAD<%Base%> %v%(%n%)
%$$
creates an $code VecAD$$ object $italic v$$ with 
$italic n$$ elements.
The initial value of the elements of $italic v$$ is unspecified.

$head size$$
The syntax
$syntax%
	%v%.size()
%$$
returns the number of elements in the vector $italic v$$;
i.e., the value of $italic n$$ when it was constructed.

$head i$$
The operand $italic i$$ has prototype
$syntax%
	size_t %i%
%$$
It must be greater than or equal zero
and less than $italic n$$; i.e., less than
the number of elements in $italic v$$. 

$head b$$
The result $italic b$$ has prototype
$syntax%
	%Base% &%b%
%$$
and is a reference to the $th i$$ element in the vector $italic v$$.
This syntax is only valid if the
tape that records $xref/glossary/AD of Base/AD of/$$ $italic Base$$ operations is in the
$xref/glossary/Tape State/Empty state/$$.
Because it is a reference, it can be used to change the element value;
for example,
$syntax%
	%v%[%i%] = %c%
%$$
is valid where $italic c$$ is a $italic Base$$ object.
As a reference, it is no longer valid once the
destructor for $italic v$$ is called; for example,
when $italic v$$ falls out of scope.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$
The value of $italic x$$ must be greater than or equal zero
and less than $italic n$$; i.e., less than
the number of elements in $italic v$$. 

$head y$$
The result $italic y$$ has prototype
$syntax%
	VecAD<%Base%>::reference %y%
%$$
The object $italic y$$ has an AD type and its 
operations are recorded as part of the same
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ as
for $syntax%AD<%Base%>%$$ objects.
It acts as a reference to the 
element with index $latex {\rm floor} (x)$$ in the vector $italic v$$
($latex {\rm floor} (x)$$ is 
the greatest integer less than or equal $italic x$$).
Because it is a reference, it can be used to change the element value;
for example,
$syntax%
	%v%[%x%] = %z%
%$$
is valid where $italic z$$ is an
$syntax%VecAD<%Base%>::reference%$$ object.
As a reference, it is no longer valid once the
destructor for $italic v$$ is called; for example,
when $italic v$$ falls out of scope.

$head Example$$
$children%
	Example/VecAD.cpp
%$$
The file
$xref/VecAD.cpp/$$
contains an example and test using $code VecAD$$ vectors.
It returns true if it succeeds and false otherwise.


$head Speed and Memory$$
The $xref/VecAD/$$ vector type is inefficient because every
time an element of a vector is accessed, a new CppAD 
$xref/glossary/Variable/variable/$$ is created on the tape
using either the $code Ldp$$ or $code Ldv$$ operation
(unless all of the elements of the vector are
$xref/glossary/Parameter/parameters/$$).
The effect of this can be seen by executing the following steps:

$list number$$
In the file $code CppAD/local/ForwardSweep.h$$,
change the definition of $code CppADForwardSweepTrace$$ to
$codep
	# define CppADForwardSweepTrace 1
$$
$lnext
In the $code Example$$ directory, execute the command
$codep
	./OneTest LuVecADOk "LuVecAD.cpp -DNDEBUG" > LuVecADOk.log
$$
This will write a trace of all the forward tape operations,
for the test case $xref/LuVecADOk.cpp/$$,
to the file $code LuVecADOk.log$$.
$lnext
In the $code Example$$ directory execute the commands
$codep
	grep "op="           LuVecADOk.log | wc -l
	grep "op=Ld[vp]"     LuVecADOk.log | wc -l
	grep "op=St[vp][vp]" LuVecADOk.log | wc -l
$$
The first command counts the number of operators in the tracing,
the second counts the number of VecAD load operations,
and the third counts the number of VecAD store operations.
(For CppAD version 05-11-20 these counts were 956, 348, and 118
respectively.)
$lend

$end
------------------------------------------------------------------------ 
*/

# define CPPAD_VEC_AD_COMPUTED_ASSIGNMENT(op, name)                     \
VecAD_reference& operator op (const VecAD_reference<Base> &right)       \
{	CppADUsageError(                                                \
		0,                                                      \
		"Cannot use a ADVec element on left side of" name       \
	);                                                              \
	return *this;                                                   \
}                                                                       \
VecAD_reference& operator op (const AD<Base> &right)                    \
{	CppADUsageError(                                                \
		0,                                                      \
		"Cannot use a ADVec element on left side of" name       \
	);                                                              \
	return *this;                                                   \
}                                                                       \
VecAD_reference& operator op (const Base &right)                        \
{	CppADUsageError(                                                \
		0,                                                      \
		"Cannot use a ADVec element on left side of" name       \
	);                                                              \
	return *this;                                                   \
}


//  BEGIN CppAD namespace
namespace CppAD {

// Element of VecAD
template <class Base>
class VecAD_reference {
	friend class VecAD<Base>;
	friend class ADTape<Base>;

public:
	VecAD_reference(VecAD<Base> *v, const AD<Base> &x_) 
		: vec( v ) , x(x_)
	{ }

	// assignment operators
	inline void operator = (const VecAD_reference<Base> &right);
	void operator = (const AD<Base> &right);
	void operator = (const Base     &right);
	void operator = (int             right);

	// computed assignments
	CPPAD_VEC_AD_COMPUTED_ASSIGNMENT( += , " += " )
	CPPAD_VEC_AD_COMPUTED_ASSIGNMENT( -= , " -= " )
	CPPAD_VEC_AD_COMPUTED_ASSIGNMENT( *= , " *= " )
	CPPAD_VEC_AD_COMPUTED_ASSIGNMENT( /= , " /= " )


	// AD<Base> constructor
	AD<Base> ADBase(void) const
	{	AD<Base> result;
		CppADUnknownError( Parameter(result) );

		size_t i = static_cast<size_t>( Integer(x) );
		CppADUnknownError( i < vec->length );

		// value corresponding to this element
		result.value = *(vec->data + i);

		// index corresponding to this element
		if( AD<Base>::Tape()->State() == Empty )
		{	CppADUnknownError( vec->id != *ADTape<Base>::Id() );
		}
		else
		{	CppADUnknownError( vec->id == *ADTape<Base>::Id() );
			CppADUnknownError( vec->offset > 0  );
	
			if( IdenticalPar(x) )
			{	// use parameter indexing
				AD<Base>::Tape()->RecordLoadOp(
					LdpOp,
					result,
					vec->offset,
					static_cast<size_t>(i)
				);
				CppADUnknownError( Parameter(x) );
			}
			else
			{	// check if we need to convert x to a variable
				// note that x is mutable
				if( Parameter(x) )
				{	x.id = *ADTape<Base>::Id();
					x.taddr = 
					AD<Base>::Tape()->RecordParOp(x.value);
				}
	
				// use variable indexing
				AD<Base>::Tape()->RecordLoadOp(
					LdvOp,
					result,
					vec->offset,
					x.taddr
				);
				CppADUnknownError( 
					x.taddr > 0 && Variable(x)
				);
			}
		}
		return result;
	}

private:
	VecAD<Base>      *vec;         // pointer to entire vector
	mutable AD<Base>  x;           // index for this element
};

// VecAD
template <class Base>
class VecAD {
	// friends
	friend std::ostream& operator << <Base>
		(std::ostream &os, const VecAD<Base> &vec);

	friend class ADTape<Base>;
	friend class VecAD_reference<Base>;
public:
	// declare the user's view of this type here
	typedef VecAD_reference<Base> reference;

	// default constructor
	VecAD(void) : length(0) , data(CppADNull)
	{ }

	// constructor 
	VecAD(size_t n) : length(n) , id(0)
	{	CppADUnknownError( *ADTape<Base>::Id() > id );
		data  = CppADNull;
		if( length > 0 )
		{	size_t i;
			Base zero(0);
			data  = CppADTrackNewVec(length, data);

			// Initialize data to zero so all have same value.
			// This uses less memory and avoids a valgrind error
			// during TapeRec<Base>::PutPar 
			for(i = 0; i < length; i++)
				data[i] = zero;
		}
	}

	// destructor
	~VecAD(void)
	{	if( data != CppADNull )
			CppADTrackDelVec(data); 
	}

	// size
	size_t size(void)
	{	return length; }

	// not taped element access
	Base &operator[](size_t i)
	{
		CppADUsageError( 
			AD<Base>::Tape()->State() == Empty,
			"VecAD: cannot use size_t indexing while recording"
		);
		CppADUsageError(
			i < length,
			"VecAD: element index is >= vector length"
		);

		return data[i];
	}

	// taped elemement access
	VecAD_reference<Base> operator[](const AD<Base> &x) 
	{
		CppADUnknownError( 
			( id != *ADTape<Base>::Id() )
			| ( AD<Base>::Tape()->State() == Recording )
		);
		CppADUsageError(
			0 <= Integer(x),
			"VecAD: element index is < zero"
		);
		CppADUsageError(
			static_cast<size_t>( Integer(x) ) < length,
			"VecAD: element index is >= vector length"
		);

		// if no need to track indexing operation, return now
		if( (AD<Base>::Tape()->State() != Recording) )
			return VecAD_reference<Base>(this, x);

		if( id != *ADTape<Base>::Id() )
		{	// must place a copy of vector in tape
			offset = AD<Base>::Tape()->AddVec(length, data);

			// advance pointer by one so is always nonzero
			offset++; 
			CppADUnknownError( offset > 0 );

			// tape id corresponding to this offest
			id = *ADTape<Base>::Id();
		}

		return VecAD_reference<Base>(this, x); 
	}

private:
	const  size_t   length; // size of this VecAD vector
	Base           *data;   // value of elements of this vector 

	// offset in cumulate vector corresponding to this object
	size_t offset; 

	// tape id corresponding to the offset
	size_t id;
};


template <class Base>
void VecAD_reference<Base>::operator=(const AD<Base> &y)
{
	if( Parameter(y) )
	{	*this = y.value;
		return;
	}

	CppADUnknownError( AD<Base>::Tape()->State() == Recording );

	size_t i = static_cast<size_t>( Integer(x) );
	CppADUnknownError( i < vec->length );

	// assign value both in the element and the original array
	*(vec->data + i) = y.value;

	// record the setting of this array element
	CppADUnknownError( vec->id == *ADTape<Base>::Id() );
	CppADUnknownError( vec->offset > 0 );
	if( Parameter(x) ) AD<Base>::Tape()->RecordStoreOp(
			StpvOp, vec->offset, i, y.taddr );
	else	AD<Base>::Tape()->RecordStoreOp(
			StvvOp, vec->offset, x.taddr, y.taddr );
}

template <class Base>
void VecAD_reference<Base>::operator=(const Base &y)
{ 
	size_t y_taddr;

	size_t i = static_cast<size_t>( Integer(x) );
	CppADUnknownError( i < vec->length );

	// assign value both in the element and the original array
	*(vec->data + i) = y;

	if( AD<Base>::Tape()->State() == Empty )
		return;

	// place a copy of y in the tape
	y_taddr = AD<Base>::Tape()->Rec.PutPar(y);

	// record the setting of this array element
	CppADUnknownError( vec->id == *ADTape<Base>::Id() );
	CppADUnknownError( vec->offset > 0 );
	if( Parameter(x) ) AD<Base>::Tape()->RecordStoreOp(
			StppOp, vec->offset, i, y_taddr );
	else	AD<Base>::Tape()->RecordStoreOp(
			StvpOp, vec->offset, x.taddr, y_taddr );
}

// fold this case into AD<Base> case above
template <class Base>
inline void VecAD_reference<Base>::operator=(const VecAD_reference<Base> &y)
{	*this = y.ADBase(); }

// fold this case into Base case above
template <class Base>
inline void VecAD_reference<Base>::operator=(int y)
{	*this = Base(y); }

template <class Base>
inline std::ostream& operator << (std::ostream &os, const VecAD<Base> &v)
{
 	os << "vecAD( length = " << v.length 
	   << ", offset = "      << v.offset << ")";
	return os;
}


} // END CppAD namespace

# undef CPPAD_VEC_AD_COMPUTED_ASSIGNMENT

# endif
