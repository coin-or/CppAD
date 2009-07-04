/* $Id$ */
# ifndef CPPAD_REV_HES_SWEEP_INCLUDED
# define CPPAD_REV_HES_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin RevHesSweep$$ $comment CppAD Developer Documentation$$
$spell
	Hessian
	RevHesSweep
	npv
	numvar
	Num
	Var
	Op
	Taylor
	Inv
$$

$section Reverse Computation of Hessian Sparsity Pattern$$
$index RevHesSweep$$
$index sparsity, reverse Hessian$$
$index reverse, Hessian sparsity$$
$index pattern, reverse Hessian$$
$index bit pattern, Hessian$$

$head Syntax$$
$syntax%void RevHesSweep(
	size_t %npv%,
	size_t %numvar%,
	player<%Base%> *%Rec%,
	size_t %TaylorColDim%,
	const %Base% *%Taylor%,
	const %Pack% *%ForJac%,
	const %Pack% *%RevJac%,
	%Pack% *%RevHes%
)%$$


$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^n \rightarrow B^m
\] $$

$head Description$$
Given the forward Jacobian sparsity pattern for all the variables
$code RevHesSweep$$ computes the Hessian sparsity pattern
for the dependent variables with respect to the independent variables.

$head numvar$$
is the number of rows in the entire sparsity patterns
$italic ForJac$$ and $italic RevHes$$.
It must also be equal to $syntax%%Rec%->num_rec_var()%$$.

$head npv$$
Is the number of elements of type $italic Pack$$
(per variable) in the sparsity patterns 
$itlaic ForJac$$ and $italic RevJac$$.
(The sparsity pattern $italic RevJac$$ only has one element per variables
and all of the bits in that element are the same.)
 
$head TaylorColDim$$
Is the number of columns currently stored in the matrix $italic Taylor$$.

$head Taylor$$
For $latex i = 1 , \ldots , numvar$$,
$syntax%%Taylor%[%i% * %TaylorColDim%]%$$
is the value of the variable with index $italic i$$.


$head ForJac$$
For $latex i = 0, \ldots , numvar-1$$ (all the variables),
and $latex j = 0, \ldots , npv-1$$,
$syntax%%ForJac%[%i% * %npv% + %j%]%$$ 
is the $th j$$ subset of the 
the forward mode Jacobian sparsity pattern for 
the variable with index $italic i$$.   

$head RevJac$$
For $latex i = 0, \ldots , numvar-1$$ (all the variables),
$syntax%%RevJac%[%i%]%$$ 
is all true (ones complement of 0) if the function we are computing
the Hessian for has non-zero Jacobian with respect to variable
with index $italic i$$.


$head RevHes$$
For $latex i = 1, \ldots , numvar-1$$ 
and $latex j = 0 , \ldots , npv-1$$,
$syntax%%RevHes%[%i% * %npv% + %j%]%$$ is the 
Hessian sparsity pattern with for the partial of the 
function we are computing the Hessian for 
(usually one variable) with respect to
the $th i$$ variable and with respect to all the variables
corresponding to the columns of $italic ForJac$$.

$end
------------------------------------------------------------------------------
*/
# define CPPAD_REV_HES_SWEEP_TRACE 0


// BEGIN CppAD namespace
namespace CppAD {

template <class Base, class Pack>
void RevHesSweep(
	size_t                npv,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                TaylorColDim,
	const Base           *Taylor,
	const Pack           *ForJac,
	const Pack           *RevJac,
	Pack                 *RevHes
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        n_res;
	size_t        n_arg;

	const size_t   *arg = 0;

	const Pack       *Xf = 0;
	Pack             *Xh = 0;

	const Pack       *Yf = 0;
	Pack             *Yh = 0;

	const Pack       *Zf = 0;
	const Pack       *Zr = 0;
	const Pack       *Zh = 0;


	// used by CExp operator 
	const Base *left = 0, *right = 0;
	Pack  *trueCaseh = 0;
	Pack  *falseCaseh = 0;
	Pack  zero(0);

	size_t             j;

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );
	CPPAD_ASSERT_UNKNOWN( numvar > 0 );

	// Initialize
	Rec->start_reverse();
	i_op = 2;
	while(i_op > 1)
	{
		// next op
		Rec->next_reverse(op, arg, i_op, i_var);

		// corresponding number of varable and indices
		n_res  = NumRes(op);
		n_arg  = NumArg(op);

		// sparsity for z corresponding to this op
		Zf     = ForJac + i_var * npv;
		Zr     = RevJac + i_var;
		Zh     = RevHes + i_var * npv;

# if CPPAD_REV_HES_SWEEP_TRACE
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			arg,
			npv, 
			Zf, 
			npv, 
			Zh
		);
# endif

		// rest of information depends on the case
		switch( op )
		{
			case AbsOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Xh  = RevHes + arg[0] * npv;
			Yh  = RevHes + arg[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
				Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[1], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case AcosOp:
			// acos(x) and sqrt(1 - x * x) are computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case AsinOp:
			// asin(x) and sqrt(1 - x * x) are computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case AtanOp:
			// atan(x) and 1 + x * x must be computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------
			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 6);
			CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

			if( arg[1] & 1 )
				left = Taylor + arg[2] * TaylorColDim;
			else	left = Rec->GetPar(arg[2]);
			if( arg[1] & 2 )
				right = Taylor + arg[3] * TaylorColDim;
			else	right = Rec->GetPar(arg[3]);
			if( arg[1] & 4 )
			{	trueCaseh = RevHes + arg[4] * npv;
				for(j = 0; j < npv; j++)
				{	trueCaseh[j] |= CondExpTemplate(
						CompareOp( arg[0] ),
						*left,
						*right,
						Zh[j],
						zero
					);
				}
			}
			if( arg[1] & 8 )
			{	falseCaseh = RevHes + arg[5] * npv;
				for(j = 0; j < npv; j++)
				{	falseCaseh[j] |= CondExpTemplate(
						CompareOp( arg[0] ),
						*left,
						*right,
						zero,
						Zh[j]
					);
				}
			}
			break;
			// ---------------------------------------------------

			case ComOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0 );
			CPPAD_ASSERT_UNKNOWN( n_arg == 4 );
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			// cosine and sine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// hyperbolic cosine and sine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );

			break;
			// -------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Xf = ForJac + arg[0] * npv;
			Xh = RevHes + arg[0] * npv;
			Yf = ForJac + arg[1] * npv;
			Yh = RevHes + arg[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Yf[j]);
				Yh[j] |= Zh[j] | (*Zr & (Xf[j] | Yf[j]));
			}
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[1], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
			// Z is already defined
			break;
			// -------------------------------------------------

			case LdpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );

			// arg[2] is variable corresponding to this load
			if( arg[2] > 0 )
			{	Xh = RevHes + arg[2] * npv;
				for(j = 0; j < npv; j++)
					Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );

			// arg[2] is variable corresponding to this load
			if( arg[2] > 0 )
			{	Xh = RevHes + arg[2] * npv;
				for(j = 0; j < npv; j++)
					Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Xf = ForJac + arg[0] * npv;
			Xh = RevHes + arg[0] * npv;
			Yf = ForJac + arg[1] * npv;
			Yh = RevHes + arg[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Yf[j]);
				Yh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[1], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
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

			break;
			// -------------------------------------------------

			case PowpvOp:
                        // Pow operator is a special case where final result
                        // comes at the end of the three variables
			CPPAD_ASSERT_UNKNOWN( n_res == 3 );
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var+2, arg[1], *(Zr+2), npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case PowvpOp:
                        // Pow operator is a special case where final result
                        // comes at the end of the three variables
			CPPAD_ASSERT_UNKNOWN( n_res == 3 );
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var+2, arg[0], *(Zr+2), npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case PowvvOp:
                        // Pow operator is a special case where final result
                        // comes at the end of the three variables
			Zr += 2;
			Zh += 2 * npv;
			CPPAD_ASSERT_UNKNOWN( n_res == 3 );
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Xf = ForJac + arg[0] * npv;
			Xh = RevHes + arg[0] * npv;
			Yf = ForJac + arg[1] * npv;
			Yh = RevHes + arg[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & (Xf[j] | Yf[j])); 
				Yh[j] |= Zh[j] | (*Zr & (Xf[j] | Yf[j])); 
			}
			break;
			// -------------------------------------------------

			case PripOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);

			break;
			// -------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);

			break;
			// -------------------------------------------------

			case SinOp:
			// sine and cosine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case SinhOp:
			// sine and cosine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			reverse_sparse_hessian_nonlinear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case StppOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			break;
			// -------------------------------------------------

			case StpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			break;
			// -------------------------------------------------

			case StvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			break;
			// -------------------------------------------------

			case StvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			break;
			// -------------------------------------------------

			case SubvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Xh  = RevHes + arg[0] * npv;
			Yh  = RevHes + arg[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
				Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[1], *Zr, npv, ForJac, RevHes
			);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			reverse_sparse_hessian_linear_unary_op(
				i_var, arg[0], *Zr, npv, ForJac, RevHes
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

	return;
}

} // END CppAD namespace

# undef CPPAD_REV_HES_SWEEP_TRACE

# endif
