# ifndef CPPAD_CORE_SUBGRAPH_DEP_HPP
# define CPPAD_CORE_SUBGRAPH_DEP_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# endif

/*
$begin subgraph_depend$$

$section Computing Dependency Using A Sub-graph Method$$

$head Under Construction$$

$head Syntax$$
$icode%f%.subgraph_dep(%pattern_out%)%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$cref/AD function/glossary/AD Function/$$ corresponding to $icode f$$.
This routine computes a
$cref/dependency pattern/dependency.cpp/Dependency Pattern/$$ for
$latex F ( x )$$ using a sub-graph technique.
To be specific, for $latex i = 0 , \ldots , m-1$$,
the dependency for $latex F_i (x)$$ is computed by traversing
the sub-graph corresponding to the $th i$$ dependent variable.

$head SizeVector$$
We use $icode SizeVector$$ to denote $cref SimpleVector$$ class
$cref/with elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head pattern_out$$
This argument has prototype
$codei%
	sparse_rc<%SizeVector%>& %pattern_out%
%$$
This input value of $icode pattern_out$$ does not matter.
Upon return $icode pattern_out$$ is a
$cref/dependency pattern/dependency.cpp/Dependency Pattern/$$
for $latex F(x)$$.
To be specific, if $latex F_i (x)$$ depends on the value of $latex x_j$$
then the row, column pair  $latex (i, j)$$ appears in $icode pattern_out$$.

$end
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file subgraph_dep.hpp
Dependency for on dependent variable .
*/

/*!
Under Construction

\tparam VectorSize
is a Simple Vector class with eleements of type size_t.

\param pattern_out
Input value does not matter, upon return is dependency sparsity patter
for this function.
*/

template <typename Base>
template <typename SizeVector>
void ADFun<Base>::subgraph_dep(sparse_rc<SizeVector>& pattern_out)
{
	// number of operators in the tape
	size_t num_op = play_.num_op_rec();

	// set of operators to visit in order found
	local::pod_vector<addr_t> subgraph;

	// if in_subgrah[i_op] == i, operator i_op is already in sub-graph
	// for dependent variable with index i
	local::pod_vector<addr_t> in_subgraph;
	in_subgraph.extend( num_op );
	//
	// initilaize in_subgraph to an impossible dependent variable index
	for(size_t i_op = 0; i_op < num_op; ++i_op)
		in_subgraph[i_op] = Range();

	//
	// which arguments are variables
	local::pod_vector<bool> is_var(5);

	// for each dependent variable
	for(size_t i = 0; i < Range(); ++i)
	{	// start with an empty subgraph for this dependent variable
		subgraph.erase();

		// tape index corresponding to this dependent variable
		size_t var_index = dep_taddr_[i];

		// put operator for this dependent variable in the subgraph
		size_t sub_index    = subgraph.extend(1);
		subgraph[sub_index] = play_.var2op( var_index );

		while(sub_index < subgraph.size() )
		{	// operator for this node in the subgraph
			size_t i_op = subgraph[sub_index];
			//
			local::OpCode  op;
			addr_t* op_arg;
			size_t  i_var;
			play_.get_info(i_op, op, op_arg, i_var);

			// only operators that have results appear in subgraph
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
			CPPAD_ASSERT_UNKNOWN( op != local::CSkipOp)

			// number of arguments for this operator
			if( op == local::CSumOp )
			{
				// special case where NumArg is not correct
				CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
				//
				// The actual number of arugments for this operator is
				// op_arg[0] + op_arg[1] + 4.
			}
		}
	}

}

} // END_CPPAD_NAMESPACE
