/* $Id$ */
# ifndef CPPAD_REVERSE_SWEEP_INCLUDED
# define CPPAD_REVERSE_SWEEP_INCLUDED

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
\file reverse_sweep.hpp
Compute derivatives of arbitrary order Taylor coefficients.
*/

/*!
\def CPPAD_REVERSE_SWEEP_TRACE
This value is either zero or one. 
Zero is the normal operational value.
If it is one, a trace of every reverse_sweep computation is printed.
*/
# define CPPAD_REVERSE_SWEEP_TRACE 0

/*!
Compute derivative of arbitrary order forward mode Taylor coefficients.

\tparam Base
base type for the operator; i.e., this operation sequence was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param d
is the highest order Taylor coefficients that 
we are computing the derivative of.

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
We define the function 
\f$ G : {\bf R}^{n \times d} \rightarrow {\bf R} \f$ by
\f[
G( u ) = \frac{1}{d !} \frac{ \partial^d }{ \partial t^d } 
\left[ 
	\sum_{i=1}^m w_i  F_i ( u^{(0)} + u^{(1)} t + \cdots + u^{(d)} t^d )
\right]_{t=0}
\f]
Note that the scale factor  1 / a d  converts 
the \a d-th partial derivative to the \a d-th order Taylor coefficient.
This routine computes the derivative of \f$ G(u) \f$
with respect to all the Taylor coefficients
\f$ u^{(k)} \f$ for \f$ k = 0 , ... , d \f$.
The vector \f$ w \in {\bf R}^m \f$, and
value of \f$ u \in {\bf R}^{n \times d} \f$
at which the derivative is computed,
are defined below.
\n
\n
The object \a Rec is effectly constant.
There is an exception to this,
while palying back the tape
the object \a Rec holds information about the current location
with in the tape and this changes during palyback. 

\param J
Is the number of columns in the coefficient matrix \a Taylor.
This must be greater than or equal \a d + 1.

\param Taylor
For i = 1 , ... , \a numvar, and for k = 0 , ... , \a d,
\a Taylor [ i * J + k ]
is the k-th order Taylor coefficient corresponding to 
variable with index i on the tape.
The value \f$ u \in {\bf R}^{n \times d} \f$,
at which the derivative is computed,
is defined by
\f$ u_j^{(k)} \f$ = \a Taylor [ j * J + k ]
for j = 1 , ... , \a n, and for k = 0 , ... , \a d.

\param K
Is the number of columns in the partial derivative matrix \a Partial.
It must be greater than or equal \a d + 1.

\param Partial
\b Input:
The last \f$ m \f$ rows of \a Partial are inputs.
The vector \f$ v \f$, used to define \f$ G(u) \f$,
is specified by these rows. 
For i = 0 , ... , m - 1, \a Partial [ ( \a numvar - m + i ) * K + d ] = v_i.
For i = 0 , ... , m - 1 and for k = 0 , ... , d - 1, 
\a Partial [ ( \a numvar - m + i ) * K + k ] = 0.
\n
\n
\b Temporary:
For i = n+1 , ... , \a numvar - 1 and for k = 0 , ... , d, 
the value of \a Partial [ i * K + k ] is used for temporary work space
and its output value is not defined. 
\n
\n
\b Output:
For j = 1 , ... , n and for k = 0 , ... , d, 
\a Partial [ j * K + k ] 
is the partial derivative of \f$ G( u ) \f$ with 
respect to \f$ u_j^{(k)} \f$.

\par Assumptions
The first operator on the tape is a BeginOp,
and the next \a n operators are InvOp operations for the 
corresponding independent variables.
*/
template <class Base>
void ReverseSweep(
	size_t                d,
	size_t                n,
	size_t                numvar,
	player<Base>*         Rec,
	size_t                J,
	const Base*           Taylor,
	size_t                K,
	Base*                 Partial
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;

	const size_t   *arg = 0;

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );
	CPPAD_ASSERT_UNKNOWN( numvar > 0 );

	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = Rec->num_rec_par();

	// pointer to the beginning of the parameter vector
	const Base* parameter = 0;
	if( num_par > 0 )
		parameter = Rec->GetPar();

	// Initialize
	Rec->start_reverse(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == EndOp );
# if CPPAD_REVERSE_SWEEP_TRACE
	std::cout << std::endl;
# endif
	while(op != BeginOp )
	{	// next op
		Rec->next_reverse(op, arg, i_op, i_var);
# ifndef NDEBUG
		if( i_op <= n )
		{	CPPAD_ASSERT_UNKNOWN((op == InvOp) | (op == BeginOp));
		}
		else	CPPAD_ASSERT_UNKNOWN((op != InvOp) & (op != BeginOp));
# endif

		// rest of informaiton depends on the case
# if CPPAD_REVERSE_SWEEP_TRACE
		size_t       i_tmp  = i_var;
		const Base*  Z_tmp  = Taylor + i_var * J;
		const Base*  pZ_tmp = Partial + i_var * K;

		printOp(
			std::cout, 
			Rec,
			i_tmp,
			op, 
			arg,
			d + 1, 
			Z_tmp, 
			d + 1, 
			pZ_tmp 
		);
# endif

		switch( op )
		{

			case AbsOp:
			reverse_abs_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AddvvOp:
			reverse_addvv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			reverse_addpv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AcosOp:
                        // results: sqrt(1 - x * x), acos(x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar );
			reverse_acos_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AsinOp:
                        // results: sqrt(1 - x * x), sin(x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar );
			reverse_asin_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AtanOp:
                        // results: 1 + x * x, atan(x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar );
			reverse_atan_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// -------------------------------------------------

			case BeginOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			break;
			// --------------------------------------------------

			case CSumOp:
			// CSumOp has a variable number of arguments and
			// next_reverse thinks it one has one argument.
			// We must inform next_reverse of this special case.
			Rec->reverse_csum(op, arg, i_op, i_var);
			reverse_csum_op(
				d, i_var, arg, K, Partial
			);
			// end of a cummulative summation
			break;
			// -------------------------------------------------

			case CExpOp:
			reverse_cond_op(
				d, 
				i_var, 
				arg, 
				num_par, 
				parameter, 
				J, 
				Taylor,
				K, 
				Partial
			);
			break;
			// --------------------------------------------------

			case ComOp:
			break;
			// --------------------------------------------------

			case CosOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar );
			reverse_cos_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case CoshOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar );
			reverse_cosh_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case DisOp:
			// Derivative of discrete operation is zero so no
			// contribution passes through this operation. 
			break;
			// --------------------------------------------------

			case DivvvOp:
			reverse_divvv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			reverse_divpv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			reverse_divvp_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case ExpOp:
			reverse_exp_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------
			case LdpOp:
			reverse_load_op(
				op, d, i_var, arg, J, Taylor, K, Partial
			);
			break;
			// -------------------------------------------------

			case LdvOp:
			reverse_load_op(
				op, d, i_var, arg, J, Taylor, K, Partial
			);
			break;
			// -------------------------------------------------

			case InvOp:
			break;
			// --------------------------------------------------

			case LogOp:
			reverse_log_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case MulvvOp:
			reverse_mulvv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			reverse_mulpv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case ParOp:
			break;
			// --------------------------------------------------

			case PowvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			reverse_powvp_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// -------------------------------------------------

			case PowpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			reverse_powpv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// -------------------------------------------------

			case PowvvOp:
			reverse_powvv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case PripOp:
			// no result so nothing to do
			break;
			// --------------------------------------------------

			case PrivOp:
			// no result so nothing to do
			break;

			// -------------------------------------------------

			case SinOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar );
			reverse_sin_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// -------------------------------------------------

			case SinhOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar );
			reverse_sinh_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case SqrtOp:
			reverse_sqrt_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case StppOp:
			break;
			// --------------------------------------------------

			case StpvOp:
			break;
			// -------------------------------------------------

			case StvpOp:
			break;
			// -------------------------------------------------

			case StvvOp:
			break;
			// --------------------------------------------------

			case SubvvOp:
			reverse_subvv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			reverse_subpv_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			reverse_subvp_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
# if CPPAD_REVERSE_SWEEP_TRACE
	std::cout << std::endl;
# endif
	// values corresponding to BeginOp
	CPPAD_ASSERT_UNKNOWN( i_op == 0 );
	CPPAD_ASSERT_UNKNOWN( i_var == 0 );
}

# undef CPPAD_REVERSE_SWEEP_TRACE

CPPAD_END_NAMESPACE
# endif
