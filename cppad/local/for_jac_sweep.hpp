/* $Id$ */
# ifndef CPPAD_FOR_JAC_SWEEP_INCLUDED
# define CPPAD_FOR_JAC_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForJacSweep$$ $comment CppAD Developer Documentation$$
$spell
	const
	Jacobian
	ForJacSweep
	npv
	numvar
	Num
	Var
	Op
	Taylor
	Inv
$$

$section Forward Computation of Sparsity Pattern$$
$index ForJacSweep$$
$index sparsity, forward Jacobian$$
$index forward, Jacobian sparsity$$
$index pattern, forward Jacobian$$
$index bit pattern, Jacobian$$

$head Syntax$$
$syntax%void ForJacSweep(
	size_t %npv%,
	size_t %numvar%,
	const player<%Base%> *%Rec%,
	size_t %TaylorColDim%,
	const %Base% *%Taylor%,
	%Pack% *%ForJac%
)%$$


$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^n \rightarrow B^m
\] $$

$head Description$$
Given the sparsity pattern for the independent variables,
$code ForJacSweep$$ computes the sparsity pattern
for all the other variables.


$head numvar$$
is the number of rows in the entire sparsity pattern $italic ForJac$$.
It must also be equal to $syntax%%Rec%->num_rec_var()%$$.

$head npv$$
Is the number of elements of type $italic Pack$$
(per variable) in the sparsity pattern $italic ForJac$$.

$head TaylorColDim$$
Is the number of columns currently stored in the matrix $italic Taylor$$.

$head Taylor$$
For $latex i = 1 , \ldots , numvar$$,
$syntax%%Taylor%[%i% * %TaylorColDim%]%$$
is the value of the variable with index $italic i$$.


$head On Input$$

$subhead Independent Variables and Operators$$
The independent variable records come first.
For $latex i = 1, \ldots , n$$ and $latex j = 0 , \ldots , npv$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%ForJac%[%0% * %npv% + %j%]%$$      $cnext 
	the variable with index zero is not used
$rnext
	$syntax%%Rec%->GetOp(0)%$$                $cnext 
	the operator with index zero must be a $code NonOp$$
$rnext
	$syntax%%ForJac%[%i% * %npv% + %j%]%$$      $cnext 
	$th j$$ subset of sparsity pattern for variable with index $italic i$$
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	the operator with index $italic i$$ must be a $code InvOp$$
$tend

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = n+1, \ldots , numvar-1$$,
$latex j = 0 , \ldots , npv-1$$,
and $latex k = n+1, \ldots ,$$ $syntax%%Rec%->num_rec_op() - 1%$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%ForJac%[%i% * %npv% + %j%]%$$      $cnext 
	$th j$$ set of sparsity pattern for variable with index $italic i$$     
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	any operator except for $code InvOp$$ 
$tend

$head On Output$$

$subhead Independent Variables$$
For $latex i = 1, \ldots , n$$ and $latex j = 0 , \ldots , npv-1$$,
$syntax%%Taylor%[%i% * %npv% + %j%]%$$ is not modified.


$subhead Other Variables$$
For $latex i = m+1, \ldots , numvar-1$$ and $latex j = 0 , \ldots , npv-1$$,
$syntax%%ForJac%[%i% * %npv% + %j%]%$$ is set equal to the
$th j$$ set of sparsity pattern for the variable with index $italic i$$.


$end
------------------------------------------------------------------------------
*/
# define CPPAD_FOR_JAC_SWEEP_TRACE 0


// BEGIN CppAD namespace
namespace CppAD {

template <class Base, class Pack>
void ForJacSweep(
	size_t                npv,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                TaylorColDim,
	const Base           *Taylor,
	Pack                 *ForJac
)
{
	size_t     numop_m1;
	OpCode           op;
	size_t         i_op;
	size_t        i_var;

	const size_t   *arg = 0;
	const size_t *arg_0 = 0;
	const Pack       *X = 0;
	const Pack       *Y = 0;

	Pack             *Z = 0;

	size_t            j;

	// used by CExp operator 
	bool use_VecAD = Rec->num_rec_vecad_ind() > 0;
	const Base  *left = 0, *right = 0;
	const Pack  *trueCase = 0, *falseCase = 0;
	Pack  *zero = CPPAD_NULL;
	zero        = CPPAD_TRACK_NEW_VEC(npv, zero);
	for(j = 0; j < npv; j++)
		zero[j] = 0;
	
	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );

	// set the number of operators
	numop_m1 = Rec->num_rec_op() - 1;

	// skip the NonOp at the beginning of the recording
        Rec->start_forward(op, arg, i_op, i_var);
        arg_0 = arg;
	while(i_op < numop_m1)
	{
		// this op
		Rec->next_forward(op, arg, i_op, i_var);

		// value of z for this op
		Z      = ForJac + i_var * npv;

		// rest of information depends on the case
		switch( op )
		{
			case AbsOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = ForJac + arg[0] * npv;
			Y = ForJac + arg[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case AcosOp:
			// acos(x) and sqrt(1 - x * x) are computed in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case AsinOp:
			// asin(x) and sqrt(1 - x * x) are computed in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case AtanOp:
			// atan(x) and 1 + x * x must be computed in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case CExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 6, 1);
			CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

			if( arg[1] & 4 )
				trueCase = ForJac + arg[4] * npv;
			else	trueCase = zero;
			if( arg[1] & 8 )
				falseCase = ForJac + arg[5] * npv;
			else	falseCase = zero;
			if( ! use_VecAD )
			{	// result valid for all independent var values
				for(j = 0; j < npv; j++)
					Z[j] = trueCase[j] | falseCase[j];
			}
			else
			{	// result only valid for current values
				if( arg[1] & 1 )
					left = Taylor + arg[2] * TaylorColDim;
				else	left = Rec->GetPar(arg[2]);
				if( arg[1] & 2 )
					right = Taylor + arg[3] * TaylorColDim;
				else	right = Rec->GetPar(arg[3]);
				for(j = 0; j < npv; j++)
				{	Z[j] = CondExpTemplate(
						CompareOp( arg[0] ),
						*left,
						*right,
						trueCase[j],
						falseCase[j]
					);
				}
			}
			break;
			break;
			// ---------------------------------------------------

			case ComOp:
			CPPAD_ASSERT_NARG_NRES(op, 4, 0);
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			// cosine and sine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// hyperbolic cosine and sine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);

			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;
			// -------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = ForJac + arg[0] * npv;
			Y = ForJac + arg[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			// Z is already defined
			break;
			// -------------------------------------------------

			case LdpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );

			// arg[2] is variable corresponding to this load
			if( arg[2] > 0 )
			{	X = ForJac + arg[2] * npv;
				for(j = 0; j < npv; j++)
					Z[j] = X[j];
			}
			else
			{	for(j = 0; j < npv; j++)
					Z[j] = 0;
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );


			// arg[2] is variable corresponding to this load
			if( arg[2] > 0 )
			{	X = ForJac + arg[2] * npv;
				for(j = 0; j < npv; j++)
					Z[j] = X[j];
			}
			else
			{	for(j = 0; j < npv; j++)
					Z[j] = 0;
			}
			break;
			// -------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = ForJac + arg[0] * npv;
			Y = ForJac + arg[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;

			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;
			// -------------------------------------------------

			case PowvpOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			forward_sparse_jacobian_unary_op(
				i_var + 2, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case PowpvOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			forward_sparse_jacobian_unary_op(
				i_var + 2, arg[1], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case PowvvOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			Z   += 2 * npv;
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = ForJac + arg[0] * npv;
			Y = ForJac + arg[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case PripOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;
			// -------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			break;
			// -------------------------------------------------

			case SinOp:
			// sine and cosine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case SinhOp:
			// hyperbolic sine and cosine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case StppOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			break;
			// -------------------------------------------------

			case StpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			break;
			// -------------------------------------------------

			case StvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			break;
			// -------------------------------------------------

			case StvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			break;
			// -------------------------------------------------

			case SubvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = ForJac + arg[0] * npv;
			Y = ForJac + arg[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], npv, ForJac
			);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], npv, ForJac
			);
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
# if CPPAD_FOR_JAC_SWEEP_TRACE
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			arg,
			npv, 
			Z, 
			0, 
			(Pack *) CPPAD_NULL
		);
	}
	std::cout << std::endl;
# else
	}
# endif
	CPPAD_ASSERT_UNKNOWN( (i_var + NumRes(op) ) == Rec->num_rec_var() );

	// free vector of zeros
	CPPAD_TRACK_DEL_VEC(zero);

	return;
}

} // END CppAD namespace

# undef CPPAD_FOR_JAC_SWEEP_TRACE

# endif
