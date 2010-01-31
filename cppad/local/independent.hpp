/* $Id$ */
# ifndef CPPAD_INDEPENDENT_INCLUDED
# define CPPAD_INDEPENDENT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

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
Start a recording the 
$xref/glossary/AD of Base/AD of Base/$$ operations
with $italic x$$ as the vector of independent variables.
Once the 
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ is completed,
it must be transferred to a function object; see below.

$head Variables for a Tape$$
A tape is create by the call 
$syntax%
	Independent(%x%)
%$$
The corresponding operation sequence is transferred to a function object,
and the tape is deleted,
using either (see $cref/ADFun<Base> f(x, y)/FunConstruct/$$)
$syntax%
	ADFun<%Base%> %f%( %x%, %y%)
%$$
or using (see $cref/f.Dependent(x, y)/Dependent/$$)
$syntax%
	%f%.Dependent( %x%, %y%)
%$$
Between when the tape is created and when it is destroyed,
we refer to the elements of $italic x$$, 
and the values that depend on the elements of $italic x$$,
as variables for the tape created by the call to $code Independent$$. 

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

$head Memory Leak$$
A memory leak will result if
a tape is create by a call to $code Independent$$
and not deleted by a corresponding call to 
$syntax%
	ADFun<%Base%> %f%( %x%, %y%)
%$$
or using 
$syntax%
	%f%.Dependent( %x%, %y%)
%$$

$head OpenMP$$
$index OpenMP, Independent$$
$index Independent, OpenMP$$
In the case of multi-threading with OpenMP,
the call to $code Independent$$
and the corresponding call to
$syntax%
	ADFun<%Base%> %f%( %x%, %y%)
%$$
or 
$syntax%
	%f%.Dependent( %x%, %y%)
%$$
must be preformed by the same thread.

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

	// dimension of the domain space
	size_t n = x.size();
	CPPAD_ASSERT_KNOWN(
		n > 0,
		"Indepdendent: the argument vector x has zero size"
	);
	CPPAD_ASSERT_UNKNOWN( Rec_.num_rec_var() == 0 );

	// mark the beginning of the tape and skip the first variable index 
	// (zero) because parameters use taddr zero
	CPPAD_ASSERT_NARG_NRES(BeginOp, 0, 1);
	Rec_.PutOp(BeginOp);

	// place each of the independent variables in the tape
	CPPAD_ASSERT_NARG_NRES(InvOp, 0, 1);
	size_t j;
	for(j = 0; j < n; j++)
	{	// tape address for this independent variable
		x[j].taddr_ = Rec_.PutOp(InvOp);
		x[j].id_    = id_;
		CPPAD_ASSERT_UNKNOWN( x[j].taddr_ == j+1 );
		CPPAD_ASSERT_UNKNOWN( Variable(x[j] ) );
	}

	// done specifying all of the independent variables
	size_independent_ = n;
}

template <typename VectorAD>
inline void Independent(VectorAD &x)
{	typedef typename VectorAD::value_type ADBase;
	typedef typename ADBase::value_type   Base;
	CPPAD_ASSERT_KNOWN(
		ADBase::tape_ptr() == CPPAD_NULL,
		"Independent: cannot create a new tape because\n"
		"a previous tape is still active (for this thread).\n"
		"AD<Base>::abort_recording() would abort this previous recording."
	);
	size_t id = ADBase::tape_new();

	ADBase::tape_ptr(id)->Independent(x); 
}


} 
// END CppAD namespace

# endif
