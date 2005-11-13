# ifndef CppADRevJacSweepIncluded
# define CppADRevJacSweepIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin RevJacSweep$$ $comment CppAD Developer Documentation$$
$spell
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

$table
$bold Syntax$$ $cnext
$syntax%void RevJacSweep(
	size_t                 %npv%,
	size_t                 %numvar%,
	TapeRec<%Base%>       *%Rec%,
	%Pack%                *%RevJac%
)%$$
$tend

$fend 20$$

$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^n \rightarrow B^m
\] $$

$head Description$$
Given the sparsity pattern for the dependent variables,
$code RevJacSweep$$ computes the sparsity pattern
for all the other variables.


$head numvar$$
is the number of rows in the entire sparsity pattern $italic RevJac$$.
It must also be equal to $syntax%%Rec%->TotNumVar()%$$.


$head npv$$
Is the number of elements of type $italic Pack$$
(per variable) in the sparsity pattern $italic RevJac$$.

$head On Input$$

$subhead Dependent Variables and Operators$$
The dependent variable records come first last.
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

$subhead Rec$$
None of the values stored in $italic Rec$$ are modified.

$subhead Dependent Variables$$
For $latex i = numvar-m, \ldots , numvar-1$$ 
and $latex j = 0 , \ldots , npv-1$$,
$syntax%%Taylor%[%i% * %npv% + %j%]%$$ is not modified.


$subhead Other Variables$$
For $latex i = 1, \ldots , numvar-m-1$$ and $latex j = 0 , \ldots , npv-1$$,
$syntax%%RevJac%[%i% * %npv% + %j%]%$$ is the
$th j$$ subset of the sparsity pattern for the variable with index $italic i$$.

$end
------------------------------------------------------------------------------
*/
# define CppADRevJacSweepTrace 0


// BEGIN CppAD namespace
namespace CppAD {

template <class Base, class Pack>
void RevJacSweep(
	size_t                npv,
	size_t                numvar,
	TapeRec<Base>        *Rec,
	Pack                 *RevJac
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        i_ind;
	size_t        n_var;
	size_t        n_ind;

	const size_t   *ind;
	Pack             *X;
	Pack             *Y;
	const Pack       *Z;

	// used by CExp operator 
	Pack      *trueCase;
	Pack     *falseCase;

	size_t            i;
	size_t            j;

	// initial VecAD sparsity pattern (inefficient because just the index
	// corresponding to size is used for all elements in a VecAD array)
	Pack   *VectorSto;
	i  = Rec->NumVecInd();
	if( i > 0 )
	{	VectorSto = new Pack[i * npv];
		while(i--)
		for(j = 0; j < npv; j++)
			VectorSto[i * npv + j] = 0;
	}
	else	VectorSto = CppADNull;

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
		Z      = RevJac + i_var * npv;

# if CppADForJacSweepTrace
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			ind,
			npv, 
			Z, 
			0, 
			(Pack *) CppADNull
		);
# endif

		// rest of information depends on the case
		switch( op )
		{
			case AbsOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );
			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AddvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case AddpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AddvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AcosOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// acos(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AsinOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// asin(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case AtanOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// atan(x) and 1 + x * x must be computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case CExpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 6);
			CppADUnknownError( ind[1] != 0 );

			trueCase  = RevJac + ind[4] * npv; // if ind[1] & 4 true
			falseCase = RevJac + ind[5] * npv; // if ind[1] & 8 true
			for(j = 0; j < npv; j++)
			{	if( ind[1] & 4 )
					trueCase[j] |= Z[j];
				if( ind[1] & 8 )
					falseCase[j] |= Z[j];
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

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// ---------------------------------------------------

			case CoshOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// hyperbolic cosine and sine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case DivpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case DivvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case ExpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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
			CppADUnknownError( VectorSto != CppADNull );

			// use sparsity for entire vector for this value
			for(j = 0; j < npv; j++)
				VectorSto[ j + (ind[0] - 1) * npv ] |= Z[j];
			break;
			// -------------------------------------------------

			case LdvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			
			CppADUnknownError( ind[0] > 0 );
			CppADUnknownError( ind[0] < Rec->NumVecInd() );
			CppADUnknownError( VectorSto != CppADNull );

			// use sparsity for entire vector for this value
			for(j = 0; j < npv; j++)
				VectorSto[ j + (ind[0] - 1) * npv ] |= Z[j];
			break;
			// -------------------------------------------------

			case LogOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case MulvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case MulpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case MulvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
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

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case SinhOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// sine and cosine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			X   = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case SqrtOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			case StppOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			break;
			// -------------------------------------------------

			case StpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			// update sparsity for entire vector
			Y = RevJac + ind[2] * npv;
			for(j = 0; j < npv; j++)
			{	Y[j] |= Z[j];
				VectorSto[ j + (ind[0] - 1) * npv] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case StvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			break;
			// -------------------------------------------------

			case StvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			Y    = RevJac + ind[2] * npv;
			for(j = 0; j < npv; j++)
			{	Y[j] |= Z[j];
				VectorSto[ j + (ind[0] - 1) * npv] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case SubvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = RevJac + ind[0] * npv;
			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
			{	X[j] |= Z[j];
				Y[j] |= Z[j];
			}
			break;
			// -------------------------------------------------

			case SubpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = RevJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Y[j] |= Z[j];
			break;
			// -------------------------------------------------

			case SubvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = RevJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				X[j] |= Z[j];
			break;
			// -------------------------------------------------

			default:
			CppADUnknownError(0);
		}
	}
	CppADUnknownError( i_op == 1 );
	CppADUnknownError( Rec->GetOp(i_op-1) == NonOp );
	CppADUnknownError( i_var == NumVar(NonOp)  );

	if( VectorSto != CppADNull )
		delete [] VectorSto;

	return;
}

} // END CppAD namespace

# undef CppADRevJacSweepTrace

# endif
