/* $Id$ */
# ifndef CPPAD_REV_JAC_SWEEP_INCLUDED
# define CPPAD_REV_JAC_SWEEP_INCLUDED
CPPAD_BEGIN_NAMESPACE

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*!
\file rev_jac_sweep.hpp
Compute Reverse mode Jacobian sparsity patterns.
*/

/*!
\def CPPAD_REV_JAC_SWEEP_TRACE
This value is either zero or one. 
Zero is the normal operational value.
If it is one, a trace of every rev_jac_sweep computation is printed.
*/
# define CPPAD_REV_JAC_SWEEP_TRACE 0

/*!
Given the sparsity pattern for the dependent variables,
RevJacSweep computes the sparsity pattern for all the independent variables.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param n
is the number of independent variables on the tape.

\param npv
Is the number of elements of type \a Pack
(per variable) in the sparsity pattern \a RevJac.

\param numvar
is the total number of variables on the tape; i.e.,
\a Rec->num_rec_var().
This is also the number of rows in the entire sparsity pattern \a RevJac.

\param Rec
The information stored in \a Rec
is a recording of the operations corresponding to a function
\f[
	F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables
and \f$ m \f$ is the number of dependent variables.
The object \a Rec is effectly constant.
It is not declared const because while playing back the tape
the object \a Rec holds information about the currentl location
with in the tape and this changes during playback.

\param RevJac
\b Input: For i = \a numvar - m , ... , \a numvar - 1, 
the sparsity pattern for the dependent variable with index 
(i - \a numvar + m)
is given by \a RevJac[ i * \a npv + k ] for k = 0 , ... , \a npv - 1.
\n
\n
Input: For i = 0 , ... , \a numvar - m - 1,
the sparsity pattern for the variable with index i on the tape
is zero, i.e.,
\a RevJac[ i * \a npv + k ] == 0 (all false) for k = 0 , ... , \a npv - 1.
\n
\n
\b Output: For j = 1 , ... , \a n,
the sparsity pattern for the dependent variable with index (j-1) 
is given by \a RevJac[ j * \a npv + k ] for k = 0 , ... , \a npv - 1.
*/

template <class Base, class Pack>
void RevJacSweep(
	size_t                n,
	size_t                npv,
	size_t                numvar,
	player<Base>         *Rec,
	Pack                 *RevJac
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;

	const size_t   *arg = 0;
	Pack             *X = 0;
	Pack             *Y = 0;
	const Pack       *Z = 0;

	size_t            i, j, k;

	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = Rec->num_rec_par();

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );
	CPPAD_ASSERT_UNKNOWN( numvar > 0 );

	// vecad_pattern contains a sparsity pattern for each VecAD object.
	// vecad maps a VecAD index (which corresponds to the beginning of the
	// VecAD object) to the vecad_pattern index for the VecAD object.
	size_t num_vecad_ind   = Rec->num_rec_vecad_ind();
	size_t num_vecad_vec   = Rec->num_rec_vecad_vec();
	Pack*  vecad_pattern   = CPPAD_NULL;
	size_t* vecad          = CPPAD_NULL;
	if( num_vecad_vec > 0 )
	{	size_t length;
		vecad_pattern = CPPAD_TRACK_NEW_VEC(
			num_vecad_vec * npv, vecad_pattern
		);
		vecad         = CPPAD_TRACK_NEW_VEC(num_vecad_ind, vecad);
		j             = 0;
		for(i = 0; i < num_vecad_vec; i++)
		{	for(k = 0; k < npv; k++)
				vecad_pattern[ i * npv + k ] = Pack(0);
			// length of this VecAD
			length   = Rec->GetVecInd(j);
			// set to proper index for this VecAD
			vecad[j] = i; 
			for(k = 1; k <= length; k++)
				vecad[j+k] = num_vecad_vec; // invalid index
			// start of next VecAD
			j       += length + 1;
		}
		CPPAD_ASSERT_UNKNOWN( j == Rec->num_rec_vecad_ind() );
	}

	// Initialize
	Rec->start_reverse();
	i_op = 2;
	while(i_op > 1)
	{
		// next op
		Rec->next_reverse(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );

		// sparsity for z corresponding to this op
		Z      = RevJac + i_var * npv;

# if CPPAD_REV_JAC_SWEEP_TRACE
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			arg,
			0, 
			(Pack *) CPPAD_NULL,
			npv, 
			Z 
		);
# endif

		// rest of information depends on the case
		switch( op )
		{
			case AbsOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_binary_op(
				i_var, arg, npv, RevJac
			);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[1], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case AcosOp:
			// acos(x) and sqrt(1 - x * x) are computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case AsinOp:
			// asin(x) and sqrt(1 - x * x) are computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case AtanOp:
			// atan(x) and 1 + x * x must be computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case CExpOp:
			reverse_sparse_jacobian_cond_op(
				i_var, arg, num_par, npv, RevJac
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			CPPAD_ASSERT_NARG_NRES(op, 4, 0);
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			// cosine and sine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// hyperbolic cosine and sine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);

			break;
			// -------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_binary_op(
				i_var, arg, npv, RevJac
			);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[1], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			break;
			// -------------------------------------------------

			case LdpOp:
			reverse_sparse_load_op(
				op,
				i_var,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				npv,
				RevJac,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case LdvOp:
			reverse_sparse_load_op(
				op,
				i_var,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				npv,
				RevJac,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_binary_op(
				i_var, arg, npv, RevJac
			);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[1], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);

			break;
			// -------------------------------------------------

			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);

			break;
			// -------------------------------------------------

			case PowvpOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			reverse_sparse_jacobian_unary_op(
				i_var + 2, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case PowpvOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			reverse_sparse_jacobian_unary_op(
				i_var + 2, arg[1], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case PowvvOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			reverse_sparse_jacobian_binary_op(
				i_var + 2, arg, npv, RevJac
			);
			break;
			// -------------------------------------------------

			case PripOp:
 			CPPAD_ASSERT_NARG_NRES(op, 2, 0);

			break;
			// -------------------------------------------------

			case PrivOp:
 			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			break;
			// -------------------------------------------------

			case SinOp:
			// sine and cosine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case SinhOp:
			// hyperbolic sine and cosine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case StppOp:
			// sparsity cannot proagate through a parameter
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			break;
			// -------------------------------------------------

			case StpvOp:
			reverse_sparse_store_op(
				op,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				numvar,
				npv,
				RevJac,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case StvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			break;
			// -------------------------------------------------

			case StvvOp:
			reverse_sparse_store_op(
				op,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				numvar,
				npv,
				RevJac,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case SubvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = RevJac + arg[0] * npv;
			Y = RevJac + arg[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[1], npv, RevJac
			);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			reverse_sparse_jacobian_unary_op(
				i_var, arg[0], npv, RevJac
			);
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
	CPPAD_ASSERT_UNKNOWN( i_op == 1 );
	CPPAD_ASSERT_UNKNOWN( Rec->GetOp(i_op-1) == NonOp );
	CPPAD_ASSERT_UNKNOWN( i_var == NumRes(NonOp)  );

	if( vecad_pattern != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC( vecad_pattern );
	if( vecad != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC( vecad );

	return;
}

# undef CPPAD_REV_JAC_SWEEP_TRACE

CPPAD_END_NAMESPACE
# endif
