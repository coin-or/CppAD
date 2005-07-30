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
$syntax%Independent(%u%)%$$
$tend

$fend 20$$

$head Description$$
Calling the $code Independent$$ 
starts the recording of $syntax%AD<%Base%>%$$ operations.
This is the first step in defining a CppAD differentiable function object
($xref/ADFun/$$ object).

$head VectorADBase$$
The type $italic VectorADBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type AD<Base>/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head u$$
The vector $italic u$$ has prototype
$syntax%
	%VectorADBase% &%u%
%$$
The length of $italic u$$ is the number of independent variables
(dimension of the domain space for the $xref/ADFun/$$ object)


$head Tape State$$
$index state, tape$$
$index tape, state$$
The operation can only be done while the tape 
that records $syntax%AD<%Base%>%$$ operations is in the 
Empty $xref/glossary/Tape State/state/$$.
After this operation, the tape will be in the Recording state.

$head Parameters$$
$index parameter$$
Directly before $code Independent$$ is called, 
all other $syntax%AD<%Base%>%$$ objects are 
$xref/glossary/Parameter/parameters/$$; i.e.,
they do not dependent on the independent variables.
For example, after the code sequence
$syntax%
	%v% = %u%;
	Independent(%u%);
%$$
all of the elements of the vector $italic v$$ are parameters.
Hence their partial derivatives with respect to each of the 
elements of $italic u$$ are identically zero.

$head Variables$$
$index variables$$
An $syntax%AD<%Base%>%$$ object is a 
$xref/glossary/Variable/variable/$$
if and only if its value depends on the value
of one of the independent variables.
For example, after the code sequence
$syntax%
	Independent(%v%);
	%v% = %u%;
%$$
all of the elements of the vector $italic v$$ are variables.
In fact, the partial of $syntax%%v%[%i%]%$$ with respect to
$syntax%%u%[%i%]%$$ is one.


$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {
// ---------------------------------------------------------------------------

template <typename Base>
template <typename VectorADBase>
void ADTape<Base>::Independent(VectorADBase &u)
{
	// check VectorADBase is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorADBase>();

	CppADUsageError(
		State() == Empty ,
		"Independent with vector argument:\n"
		"can only be used when tape is empty"
	);
	CppADUnknownError( Rec.NumOp() == 0 );
	CppADUnknownError( Rec.TotNumVar() == 0 );

	// skip the first record (parameters use taddr zero)
	Rec.PutOp(NonOp);

	// no Ind values for this operator
	CppADUnknownError( NumInd(NonOp) == 0 );

	// place each of the independent variables in the tape
	size_t i;
	for(i = 0; i < u.size(); i++)
	{	RecordInvOp(u[i]);
		CppADUnknownError( u[i].taddr == i+1 );
	}

	// done specifying all of the independent variables
	state = Recording;

}

template <typename VectorADBase>
inline void Independent(VectorADBase &u)
{	typedef typename VectorADBase::value_type ADBase;
	ADBase::Tape()->Independent(u); 
}


} 
// END CppAD namespace

# endif
