# ifndef CppADFunIncluded
# define CppADFunIncluded

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
$begin ADFun$$
$spell 
	xk
	Ind
	bool
	Taylor
	sizeof
	const
	std
	ind_taddr
	dep_taddr
$$

$spell
$$

$section Constructing an ADFun Object From The Tape$$

$index constructor,  ADFun$$
$index ADFun, constructor$$
$index tape, convert to ADFun$$

$table
$bold Syntax$$ $cnext 
$syntax%ADFun<%Base%> %F%(%X%, %Y%)%$$
$tend

$fend 20$$

$head Description$$
This constructs a function object corresponding to a
$xref/glossary/Base Function/Base function/$$
$latex F : B^n \rightarrow B^m$$
where $italic B$$ is the space corresponding to objects of type $italic Base$$,
$italic n$$ is the size of $italic X$$,
and $italic m$$ is the size of $italic Y$$.
The $syntax%AD<%Base%>%$$ operations that 
map from the elements of $italic X$$
to the elements of $italic Y$$
define the function.

$head Tape State$$
The tape that records $syntax%AD<%Base%>%$$ operations
must be in the Recording $xref/glossary/Tape State/state/$$
when this constructed is called.
The tape that records $syntax%AD<%Base%>%$$ operations
will be in the Empty state after this constructor is called.

$head VectorADBase$$
The type $italic VectorADBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type AD<Base>/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head X$$
The vector $italic X$$ has prototype
$syntax%
	const %VectorADBase% &%X%
%$$
The length of $italic X$$ is the number of independent variables
(dimension of the domain space for the $syntax%ADFun<%Base%>%$$ object).
The current recording of $syntax%AD<%Base%>%$$ operations
must have started recording with the call 
$xref/Independent//Independent(X)/$$.
In addition, none of the element of $italic X$$
may change value between the call to $code Independent$$
and the $code ADFun$$ constructor.

$head Y$$
The vector $italic Y$$ has prototype
$syntax%
	const %VectorADBase% &%Y%
%$$
The length of $italic Y$$, must be greater than zero,
and is the number of dependent variables
(dimension of the range space for the $syntax%ADFun<%Base%>%$$ object).

$head Size$$
$index Size, ADFun$$
The operation 
$syntax%
	size_t %F%.Size(void) const
%$$
returns the total number of variables
that are used to calculate the function $italic F$$.

$head Order$$
$index Order, ADFun$$
The function 
$syntax%
	size_t %F%.Order(void) const
%$$
returns the order of the previous forward operation
using the function $italic F$$.
This is the highest order of the 
$xref/glossary/Taylor Coefficient/Taylor coefficients/$$
that are currently stored in $italic F$$.

$head Domain$$
$index Domain, ADFun$$
The function
$syntax%
	size_t %F%.Domain(void) const
%$$
returns the dimension of the domain space; i.e., the length of the
vector $italic X$$ in the constructor for $italic F$$.

$head Range$$
$index Range, ADFun$$
The function
$syntax%
	size_t %F%.Range(void) const
%$$
returns the dimension of the range space; i.e., the length of the
vector $italic Y$$ in the constructor for $italic F$$.

$head Parameter$$
$index Parameter, ADFun$$
The function
$syntax%
	bool %F%.Parameter(size_t i) const
%$$
returns true if the range space component of $italic F$$ 
with index $italic i$$ corresponds to a
$xref/glossary/Parameter/parameter/$$ 
where $italic i$$
is less than $syntax%%F%.Range()%$$.
If a component of the range of $italic F$$ is a parameter, 
it does not depend on any of the components in the domain of $italic F$$; 
i.e., all of its partial derivatives are zero.

$head Memory$$
$index Memory, ADFun$$
The operation
$syntax%
	size_t %F%.Memory(void) const
%$$
returns the number of memory units ($code sizeof$$) required for the
information currently stored in $italic F$$.
This memory is returned to the system when the destructor for 
$italic F$$ is called.

$head CompareChange$$
$index Compare, tape operator$$
$index tape, compare operator$$
$index operator, compare tape$$
If $code NDEBUG$$ is defined,
this function does not exist.
Otherwise, the operation
$syntax%
	size_t %F%.CompareChange(void) const
%$$
return the number of 
$syntax%AD<%Base%>%$$ $xref/Compare//comparison/$$ 
operations,
corresponding to the previous call to $xref/Forward/$$ 
$syntax%
	%F%.Forward(%k%, %xk%)
%$$
where $italic k$$ was equal to zero,
that have a different result from
when $italic F$$ was created by taping an algorithm.
When it is non-zero, some comparison operator results are different.
This can be used to detect when $italic F$$
no exactly represents the algorithm that was taped.

$head Example$$
The files
$xref/Fun.cpp/$$ and $xref/CompareChange.cpp/$$
contain examples and tests of these operations.
They return true if they succeed and false otherwise.

$contents%
	CppAD/local/Parameter.h%
	CppAD/local/Independent.h%
	omh/Drivers.omh%
	CppAD/local/Forward.h%
	CppAD/local/Reverse.h%
	Example/Fun.cpp%
	Example/CompareChange.cpp%
	omh/MulTape.omh%
	CppAD/local/ForSparseJac.h%
	CppAD/local/RevSparseJac.h%
	CppAD/local/RevSparseHes.h
%$$

$end
*/


// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class ADFun {
	// type used for packing sparsity patters
	typedef size_t Pack;

public:
	// constructor
	template <typename VectorADBase>
	ADFun(const VectorADBase &u, const VectorADBase &z);

	// destructor
	~ADFun(void)
	{	delete Rec;
		CppADTrackDelVec(Taylor);
		if( ForJac != CppADNull )
			CppADTrackDelVec(ForJac);
	}

	// forward mode sweep
	template <typename VectorBase>
	VectorBase Forward(size_t p, const VectorBase &u);

	// reverse mode sweep
	template <typename VectorBase>
	VectorBase Reverse(size_t p, const VectorBase &v);

	// forward mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase ForSparseJac(size_t q, const VectorBase &Px);

	// reverse mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseJac(size_t q, const VectorBase &Py);

	// reverse mode Hessian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseHes(size_t q, const VectorBase &Py);

	// size of this function object
	size_t Size(void) const
	{	return totalNumVar; }

	// order of this function object
	size_t Order(void) const
	{	return order; }

	// number of independent variables
	size_t Domain(void) const
	{	return ind_taddr.size(); }

	// number of dependent variables
	size_t Range(void) const
	{	return dep_taddr.size(); }

	// is variable a parameter
	bool Parameter(size_t i)
	{	CppADUsageError(
			i < dep_taddr.size(),
			"Argument to Parameter is >= dimension of range space"
		);
		return dep_parameter[i]; 
	}

	// amount of memory for each variable
	size_t Memory(void) const
	{	size_t pervar  = TaylorColDim * sizeof(Base)
		+ ForJacColDim * sizeof(Pack);
		size_t total   = totalNumVar * pervar + Rec->Memory();
		return total;
	}

# ifndef NDEBUG
	// in not NDEBUG case, number of comparison operations that change
	size_t CompareChange(void) const
	{	return compareChange; }
# endif

	// calculate entire Jacobian
	template <typename VectorBase>
	VectorBase Jacobian(const VectorBase &x); 

	// calculate Hessian for one component of f
	template <typename VectorBase>
	VectorBase Hessian(const VectorBase &x, size_t i); 

	// forward mode calculation of partial w.r.t one domain component
	template <typename VectorBase>
	VectorBase ForOne(
		const VectorBase   &x ,
		size_t              j );

	// reverse mode calculation of derivative of one range component
	template <typename VectorBase>
	VectorBase RevOne(
		const VectorBase   &x ,
		size_t              i );

	// forward mode calculation of a subset of second order partials
	template <typename VectorBase, typename VectorSize_t>
	VectorBase ForTwo(
		const VectorBase   &x ,
		const VectorSize_t &J ,
		const VectorSize_t &K );

	// reverse mode calculation of a subset of second order partials
	template <typename VectorBase, typename VectorSize_t>
	VectorBase RevTwo(
		const VectorBase   &x ,
		const VectorSize_t &I ,
		const VectorSize_t &J );

private:
	// maximum amount of memory required for this function object
	size_t memoryMax;

	// debug checking number of comparision operations that changed
	size_t compareChange;

	// order of the informaiton currently stored in Taylor array
	size_t order;

	// number of bits currently calculated per row of the ForJac array
	size_t ForJacBitDim; 

	// number of columns currently allocated for Taylor array
	size_t TaylorColDim;

	// number of columns currently allocated for ForJac array
	size_t ForJacColDim;

	// number of rows (variables) in the recording (Rec)
	size_t totalNumVar;

	// tape address for the independent variables
	CppAD::vector<size_t> ind_taddr;

	// tape address and parameter flag for the dependent variables
	CppAD::vector<size_t> dep_taddr;
	CppAD::vector<bool>   dep_parameter;

	// the operations corresponding to this function
	TapeRec<Base> *Rec;

	// results of the forward mode calculations
	Base *Taylor;

	// results of the forward mode Jacobian sparsity calculations
	Pack *ForJac;
};
// ---------------------------------------------------------------------------

template <typename Base>
template <typename VectorADBase>
ADFun<Base>::ADFun(const VectorADBase &x, const VectorADBase &y)
{	size_t   n = x.size();
	size_t   m = y.size();
	size_t   i, j;
	size_t   y_taddr;
	OpCode   op;

	// check VectorADBase is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorADBase>();

	CppADUsageError(
		AD<Base>::Tape()->state == Recording,
		"Can not create an ADFun object because "
		" tape is not currently recording."
	);
	CppADUsageError(
		y.size() > 0,
		"ADFun constructor second argument vector Y has zero size"
	); 
	CppADUnknownError( x.size() > 0 );

	// set total number of variables in tape, parameter flag, 
	// make a tape copy of dependent variables that are parameters, 
	// and store tape address for each dependent variable
	CppADUnknownError( NumInd(ParOp) == 1 );
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

	// current order and row dimensions
	memoryMax     = 0;
	order         = 0;
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
	for(i = 0; i < m; i++)
		CppADUnknownError( Taylor[dep_taddr[i]] == y[i].value );
}

} // END CppAD namespace

# include <CppAD/local/Parameter.h>
# include <CppAD/local/Independent.h>
# include <CppAD/local/ForwardSweep.h>
# include <CppAD/local/ReverseSweep.h>
# include <CppAD/local/Forward.h>
# include <CppAD/local/Reverse.h>
# include <CppAD/local/ForJacSweep.h>
# include <CppAD/local/RevJacSweep.h>
# include <CppAD/local/RevHesSweep.h>
# include <CppAD/local/ForSparseJac.h>
# include <CppAD/local/RevSparseJac.h>
# include <CppAD/local/RevSparseHes.h>
//
// driver routines
# include <CppAD/local/Jacobian.h>
# include <CppAD/local/Hessian.h>
# include <CppAD/local/ForOne.h>
# include <CppAD/local/RevOne.h>
# include <CppAD/local/ForTwo.h>
# include <CppAD/local/RevTwo.h>

# endif
