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

$section ADFun Objects$$

$index ADFun, object$$

$table
$bold Syntax$$ $cnext 
$syntax%ADFun<%Base%> %F%(%X%, %Y%)%$$
$tend

$fend 20$$

$head F$$
See $xref/FunConstruct/$$ for a description of the
constructor for $italic F$$.

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
	CppAD/local/Independent.h%
	CppAD/local/FunConstruct.h%
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

} // END CppAD namespace

# include <CppAD/local/Independent.h>
# include <CppAD/local/FunConstruct.h>
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
