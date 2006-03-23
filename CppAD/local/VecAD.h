# ifndef CppADVecIncluded
# define CppADVecIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
/*
$begin VecAD$$
$spell
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

$index vector, record index$$
$index record, index$$
$index tape, index$$
$index index, record$$

$section Vectors That Record Indexing Operations$$


$table 
$bold Syntax$$ $cnext 
$syntax%VecAD<%Base%> %V%(%n%)%$$
$tend

$index VecAD$$
$index tape, index$$
$index record, index$$
$index index, record$$

$fend 20$$

$head Description$$
The class $syntax%VecAD<%Base%>%$$ tapes its indexing operations; i.e.,
these operations are transferred to the corresponding
$xref/ADFun/$$ object $italic f$$.
The indices are evaluated each time
$xref/Forward//f.Forward/$$ is used to evaluate the zero order Taylor
coefficients.

$head Speed$$
If $italic Vector$$ is a
$xref/SimpleVector/$$ template class, 
the class $syntax%%Vector%< AD<%Base%> >%$$ 
has elements with type $syntax%AD<%Base%>%$$ 
and is faster and uses less memory than
$syntax%VecAD<%Base%>%$$ 
(because it does not tape its indexing operations).

$head Constructor$$
If $italic n$$ has type $code size_t$$,
$syntax%
	VecAD<%Base%> %V%(%n%)
%$$
creates an $code VecAD$$ object $italic V$$ with 
$italic n$$ elements.
The initial value of the elements of $italic v$$ is unspecified.

$head Size$$
The syntax
$syntax%
	%V%.size()
%$$
returns the number of elements in the vector $italic V$$.

$head Size_t Indexing$$
If $italic i$$ has type $code size_t$$,
$syntax%
	%V%[ %i% ]
%$$
returns a $italic Base$$ object equal to the value
of the $th i$$ element of $italic V$$.
This operation can only done while the tape is in the
$xref/glossary/Tape State/Empty state/$$.
$pre

$$
If $italic i$$ has type $code size_t$$
and $italic x$$ has type $italic Base$$,
$syntax%
	%V%[ %i% ] = %x%
%$$
assigns the $th i$$ element of $italic V$$
the have value $italic x$$.
This operation can only done while the tape is in the
$xref/glossary/Tape State/Empty state/$$.
$pre

$$
The value of $italic i$$ must be greater than or equal zero
and less than the number of elements in $italic V$$.


$head AD<Base> Indexing$$
If $italic I$$ has type $syntax%AD<%Base%>%$$,
$syntax%
	%V%[ %I% ]
%$$
returns the element of $italic V$$,
with index $syntax%floor(%I%)%$$,
in a form that acts like a $syntax%AD<%Base%>%$$
(see $xref/VecAD/Exceptions/exceptions/$$ below).
Here $syntax%floor(%I%)%$$ is
the greatest integer less than or equal $italic I$$.
If $italic I$$ is a complex type, 
the floor of the real part is used for the indexing operation.
$pre

$$
If $italic I$$ has type $syntax%AD<%Base%>%$$
and $italic x$$ has type $italic Base$$,
$syntax%
	%V%[%I%] = %x%
%$$
assigns the value of $italic x$$ to the 
corresponding element of $italic V$$.
The return value of this assignment is $code void$$;
i.e., it can not be used in further multiple assignments.
$pre

$$
If $italic I$$ has type $syntax%AD<%Base%>%$$
and $italic X$$ has type $syntax%AD<%Base%>%$$,
$syntax%
	%V%[%I%] = %X%
%$$
assigns the value of $italic X$$ to the 
corresponding element of $italic V$$.
The return value of this assignment is $code void$$;
i.e., it can not be used in further multiple assignments.
$pre

$$
The value of $syntax%floor(%I%)%$$ must be greater than or equal zero
and less than the number of elements in $italic V$$.
$pre

$$
If the tape is in the Recording state,
this operation is recorded; i.e.,
if $italic I$$ depends on the independent variables,
$syntax%%V%[%I%]%$$ also depends on the independent variables.
On the other hand, the derivative of
$syntax%%V%[%I%]%$$ with respect to $italic I$$
is computed by CppAD as identically zero.
This is similar to the 
$xref/Discrete/$$ functions.

$head Simple Vector$$
The vector template class $code VecAD$$ does not have all the properties
necessary for a $xref/SimpleVector/$$ template class and 
hence can not be used as such.
For example, a $syntax%VecAD<%Base%>%$$ object can not be the argument to the 
$xref/Independent/$$ function.

$head Example$$
$children%
	Example/Vec.cpp
%$$
The file
$xref/Vec.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$head Exceptions$$
Each element of a $syntax%VecAD<%Base%>%$$ 
object has the all the properties of an 
$syntax%AD<%Base%>%$$  with the following exceptions:

$list number$$
Elements of a $syntax%VecAD<%Base%>%$$ object 
cannot be used
with the computed assignments operators 
$code +=$$, 
$code -=$$, 
$code *=$$, or
$code /=$$.
For example, if $italic v$$ is a 
$syntax%VecAD<%Base%>%$$ 
object, the following syntax is not valid:
$syntax%
	%v%[%x%] += %y%;
%$$

$lnext
Assignment to 
an element of a $syntax%VecAD<%Base%>%$$ object 
returns a void value.
For example, if $italic v$$ is a 
$syntax%VecAD<%Base%>%$$ 
object, the following syntax is not valid:
$syntax%
	%x% = %v%[%y%] = %z%;
%$$
$lend

$head Inefficient$$
$index inefficient, VecAD$$
$index efficient, VecAD$$
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
	OneTest LuVecADOk "LuVecAD.cpp -DNDEBUG" > LuVecADOk.log
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

# define CppADVecADComputedAssignment(op, name)                         \
VecADelem& operator op (const VecADelem<Base> &right)                   \
{	CppADUsageError(                                                \
		0,                                                      \
		"Cannot use a ADVec element on left side of" name       \
	);                                                              \
	return *this;                                                   \
}                                                                       \
VecADelem& operator op (const AD<Base> &right)                          \
{	CppADUsageError(                                                \
		0,                                                      \
		"Cannot use a ADVec element on left side of" name       \
	);                                                              \
	return *this;                                                   \
}                                                                       \
VecADelem& operator op (const Base &right)                              \
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
class VecADelem {
	friend class VecAD<Base>;
	friend class ADTape<Base>;

	// output
	friend std::ostream& operator << <Base>
		(std::ostream &os, const VecADelem<Base> &e);
 
public:
	VecADelem(VecAD<Base> *v, const AD<Base> &x_) 
		: vec( v ) , x(x_)
	{ }

	// assignment operators
	inline void operator = (const VecADelem<Base> &right);
	void operator = (const AD<Base> &right);
	void operator = (const Base     &right);

	// computed assignments
	CppADVecADComputedAssignment( += , " += " )
	CppADVecADComputedAssignment( -= , " -= " )
	CppADVecADComputedAssignment( *= , " *= " )
	CppADVecADComputedAssignment( /= , " /= " )


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

	// output
	friend std::ostream& operator << <Base>
		(std::ostream &os, const VecAD<Base> &vec);
	friend std::ostream& operator << <Base>
	(std::ostream &os, const VecADelem<Base> &e);
 

	friend class ADTape<Base>;
	friend class VecADelem<Base>;
public:
	// default constructor
	VecAD(void) : length(0) , data(CppADNull)
	{ }

	// constructor 
	VecAD(size_t n) : length(n) , id(0)
	{	CppADUnknownError( *ADTape<Base>::Id() > id );
		data  = CppADNull;
		if( length > 0 )
			data  = CppADTrackNewVec(length, data);
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
	VecADelem<Base> operator[](AD<Base> &x) 
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
			return VecADelem<Base>(this, 0);

		if( id != *ADTape<Base>::Id() )
		{	// must place a copy of vector in tape
			offset = AD<Base>::Tape()->AddVec(length, data);

			// advance pointer by one so is always nonzero
			offset++; 
			CppADUnknownError( offset > 0 );

			// tape id corresponding to this offest
			id = *ADTape<Base>::Id();
		}

		return VecADelem<Base>(this, x); 
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
void VecADelem<Base>::operator=(const AD<Base> &y)
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
void VecADelem<Base>::operator=(const Base &y)
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

// fold this case into those above
template <class Base>
inline void VecADelem<Base>::operator=(const VecADelem<Base> &y)
{	*this = y.ADBase(); }

template <class Base>
inline std::ostream& operator << (std::ostream &os, const VecADelem<Base> &e)
{	size_t i = static_cast<size_t>( Integer(e.x) );
	CppADUnknownError( i < e.vec->length );

 	return os << *(e.vec->data + i); 
}

template <class Base>
inline std::ostream& operator << (std::ostream &os, const VecAD<Base> &v)
{
 	os << "vecAD( length = " << v.length 
	   << ", offset = "      << v.offset << ")";
	return os;
}


} // END CppAD namespace

# undef CppADVecADComputedAssignment

# endif
