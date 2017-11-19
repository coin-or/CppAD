# ifndef CPPAD_CORE_REVERSE_SUBGRAPH_HPP
# define CPPAD_CORE_REVERSE_SUBGRAPH_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin reverse_subgraph$$
$spell
	subgraph
	Subgraphs
	dw
	Taylor
	Bool
	const
$$

$section Reverse Mode Using Subgraphs$$

$head Under Construction$$

$head Syntax$$
$icode%f%.reverse_subgraph(%select_domain%)
%$$
$icode%f%.reverse_subgraph(%dw%, %q%, %ell%)
%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$cref/AD function/glossary/AD Function/$$ corresponding to $icode f$$.
Reverse mode computes the derivative of the $cref Forward$$ mode
$cref/Taylor coefficients/glossary/Taylor Coefficient/$$
with respect to the domain variable $latex x$$.

$head Notation$$
We use the reverse mode
$cref/notation/reverse_any/Notation/$$ with the following change:
the vector
$cref/w^(k)/reverse_any/Notation/w^(k)/$$ is defined
$latex \[
w_i^{(k)} = \left\{ \begin{array}{ll}
	1 & {\rm if} \; k = q-1 \; \R{and} \; i = \ell
	\\
	0       & {\rm otherwise}
\end{array} \right.
\] $$

$head BaseVector$$
The type $icode BaseVector$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$icode Base$$.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$head BoolVector$$
The type $icode BoolVector$$ is a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code bool$$.

$head select_domain$$
The argument $icode select_domain$$ has prototype
$codei%
	const %BoolVector%& %select_domain%
%$$
It has size $latex n$$ and specifies which independent variables
to include in the calculation.
If $icode%select_domain%[%j%]%$$ is false,
it is assumed that $latex u^{(k)}_j = 0$$ for $latex k > 0$$; i.e.,
the $th j$$ component of the Taylor coefficient for $latex x$$,
with order greater that zero, are zero; see
$cref/u^(k)/reverse_any/Notation/u^(k)/$$.

$head q$$
The argument $icode q$$ has prototype
$codei%
	size_t %q%
%$$
and specifies the number of Taylor coefficient orders to be differentiated.

$head ell$$
The argument $icode ell$$ has prototype
$codei%
	size_t %ell%
%$$
and specifies the dependent variable index that we are computing
the derivatives for; i.e. $latex \ell$$.
This index can only be used once per, and after, a call that selects
the independent variables using $icode select_domain$$.

$head dw$$
The argument $icode dw$$ has prototype
$codei%
	%Vector% %dw%
%$$
Its input size and value does not matter.
Upon return,
it is a vector with size $latex n \times q$$.
For $latex k = 0 , \ldots , q-1$$,
and $icode j$$ such that $icode%select_domain%[%j%]%$$ is true,
$latex \[
	dw[ j * q + k ] = W^{(1)} ( x )_{j,k}
\] $$
If $icode%select_domain%[%j%]%$$ is false,
$latex \[
	dw[ j * q + k ] = 0.0
\] $$
Note that this corresponds to the convention when
$cref/w/reverse_any/w/$$ has size $icode%m% * %q%$$ in
normal reverse mode.

$end
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

template <typename Base>
template <typename VectorBool>
void ADFun<Base>::reverse_subgraph( const VectorBool& select_domain )
{	using local::pod_vector;

	CPPAD_ASSERT_UNKNOWN(
		dep_taddr_.size() == subgraph_info_.n_dep()
	);
	CPPAD_ASSERT_UNKNOWN(
		select_domain.size() == subgraph_info_.n_ind()
	);

	// map_user_op
	if( subgraph_info_.map_user_op().size() == 0 )
		subgraph_info_.set_map_user_op(&play_);
	else
	{	CPPAD_ASSERT_UNKNOWN( subgraph_info_.check_map_user_op(&play_) );
	}
	const pod_vector<addr_t>& map_user_op( subgraph_info_.map_user_op() );
	CPPAD_ASSERT_UNKNOWN( map_user_op.size() == play_.num_op_rec() );

	// initialize for reverse mode subgraph computations
	subgraph_info_.init_rev(&play_, select_domain);
	pod_vector<addr_t>& in_subgraph( subgraph_info_.in_subgraph() );
	CPPAD_ASSERT_UNKNOWN( in_subgraph.size() == play_.num_op_rec() );

	return;
}

template <typename Base>
template <typename VectorBase>
void ADFun<Base>::reverse_subgraph(
	VectorBase& dw  ,
	size_t      q   ,
	size_t      ell )
{	using local::pod_vector;

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();
	CPPAD_ASSERT_KNOWN(
		q > 0,
		"The second argument to Reverse must be greater than zero."
	);
	CPPAD_ASSERT_KNOWN(
		num_order_taylor_ >= q,
		"Less than q Taylor coefficients are currently stored"
		" in this ADFun object."
	);
	CPPAD_ASSERT_KNOWN(
		num_direction_taylor_ == 1,
		"reverse mode for Forward(q, r, xq) with more than one direction"
		"\n(r > 1) is not yet supported."
	);
	CPPAD_ASSERT_KNOWN(
		ell < dep_taddr_.size(),
		"dependent variable index in to large for this function"
	);
	CPPAD_ASSERT_KNOWN(
		subgraph_info_.process_range()[ell] == false,
		"This dependent variable index has already been processed\n"
		"after the previous reverse_subgraph(select_domain)."
	);


	// subgraph of operators connected to dependent variable ell
	pod_vector<addr_t> subgraph;
	subgraph_info_.get_rev(
		&play_, dep_taddr_, addr_t(ell), subgraph
	);

	// Add all the atomic function call operators
	// for calls that have first operator in the subgraph
	local::subgraph::entire_call(&play_, subgraph);

	// initialize Partial matrix to zero on subgraph
	Base zero(0);
	local::pod_vector<Base> Partial(num_var_tape_ * q);
	for(size_t k = 0; k < subgraph.size(); ++k)
	{	size_t               i_op = size_t( subgraph[k] );
		local::OpCode        op;
		const addr_t*        arg;
		size_t               i_var;
		play_.get_op_info(i_op, op, arg, i_var);
		if( NumRes(op) == 0 )
		{	CPPAD_ASSERT_UNKNOWN(
				op == local::UserOp  ||
				op == local::UsrapOp ||
				op == local::UsravOp ||
				op == local::UsrrpOp
			);
		}
		else
		{	CPPAD_ASSERT_UNKNOWN( i_var > NumRes(op) );
			for(size_t i = i_var - NumRes(op); i <= i_var; ++i)
			{	for(size_t j = 0; j < q; ++j)
					Partial[i * q + j] = zero;
			}
		}
	}

	// set partial to one for component we are differentiating
	Partial[ dep_taddr_[ell] * q + q - 1] = Base(1);

	// evaluate the derivatives
	CPPAD_ASSERT_UNKNOWN( cskip_op_.size() == play_.num_op_rec() );
	CPPAD_ASSERT_UNKNOWN( load_op_.size()  == play_.num_load_op_rec() );
	size_t n = Domain();
	local::reverse_sweep(
		q - 1,
		n,
		num_var_tape_,
		&play_,
		cap_order_taylor_,
		taylor_.data(),
		q,
		Partial.data(),
		cskip_op_.data(),
		load_op_,
		subgraph
	);

	// return the derivative values
	const pod_vector<bool>& select_domain( subgraph_info_.select_domain() );
	dw.resize(n * q);
	for(size_t j = 0; j < n; j++)
	{	// independent variable taddr equals its operator taddr
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );
		//
		if( select_domain[j] )
		{	for(size_t k = 0; k < q; k++)
				dw[j * q + k ] = Partial[ind_taddr_[j] * q + k];
		}
		else
		{	for(size_t k = 0; k < q; k++)
				dw[j * q + k ] = zero;
		}
	}
	CPPAD_ASSERT_KNOWN( ! ( hasnan(dw) && check_for_nan_ ) ,
		"f.reverse_subgraph(dw, q, ell): dw has a nan,\n"
		"but none of f's Taylor coefficents are nan."
	);
	return;
}

} // END_CPPAD_NAMESPACE
# endif
