# ifndef CPPAD_DEPENDENT_INCLUDED
# define CPPAD_DEPENDENT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin Dependent$$
$spell 
	Taylor
	ADvector
	const
$$

$spell
$$

$section Stop Recording and Store Operation Sequence$$

$index ADFun, operation sequence$$
$index operation, sequence store$$
$index sequence, operation store$$
$index recording, stop$$
$index tape, stop recording$$
$index Dependent$$

$head Syntax$$
$syntax%%f%.Dependent(%y%)%$$

$head Purpose$$
Stop recording and the AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
and store the new operation sequence in $italic f$$.
The operation sequence defines an 
$xref/glossary/AD Function/AD function/$$
$latex \[
	F : B^n \rightarrow B^m
\] $$
where $latex B$$ is the space corresponding to objects of type $italic Base$$.
The value $latex n$$ is the dimension of the 
$xref/SeqProperty/Domain/domain/$$ space for the operation sequence.
The value $latex m$$ is the dimension of the 
$xref/SeqProperty/Range/range/$$ space for the operation sequence
(which is determined by the size of $italic y$$).

$head f$$
The object $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$
The AD of $italic Base$$ operation sequence is stored in $italic f$$; i.e.,
it becomes the operation sequence corresponding to $italic f$$.
If a previous operation sequence was stored in $italic f$$,
it is deleted. 

$head y$$
The vector $italic y$$ has prototype
$syntax%
	const %ADvector% &%y%
%$$
(see $xref/FunConstruct//ADvector/$$ below).
The length of $italic y$$ must be greater than zero
and is the dimension of the range space for $italic f$$.

$head ADvector$$
The type $italic ADvector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$syntax%AD<%Base%>%$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Tape State$$
The tape that records 
$xref/glossary/AD of Base/AD of Base/$$ operations must be in the 
$xref/glossary/Tape State/Recording/recording state/1/$$
when this operation is preformed.
The recording will stop and
the AD operation sequence will be stored in $italic f$$.
The tape will be in the $xref/glossary/Tape State/Empty/empty state/1/$$
after this operation is preformed.

$head Forward$$
No $xref/Forward/$$ calculation is preformed during this operation.
Thus, directly after this operation,
$syntax%
	%f%.size_taylor()
%$$ 
is zero (see $xref/size_taylor/$$).

$head Example$$
The file
$xref/FunCheck.cpp/$$ 
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
----------------------------------------------------------------------------
*/


// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename ADvector>
void ADFun<Base>::Dependent(const ADvector &y)
{	size_t   m = y.size();
	size_t   n = AD<Base>::Tape()->size_independent;
	size_t   i, j;
	size_t   y_taddr;

	// check ADvector is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, ADvector>();

	CppADUsageError(
		AD<Base>::Tape()->state == Recording,
		"Can't store current operation sequence in this ADFun object"
		"\nbecause corresponding tape is not currently recording."
	);
	CppADUsageError(
		y.size() > 0,
		"ADFun operation sequence dependent variable size is zero size"
	); 

	// set total number of variables in tape, parameter flag, 
	// make a tape copy of dependent variables that are parameters, 
	// and store tape address for each dependent variable
	CppADUnknownError( NumVar(ParOp) == 1 );
	dep_parameter.resize(m);
	dep_taddr.resize(m);
	totalNumVar = AD<Base>::Tape()->Rec.TotNumVar();
	for(i = 0; i < m; i++)
	{	dep_parameter[i] = CppAD::Parameter(y[i]);
		if( dep_parameter[i] )
		{	y_taddr = AD<Base>::Tape()->RecordParOp( y[i].value );
			totalNumVar++;
		}
		else	y_taddr = y[i].taddr;

		CppADUnknownError( y_taddr > 0 );
		CppADUnknownError( y_taddr < totalNumVar );
		dep_taddr[i] = y_taddr;
	}

	// now that each dependent variable has a place in the tape,
	// we can make a copy for this function and erase the tape.
	Rec = AD<Base>::Tape()->Rec;
	AD<Base>::Tape()->Erase();

	// total number of varables in this recording 
	CppADUnknownError( totalNumVar == Rec.TotNumVar() );

	// used to determine if there is an operation sequence in *this
	CppADUnknownError( totalNumVar > 0 );

	// free old buffers
	if( Taylor != CppADNull )
		CppADTrackDelVec(Taylor);
	if( ForJac != CppADNull )
		CppADTrackDelVec(ForJac);

	// initialize buffers
	Taylor  = CppADNull;
	ForJac  = CppADNull;

	// initial row and column dimensions
	// memoryMax  = 0;
	taylor_per_var= 0;
	ForJacColDim  = 0;
	ForJacBitDim  = 0;
	TaylorColDim  = 0;

	// set tape address 
	ind_taddr.resize(n);
	CppADUnknownError(
		n < totalNumVar
	);
	for(j = 0; j < n; j++)
	{	CppADUnknownError( Rec.GetOp(j+1) == InvOp );
		ind_taddr[j] = j+1;
	}

}

} // END CppAD namespace

# endif
