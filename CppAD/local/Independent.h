# ifndef CppADIndependentIncluded
# define CppADIndependentIncluded

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
$index start, tape$$
$index tape, start$$
$index variable, independent$$

$section Declare Independent Variables and Start Recording$$

$table
$bold Syntax$$ $cnext
$syntax%Independent(%x%)%$$
$tend

$fend 20$$

$head Purpose$$
Calling $code Independent$$ 
starts the recording of $syntax%AD<%Base%>%$$ operations
with $italic x$$ being the independent variable vector.
This is the first step in defining a CppAD differentiable function object
($xref/ADFun/$$ object).

$head VectorADBase$$
The type $italic VectorADBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type AD<Base>/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head x$$
The vector $italic x$$ has prototype
$syntax%
	%VectorADBase% &%x%
%$$
The length of $italic x$$, must be greater than zero,
and is the number of independent variables
(dimension of the domain space for the $xref/ADFun/$$ object)


$head Tape State$$
$index state, tape$$
$index tape, state$$
The operation can only be done while the tape 
that records $syntax%AD<%Base%>%$$ operations is in the 
Empty $xref/glossary/Tape State/state/$$.
After this operation, the tape will be in the Recording state.

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
