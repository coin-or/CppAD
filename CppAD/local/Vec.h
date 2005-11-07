# ifndef CppADVecIncluded
# define CppADVecIncluded

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
$syntax%VecAD<%Base%> %v%( size_t %n% )%$$
$rnext $cnext
$syntax%%Type% %v%[ size_t %i% ]%$$
$rnext $cnext
$syntax%%Type% %v%[ AD<%Base%> &%x% ]%$$
$rnext $cnext
$rnext 
$bold See Also$$ $cnext $xref/Discrete/$$
$tend

$fend 20$$

$head Description$$
Each element of a 
$syntax%VecAD<%Base%>%$$ 
object has the properties of an 
$syntax%AD<%Base%>%$$ 
object with the exception that the computed assignments
$xref%CompEq%%+= , -= , *= , /=%$$ 
are not allowed with these elements.
$pre

$$
The class $syntax%VecAD<%Base%>%$$ tapes its indexing
and these operations are transferred to the corresponding
$xref/ADFun/$$ object $italic f$$.
The indices are evaluated each time
$xref/Forward//f.Forward/$$ is used to evaluate the zero order Taylor
coefficients.
$pre

$$
If $italic Vector$$ is 
$xref/SimpleVector/$$ template class, 
the class $syntax%%Vector%< AD<%Base%> >%$$ 
has elements with type $syntax%AD<%Base%>%$$ 
and is faster and uses less memory than
$syntax%VecAD<%Base%>%$$ 
because it does not tape its indexing operations.

$head Constructor$$
$mindex VecAD construct$$
The syntax
$syntax%
	VecAD<%Base%> %v%(%n%)
%$$
creates an $code VecAD$$ object $italic v$$ with 
$italic n$$ elements.
The initial value of the elements is unspecified.

$head Size$$
The syntax
$syntax%
	%v%.size()
%$$
returns the number of elements in the vector $italic v$$.

$head Element Access$$
The syntax
$syntax%
	%v%[ %i% ]
	%v%[ %x% ]
%$$
returns the element of $italic v$$ with index $italic i$$
and 
$syntax%floor(%x%)%$$ 
respectively.
We use 
$syntax%floor(%x%)%$$ 
to denote the greatest integer less than or equal $italic x$$.
If $italic x$$ is a complex type, the index is
$syntax%floor(real(%x%))%$$ is used for the indexing operation.
$pre

$$
The $code size_t$$ indexing form
$syntax%
	%v%[ %i% ]
%$$
can only be used when the tape is in the
$xref/glossary/Tape State/Empty state/$$.
If the tape is in the Recording state,
the dependence of the value of
$syntax%%v%[%x%]%$$ 
on $italic x$$ is recorded.
On the other hand,
the derivative of
$syntax%%v%[%x%]%$$ 
with respect to $italic x$$
is computed by CppAD as identically zero.
This is similar to the 
$xref/Discrete/$$ functions.
$pre

$$
The value of $italic x$$ (and $italic i$$) must be greater than or equal zero
and less than the number of elements in $italic v$$.
$pre

$$
The return $italic Type$$ is unspecified except for the fact that
it is derived from
$syntax%AD<%Base%>%$$ and inherits all the properties of that type
(with the exception that the
$xref%CompEq%%+= , -= , *= , /=%$$ 
are not allowed with these elements.)


$subhead Assignment Operations$$
The syntax
$syntax%
	%v%[%i%] = %y%
	%v%[%x%] = %y%
%$$
assigns the value of $italic y$$ to the corresponding element of $italic v$$
where $italic y$$ has type $italic Base$$ or $syntax%AD<%Base%>%$$.

$head Example$$
$children%
	Example/Vec.cpp
%$$
The file
$xref/Vec.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

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
	wc -l LuVecADOk.log
	grep "op= Ld[vp]" LuVecADOk.log | wc -l
$$
The first command will give a rough idea of how many operations
there are in total (1018).
The second command will give a rough idea how many are 
correspond to accessing an element of a $code VecAD$$ array (348).
$lend

$end
------------------------------------------------------------------------ 
*/

# define CppADVecADComputedAssignment(op, name)                         \
VecADelem& operator op (const VecADelem<Base> &right)                   \
{	CppADUsageError(                                            \
		0,                                                      \
		"Cannot use a ADVec element on left side of" name       \
	);                                                              \
	return *this;                                                   \
}                                                                       \
VecADelem& operator op (const AD<Base> &right)                          \
{	CppADUsageError(                                            \
		0,                                                      \
		"Cannot use a ADVec element on left side of" name       \
	);                                                              \
	return *this;                                                   \
}                                                                       \
VecADelem& operator op (const Base &right)                              \
{	CppADUsageError(                                            \
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
	inline AD<Base> operator = (const VecADelem<Base> &right);
	AD<Base> operator = (const AD<Base> &right);
	AD<Base> operator = (const Base     &right);

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
		if( length == 0 )
			data  = CppADNull;
		else	data  = new Base [length]; 
	}

	// destructor
	~VecAD(void)
	{	if( data != CppADNull )
			delete [] data; 
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
AD<Base> VecADelem<Base>::operator=(const AD<Base> &y)
{	AD<Base> result;
	
	if( Parameter(y) )
		return *this = y.value;

	CppADUnknownError( AD<Base>::Tape()->State() == Recording );

	size_t i = static_cast<size_t>( Integer(x) );
	CppADUnknownError( i < vec->length );

	// assign value both in the element and the original array
	*(vec->data + i) = result.value = y.value;

	// record the setting of this array element
	CppADUnknownError( vec->id == *ADTape<Base>::Id() );
	CppADUnknownError( vec->offset > 0 );
	if( Parameter(x) ) AD<Base>::Tape()->RecordStoreOp(
			StpvOp, result, vec->offset, i, y.taddr );
	else	AD<Base>::Tape()->RecordStoreOp(
			StvvOp, result, vec->offset, x.taddr, y.taddr );

	return result;
}

template <class Base>
AD<Base> VecADelem<Base>::operator=(const Base &y)
{	AD<Base> result;

	size_t y_taddr;

	size_t i = static_cast<size_t>( Integer(x) );
	CppADUnknownError( i < vec->length );

	// assign value both in the element and the original array
	*(vec->data + i) = result.value = y;

	if( AD<Base>::Tape()->State() == Empty )
	{	CppADUnknownError( Parameter(result) );

		return result;
	}

	// place a copy of y in the tape
	y_taddr = AD<Base>::Tape()->Rec.PutPar(y);

	// record the setting of this array element
	CppADUnknownError( vec->id == *ADTape<Base>::Id() );
	CppADUnknownError( vec->offset > 0 );
	if( Parameter(x) ) AD<Base>::Tape()->RecordStoreOp(
			StppOp, result, vec->offset, i, y_taddr );
	else	AD<Base>::Tape()->RecordStoreOp(
			StvpOp, result, vec->offset, x.taddr, y_taddr );

	return result;
}

// fold this case into those above
template <class Base>
inline AD<Base> VecADelem<Base>::operator=(const VecADelem<Base> &y)
{	return *this = y.ADBase(); }

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
