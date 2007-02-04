# ifndef CPPAD_REV_HES_SWEEP_INCLUDED
# define CPPAD_REV_HES_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

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
	const TapeRec<%Base%> *%Rec%,
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
It must also be equal to $syntax%%Rec%->TotNumVar()%$$.

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
	const TapeRec<Base>  *Rec,
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
	size_t        i_ind;
	size_t        n_var;
	size_t        n_ind;

	const size_t   *ind;

	const Pack       *Xf;
	Pack             *Xh;

	const Pack       *Yf;
	Pack             *Yh;

	const Pack       *Zf;
	const Pack       *Zr;
	const Pack       *Zh;


	// used by CExp operator 
	const Base *left, *right;
	Pack  *trueCaseh;
	Pack  *falseCaseh;
	Pack  zero(0);

	size_t             j;

	// check numvar argument
	CppADUnknownError( Rec->TotNumVar() == numvar );
	CppADUnknownError( numvar > 0 );

	// Initialize
	i_op   = Rec->NumOp();
	i_var  = Rec->TotNumVar();
	i_ind  = Rec->NumInd();
	op     = NonOp;         // phony operator that is not there

	while(i_op > 1)
	{	--i_op;

		// next op
		op     = Rec->GetOp(i_op);

		// corresponding varable
		n_var  = NumVar(op);
		CppADUnknownError( i_var >= n_var );
		i_var -= n_var;

		// corresponding index values
		n_ind  = NumInd(op);
		CppADUnknownError( i_ind >= n_ind );
		i_ind -= n_ind;
		ind    = Rec->GetInd(n_ind, i_ind);

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
			ind,
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
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AddvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			Xh  = RevHes + ind[0] * npv;
			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
				Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AddpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AddvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AcosOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// acos(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case AsinOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// asin(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );


			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case AtanOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// atan(x) and 1 + x * x must be computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------
			case CExpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 6);
			CppADUnknownError( ind[1] != 0 );

			if( ind[1] & 1 )
				left = Taylor + ind[2] * TaylorColDim;
			else	left = Rec->GetPar(ind[2]);
			if( ind[1] & 2 )
				right = Taylor + ind[3] * TaylorColDim;
			else	right = Rec->GetPar(ind[3]);
			if( ind[1] & 4 )
			{	trueCaseh = RevHes + ind[4] * npv;
				for(j = 0; j < npv; j++)
				{	trueCaseh[j] |= CondExpTemplate(
						CompareOp( ind[0] ),
						*left,
						*right,
						Zh[j],
						zero
					);
				}
			}
			if( ind[1] & 8 )
			{	falseCaseh = RevHes + ind[5] * npv;
				for(j = 0; j < npv; j++)
				{	falseCaseh[j] |= CondExpTemplate(
						CompareOp( ind[0] ),
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
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 4 );
			CppADUnknownError( ind[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// cosine and sine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// ---------------------------------------------------

			case CoshOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// hyperbolic cosine and sine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );


			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case DisOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );

			break;
			// -------------------------------------------------

			case DivvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			Xf = ForJac + ind[0] * npv;
			Xh = RevHes + ind[0] * npv;
			Yf = ForJac + ind[1] * npv;
			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Yf[j]);
				Yh[j] |= Zh[j] | (*Zr & (Xf[j] | Yf[j]));
			}
			break;
			// -------------------------------------------------

			case DivpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Yf = ForJac + ind[1] * npv;
			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j] | (*Zr & Yf[j]);
			}
			break;
			// -------------------------------------------------

			case DivvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			Xh = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case ExpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );


			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case InvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 0 );
			// Z is already defined
			break;
			// -------------------------------------------------

			case LdpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			
			CppADUnknownError( ind[0] > 0 );
			CppADUnknownError( ind[0] < Rec->NumVecInd() );

			// ind[2] is variable corresponding to this load
			if( ind[2] > 0 )
			{	Xh = RevHes + ind[2] * npv;
				for(j = 0; j < npv; j++)
					Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			
			CppADUnknownError( ind[0] > 0 );
			CppADUnknownError( ind[0] < Rec->NumVecInd() );

			// ind[2] is variable corresponding to this load
			if( ind[2] > 0 )
			{	Xh = RevHes + ind[2] * npv;
				for(j = 0; j < npv; j++)
					Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case LogOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case MulvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			Xf = ForJac + ind[0] * npv;
			Xh = RevHes + ind[0] * npv;
			Yf = ForJac + ind[1] * npv;
			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Yf[j]);
				Yh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case MulpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case MulvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			Xh = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case NonOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 0 );

			break;
			// -------------------------------------------------

			case ParOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );

			break;
			// -------------------------------------------------

			case PowpvOp:
			CppADUnknownError( n_var == 3 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Yf = ForJac + ind[1] * npv;
			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Yf[j]); 
				Yh[j] |= Zh[j] | (*Zr & Xf[j]); 
			}
			break;
			// -------------------------------------------------

			case PowvpOp:
			CppADUnknownError( n_var == 3 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			Xf = ForJac + ind[0] * npv;
			Xh = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]); 
				Yh[j] |= Zh[j] | (*Zr & Xf[j]); 
			}
			break;
			// -------------------------------------------------

			case PowvvOp:
			CppADUnknownError( n_var == 3 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			Xf = ForJac + ind[0] * npv;
			Xh = RevHes + ind[0] * npv;
			Yf = ForJac + ind[1] * npv;
			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & (Xf[j] | Yf[j])); 
				Yh[j] |= Zh[j] | (*Zr & (Xf[j] | Yf[j])); 
			}
			break;
			// -------------------------------------------------

			case PripOp:
			CppADUnknownError( n_var == 1);

			break;
			// -------------------------------------------------

			case PrivOp:
			// nvar should be zero for this case
			CppADUnknownError( n_var == 1);

			break;
			// -------------------------------------------------

			case SinOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// sine and cosine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case SinhOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// sine and cosine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case SqrtOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case StppOp:
			CppADUnknownError( n_var == 0);
			CppADUnknownError( n_ind == 3 );
			break;
			// -------------------------------------------------

			case StpvOp:
			CppADUnknownError( n_var == 0);
			CppADUnknownError( n_ind == 3 );
			break;
			// -------------------------------------------------

			case StvpOp:
			CppADUnknownError( n_var == 0);
			CppADUnknownError( n_ind == 3 );
			break;
			// -------------------------------------------------

			case StvvOp:
			CppADUnknownError( n_var == 0);
			CppADUnknownError( n_ind == 3 );
			break;
			// -------------------------------------------------

			case SubvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			Xh  = RevHes + ind[0] * npv;
			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
				Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case SubpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case SubvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			default:
			CppADUnknownError(0);
		}
	}
	CppADUnknownError( i_op == 1 );
	CppADUnknownError( Rec->GetOp(i_op-1) == NonOp );
	CppADUnknownError( i_var == NumVar(NonOp)  );

	return;
}

} // END CppAD namespace

# undef CPPAD_REV_HES_SWEEP_TRACE

# endif
