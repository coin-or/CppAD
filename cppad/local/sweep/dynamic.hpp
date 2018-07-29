# ifndef CPPAD_LOCAL_SWEEP_DYNAMIC_HPP
# define CPPAD_LOCAL_SWEEP_DYNAMIC_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/play/user_op_info.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/dynamic.hpp
Under Consruction
*/

/*
\def CPPAD_ATOMIC_CALL
This avoids warnings when NDEBUG is defined and user_ok is not used.
If NDEBUG is defined, this resolves to
\code
	user_atom->forward
\endcode
otherwise, it respolves to
\code
	user_ok = user_atom->forward
\endcode
This maco is undefined at the end of this file to facillitate is
use with a different definition in other files.
*/
# ifdef NDEBUG
# define CPPAD_ATOMIC_CALL user_atom->forward
# else
# define CPPAD_ATOMIC_CALL user_ok = user_atom->forward
# endif

/*!
\def CPPAD_DYNAMIC_TRACE
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace for each dynamic parameter is printed.
*/
# define CPPAD_DYNAMIC_TRACE 0

/*!
Compute dynamic parameters.

\tparam Base
The type of the parameters.

\param num_ind_dynamic
number of independent dynamic parameters

\param all_par_vec
is the vector of all the parameters.
Ths constant parameters and the independent dynamic parameters are inputs.
The other dynamic parameters are outputs.

\param dyn_par_is
is a vector with the same length as par_vec.
The i-th parameter is dynamic if and only if dyn_par_is[i] is true.

\param dyn_par_op
is a vector with length equal to the number of dynamic parameters.
The element dyn_par_op_[j] is the operator for the j-th dynamic parameter.
Note that if dyn_par_is[i] is false, the i-th parameter does not
have a parameter in this list.

\param dyn_par_arg
is a vector containing the arguments for the dynamic parameters.
The first argument for the j-th dynamic parameter is dyn_par_arg[k]
where
\code
	k = NumArg( dyn_par_op[0] ) + ... + NumArg( dyn_par_op[j-1] )
\endcode
The arguments for each dynamic parameter have index value
lower than the index value for the parameter.
*/
template <class Base>
void dynamic(
	size_t                        num_ind_dynamic ,
	pod_vector_maybe<Base>&       all_par_vec     ,
	const pod_vector<bool>&       dyn_par_is      ,
	const pod_vector<opcode_t>&   dyn_par_op      ,
	const pod_vector<addr_t>&     dyn_par_arg     )
{
# ifndef NDEBUG
	for(size_t j = 0; j < num_ind_dynamic; ++j)
		CPPAD_ASSERT_UNKNOWN(
			dyn_par_is[j] && op_code_dyn( dyn_par_op[j] ) == inv_dyn
	);
# endif
# if CPPAD_DYNAMIC_TRACE
	std::cout << std::endl;
# endif
	const Base* par[2];
	size_t i_op  = num_ind_dynamic;
	size_t i_arg = 0;
	for(size_t i_par = num_ind_dynamic; i_par < all_par_vec.size(); ++i_par)
	if( dyn_par_is[i_par] )
	{	op_code_dyn op = op_code_dyn( dyn_par_op[i_op] );
		size_t n_arg   = num_arg_dyn(op);
		CPPAD_ASSERT_UNKNOWN( 0 < n_arg && n_arg < 3 );
		par[0] = & all_par_vec[ dyn_par_arg[i_arg + 0] ];
		if( 1 < n_arg )
			par[1] = & all_par_vec[ dyn_par_arg[i_arg + 1] ];
		switch(op)
		{	case abs_dyn:
			all_par_vec[i_par] = fabs( *par[0] );
			break;

			case add_dyn:
			all_par_vec[i_par] = *par[0] + *par[1];
			break;

			case fabs_dyn:
			all_par_vec[i_par] = fabs( *par[0] );
			break;

			case sin_dyn:
			all_par_vec[i_par] = sin( *par[0] );
			break;

			default:
			CPPAD_ASSERT_UNKNOWN(false);
			break;
		}
# if CPPAD_DYNAMIC_TRACE
		using std::setw;
		using std::left;
		using std::right;
		std::cout << setw(10) << left << all_par_vec[i_par]
		<< " = " << setw(8) << right << op_name_dyn(op)
		<< "(" << setw(8) << right << *par[0];
		if( 1 < n_arg )
			std::cout << ", " << setw(8) << right << *par[1];
		std::cout << ")" << std::endl;
# endif
		++i_op;
		i_arg += num_arg_dyn(op);
	}
	return;
}

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE

# endif
