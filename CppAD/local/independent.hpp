# ifndef CPPAD_INDEPENDENT_INCLUDED
# define CPPAD_INDEPENDENT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
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

$head Syntax$$
$syntax%Independent(%x%)%$$


$head Purpose$$
Calling $code Independent$$ starts the recording 
$xref/glossary/AD of Base/AD of/$$ $italic Base$$ operations
with $italic x$$ as the vector of independent variables.
Once the 
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ is completed,
it is transferred to a function object by calling the
$syntax%ADFun<%Base%>%$$ $xref/FunConstruct//constructor/$$. 

$head x$$
The vector $italic x$$ has prototype
$syntax%
	%VectorAD% &%x%
%$$
(see $italic VectorAD$$ below).
The size of the vector $italic x$$, must be greater than zero,
and is the number of independent variables for this
AD operation sequence.


$head VectorAD$$
The type $italic VectorAD$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$syntax%AD<%Base%>%$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Tape State$$
$index state, tape$$
$index tape, state$$
The tape that records $xref/glossary/AD of Base/AD of/$$ $italic Base$$ operations must be 
in the $xref/glossary/Tape State/Empty/empty state/1/$$ when
$code Independent$$ is called.
After this operation, the tape will be in the Recording state.

$head Example$$
$children%
	example/independent.cpp
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
template <typename VectorAD>
void ADTape<Base>::Independent(VectorAD &x)
{
	// check VectorAD is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorAD>();

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
	size_t j;
	for(j = 0; j < n; j++)
	{	RecordInvOp(x[j]);
		CppADUnknownError( x[j].taddr == j+1 );
	}

	// done specifying all of the independent variables
	state            = Recording;
	size_independent = n;
}

template <typename VectorAD>
inline void Independent(VectorAD &x)
{	typedef typename VectorAD::value_type ADBase;
	ADBase::Tape()->Independent(x); 
}


} 
// END CppAD namespace

# endif
