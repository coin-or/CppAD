# ifndef CppADFunConstructIncluded
# define CppADFunConstructIncluded

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
$begin FunConstruct$$
$spell 
	const
$$

$spell
$$

$section Construct an ADFun Object and Stop Recording$$

$index ADFun, construct$$
$index construct, ADFun$$
$index tape, stop recording$$
$index recording, stop tape$$

$table
$bold Syntax$$ $cnext 
$syntax%ADFun<%Base%> %f%(%x%, %y%)%$$
$tend

$fend 20$$

$head Purpose$$
Constructs an $syntax%ADFun<%Base%>%$$ object 
corresponding to the current $syntax%AD<%Base%>%$$
$xref/AD/AD Operation Sequence/operation sequence/$$.
The operation sequence defines an ADFun function 
$latex \[
	F : B^n \rightarrow B^m
\] $$
where $latex B$$ is the space corresponding to objects of type $italic Base$$,
$italic n$$ is the size of the domain vector $italic x$$,
and $italic m$$ is the size of range vector $italic y$$.

$head Tape State$$
The tape that records $syntax%AD<%Base%>%$$ operations
must be in the 
$xref/glossary/Tape State/Recording/recording state/1/$$
when this constructor is called.
The recording will stop and
the AD operation sequence will be transferred to $italic f$$.
The tape will be in the $xref/glossary/Tape State/Empty/empty state/1/$$
after this constructor is called.

$head x$$
The vector $italic x$$ has prototype
$syntax%
	const %VectorAD% &%x%
%$$
(see $xref/FunConstruct//VectorAD/$$ below).
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
	const %VectorAD% &%y%
%$$
(see $xref/FunConstruct//VectorAD/$$ below).
The length of $italic y$$ must be greater than zero
and is the dimension of the range space for $italic f$$.

$subhead Range Space$$
The size of $italic y$$ is referred to as $latex m$$ above and the
domain space is $latex B^m$$.

$head VectorAD$$
The type $italic VectorAD$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$syntax%AD<%Base%>%$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Example$$
The file
$xref/Independent.cpp/$$ 
contains an example and test of this constructor.
It returns true if it succeeds and false otherwise.

$end
*/


// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorAD>
ADFun<Base>::ADFun(const VectorAD &x, const VectorAD &y)
{	size_t   n = x.size();
	size_t   m = y.size();
	size_t   i, j;
	size_t   y_taddr;
	OpCode   op;

	// check VectorAD is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorAD>();

	CppADUsageError(
		AD<Base>::Tape()->state == Recording,
		"Can not create an ADFun object because "
		" tape is not currently recording."
	);
	CppADUsageError(
		y.size() > 0,
		"ADFun constructor second argument vector Y has zero size"
	); 
	CppADUsageError(
		x.size() > 0,
		"ADFun constructor first argument vector X has zero size"
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
	Rec = new TapeRec<Base>( AD<Base>::Tape()->Rec );
	AD<Base>::Tape()->Erase();

	// total number of varables in this recording 
	CppADUnknownError( totalNumVar == Rec->TotNumVar() );

	// initial row and column dimensions
	memoryMax     = 0;
	taylor_per_var= 1;
	ForJacColDim  = 0;
	ForJacBitDim  = 0;
	TaylorColDim  = 1;

	// buffers
	Taylor  = CppADNull;
	ForJac  = CppADNull;
	Taylor  = CppADNull;
	Taylor  = CppADTrackNewVec(totalNumVar, Taylor);

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
		op = Rec->GetOp(j+1);
		CppADUsageError(
			op == InvOp,
			"independent variable vector has changed"
		);
		ind_taddr[j] = x[j].taddr;
		Taylor[j+1]  = x[j].value;
	}

	// use independent variable values to fill in values for others
	compareChange = ForwardSweep(
		false, 0, totalNumVar, Rec, TaylorColDim, Taylor
	);
	CppADUnknownError( compareChange == 0 );

	// check the dependent variable values
	for(i = 0; i < m; i++) CppADUsageError(
		Taylor[dep_taddr[i]] == y[i].value,
		"independent variable not equal its tape evaluation"
		", it may be nan"
	);
}

} // END CppAD namespace

# endif
