# ifndef CPPAD_CORE_BASE2AD_HPP
# define CPPAD_CORE_BASE2AD_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin base2ad$$
$spell
	af
$$

$spell
$$

$section Create an AD<Base> Function From an Base Function$$

$head Syntax$$
$icode%af% = %f%.base2ad()%$$

$head Base$$
This is the base type used to recorded the operation sequence in $icode f$$
and $icode af$$; i.e., the type $codei%AD<%Base%>%$$ was used to record
the operation sequence.

$head f$$
This object has prototype
$codei%
	ADFun<%Base%> %f%
%$$
It does it's derivative calculations using the type $icode Base$$.

$head af$$
This object has prototype
$codei%
	ADFun< AD<%Base%> , %Base% > %af%
%$$
It has the same operation sequence as $icode f$$,
but it does it's derivative calculations using the type
$codei%AD<%Base>%$$.
This enables one to record new functions that are defined
using derivatives of the function $icode f$$.

$children%
	example/general/base2ad.cpp
%$$
$head Example$$
The file $cref base2ad.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file base2ad.hpp
*/
/// Create an ADFun< AD<Base>, Base > from this ADFun<Base>
template <typename Base, typename RecBase>
ADFun< AD<Base>, RecBase > ADFun<Base,RecBase>::base2ad(void) const
{	ADFun< AD<Base>, RecBase > fun;
	//
	// size_t objects
	fun.has_been_optimized_        = has_been_optimized_;
	fun.check_for_nan_             = check_for_nan_;
	fun.compare_change_count_      = compare_change_count_;
	fun.compare_change_number_     = compare_change_number_;
	fun.compare_change_op_index_   = compare_change_op_index_;
	fun.num_order_taylor_          = num_order_taylor_;
	fun.cap_order_taylor_          = cap_order_taylor_;
	fun.num_direction_taylor_      = num_direction_taylor_;
	fun.num_var_tape_              = num_var_tape_;
	//
	// pod_vector objects
	fun.ind_taddr_                 = ind_taddr_;
	fun.dep_taddr_                 = dep_taddr_;
	fun.dep_parameter_             = dep_parameter_;
	fun.cskip_op_                  = cskip_op_;
	fun.load_op_                   = load_op_;
	//
	// pod_maybe_vector< AD<Base> > = pod_maybe_vector<Base>
	fun.taylor_.resize( taylor_.size() );
	for(size_t i = 0; i < taylor_.size(); ++i)
		fun.taylor_[i] = taylor_[i];
	//
	// player
	// (uses move semantics when CPPAD_USE_CPLUSPLUS_2011 is 1)
	fun.play_ = play_.base2ad();
	//
	// subgraph
	fun.subgraph_info_ = subgraph_info_;
	//
	// sparse_pack
	fun.for_jac_sparse_pack_ = for_jac_sparse_pack_;
	//
	// sparse_list
	fun.for_jac_sparse_set_  = for_jac_sparse_set_;
	//
	return fun;
}

} // END_CPPAD_NAMESPACE
# endif
