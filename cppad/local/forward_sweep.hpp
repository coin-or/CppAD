/* $Id$ */
# ifndef CPPAD_FORWARD_SWEEP_INCLUDED
# define CPPAD_FORWARD_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

CPPAD_BEGIN_NAMESPACE
/*!
\file forward_sweep.hpp
Compute zero order forward mode Taylor coefficients.
*/

/*!
\def CPPAD_FORWARD_SWEEP_TRACE
This value is either zero or one. 
Zero is the normal operational value.
If it is one, a trace of every forward_sweep computation is printed.
*/
# define CPPAD_FORWARD_SWEEP_TRACE 0

/*!
Compute arbitrary order forward mode Taylor coefficients.

\tparam Base
base type for the operator; i.e., this operation sequence was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param print
If \a print is false,
suppress the output that is otherwise generated 
by the PripOp and PrivOp instructions.

\param d
is the order of the Taylor coefficients that are computed during this call. 

\param n
is the number of independent variables on the tape.

\param numvar
is the total number of variables on the tape.
This is also equal to the number of rows in the matrix \a Taylor; i.e.,
Rec->num_rec_var().

\param Rec
The information stored in \a Rec
is a recording of the operations corresponding to the function
\f[
	F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables and
\f$ m \f$ is the number of dependent variables.
\n
\n
The object \a Rec is effectly constant.
There are two exceptions to this.
The first exception is that while palying back the tape
the object \a Rec holds information about the current location
with in the tape and this changes during palyback. 
The second exception is the fact that the 
zero order ( \a d = 0 ) versions of the VecAD operators LdpOp and LdvOp 
modify the corresponding \a op_arg values returned by 
\ref player::next_forward and \ref player::next_reverse; see the
\link load_op.hpp LdpOp and LdvOp \endlink operations.

\param J
Is the number of columns in the coefficient matrix \a Taylor.
This must be greater than or equal \a d + 1.

\param Taylor
\b Input: For j = 1 , ... , \a n, and for k = 0 , ... , \a d,
\a Taylor [ j * J + k ]
is the k-th order Taylor coefficient corresponding to 
variable with index j on the tape 
(independent variable with index (j-1) in the independent variable vector).
\n
\n
\b Output: For i = \a n + 1, ... , \a numvar - 1, and for k = 0 , ... , \a d,
\a Taylor [ i * J + k ]
is the k-th order Taylor coefficient for the variable with 
index i on the tape.

\a return
If \a d is not zero, the return value is zero.
If \a d is zero,
the return value is equal to the number of ComOp operations
that have a different result from when the information in 
\a Rec was recorded.
(Note that if NDEBUG is true, there are no ComOp operations
in Rec and hence this return value is always zero.)
*/

template <class Base>
size_t forward_sweep(
	bool                  print,
	size_t                d,
	size_t                n,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                J,
	Base                 *Taylor
)
{	CPPAD_ASSERT_UNKNOWN( J >= d + 1 );

	// op code for current instruction
	OpCode           op;

	// index for current instruction
	size_t         i_op;

	// next variables 
	size_t        i_var;

# if CPPAD_USE_FORWARD0SWEEP
	CPPAD_ASSERT_UNKNOWN( d > 0 );
# else
	size_t*         non_const_arg;
# endif
	const size_t   *arg = 0;

	size_t            i;

	// initialize the comparision operator (ComOp) counter
	size_t compareCount = 0;

	// if this is an order zero calculation, initialize vector indices
	size_t *VectorInd = CPPAD_NULL;  // address for each element
	bool   *VectorVar = CPPAD_NULL;  // is element a variable
	i = Rec->num_rec_vecad_ind();
	if( i > 0 )
	{	VectorInd = CPPAD_TRACK_NEW_VEC(i, VectorInd);
		VectorVar = CPPAD_TRACK_NEW_VEC(i, VectorVar);
		while(i--)
		{	VectorInd[i] = Rec->GetVecInd(i);
			VectorVar[i] = false;
		}
	}

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );

	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = Rec->num_rec_par();

	// length of the text vector (used by CppAD assert macros)
	const size_t num_text = Rec->num_rec_text();

	// pointer to the beginning of the parameter vector
	const Base* parameter = 0;
	if( num_par > 0 )
		parameter = Rec->GetPar();

	// pointer to the beginning of the text vector
	const char* text = 0;
	if( num_text > 0 )
		text = Rec->GetTxt(0);
	

	// skip the BeginOp at the beginning of the recording
	Rec->start_forward(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
# if CPPAD_FORWARD_SWEEP_TRACE
	std::cout << std::endl;
# endif
	while(op != EndOp)
	{
		// this op
		Rec->next_forward(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );  
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );  

		// action depends on the operator
		switch( op )
		{
			case AbsOp:
			forward_abs_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AddvvOp:
			forward_addvv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_addpv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AcosOp:
			// variables: sqrt(1 - x * x), acos(x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_acos_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AsinOp:
			// results: sqrt(1 - x * x), asin(x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_asin_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AtanOp:
			// results: 1 + x * x, atan(x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_atan_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case CSumOp:
			// CSumOp has a variable number of arguments and
			// next_forward thinks it one has one argument.
			// we must inform next_forward of this special case.
			Rec->forward_csum(op, arg, i_op, i_var);
			forward_csum_op(
				d, i_var, arg, num_par, parameter, J, Taylor
			);
			break;
			// -------------------------------------------------

			case CExpOp:
			forward_cond_op(
				d, i_var, arg, num_par, parameter, J, Taylor
			);
			break;
			// ---------------------------------------------------

			case ComOp:
# if ! USE_FORWARD0SWEEP 
			if( d == 0 ) forward_comp_op_0(
			compareCount, arg, num_par, parameter, J, Taylor
			);
# endif
			break;
			// ---------------------------------------------------

			case CosOp:
			// variables: sin(x), cos(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_cos_op(d, i_var, arg[0], J, Taylor);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// variables: sinh(x), cosh(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_cosh_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case DisOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
				forward_dis_op_0(i_var, arg, J, Taylor);
			else
# endif
			{	Taylor[ i_var * J + d] = Base(0);
			}
			break;
			// -------------------------------------------------

			case DivvvOp:
			forward_divvv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_divpv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_divvp_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case EndOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 0);
			break;
			// -------------------------------------------------

			case ExpOp:
			forward_exp_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
			break;
			// -------------------------------------------------

			case LdpOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
			{	non_const_arg = Rec->forward_non_const_arg();
				forward_load_p_op_0(
					i_var, 
					non_const_arg, 
					num_par, 
					parameter, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar,
					VectorInd
				);
			}
			else
# endif
			{	forward_load_op( op, d, i_var, arg, J, Taylor);
			}
			break;
			// -------------------------------------------------

			case LdvOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
			{	non_const_arg = Rec->forward_non_const_arg();
				forward_load_v_op_0(
					i_var, 
					non_const_arg, 
					num_par, 
					parameter, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar,
					VectorInd
				);
			}
			else
# endif
			{	forward_load_op( op, d, i_var, arg, J, Taylor);
			}
			break;
			// -------------------------------------------------

			case LogOp:
			forward_log_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case MulvvOp:
			forward_mulvv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_mulpv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case ParOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 ) forward_par_op_0(
				i_var, arg, num_par, parameter, J, Taylor
			);
			else
# endif
			{	Taylor[ i_var * J + d] = Base(0); 
			}
			break;
			// -------------------------------------------------

			case PowvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_powvp_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PowpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_powpv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PowvvOp:
			forward_powvv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PripOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( print && ( d == 0 ) ) forward_prip_0(
				arg, num_text, text, num_par, parameter
			);
# endif
			break;
			// -------------------------------------------------

			case PrivOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( print && ( d == 0 ) ) forward_priv_0(
				i_var, arg, num_text, text, J, Taylor
			);
# endif
			break;
			// -------------------------------------------------

			case SinOp:
			// variables: cos(x), sin(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_sin_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SinhOp:
			// variables: cosh(x), sinh(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_sinh_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SqrtOp:
			forward_sqrt_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case StppOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
			{	forward_store_pp_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar,
					VectorInd
				);
			}
# endif
			break;
			// -------------------------------------------------

			case StpvOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
			{	forward_store_pv_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar,
					VectorInd
				);
			}
# endif
			break;
			// -------------------------------------------------

			case StvpOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
			{	forward_store_vp_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar,
					VectorInd
				);
			}
# endif
			break;
			// -------------------------------------------------

			case StvvOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
			{	forward_store_vv_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar,
					VectorInd
				);
			}
# endif
			break;
			// -------------------------------------------------

			case SubvvOp:
			forward_subvv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_subpv_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_subvp_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
# if CPPAD_FORWARD_SWEEP_TRACE
		size_t       i_tmp  = i_var;
		Base*        Z_tmp  = Taylor + J * i_var;
		printOp(
			std::cout, 
			Rec,
			i_tmp,
			op, 
			arg,
			d + 1, 
			Z_tmp, 
			0, 
			(Base *) CPPAD_NULL
		);
	}
	std::cout << std::endl;
# else
	}
# endif
	CPPAD_ASSERT_UNKNOWN( i_var + 1 == Rec->num_rec_var() );

	if( VectorInd != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorInd);
	if( VectorVar != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorVar);

	return compareCount;
}

# undef CPPAD_FORWARD_SWEEP_TRACE

CPPAD_END_NAMESPACE
# endif
