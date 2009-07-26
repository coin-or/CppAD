/* $Id$ */
# ifndef CPPAD_FORWARD0SWEEP_INCLUDED
# define CPPAD_FORWARD0SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin forward0sweep$$ $comment CppAD Developer Documentation$$
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

$section Zero Order Forward Computation of Taylor Coefficients$$
$index forward0sweep$$
$index mode, forward$$
$index forward, mode$$
$index derivative, forward$$
$index Taylor coefficient, forward$$


$head Syntax$$
$syntax%size_t forward0sweep(
	bool %print%,
	size_t %n%,
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

$head n$$
is the number of independent variables on the tape.

$head numvar$$
is the number of rows in the matrix $italic Taylor$$.
It must also be equal to $syntax%%Rec%->num_rec_var()%$$.


$head J$$
Is the number of columns in the coefficient matrix $italic Taylor$$.
This must be greater than or equal $latex 1$$.


$head On Input$$

$subhead Independent Variables and Operators$$
The independent variable records come first.
For $latex i = 1, \ldots , n$$ 
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%0% * %J%]%$$      $cnext 
	this index zero variable is not used
$rnext
	$syntax%%Rec%->GetOp(0)%$$                $cnext 
	the operator with index zero must be a $code NonOp$$
$rnext
	$syntax%%Taylor%[%i% * %J%]%$$      $cnext 
	zero order Taylor coefficient for independent variable 
	with index $italic i$$   
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	the operator with index $italic i$$ must be a $code InvOp$$
$tend

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = n+1, \ldots , numvar-1$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%i% * %J%]%$$      $cnext 
	value of the variable with index $italic i$$     
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
For $latex i = n+1, \ldots , numvar-1$$, 
$syntax%%Taylor%[%i% * %J%]%$$ is set equal to the
zero order Taylor coefficient for the variable with index $italic i$$.


$end
------------------------------------------------------------------------------
*/
# define CPPAD_FORWARD0SWEEP_TRACE 0

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
size_t forward0sweep(
	bool                  print,
	size_t                n,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                J,
	Base                 *Taylor
)
{
	// op code for current instruction
	OpCode           op;

	// index for current instruction
	size_t         i_op;

	// next variables 
	size_t        i_var;

	// constant and non-constant version of the operation argument indices
	size_t*         non_const_arg;
	const size_t   *arg = 0;


	// initialize the comparision operator (ComOp) counter
	size_t compareCount = 0;

	// This is an order zero calculation, initialize vector indices
	size_t *VectorInd = CPPAD_NULL;  // address for each element
	bool   *VectorVar = CPPAD_NULL;  // is element a variable
	size_t  i = Rec->num_rec_vecad_ind();
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
	const size_t numop_m1 = Rec->num_rec_op() - 1;

	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = Rec->num_rec_par();

        // length of the text vector (used by CppAD assert macros)
        const size_t num_text = Rec->num_rec_text();

	// pointer to the beginning of the parameter vector
	const Base* parameter = CPPAD_NULL;
	if( num_par > 0 )
		parameter = Rec->GetPar(0);

	// pointer to the beginning of the text vector
	const char* text = CPPAD_NULL;
	if( num_text > 0 )
		text = Rec->GetTxt(0);

	// skip the NonOp at the beginning of the recording
	Rec->start_forward(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == NonOp );
	while(i_op < numop_m1)
	{
		// this op
		Rec->next_forward(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );  
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );  

		// action to take depends on the case
		switch( op )
		{
			case AbsOp:
			forward_abs_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AddvvOp:
			forward_addvv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_addpv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_addvp_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AcosOp:
			// results: acos(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_acos_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AsinOp:
			// results: asin(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_asin_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AtanOp:
			// results: atan(x),  1 + x * x 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_atan_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case CExpOp:
			// Use the general case with d == 0 
			// (could create an optimzied verison for this case)
			forward_cond_op_0(
				i_var, arg, num_par, parameter, J, Taylor
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			forward_comp_op_0(
			compareCount, arg, num_par, parameter, J, Taylor
			);
			break;
			// ---------------------------------------------------

			case CosOp:
			// results: cos(x), sin(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_cos_op_0(i_var, arg[0], J, Taylor);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// variables: cosh(x), sinh(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_cosh_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case DisOp:
			forward_dis_op_0(i_var, arg, J, Taylor);
			break;
			// -------------------------------------------------

			case DivvvOp:
			forward_divvv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_divpv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_divvp_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case ExpOp:
			forward_exp_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case InvOp:
			break;
			// -------------------------------------------------

			case LdpOp:
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
			non_const_arg = Rec->forward_non_const_arg();
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
			break;
			// -------------------------------------------------

			case LdvOp:
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
			non_const_arg = Rec->forward_non_const_arg();
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
			break;
			// -------------------------------------------------

			case LogOp:
			forward_log_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case MulvvOp:
			forward_mulvv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_mulpv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_mulvp_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
			break;
			// -------------------------------------------------

			case ParOp:
			forward_par_op_0(
				i_var, arg, num_par, parameter, J, Taylor
			);
			break;
			// -------------------------------------------------

			case PowvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_powvp_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PowpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_powpv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PowvvOp:
			forward_powvv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PripOp:
			if( print ) forward_prip_0(
				arg, num_text, text, num_par, parameter
			);
			break;
			// -------------------------------------------------

			case PrivOp:
			if( print ) forward_priv_0(
				i_var, arg, num_text, text, J, Taylor
			);
			break;
			// -------------------------------------------------

			case SinOp:
			// variables: sin(x), cos(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_sin_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SinhOp:
			// variables: sinh(x), cosh(x)
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_sinh_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SqrtOp:
			forward_sqrt_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case StppOp:
			forward_store_pp_op_0(
				i_var, 
				arg, 
				num_par, 
				J, 
				Taylor,
				Rec->num_rec_vecad_ind(),
				VectorVar,
				VectorInd
			);
			break;
			// -------------------------------------------------

			case StpvOp:
			forward_store_pv_op_0(
				i_var, 
				arg, 
				num_par, 
				J, 
				Taylor,
				Rec->num_rec_vecad_ind(),
				VectorVar,
				VectorInd
			);
			break;
			// -------------------------------------------------

			case StvpOp:
			forward_store_vp_op_0(
				i_var, 
				arg, 
				num_par, 
				J, 
				Taylor,
				Rec->num_rec_vecad_ind(),
				VectorVar,
				VectorInd
			);
			break;
			// -------------------------------------------------

			case StvvOp:
			forward_store_vv_op_0(
				i_var, 
				arg, 
				num_par, 
				J, 
				Taylor,
				Rec->num_rec_vecad_ind(),
				VectorVar,
				VectorInd
			);
			break;
			// -------------------------------------------------

			case SubvvOp:
			forward_subvv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( arg[0] < num_par );
			forward_subpv_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );
			forward_subvp_op_0(i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
# if CPPAD_FORWARD0SWEEP_TRACE
		size_t       d      = 0;
		size_t       i_tmp  = i_var;
		Base*        Z_tmp  = Taylor + i_var * J;
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
	CPPAD_ASSERT_UNKNOWN( (i_var + NumRes(op)) == Rec->num_rec_var() );
	if( VectorInd != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorInd);
	if( VectorVar != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorVar);

	return compareCount;
}

} // END CppAD namespace

# undef CPPAD_FORWARD0SWEEP_TRACE

# endif
