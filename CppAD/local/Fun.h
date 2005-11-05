# ifndef CppADADFunIncluded
# define CppADADFunIncluded

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
$begin ADFun$$
$spell 
	Ind
	bool
	Taylor
	sizeof
	const
	std
	indvar
	depvar
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
The length of $italic Y$$ is the number of dependent variables
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
returns the number of memory units ($code sizeof$$) required to store
the operations and derivative information currently stored in $italic F$$.
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
operations corresponding to
the previous zero order $xref/Forward/$$ that have a different result from
when $italic F$$ was created by taping an algorithm.
When it is non-zero, some comparison operator results are different.
This can be used to detect when $italic F$$
no longer represents the algorithm that was taped.

$head Example$$
The files
$xref/Fun.h/$$ and $xref/CompareChange.cpp/$$
contain examples and tests of these operations.
They return true if they succeed and false otherwise.

$contents%
	CppAD/local/Parameter.h%
	CppAD/local/Independent.h%
	omh/Drivers.omh%
	CppAD/local/Forward.h%
	CppAD/local/Reverse.h%
	Example/Fun.h%
	Example/CompareChange.cpp%
	omh/MulTape.omh%
	CppAD/local/ForSparseJac.h
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
		delete [] Taylor;
		if( Partial != CppADNull )
			delete [] Partial;
		if( ForJac != CppADNull )
			delete [] ForJac;

	}

	// forward mode sweep
	template <typename VectorBase>
	VectorBase Forward(size_t p, const VectorBase &u);

	// reverse mode sweep
	template <typename VectorBase>
	VectorBase Reverse(size_t p, const VectorBase &v);

	// forward mode dependency sweep
	template <typename VectorBase>
	VectorBase ForSparseJac(size_t q, const VectorBase &Dx);

	// size of this function object
	size_t Size(void) const
	{	return totalNumVar; }

	// order of this function object
	size_t Order(void) const
	{	return order; }

	// number of independent variables
	size_t Domain(void) const
	{	return indvar.size(); }

	// number of dependent variables
	size_t Range(void) const
	{	return depvar.size(); }

	// is variable a parameter
	bool Parameter(size_t i)
	{	CppADUsageError(
			i < depvar.size(),
			"Argument to Parameter is >= dimension of range space"
		);
		return parameter[i]; 
	}

	// amount of memory for each variable
	size_t Memory(void) const
	{	size_t pervar  = (TaylorColDim + PartialColDim) * sizeof(Base)
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
	// debug checking number of comparision operations that changed
	size_t compareChange;

	// order of the informaiton currently stored in Taylor array
	size_t order;

	// number of columns currently allocated for Taylor array
	size_t TaylorColDim;

	// number of columns currently allocated for Partial array
	size_t PartialColDim;

	// number of columns currently allocated for ForJac array
	size_t ForJacColDim;

	// number of rows (variables) in the recording (Rec)
	size_t totalNumVar;

	// row indices for the independent variables
	CppAD::vector<size_t> indvar;

	// row indices for the dependent variables
	CppAD::vector<size_t> depvar;

	// which of the dependent variables are parameters 
	CppAD::vector<bool> parameter;

	// the operations corresponding to this function
	TapeRec<Base> *Rec;

	// results of the forward mode calculations
	Base *Taylor;

	// results of the reverse mode calculations
	Base *Partial;

	// results of the forward mode Jacobian sparsity calculations
	Pack *ForJac;
};
// ---------------------------------------------------------------------------

template <typename Base>
template <typename VectorADBase>
ADFun<Base>::ADFun(const VectorADBase &u, const VectorADBase &z)
{	size_t   n = z.size();
	size_t   i;
	size_t   z_taddr;
	OpCode   op;

	// check VectorADBase is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorADBase>();

	CppADUsageError(
		AD<Base>::Tape()->state == Recording,
		"Can not create an ADFun because "
		" tape is not currently recording."
	);


	// set parameter flag and
	// create a copy of z where parameters are in the tape
	CppADUnknownError( NumInd(ParOp) == 1 );
	parameter.resize(n);
	VectorADBase z_copy(n);
	for(i = 0; i < n; i++)
	{	z_copy[i].value = z[i].value;
		z_taddr         = z[i].taddr;
		parameter[i]    = CppAD::Parameter(z[i]);
		if( parameter[i] )
			z_taddr = AD<Base>::Tape()->RecordParOp( z[i].value );

		z_copy[i].MakeVariable( z_taddr );
	}

	// total number of varables in this recording 
	totalNumVar = AD<Base>::Tape()->Rec.TotNumVar();

	// current order and row dimensions
	order         = 0;
	TaylorColDim  = 1;
	PartialColDim = 0;
	ForJacColDim  = 0;

	// recording
	Rec     = new TapeRec<Base>( AD<Base>::Tape()->Rec );
	Taylor  = new Base[totalNumVar];
	Partial = CppADNull;
	ForJac  = CppADNull;

	// number of elements in u
	n = u.size();
	CppADUsageError(
		n < totalNumVar,
		"independent variables vector has changed"
	);

	// set initial independent variable values
	indvar.resize(n);
	for(i = 0; i < n; i++)
	{	
		CppADUsageError( 
			u[i].taddr == i+1,
			"independent variable vector has changed"
		);
		// i+1 is both the operator and independent variable taddr
		op = Rec->GetOp(i+1);
		CppADUsageError(
			op == InvOp,
			"independent variable vector has changed"
		);
		indvar[i]   = i+1;
		Taylor[i+1] = u[i].value;
	}

	// avoid the special case where there is nothing past
	// the independent variable records
	if( n + 1 < Rec->NumOp() )
	{	op = Rec->GetOp(n+1);
		CppADUsageError(
			op != InvOp,
			"independent variable vector has changed"
		);
	}

	// dep
	n = z_copy.size();
	depvar.resize(n);
	for(i = 0; i < n; i++)
	{	z_taddr  = z_copy[i].taddr;
		CppADUnknownError( z_taddr > 0 );
		CppADUnknownError( z_taddr < totalNumVar );
		depvar[i] = z_taddr;
	}

	// use independent variable values to fill in values for others
	compareChange = ADForward(false, 0, totalNumVar, Rec, TaylorColDim, Taylor);
	CppADUnknownError( compareChange == 0 );

	// check the dependent variable values
	n = z.size();
	for(i = 0; i < n; i++)
		CppADUnknownError( Taylor[depvar[i]] == z[i].value );

	// We are now done with the AD<Base> tape so erase it
	AD<Base>::Tape()->Erase();
}

} // END CppAD namespace

# include <CppAD/local/Parameter.h>
# include <CppAD/local/Independent.h>
# include <CppAD/local/ADForward.h>
# include <CppAD/local/Forward.h>
# include <CppAD/local/ADReverse.h>
# include <CppAD/local/Reverse.h>
# include <CppAD/local/ForJacSweep.h>
# include <CppAD/local/ForSparseJac.h>
//
// driver routines
# include <CppAD/local/Jacobian.h>
# include <CppAD/local/Hessian.h>
# include <CppAD/local/ForOne.h>
# include <CppAD/local/RevOne.h>
# include <CppAD/local/ForTwo.h>
# include <CppAD/local/RevTwo.h>

# endif
