/* $Id$ */
# ifndef CPPAD_FORWARD_SWEEP_INCLUDED
# define CPPAD_FORWARD_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin forward_sweep$$ $comment CppAD Developer Documentation$$
$spell
	Var
	numvar
	bool
	Prip
	Priv
	Inv
	Num
	Len
	const
	Taylor
	CppAD
	zs
	op
	Ind
$$

$section Forward Computation of Taylor Coefficients$$
$index forward_sweep$$
$index mode, forward$$
$index forward, mode$$
$index derivative, forward$$
$index Taylor coefficient, forward$$


$head Syntax$$
$syntax%size_t forward_sweep(
	bool %print%,
	size_t %d%,
	size_t %numvar%,
	player<%Base%> *%Rec%,
	size_t %J%,
	Base *%Taylor%,
)%$$


$head Return Value$$
The return value is equal to the number of
$syntax%AD<%Base%>%$$ comparison operations have a different result
from when the information in $italic Rec$$ was recorded.


$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^n \rightarrow B^m
\] $$

$head print$$
If $italic print$$ is false,
suppress the output that is otherwise generated 
by the PripOp and PrivOp instructions.


$head d$$
Given the $th d-1$$ order Taylor coefficients matrix for all the variables,
and the $th d$$ order Taylor coefficients for all the independent variables,
$code forward_sweep$$ computes the $th d$$ order Taylor coefficients 
for all the other variables.


$head numvar$$
is the number of rows in the matrix $italic Taylor$$.
It must also be equal to $syntax%%Rec%->num_rec_var()%$$.


$head J$$
Is the number of columns in the coefficient matrix $italic Taylor$$.
This must be greater than or equal $latex d+1$$.


$head On Input$$

$subhead Independent Variables and Operators$$
The independent variable records come first.
For $latex i = 1, \ldots , n$$ and $latex j = 0 , \ldots , d$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%0% * %J% + %j%]%$$      $cnext 
	the variable with index zero is not used
$rnext
	$syntax%%Rec%->GetOp(0)%$$                $cnext 
	the operator with index zero must be a $code NonOp$$
$rnext
	$syntax%%Taylor%[%i% * %J% + %j%]%$$      $cnext 
	$th j$$ order coefficient for variable with index $italic i$$   
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	the operator with index $italic i$$ must be a $code InvOp$$
$tend

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = n+1, \ldots , numvar-1$$,
$latex j = 0 , \ldots , d-1$$,
and $latex k = n+1, \ldots ,$$ $syntax%%Rec%->num_rec_op() - 1%$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%i% * %J% + %j%]%$$      $cnext 
	$th j$$ coefficient for variable with index $italic i$$     
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	any operator except for $code InvOp$$ 
$tend

$head On Output$$

$subhead Rec$$
None of the values stored in $italic Rec$$ are modified.

$subhead Independent Variables$$
For $latex i = 1, \ldots , n$$ and $latex j = 0 , \ldots , J-1$$,
$syntax%%Taylor%[%i% * %J% + %j%]%$$ is not modified.


$subhead Other Variables$$
For $latex i = n+1, \ldots , numvar-1$$ and $latex j < d$$,
$syntax%%Taylor%[%i% * %J% + %j%]%$$ is not modified.
For $latex i = n+1, \ldots , numvar-1$$, 
$syntax%%Taylor%[%i% * %J% + %d%]%$$ is set equal to the
$th d$$ order Taylor coefficient for the variable with index $italic i$$.

$contents%
        cppad/local/forward0sweep.hpp
%$$

$end
------------------------------------------------------------------------------
*/
# include <cppad/local/forward0sweep.hpp>

# define CPPAD_FORWARD_SWEEP_TRACE 0

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
size_t forward_sweep(
	bool                  print,
	size_t                d,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                J,
	Base                 *Taylor
)
{
	size_t     numop_m1;
	OpCode           op;
	size_t         i_op;
	size_t        i_var;

# if CPPAD_USE_FORWARD0SWEEP
	CPPAD_ASSERT_UNKNOWN( d > 0 );
# else
	size_t*         non_const_arg;
# endif
	const size_t   *arg = 0;
	const size_t *arg_0 = 0;
	const Base       *P = 0;

	Base             *Z = 0;

	size_t            i;
	size_t        n_res = 0;
	size_t        n_arg = 0;


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

	// set the number of operators
	numop_m1 = Rec->num_rec_op() - 1;


	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = Rec->num_rec_par();

	// length of the text vector (used by CppAD assert macros)
	const size_t num_text = Rec->num_rec_text();

	// pointer to the beginning of the parameter vector
	const Base* parameter = 0;
	if( num_par > 0 )
		parameter = Rec->GetPar(0);

	// pointer to the beginning of the text vector
	const char* text = 0;
	if( num_text > 0 )
		text = Rec->GetTxt(0);
	

	// skip the NonOp at the beginning of the recording
	Rec->start_forward(op, arg, i_op, i_var);
	arg_0 = arg;
# if CPPAD_FORWARD_SWEEP_TRACE
	std::cout << std::endl;
# endif
	while(i_op < numop_m1)
	{
		// this op
		Rec->next_forward(op, arg, i_op, i_var);

		// number of variables
		n_res  = NumRes(op);

		// index field values for this op
		n_arg  = NumArg(op);

		// value of z for this op
		Z      = Taylor + i_var * J;

		// rest of information depends on the case

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

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_addvp_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AcosOp:
			// variables: acos(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_acos_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AsinOp:
			// results: asin(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_asin_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AtanOp:
			// results: atan(x),  1 + x * x 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_atan_op(d, i_var, arg[0], J, Taylor);
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
			// variables: cos(x), sin(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_cos_op(d, i_var, arg[0], J, Taylor);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// variables: cosh(x), sinh(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_cosh_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case DisOp:
# if ! CPPAD_USE_FORWARD0SWEEP
			if( d == 0 )
				forward_dis_op_0(i_var, arg, J, Taylor);
			else
# endif
			{	Z[d] = Base(0);
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

			case ExpOp:
			forward_exp_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
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

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_mulvp_op(d, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
			break;
			// -------------------------------------------------

			case ParOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );

			P = Rec->GetPar( arg[0] );
			if( d == 0 )
				Z[d] = *P;
			else	Z[d] = Base(0); 
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
				arg, num_text, text, num_par, paraemter
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
			// variables: sin(x), cos(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_sin_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SinhOp:
			// variables: sinh(x), cosh(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
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
		Base*        Z_tmp  = Z;
		if( op == PowvpOp || op == PowpvOp || op == PowvvOp )
		{	i_tmp  += 2;
			Z_tmp  += 2 * J;
		}
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
	CPPAD_ASSERT_UNKNOWN( (i_var + n_res) == Rec->num_rec_var() );
	if( VectorInd != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorInd);
	if( VectorVar != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorVar);

	return compareCount;
}

} // END CppAD namespace

# undef CPPAD_FORWARD_SWEEP_TRACE

# endif
