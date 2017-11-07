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

	// set of operators in the subgraph
	local::pod_vector<addr_t> subgraph;

	// set of indepndent variables operators connected to the subgraph
	local::pod_vector<addr_t> connected;

	// if sub_or_connected[i_op] == i, one of the following holds:
	// 1. operator i_op is already in sub-graph for dependent variable i
	// 2. i_op corresponds to an independent variable connected to subgraph
	local::pod_vector<addr_t> sub_or_connected( num_op );
	//
	// initilaize sub_or_connected to an impossible dependent variable index
	for(size_t i_op = 0; i_op < num_op; ++i_op)
		sub_or_connected[i_op] = addr_t( Range() );

	// which arguments, for one operator, are variables
	local::pod_vector<bool> variable;

	// for each dependent variable
	for(size_t i_dep = 0; i_dep < Range(); ++i_dep)
	{	// start with an empty subgraph and empty connected set
		subgraph.erase();
		connected.erase();

		// tape index corresponding to this dependent variable
		size_t var_index = dep_taddr_[i_dep];

		// put operator for this dependent variable in the subgraph
		size_t i_op  = play_.var2op( var_index );
		CPPAD_ASSERT_UNKNOWN( play_.GetOp(i_op) != local::InvOp );
		subgraph.push_back(i_op);
		sub_or_connected[i_op] = addr_t( i_dep );

		// check all arguments that are variables in the subgraph
		size_t sub_index = 0;
		while(sub_index < subgraph.size() )
		{	// operator for this node in the subgraph
			i_op = subgraph[sub_index];
			//
			// get the arguments for this operator
			local::OpCode  op;
			addr_t* op_arg;
			size_t  i_var;
			play_.get_info(i_op, op, op_arg, i_var);

			// only operators that have results appear in subgraph
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );

			// 2DO: handle this case
			CPPAD_ASSERT_UNKNOWN( op == local::UserOp );

			// variable = which operator arguments are variables
			// num_arg  = true number of arguments for this operator
			size_t num_arg = arg_is_variable(op, op_arg, variable);

			// loop through the variables
			for(size_t j = 0; j < num_arg; ++j) if( variable[j] )
			{	// index for this variables
				size_t j_var = op_arg[j];

				// operator for this variables
				size_t j_op  = play_.var2op(j_var);

				if( sub_or_connected[j_op] != addr_t(i_dep) )
				{	// this operator is not yet in subgraph or connected
					if( play_.GetOp(j_op) == local::InvOp )
					{	// This is an independent variable operator
						CPPAD_ASSERT_UNKNOWN( j_var == j_op - 1 );

						// add to the connected set of independent variables
						connected.push_back( addr_t(j_var) );
					}
					else
					{	// add to the subgraph
						subgraph.push_back( addr_t( j_op ) );
					}
					sub_or_connected[j_op] = addr_t( i_dep );
				}
			}
			++sub_index;
		}
	}
}

} // END_CPPAD_NAMESPACE
