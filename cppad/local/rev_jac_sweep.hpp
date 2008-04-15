# ifndef CPPAD_REV_JAC_SWEEP_INCLUDED
# define CPPAD_REV_JAC_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin RevJacSweep$$ $comment CppAD Developer Documentation$$
$spell
	const
	Jacobian
	RevJacSweep
	npv
	numvar
	Num
	Var
	Op
	Taylor
	Inv
$$

$section Reverse Computation of Jacobian Sparsity Pattern$$
$index RevJacSweep$$
$index sparsity, reverse Jacobian$$
$index reverse, Jacobian sparsity$$
$index pattern, reverse Jacobian$$
$index bit pattern, Jacobian$$

$head Syntax$$
$syntax%void RevJacSweep(
	size_t %npv%,
	size_t %numvar%,
	const player<%Base%> *%Rec%,
	size_t %TaylorColDim%,
	const %Base% *%Taylor%,
	%Pack% *%RevJac%
)%$$


$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^n \rightarrow B^m
\] $$

$head Description$$
Given the Jacobian sparsity pattern for the dependent variables,
$code RevJacSweep$$ computes the sparsity pattern for the dependent
variables with respect to the independent variables.


$head numvar$$
is the number of rows in the entire sparsity pattern $italic RevJac$$.
It must also be equal to $syntax%%Rec%->TotNumVar()%$$.


$head npv$$
Is the number of elements of type $italic Pack$$
(per variable) in the sparsity pattern $italic RevJac$$.
 
$head TaylorColDim$$
Is the number of columns currently stored in the matrix $italic Taylor$$.

$head Taylor$$
For $latex i = 1 , \ldots , numvar$$,
$syntax%%Taylor%[%i% * %TaylorColDim%]%$$
is the value of the variable with index $italic i$$.

$head On Input$$

$subhead Dependent Variables and Operators$$
The dependent variable records come last.
For $latex i = numvar-m, \ldots , numvar-1$$ 
and $latex j = 0 , \ldots , npv$$,
$syntax%%RevJac%[%i% * %npv% + %j%]%$$
is the $th j$$ subset of the sparsity pattern for 
variable with index $italic i$$.   

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = 0, \ldots , numvar-m-1$$,
$latex j = 1 , \ldots , npv-1$$,
$syntax%%RevJac%[%i% * %npv% + %j%]%$$
is equal to zero (all false).

$head On Output$$

$subhead Dependent Variables$$
For $latex i = numvar-m, \ldots , numvar-1$$ 
and $latex j = 0 , \ldots , npv-1$$,
$syntax%%RevJac%[%i% * %npv% + %j%]%$$ is not modified.


$subhead Other Variables$$
For $latex i = 1, \ldots , numvar-m-1$$ and $latex j = 0 , \ldots , npv-1$$,
$syntax%%RevJac%[%i% * %npv% + %j%]%$$ is the
$th j$$ subset of the sparsity pattern for the variable with index $italic i$$.

$end
------------------------------------------------------------------------------
*/
# define CPPAD_REV_JAC_SWEEP_TRACE 0


// BEGIN CppAD namespace
namespace CppAD {

template <class Base, class Pack>
void RevJacSweep(
	size_t                npv,
	size_t                numvar,
	const player<Base>   *Rec,
	size_t                TaylorColDim,
	Base                 *Taylor,
	Pack                 *RevJac
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        i_ind;
	size_t        n_var;
	size_t        n_ind;

	const size_t   *ind = 0;
	Pack             *X = 0;
	Pack             *Y = 0;
	const Pack       *Z = 0;

	size_t            j;

	// used by CExp operator 
	const Base  *left = 0, *right = 0;
	Pack        *trueCase = 0, *falseCase = 0;
	Pack         zero(0);

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
		Z      = RevJac + i_var * npv;

# if CPPAD_REV_JAC_SWEEP_TRACE
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			ind,
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
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AcosOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// acos(x) and sqrt(1 - x * x) are computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AsinOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// asin(x) and sqrt(1 - x * x) are computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AtanOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// atan(x) and 1 + x * x must be computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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
			{	trueCase = RevJac + ind[4] * npv;
				for(j = 0; j < npv; j++)
				{	trueCase[j] |= CondExpTemplate(
						CompareOp( ind[0] ),
						*left,
						*right,
						Z[j],
						zero
					);
				}
			}
			if( ind[1] & 8 )
			{	falseCase = RevJac + ind[5] * npv;
				for(j = 0; j < npv; j++)
				{	falseCase[j] |= CondExpTemplate(
						CompareOp( ind[0] ),
						*left,
						*right,
						zero,
						Z[j]
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

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// ---------------------------------------------------

			case CoshOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// hyperbolic cosine and sine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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
			{	X = RevJac + ind[2] * npv;
				for(j = 0; j < npv; j++)
					X[j] |= Z[j];
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
			{	X = RevJac + ind[2] * npv;
				for(j = 0; j < npv; j++)
					X[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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

			case PowvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3 );
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case PowpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3 );
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case PowvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3 );
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j]; 
				Y[j] |= Z[j]; 
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

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case SinhOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// sine and cosine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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

# undef CPPAD_REV_JAC_SWEEP_TRACE

# endif
