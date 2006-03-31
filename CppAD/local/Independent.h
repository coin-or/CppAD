# ifndef CppADIndependentIncluded
# define CppADIndependentIncluded

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
---------------------------------------------------------------------------

$begin Independent$$
$spell
	Cpp
	bool
	const
	var
	typename
$$

$index Independent$$
$index start, recording$$
$index recording, start$$
$index variable, independent$$

$section Declare Independent Variables and Start Recording$$

$table
$bold Syntax$$ $cnext
$syntax%Independent(%x%)%$$
$tend

$fend 20$$

$head Purpose$$
Calling $code Independent$$ starts the recording 
$syntax%AD<%Base%>%$$ operations
with $italic x$$ as the vector of independent variables.
Once the 
$xref/AD/AD Operation Sequence/AD operation sequence/$$ is completed,
it is transferred to a function object by calling the
$syntax%ADFun<%Base%>%$$ $xref/FunConstruct//constructor/$$. 

$head x$$
The vector $italic x$$ has prototype
$syntax%
	%VectorADBase% &%x%
%$$
(see the specifications for the class $italic VectorADBase$$ below).
The size of the vector $italic x$$, must be greater than zero,
and is the number of independent variables for this
AD operation sequence.


$head VectorADBase$$
The type $italic VectorADBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$syntax%AD<%Base%>%$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Tape State$$
$index state, tape$$
$index tape, state$$
The tape that records $syntax%AD<%Base%>%$$ operations must be 
in the $xref/glossary/Tape State/Empty/empty state/1/$$ when
$code Independent$$ is called.
After this operation, the tape will be in the Recording state.

$head Example$$
$children%
	Example/Independent.cpp
%$$
The file
$xref/Independent.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {
// ---------------------------------------------------------------------------

template <typename Base>
template <typename VectorADBase>
void ADTape<Base>::Independent(VectorADBase &x)
{
	// check VectorADBase is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorADBase>();

	CppADUsageError(
		State() == Empty ,
		"Independent can only be used when tape is empty"
	);
	// dimension of the domain space
	size_t n = x.size();
	CppADUsageError(
		n > 0,
		"Indepdendent: the argument vector x has zero size"
	);
	CppADUnknownError( Rec.NumOp() == 0 );
	CppADUnknownError( Rec.TotNumVar() == 0 );

	// skip the first record (parameters use taddr zero)
	Rec.PutOp(NonOp);

	// no Ind values for this operator
	CppADUnknownError( NumInd(NonOp) == 0 );

	// place each of the independent variables in the tape
	size_t i;
	for(i = 0; i < n; i++)
	{	RecordInvOp(x[i]);
		CppADUnknownError( x[i].taddr == i+1 );
	}

	// done specifying all of the independent variables
	state = Recording;

}

template <typename VectorADBase>
inline void Independent(VectorADBase &x)
{	typedef typename VectorADBase::value_type ADBase;
	ADBase::Tape()->Independent(x); 
}


} 
// END CppAD namespace

# endif
