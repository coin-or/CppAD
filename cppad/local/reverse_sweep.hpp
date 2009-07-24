/* $Id$ */
# ifndef CPPAD_REVERSE_SWEEP_INCLUDED
# define CPPAD_REVERSE_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ReverseSweep$$ $comment CppAD Developer Documentation$$
$spell
	Var
	numvar
	Num
	Len
	Ind
	const
	Taylor
	CppAD
	zs
	op
	Ind
$$

$section Reverse Mode Computation of Derivatives of Taylor Coefficients$$
$mindex ReverseSweep derivative Taylor coefficient$$

$head Syntax$$
$syntax%void ReverseSweep(
	size_t %d%,
	size_t %numvar%,
	player<%Base%> *%Rec%,
	size_t %J%,
	const Base *%Taylor%,
	size_t %K%,
	Base *%Partial%
)%$$



$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^m \rightarrow B^n
\] $$

$head G$$
Given the vector $latex v \in B^n$$ we define the function 
$latex G : B^{m \times d} \rightarrow B$$ by
$latex \[
	G(u) = \frac{1}{d !} 
		\sum_{i=1}^n v_i \left[
		\frac{ \partial^p}{ \partial t^d}
		F_i [ u^{(0)} + u^{(1)} t + \cdots + u^{(d)} t^d ]
	\right]_{t=0}
\] $$
Note that the scale factor of $latex 1 / d !$$ converts
$th d$$ order derivatives to $th d$$ order Taylor coefficients.
The routine $code ReverseSweep$$ computes all the first order partial
derivatives of $latex G$$ with respect to each of the Taylor coefficients
for the independent variables 
$latex u^{(j)}$$ for $latex j = 0 , \ldots , d$$.


$head numvar$$
is the number of rows in the matrices $italic Taylor$$ and $italic Partial$$.
It must also be equal to $syntax%%Rec%->num_rec_var()%$$.

$head J$$
Is the number of columns in the coefficient matrix $italic Taylor$$.
This must be greater than or equal $latex d+1$$.

$head K$$
Is the number of columns in the coefficient matrix $italic Partial$$.
This must be greater than or equal $latex d+1$$.

$head Taylor$$
For $latex i = 1 , \ldots , numvar - 1$$ and $latex j <= d$$
$syntax%%Taylor%[%i% * J + j]%$$ contains the 
$th j$$ order Taylor coefficients
for the variable corresponding to index $italic i$$.

$head On Input$$

$subhead Dependent Variables$$
The vector $italic v$$ is stored in the 
$th d$$ order components of the last $italic n$$ 
variables in $italic Partial$$; i.e.,
for $latex i = numvar-n , \ldots , numvar-1$$
and for $latex j < d$$,
$syntax%
	%Partial%[%i% * %K% + %j%] == 0
	%Partial%[%i% * %K% + %d%] == %v%[%numvar% - %i%]
%$$

$subhead Other Variables$$
The other variable records come before and have initial partials of zero; 
i.e., for $latex i = 1, \ldots , numvar-n-1$$ and 
$latex j \leq d$$,
$syntax%
	%Partial%[%i% * %K% + %j%] ==  0
%$$

$head On Output$$

$subhead Independent Variables$$
For $latex i = 1, \ldots , m$$, and for $latex j \leq d$$,
$syntax%%Partial%[%i% * %K% + %j%]%$$ 
is the partial of $latex G$$ with respect to $latex u_i^{(j)}$$.

$subhead Other Variables$$
For $latex i = m+1, \ldots , numvar-1$$ and $latex j \leq d$$,
$syntax%%Partial%[%i% * %K% + %j%]%$$ 
is temporary work space and its
output value is not specified.


$end
------------------------------------------------------------------------------
*/

# define CPPAD_REVERSE_SWEEP_TRACE 0

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
void ReverseSweep(
	size_t                d,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                J,
	const Base           *Taylor,
	size_t                K,
	Base                 *Partial
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        n_res;
	size_t        n_arg;

	const size_t   *arg = 0;
	const Base       *Z = 0;

	Base            *pZ = 0;

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );
	CPPAD_ASSERT_UNKNOWN( numvar > 0 );

	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = Rec->num_rec_par();

	// pointer to the beginning of the parameter vector
	const Base* parameter = 0;
	if( num_par > 0 )
		parameter = Rec->GetPar(0);

	// Initialize
	Rec->start_reverse();
	i_op   = 2;
# if CPPAD_REVERSE_SWEEP_TRACE
	std::cout << std::endl;
# endif
	while(i_op > 1)
	{	// next op
		Rec->next_reverse(op, arg, i_op, i_var);

		// corresponding number of varables and indices
		n_res  = NumRes(op);
		n_arg  = NumArg(op);

		// value of Z and its partials for this op
		Z   = Taylor + i_var * J;
		pZ  = Partial + i_var * K;

		// rest of informaiton depends on the case
# if CPPAD_REVERSE_SWEEP_TRACE
		size_t       i_tmp  = i_var;
		const Base*  Z_tmp  = Z;
		const Base*  pZ_tmp = pZ;

		if( op == PowvpOp || op == PowpvOp || op == PowvvOp )
		{	i_tmp  += 2;
			Z_tmp  += 2 * J;
			pZ_tmp += 2 * K;
		}
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

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			reverse_addvp_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AcosOp:
                        // results: acos(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );
			reverse_acos_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AsinOp:
                        // results: sin(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );
			reverse_asin_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case AtanOp:
                        // results: atan(x),  1 + x * x 
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );
			reverse_atan_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
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
			CPPAD_ASSERT_UNKNOWN( n_res == 0 );
			CPPAD_ASSERT_UNKNOWN( n_arg == 4 );
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );
			reverse_cos_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case CoshOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );
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
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
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

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			reverse_mulvp_op(
				d, i_var, arg, parameter, J, Taylor, K, Partial
			);
			break;
			// --------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
			break;
			// --------------------------------------------------

			case ParOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
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
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			break;
			// --------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			break;

			// -------------------------------------------------

			case SinOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );
			reverse_sin_op(
				d, i_var, arg[0], J, Taylor, K, Partial
			);
			break;
			// -------------------------------------------------

			case SinhOp:
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );
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
	CPPAD_ASSERT_UNKNOWN( i_op == 1 );
	CPPAD_ASSERT_UNKNOWN( Rec->GetOp(i_op-1) == NonOp );
	CPPAD_ASSERT_UNKNOWN( i_var == NumRes(NonOp)  );
}

} // END CppAD namespace

# undef CPPAD_REVERSE_SWEEP_TRACE

# endif
