# ifndef CPPAD_LOCAL_OPTIMIZE_CSUM_STACKS_HPP
# define CPPAD_LOCAL_OPTIMIZE_CSUM_STACKS_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <stack>
# include <cppad/local/optimize/csum_op_info.hpp>

/*!
\file csum_stacks.hpp
Information about one cumulative summation operation.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Information about one cumulative summation operation.
*/
struct struct_csum_stacks {

	/// old operator indices for this cummulative summation
	std::stack<struct struct_csum_op_info>      op_info;

	/// old variable indices to be added
	std::stack<addr_t>                          add_var;

	/// old variable indices to be subtracted
	std::stack<addr_t>                          sub_var;

	/// dynamic parameter indices to be added
	std::stack<addr_t>                          add_dyn;

	/// dynamic parameter indices to be subtracted
	std::stack<addr_t>                          sub_dyn;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
