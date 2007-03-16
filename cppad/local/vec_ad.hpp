# ifndef CPPAD_VEC_AD_INCLUDED
# define CPPAD_VEC_AD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin VecAD$$
$spell
	cppad.hpp
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
$syntax%%r% = %v%[%x%]%$$


$head Purpose$$
If either $italic v$$ or $italic x$$ is a 
$cref/variable/glossary/Variable/$$,
the indexing operation
$syntax%
	%y% = %v%[%x%]
%$$
is recorded in the corresponding
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ and 
transferred to the corresponding $xref/ADFun/$$ object $italic f$$.
Such an index can change each time
zero order $xref/Forward//f.Forward/$$ is used; i.e.,
$italic f$$ is evaluated with new value for the 
$cref/independent variables/glossary/Tape/Independent Variable/$$. 
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
If $italic v$$ is not a $cref/variable/glossary/Variable/$$
$syntax%
	v[%i%]
%$$
can be used to compute the corresponding $italic Base$$ value.

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
$syntax%VecAD<%Base%>::reference%$$ arguments
(use the entire $syntax%VecAD<%Base%>%$$ vector instead).

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

$head Constructor$$

$subhead v$$
The syntax 
$syntax%
	VecAD<%Base%> %v%(%n%)
%$$
creates an $code VecAD$$ object $italic v$$ with 
$italic n$$ elements.
The initial value of the elements of $italic v$$ is unspecified.

$head n$$
The argument $italic n$$ has prototype
$syntax%
	size_t %n%
%$$

$head size$$
The syntax
$syntax%
	%v%.size()
%$$
returns the number of elements in the vector $italic v$$;
i.e., the value of $italic n$$ when it was constructed.

$head size_t Indexing$$
We refer to the syntax
$syntax%
	%b% = %v%[%i%]
%$$
as $code size_t$$ indexing of a $code VecAD$$ object.
This indexing is only valid if the vector $italic v$$ is a 
$cref/parameter/ParVar/$$; i.e.,
it does not depend on the independent variables.

$subhead i$$
The operand $italic i$$ has prototype
$syntax%
	size_t %i%
%$$
It must be greater than or equal zero
and less than $italic n$$; i.e., less than
the number of elements in $italic v$$. 

$subhead b$$
The result $italic b$$ has prototype
$syntax%
	%Base% %b%
%$$
and is a reference to the $th i$$ element in the vector $italic v$$.
It can be used to change the element value;
for example,
$syntax%
	%v%[%i%] = %c%
%$$
is valid where $italic c$$ is a $italic Base$$ object.
The reference $italic b$$ is no longer valid once the
destructor for $italic v$$ is called; for example,
when $italic v$$ falls out of scope.

$head AD Indexing$$
We refer to the syntax
$syntax%
	%r% = %v%[%x%]
%$$
as AD indexing of a $code VecAD$$ object.

$subhead x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$
The value of $italic x$$ must be greater than or equal zero
and less than $italic n$$; i.e., less than
the number of elements in $italic v$$. 

$subhead r$$
The result $italic y$$ has prototype
$syntax%
	VecAD<%Base%>::reference %r%
%$$
The object $italic r$$ has an AD type and its 
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
As a reference, $italic r$$ is no longer valid once the
destructor for $italic v$$ is called; for example,
when $italic v$$ falls out of scope.

$head Example$$
$children%
	example/vec_ad.cpp
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
In the file $code cppad/local/ForwardSweep.h$$,
change the definition of $code CppADForwardSweepTrace$$ to
$codep
	# define CppADForwardSweepTrace 1
$$
$lnext
In the $code Example$$ directory, execute the command
$codep
	./OneTest LuVecADOk "lu_vec_ad.cpp -DNDEBUG" > LuVecADOk.log
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
	friend bool  Parameter<Base> (const VecAD<Base> &vec);
	friend bool  Variable<Base>  (const VecAD<Base> &vec);
	friend class VecAD<Base>;
	friend class ADTape<Base>;

public:
	VecAD_reference(VecAD<Base> *v, const AD<Base> &x) 
		: vec_( v ) , x_(x)
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

		size_t i = static_cast<size_t>( Integer(x_) );
		CppADUnknownError( i < vec_->length_ );

		// value_ corresponding to this element
		result.value_ = *(vec_->data_ + i);

		// index corresponding to this element
		if( Variable(*vec_) )
		{
			ADTape<Base> *tape = AD<Base>::tape_ptr(vec_->id_);
			CppADUnknownError( tape != CPPAD_NULL );

			CppADUnknownError( vec_->offset_ > 0  );
	
			if( IdenticalPar(x_) )
			{	// use parameter indexing
				tape->RecordLoadOp(
					LdpOp,
					result,
					vec_->offset_,
					static_cast<size_t>(i)
				);
				CppADUnknownError( Parameter(x_) );
			}
			else
			{	// check if we need to convert x to a variable
				// note that x is mutable
				if( Parameter(x_) )
				{	x_.id_ = vec_->id_;
					x_.taddr_ = 
					tape->RecordParOp(x_.value_);
				}
	
				// use variable indexing
				tape->RecordLoadOp(
					LdvOp,
					result,
					vec_->offset_,
					x_.taddr_
				);
				CppADUnknownError( 
					x_.taddr_ > 0 && Variable(x_)
				);
			}
		}
		return result;
	}

private:
	VecAD<Base>      *vec_;         // pointer to entire vector
	mutable AD<Base>  x_;           // index for this element
};

// VecAD
template <class Base>
class VecAD {
	friend bool  Parameter<Base> (const VecAD<Base> &vec);
	friend bool  Variable<Base>  (const VecAD<Base> &vec);
	friend class ADTape<Base>;
	friend class VecAD_reference<Base>;

	friend std::ostream& operator << <Base>
		(std::ostream &os, const VecAD<Base> &vec_);
public:
	// declare the user's view of this type here
	typedef VecAD_reference<Base> reference;

	// default constructor
	// initialize id_ to one so not a variable
	VecAD(void) : length_(0) , data_(CPPAD_NULL), offset_(0), id_(1)
	{ CppADUnknownError( Parameter(*this) ); }

	// constructor 
	// initialize id_ to one so not a variable
	VecAD(size_t n) : length_(n), offset_(0), id_(1)
	{	data_  = CPPAD_NULL;
		if( length_ > 0 )
		{	size_t i;
			Base zero(0);
			data_  = CppADTrackNewVec(length_, data_);

			// Initialize data to zero so all have same value.
			// This uses less memory and avoids a valgrind error
			// during TapeRec<Base>::PutPar 
			for(i = 0; i < length_; i++)
				data_[i] = zero;
		}
		CppADUnknownError( Parameter(*this) );
	}

	// destructor
	~VecAD(void)
	{	if( data_ != CPPAD_NULL )
			CppADTrackDelVec(data_); 
	}

	// size
	size_t size(void)
	{	return length_; }

	// not taped element access
	Base &operator[](size_t i)
	{
		CppADUsageError( 
			Parameter(*this),
			"VecAD: cannot use size_t indexing because this"
			" VecAD vector is a variable."
		);
		CppADUsageError(
			i < length_,
			"VecAD: element index is >= vector length"
		);

		return data_[i];
	}

	// taped elemement access
	VecAD_reference<Base> operator[](const AD<Base> &x) 
	{
		CppADUsageError(
			0 <= Integer(x),
			"VecAD: element index is < zero"
		);
		CppADUsageError(
			static_cast<size_t>( Integer(x) ) < length_,
			"VecAD: element index is >= vector length"
		);

		// if no need to track indexing operation, return now
		if( Parameter(*this) & Parameter(x) )
			return VecAD_reference<Base>(this, x);

		CppADUsageError( 
			Parameter(*this) | Parameter(x) | (id_ == x.id_),
			"VecAD: vector and index are variables for"
			" different tapes."
		);

		if( Parameter(*this) )
		{	// must place a copy of vector in tape
			offset_ = 
			AD<Base>::tape_ptr(x.id_)->AddVec(length_, data_);

			// advance pointer by one so is always > 0
			offset_++; 

			// tape id corresponding to this offest
			id_ = x.id_;
		}

		return VecAD_reference<Base>(this, x); 
	}

private:
	const  size_t   length_; // size of this VecAD vector
	Base           *data_;   // value_ of elements of this vector 

	// offset in cumulate vector corresponding to this object
	size_t offset_; 

	// tape id corresponding to the offset
	size_t id_;
};


template <class Base>
void VecAD_reference<Base>::operator=(const AD<Base> &y)
{
	if( Parameter(y) )
	{	// fold into the Base type assignment
		*this = y.value_;
		return;
	}

	CppADUsageError( 
		Parameter(*vec_) | (vec_->id_ == y.id_),
		"VecAD: vector and new element value are variables"
		"\nfor different tapes."
	);

	ADTape<Base> *tape = AD<Base>::tape_ptr(y.id_);
	CppADUnknownError( tape != CPPAD_NULL );
	if( Parameter(*vec_) )
	{	// must place a copy of vector in tape
		vec_->offset_ = tape->AddVec(vec_->length_, vec_->data_);

		// advance pointer by one so is always > 0
		(vec_->offset_)++; 

		// tape id corresponding to this offest
		vec_->id_ = y.id_;
	}
	CppADUnknownError( Variable(*vec_) );


	size_t i = static_cast<size_t>( Integer(x_) );
	CppADUnknownError( i < vec_->length_ );

	// assign value both in the element and the original array
	*(vec_->data_ + i) = y.value_;

	// record the setting of this array element
	CppADUnknownError( vec_->offset_ > 0 );
	if( Parameter(x_) ) tape->RecordStoreOp(
			StpvOp, vec_->offset_, i, y.taddr_ );
	else	tape->RecordStoreOp(
			StvvOp, vec_->offset_, x_.taddr_, y.taddr_ );
}

template <class Base>
void VecAD_reference<Base>::operator=(const Base &y)
{ 
	size_t y_taddr;

	size_t i = static_cast<size_t>( Integer(x_) );
	CppADUnknownError( i < vec_->length_ );

	// assign value both in the element and the original array
	*(vec_->data_ + i) = y;

	// check if this ADVec object is a parameter
	if( Parameter(*vec_) )
		return;

	ADTape<Base> *tape = AD<Base>::tape_ptr(vec_->id_);
	CppADUnknownError( tape != CPPAD_NULL );

	// place a copy of y in the tape
	y_taddr = tape->Rec.PutPar(y);

	// record the setting of this array element
	CppADUnknownError( vec_->offset_ > 0 );
	if( Parameter(x_) ) tape->RecordStoreOp(
			StppOp, vec_->offset_, i, y_taddr );
	else	tape->RecordStoreOp(
			StvpOp, vec_->offset_, x_.taddr_, y_taddr );
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
 	os << "vecAD( length = " << v.length_ 
	   << ", offset = "      << v.offset_ << ")";
	return os;
}


} // END CppAD namespace

# undef CPPAD_VEC_AD_COMPUTED_ASSIGNMENT

# endif
