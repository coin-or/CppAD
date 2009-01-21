# ifndef CPPAD_AD_FUN_INCLUDED
# define CPPAD_AD_FUN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ADFun$$
$spell 
	xk
	Ind
	bool
	taylor_
	sizeof
	const
	std
	ind_taddr_
	dep_taddr_
$$

$spell
$$

$section ADFun Objects$$

$index ADFun, object$$
$index object, ADFun$$

$head Purpose$$
An AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
is stored in an $code ADFun$$ object by its $xref/FunConstruct/$$.
The $code ADFun$$ object can then be used to calculate function values,
derivative values, and other values related to the corresponding function.

$childtable%
	cppad/local/independent.hpp%
	cppad/local/fun_construct.hpp%
	cppad/local/dependent.hpp%
	cppad/local/abort_recording.hpp%
	omh/seq_property.omh%
	cppad/local/fun_eval.hpp%
	cppad/local/drivers.hpp%
	cppad/local/fun_check.hpp%
	cppad/local/omp_max_thread.hpp%
	omh/fun_deprecated.omh
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
	// default constructor
	ADFun(void) 
	: total_num_var_(0), taylor_(CPPAD_NULL), for_jac_(CPPAD_NULL)
	{ }

	// sequence constructor
	template <typename ADvector>
	ADFun(const ADvector &x, const ADvector &y);

	// destructor
	~ADFun(void)
	{	if( taylor_ != CPPAD_NULL )
			CPPAD_TRACK_DEL_VEC(taylor_);
		if( for_jac_ != CPPAD_NULL )
			CPPAD_TRACK_DEL_VEC(for_jac_);
	}

	// assign a new operation sequence
	template <typename ADvector>
	void Dependent(const ADvector &y);  // deprecated 

	template <typename ADvector>
	void Dependent(const ADvector &x, const ADvector &y);

	// forward mode sweep
	template <typename VectorBase>
	VectorBase Forward(size_t p, const VectorBase &u);

	// reverse mode sweep
	template <typename VectorBase>
	VectorBase Reverse(size_t p, const VectorBase &v) const;

	// forward mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase ForSparseJac(size_t q, const VectorBase &Px);

	// reverse mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseJac(size_t q, const VectorBase &Py) const;

	// reverse mode Hessian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseHes(size_t q, const VectorBase &Py) const;

	// does this AD operation sequence use VecAD<Base>::reference operands
	bool use_VecAD(void) const
	{	return play_.NumVecInd() > 0; }

	// number of variables in opertion sequence
	size_t size_var(void) const
	{	return total_num_var_; }

	// number of taylor_ coefficients currently calculated (per variable)
	size_t size_taylor(void) const
	{	return taylor_per_var_; } 

	// set number of coefficients currently allocated (per variable)
	void capacity_taylor(size_t per_var);   

	// number of independent variables
	size_t Domain(void) const
	{	return ind_taddr_.size(); }

	// number of dependent variables
	size_t Range(void) const
	{	return dep_taddr_.size(); }

	// is variable a parameter
	bool Parameter(size_t i)
	{	CPPAD_ASSERT_KNOWN(
			i < dep_taddr_.size(),
			"Argument to Parameter is >= dimension of range space"
		);
		return dep_parameter_[i]; 
	}

# ifndef NDEBUG
	// in not NDEBUG case, number of comparison operations that change
	size_t CompareChange(void) const
	{	return compare_change_; }
# endif

	// calculate entire Jacobian
	template <typename VectorBase>
	VectorBase Jacobian(const VectorBase &x); 

	// calculate Hessian for one component of f
	template <typename VectorBase>
	VectorBase Hessian(const VectorBase &x, const VectorBase &w); 
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

	// calculate sparse Jacobians 
	template <typename BaseVector>
	BaseVector SparseJacobian(const BaseVector &x); 

	template <typename BaseVector, typename BoolVector>
	BaseVector SparseJacobian(const BaseVector &x, const BoolVector &p); 

	// calculate sparse Hessians 
	template <typename BaseVector>
	BaseVector SparseHessian(const BaseVector &x, const BaseVector &w); 

	template <typename BaseVector, typename BoolVector>
	BaseVector SparseHessian(
		const BaseVector &x, const BaseVector &w, const BoolVector &p
	); 

	// ------------------- Deprecated -----------------------------

	// number of variables in opertion sequence
	size_t Size(void) const
	{	return total_num_var_; }

	// number of taylor_ coefficients currently stored (per variable)
	size_t Order(void) const
	{	return taylor_per_var_ - 1; }

	// amount of memory for each variable 
	size_t Memory(void) const
	{	size_t pervar  = taylor_col_dim_ * sizeof(Base)
		+ for_jac_col_dim_ * sizeof(Pack);
		size_t total   = total_num_var_ * pervar + play_.Memory();
		return total;
	}

	// number of taylor_ coefficients currently calculated (per variable)
	size_t taylor_size(void) const
	{	return taylor_per_var_; } 
	// ------------------------------------------------------------
private:
	// maximum amount of memory required for this function object
	// mutable size_t memoryMax;

	// debug checking number of comparision operations that changed
	size_t compare_change_;

	// number of taylor_ coefficieint per variable (currently stored)
	size_t taylor_per_var_;

	// number of bits currently calculated per row of the for_jac_ array
	size_t for_jac_bit_dim_; 

	// number of columns currently allocated for taylor_ array
	size_t taylor_col_dim_;

	// number of columns currently allocated for for_jac_ array
	size_t for_jac_col_dim_;

	// number of rows (variables) in the recording (play_)
	size_t total_num_var_;

	// tape address for the independent variables
	CppAD::vector<size_t> ind_taddr_;

	// tape address and parameter flag for the dependent variables
	CppAD::vector<size_t> dep_taddr_;
	CppAD::vector<bool>   dep_parameter_;

	// the operations corresponding to this function
	player<Base> play_;

	// results of the forward mode calculations
	Base *taylor_;

	// results of the forward mode Jacobian sparsity calculations
	Pack *for_jac_;


	template <typename ADvector>
	void Dependent(ADTape<Base> *tape, const ADvector &y);
};
// ---------------------------------------------------------------------------

} // END CppAD namespace

// non-user interfaces
# include <cppad/local/forward_sweep.hpp>
# include <cppad/local/reverse_sweep.hpp>
# include <cppad/local/for_jac_sweep.hpp>
# include <cppad/local/rev_jac_sweep.hpp>
# include <cppad/local/rev_hes_sweep.hpp>

// user interfaces
# include <cppad/local/independent.hpp>
# include <cppad/local/dependent.hpp>
# include <cppad/local/fun_construct.hpp>
# include <cppad/local/abort_recording.hpp>
# include <cppad/local/fun_eval.hpp>
# include <cppad/local/drivers.hpp>
# include <cppad/local/fun_check.hpp>
# include <cppad/local/omp_max_thread.hpp>

# endif
