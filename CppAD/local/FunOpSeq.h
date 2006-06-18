# ifndef CppADFunOpSeqIncluded
# define CppADFunOpSeqIncluded

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
$begin FunOpSeq$$
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

$head Syntax$$
$syntax%%f%(%x%, %y%)%$$

$head Purpose$$
Stop recording and store the AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ 
that was started by the previous 
$xref/Independent//Independent(x)/$$
where $italic x$$ was a 
vector with elements of type $syntax%AD<%Base%>%$$.
The operation sequence defines an 
$xref/glossary/AD Function/AD function/$$
$latex \[
	F : B^n \rightarrow B^m
\] $$
where $latex B$$ is the space corresponding to objects of type $italic Base$$,
$italic n$$ is the size of the domain vector $italic x$$,
and $italic m$$ is the size of range vector $italic y$$.

$head f$$
The object $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$
The AD of $italic Base$$ operation sequence is stored in $italic f$$; i.e.,
it becomes the operation sequence corresponding to $italic f$$.
If a previous operation sequence was stored in $italic f$$,
it is deleted. 

$head x$$
The vector $italic x$$ has prototype
$syntax%
	const %ADvector% &%x%
%$$
(see $xref/FunConstruct//ADvector/$$ below).
The length of $italic x$$ must be greater than zero
and it must be the 
$xref/Independent//independent variable vector/$$ corresponding to
the current $syntax%AD<%Base%>%$$ operation sequence; i.e.,
the current recording must have started with
$syntax%
	Independent(%x%)
%$$
In addition, none of the element of $italic x$$
can be assigned a new value between the call to $xref/Independent/$$
and the $syntax%ADFun<%Base%>%$$ constructor call.

$subhead Domain Space$$
The size of $italic x$$ is referred to as $latex n$$ above and the
domain space is $latex B^n$$.

$head y$$
The vector $italic y$$ has prototype
$syntax%
	const %ADvector% &%y%
%$$
(see $xref/FunConstruct//ADvector/$$ below).
The length of $italic y$$ must be greater than zero
and is the dimension of the range space for $italic f$$.

$subhead Range Space$$
The size of $italic y$$ is referred to as $latex m$$ above and the
domain space is $latex B^m$$.

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
This operation preforms an implicit call to 
$syntax%
	%f%.Forward(0, %x_p%)
%$$ 
(see $xref/ForwardZero/$$) 
with the elements of $italic x_p$$ equal to 
the corresponding elements of $italic x$$; i.e.,
the zero order Taylor coefficients corresponding to 
the value of $italic x$$ are also stored $italic f$$
(see $xref/size_taylor/$$ and $xref/capacity_taylor/$$). 

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
void ADFun<Base>::operator()(const ADvector &x, const ADvector &y)
{	size_t   n = x.size();
	size_t   m = y.size();
	size_t   i, j;
	size_t   y_taddr;
	OpCode   op;

	// check ADvector is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, ADvector>();

	CppADUsageError(
		AD<Base>::Tape()->state == Recording,
		"Can't store current operation sequence in this ADFun object"
		"\nbecause corresponding tape is not currently recording."
	);
	CppADUsageError(
		y.size() > 0,
		"ADFun operation sequence second argument y has zero size"
	); 
	CppADUsageError(
		x.size() > 0,
		"ADFun operation sequence first argument x has zero size"
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

	// free old buffers
	if( Taylor != CppADNull )
		CppADTrackDelVec(Taylor);
	if( ForJac != CppADNull )
		CppADTrackDelVec(ForJac);

	// initialize buffers
	Taylor  = CppADTrackNewVec(totalNumVar, Taylor);
	ForJac  = CppADNull;

	// initial row and column dimensions
	// memoryMax  = 0;
	taylor_per_var= 1;
	ForJacColDim  = 0;
	ForJacBitDim  = 0;
	TaylorColDim  = 1;

	// set tape address and initial value for independent variables
	ind_taddr.resize(n);
	CppADUsageError(
		n < totalNumVar,
		"independent variables vector has changed"
	);
	for(j = 0; j < n; j++)
	{	
		CppADUsageError( 
			x[j].taddr == j+1,
			"independent variable vector has changed"
		);
		// j+1 is both the operator and independent variable taddr
		op = Rec.GetOp(j+1);
		CppADUsageError(
			op == InvOp,
			"independent variable vector has changed"
		);
		ind_taddr[j] = x[j].taddr;
		Taylor[j+1]  = x[j].value;
	}

	// use independent variable values to fill in values for others
	compareChange = ForwardSweep(
		false, 0, totalNumVar, &Rec, TaylorColDim, Taylor
	);
	CppADUnknownError( compareChange == 0 );

	// check the dependent variable values
	for(i = 0; i < m; i++) CppADUsageError(
		Taylor[dep_taddr[i]] == y[i].value,
		"independent variable not equal its tape evaluation"
		", it may be nan"
	);

	// used to determine if there is an operation sequence in *this
	CppADUnknownError( totalNumVar > 0 );
}

} // END CppAD namespace

# endif
