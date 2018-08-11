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

\param dyn_ind2par_ind
is a vector with length equal to the number of dynamic parameters.
The element dyn_ind2par_ind[j] is the index in all_par_vec corresponding
to the j-th dynamic parameter.
Note that if dyn_par_is[i] is false, the i-th parameter does not
appear in this vector.

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
	const pod_vector<addr_t>&     dyn_ind2par_ind ,
	const pod_vector<opcode_t>&   dyn_par_op      ,
	const pod_vector<addr_t>&     dyn_par_arg     )
{
# if CPPAD_DYNAMIC_TRACE
	std::cout << std::endl;
	const char* cond_exp_name[] = {
		"CondExpLt",
		"CondExpLe",
		"CondExpEq",
		"CondExpGe",
		"CondExpGt",
		"CondExpNe"
	};
# endif
# ifndef NDEBUG
	for(size_t j = 0; j < num_ind_dynamic; ++j)
		CPPAD_ASSERT_UNKNOWN(
			dyn_par_is[j] && op_code_dyn( dyn_par_op[j] ) == ind_dyn
	);
# endif
	// used to hold the first two parameter arguments
	const Base* par[2];
	//
	// Initialize index in all_par_vec (none used ind_dyn operators).
	size_t i_arg = 0;
	//
	// number of dynamic parameters
	size_t num_dynamic_par = dyn_ind2par_ind.size();
	CPPAD_ASSERT_UNKNOWN( num_ind_dynamic <= num_dynamic_par );
	//
	// Loop the dynamic parameters skipping independent dynamics at beginning.
	for(size_t i_dyn = num_ind_dynamic; i_dyn < num_dynamic_par; ++i_dyn)
	{	// parametere index for this dynamic parameter
		size_t i_par = size_t( dyn_ind2par_ind[i_dyn] );
		//
		// operator for this dynamic parameter
		op_code_dyn op = op_code_dyn( dyn_par_op[i_dyn] );
		//
		// number of arguments
		size_t n_arg   = num_arg_dyn(op);
		//
		// first arguments
		par[0] = & all_par_vec[ dyn_par_arg[i_arg + 0] ];
		//
		// second argument if this operator has a second argument
		if( 1 < n_arg )
			par[1] = & all_par_vec[ dyn_par_arg[i_arg + 1] ];
		else
		{	// to avoid warning about may not be initialized
			par[1] = CPPAD_NULL;
		}
		switch(op)
		{
			// ---------------------------------------------------------------
			// standard_math_98
			// acos
			case acos_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = acos( *par[0] );
			break;

			// asin
			case asin_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = asin( *par[0] );
			break;

			// atan
			case atan_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = atan( *par[0] );
			break;

			// cos
			case cos_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = cos( *par[0] );
			break;

			// cosh
			case cosh_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = cosh( *par[0] );
			break;

			// exp
			case exp_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = exp( *par[0] );
			break;

			// fabs
			case fabs_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = fabs( *par[0] );
			break;

			// log
			case log_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = log( *par[0] );
			break;

			// sin
			case sin_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = sin( *par[0] );
			break;

			// sinh
			case sinh_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = sinh( *par[0] );
			break;

			// sqrt
			case sqrt_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = sqrt( *par[0] );
			break;

			// tan
			case tan_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = tan( *par[0] );
			break;

			// tanh
			case tanh_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = tanh( *par[0] );
			break;

# if CPPAD_USE_CPLUSPLUS_2011
			// ---------------------------------------------------------------
			// asinh
			case asinh_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = asinh( *par[0] );
			break;

			// acosh
			case acosh_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = acosh( *par[0] );
			break;

			// atanh
			case atanh_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = atanh( *par[0] );
			break;

			// expm1
			case expm1_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = expm1( *par[0] );
			break;

			// erf
			case erf_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = erf( *par[0] );
			break;

			// log1p
			case log1p_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = log1p( *par[0] );
			break;
# endif
			// ---------------------------------------------------------------
			// abs
			case abs_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = fabs( *par[0] );
			break;

			// add
			case add_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			all_par_vec[i_par] = *par[0] + *par[1];
			break;

			// div
			case div_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			all_par_vec[i_par] = *par[0] / *par[1];
			break;

			// mul
			case mul_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			all_par_vec[i_par] = *par[0] * *par[1];
			break;

			// pow
			case pow_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			all_par_vec[i_par] = pow( *par[0], *par[1] );
			break;

			// sign
			case sign_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			all_par_vec[i_par] = sign( *par[0] );
			break;

			// sub
			case sub_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			all_par_vec[i_par] = *par[0] - *par[1];
			break;

			// zmul
			case zmul_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			all_par_vec[i_par] = azmul( *par[0],  *par[1] );
			break;

			// ---------------------------------------------------------------
			// cond_exp(cop, left, right, if_true, if_false)
			// (not yet implemented)
			case cond_exp_dyn:
			CPPAD_ASSERT_UNKNOWN( n_arg == 5 );
			all_par_vec[i_par] = CondExpOp(
				CompareOp(   dyn_par_arg[i_arg + 0] ) , // cop
				all_par_vec[ dyn_par_arg[i_arg + 1] ] , // left
				all_par_vec[ dyn_par_arg[i_arg + 2] ] , // right
				all_par_vec[ dyn_par_arg[i_arg + 3] ] , // if_true
				all_par_vec[ dyn_par_arg[i_arg + 4] ]   // if_false
			);
# if CPPAD_DYNAMIC_TRACE
			std::cout << std::setw(10) << std::left << all_par_vec[i_par]
			<< " = " << std::right << std::setw(10)
			<< cond_exp_name[ dyn_par_arg[i_arg + 0] ] << "(";
			for(size_t i = 1; i < 5; ++i)
			{	std::cout << std::setw(10) << std::right
				<< all_par_vec[ dyn_par_arg[i_arg + i] ];
				if( i < 4 )
					std::cout << ",";
			}
			std::cout << ")" << std::endl;
# endif
			break;

			// ---------------------------------------------------------------
			default:
			std::cerr << "op_code_dyn = " << op_name_dyn(op) << std::endl;
			CPPAD_ASSERT_UNKNOWN(false);
			break;
		}
# if CPPAD_DYNAMIC_TRACE
		if( op != cond_exp_dyn )
		{	std::cout << std::setw(10) << std::left << all_par_vec[i_par]
			<< " = " << std::setw(10) << std::right << op_name_dyn(op)
			<< "(" << std::setw(10) << std::right << *par[0];
			if( 1 < n_arg )
				std::cout << ", " << std::setw(8) << std::right << *par[1];
			std::cout << ")" << std::endl;
		}
# endif
		i_arg += num_arg_dyn(op);
	}
	CPPAD_ASSERT_UNKNOWN( i_arg == dyn_par_arg.size() )
	return;
}

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE

# endif
