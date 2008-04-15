# ifndef CPPAD_REV_HES_SWEEP_INCLUDED
# define CPPAD_REV_HES_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

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
	const player<%Base%> *%Rec%,
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
	const player<Base>   *Rec,
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

	const size_t   *ind = 0;

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
	CPPAD_ASSERT_UNKNOWN( Rec->TotNumVar() == numvar );
	CPPAD_ASSERT_UNKNOWN( numvar > 0 );

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
		CPPAD_ASSERT_UNKNOWN( i_var >= n_var );
		i_var -= n_var;

		// corresponding index values
		n_ind  = NumInd(op);
		CPPAD_ASSERT_UNKNOWN( i_ind >= n_ind );
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
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Xh  = RevHes + ind[0] * npv;
			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
				Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case AcosOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// acos(x) and sqrt(1 - x * x) are computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case AsinOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// asin(x) and sqrt(1 - x * x) are computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );


			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case AtanOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// atan(x) and 1 + x * x must be computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------
			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 6);
			CPPAD_ASSERT_UNKNOWN( ind[1] != 0 );

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
			CPPAD_ASSERT_UNKNOWN( n_var == 0 );
			CPPAD_ASSERT_UNKNOWN( n_ind == 4 );
			CPPAD_ASSERT_UNKNOWN( ind[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// cosine and sine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// ---------------------------------------------------

			case CoshOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// hyperbolic cosine and sine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );


			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );

			break;
			// -------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

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
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Yf = ForJac + ind[1] * npv;
			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j] | (*Zr & Yf[j]);
			}
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xh = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );


			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 0 );
			// Z is already defined
			break;
			// -------------------------------------------------

			case LdpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			
			CPPAD_ASSERT_UNKNOWN( ind[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );

			// ind[2] is variable corresponding to this load
			if( ind[2] > 0 )
			{	Xh = RevHes + ind[2] * npv;
				for(j = 0; j < npv; j++)
					Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			
			CPPAD_ASSERT_UNKNOWN( ind[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );

			// ind[2] is variable corresponding to this load
			if( ind[2] > 0 )
			{	Xh = RevHes + ind[2] * npv;
				for(j = 0; j < npv; j++)
					Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

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
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xh = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 0 );

			break;
			// -------------------------------------------------

			case ParOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );

			break;
			// -------------------------------------------------

			case PowpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3 );
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Yf = ForJac + ind[1] * npv;
			Yh = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Yf[j]); 
				Yh[j] |= Zh[j] | (*Zr & Xf[j]); 
			}
			break;
			// -------------------------------------------------

			case PowvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3 );
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xf = ForJac + ind[0] * npv;
			Xh = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]); 
				Yh[j] |= Zh[j] | (*Zr & Xf[j]); 
			}
			break;
			// -------------------------------------------------

			case PowvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3 );
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

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
			CPPAD_ASSERT_UNKNOWN( n_var == 1);

			break;
			// -------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);

			break;
			// -------------------------------------------------

			case SinOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// sine and cosine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case SinhOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// sine and cosine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xf  = ForJac + ind[0] * npv;
			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j] | (*Zr & Xf[j]);
			}
			break;
			// -------------------------------------------------

			case StppOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			break;
			// -------------------------------------------------

			case StpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			break;
			// -------------------------------------------------

			case StvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			break;
			// -------------------------------------------------

			case StvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			break;
			// -------------------------------------------------

			case SubvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Xh  = RevHes + ind[0] * npv;
			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
				Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Yh  = RevHes + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	Yh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			Xh  = RevHes + ind[0] * npv;
			for(j = 0; j < npv; j++)
			{	Xh[j] |= Zh[j];
			}
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
	CPPAD_ASSERT_UNKNOWN( i_op == 1 );
	CPPAD_ASSERT_UNKNOWN( Rec->GetOp(i_op-1) == NonOp );
	CPPAD_ASSERT_UNKNOWN( i_var == NumVar(NonOp)  );

	return;
}

} // END CppAD namespace

# undef CPPAD_REV_HES_SWEEP_TRACE

# endif
