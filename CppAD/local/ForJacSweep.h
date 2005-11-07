# ifndef CppADForJacSweepIncluded
# define CppADForJacSweepIncluded

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
$begin ForJacSweep$$ $comment CppAD Developer Documentation$$
$spell
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

$section Forward Computation of Dependency Matrix$$
$index ForJacSweep$$
$index sparsity, forward Jacobian$$
$index forward, Jacobian sparsity$$
$index pattern, forward Jacobian$$
$index bit pattern, Jacobian$$

$table
$bold Syntax$$ $cnext
$syntax%void ForJacSweep(
	size_t                 %npv%,
	size_t                 %numvar%,
	TapeRec<%Base%>       *%Rec%,
	%Pack%                *%ForJac%
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
Given the sparsity pattern for the independent variables,
$code ForJacSweep$$ computes the sparsity pattern
for all the other variables.


$head numvar$$
is the number of rows in the entire sparsity pattern $italic ForJac$$.
It must also be equal to $syntax%%Rec%->TotNumVar()%$$.


$head npv$$
Is the number of elements of type $italic P$$
(per variable) in the sparsity pattern $italic ForJac$$.

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
	$th j$$ set of sparsity pattern for variable with index $italic i$$   
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	the operator with index $italic i$$ must be a $code InvOp$$
$tend

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = n+1, \ldots , numvar-1$$,
$latex j = 0 , \ldots , npv-1$$,
and $latex k = n+1, \ldots ,$$ $syntax%%Rec%->NumOp() - 1%$$,
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

$subhead Rec$$
None of the values stored in $italic Rec$$ are modified.

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
# define CppADForJacSweepTrace 0


// BEGIN CppAD namespace
namespace CppAD {

template <class Base, class Pack>
void ForJacSweep(
	size_t                npv,
	size_t                numvar,
	TapeRec<Base>        *Rec,
	Pack                 *ForJac
)
{
	size_t        numop;
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        i_ind;
	size_t        n_var;
	size_t        n_ind;

	const size_t   *ind;
	const Pack       *X;
	const Pack       *Y;

	// used by CExp operator 
	const Pack  *trueCase, *falseCase;

	Pack             *Z;
	Pack           *Tmp;

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
			VectorSto[j + i * npv ] = 0;
	}
	else	VectorSto = CppADNull;

	// check numvar argument
	CppADUnknownError( Rec->TotNumVar() == numvar );

	// set the number of operators
	numop = Rec->NumOp();

	// skip the NonOp at the beginning of the recording
	i_op  = 0;
	i_var = 0;
	i_ind = 0;
	op    = Rec->GetOp(i_op);
	n_var = NumVar(op);
	n_ind = NumInd(op);
	CppADUnknownError( op == NonOp );
	CppADUnknownError( n_var == 1 );
	CppADUnknownError( n_ind == 0 );

	while(++i_op < numop)
	{
		// increment for previous op
		i_var += n_var;
		i_ind += n_ind;

		// this op
		op     = Rec->GetOp(i_op);

		// number of variables
		n_var  = NumVar(op);

		// index field values for this op
		n_ind  = NumInd(op);
		ind    = Rec->GetInd(n_ind, i_ind);

		// value of z for this op
		Z      = ForJac + i_var * npv;

		// rest of information depends on the case
		switch( op )
		{
			case AbsOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
			break;
			// -------------------------------------------------

			case AddvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = ForJac + ind[0] * npv;
			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case AddpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = Y[j];
			break;
			// -------------------------------------------------

			case AddvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
			break;
			// -------------------------------------------------

			case AcosOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// acos(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = ForJac + (i_var+1) * npv;
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Tmp[j] = Z[j] = X[j];
			break;
			// -------------------------------------------------

			case AsinOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// asin(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = ForJac + (i_var+1) * npv;
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Tmp[j] = Z[j] = X[j];
			break;
			// -------------------------------------------------

			case AtanOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// atan(x) and 1 + x * x must be computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = ForJac + (i_var+1) * npv;
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Tmp[j] = Z[j] = X[j];
			break;
			// -------------------------------------------------

			case CExpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 6);
			CppADUnknownError( ind[1] != 0 );

			trueCase  = ForJac + ind[4] * npv; // if ind[1] & 4 true
			falseCase = ForJac + ind[5] * npv; // if ind[1] & 8 true
			for(j = 0; j < npv; j++)
			{	if( ind[1] & 4 )
					Z[j] = trueCase[j];
				else	Z[j] = 0;
				if( ind[1] & 8 )
					Z[j] |= falseCase[j];
			}
			break;
			// ---------------------------------------------------

			case CosOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// cosine and sine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = ForJac + (i_var+1) * npv;
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Tmp[j] = Z[j] = X[j];
			break;
			// ---------------------------------------------------

			case CoshOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// hyperbolic cosine and sine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = ForJac + (i_var+1) * npv;
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Tmp[j] = Z[j] = X[j];
			break;
			// -------------------------------------------------

			case DisOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );

			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;
			// -------------------------------------------------

			case DivvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = ForJac + ind[0] * npv;
			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case DivpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = Y[j];
			break;
			// -------------------------------------------------

			case DivvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
			break;
			// -------------------------------------------------

			case EqfppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			break;
			// -------------------------------------------------

			case EqtppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			break;
			// -------------------------------------------------

			case EqfpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case EqtpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case EqfvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			break;
			// -------------------------------------------------

			case EqtvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			break;
			// -------------------------------------------------

			case EqfvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case EqtvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case ExpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
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
				Z[j] = VectorSto[ j + (ind[0] - 1) * npv ];
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
				Z[j] = VectorSto[ j + (ind[0] - 1) * npv ];
			break;
			// -------------------------------------------------

			case LefppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			break;
			// -------------------------------------------------

			case LetppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			break;
			// -------------------------------------------------

			case LefpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case LetpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case LefvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			break;
			// -------------------------------------------------

			case LetvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			break;
			// -------------------------------------------------

			case LefvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case LetvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case LogOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
			break;
			// -------------------------------------------------

			case LtfppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			break;
			// -------------------------------------------------


			case LttppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			break;
			// -------------------------------------------------

			case LtfpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case LttpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case LtfvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			break;
			// -------------------------------------------------

			case LttvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			break;
			// -------------------------------------------------

			case LtfvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case LttvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			break;
			// -------------------------------------------------

			case MulvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = ForJac + ind[0] * npv;
			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case MulpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = Y[j];
			break;
			// -------------------------------------------------

			case MulvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
			break;
			// -------------------------------------------------

			case NonOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 0 );
			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;

			case ParOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;
			// -------------------------------------------------

			case PripOp:
			CppADUnknownError( n_var == 1);
			for(j = 0; j < npv; j++)
				Z[j] = 0;
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

			// use Tmp for data stored in second variable
			Tmp = ForJac + (i_var+1) * npv;
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = Tmp[j] = X[j];
			break;
			// -------------------------------------------------

			case SinhOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// sine and cosine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in second variable
			Tmp = ForJac + (i_var+1) * npv;
			X   = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = Tmp[j] = X[j];
			break;
			// -------------------------------------------------

			case SqrtOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
			break;
			// -------------------------------------------------

			case StppOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			// sparsity of vector does not change in this case
			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;
			// -------------------------------------------------

			case StpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			// update sparsity for entire vector
			Y = ForJac + ind[2] * npv;
			for(j = 0; j < npv; j++)
			{	Z[j] = Y[j];
				VectorSto[ j + (ind[0] - 1) * npv] |= Y[j];
			}
			break;
			// -------------------------------------------------

			case StvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			// sparsity of vector does not change in this case
			for(j = 0; j < npv; j++)
				Z[j] = 0;
			break;
			// -------------------------------------------------

			case StvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			Y    = ForJac + ind[2] * npv;
			for(j = 0; j < npv; j++)
			{	Z[j] = Y[j];
				VectorSto[ j + (ind[0] - 1) * npv] |= Y[j];
			}
			break;
			// -------------------------------------------------

			case SubvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = ForJac + ind[0] * npv;
			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j] | Y[j];
			break;
			// -------------------------------------------------

			case SubpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = ForJac + ind[1] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = Y[j];
			break;
			// -------------------------------------------------

			case SubvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = ForJac + ind[0] * npv;
			for(j = 0; j < npv; j++)
				Z[j] = X[j];
			break;
			// -------------------------------------------------

			default:
			CppADUnknownError(0);
		}
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
	}
	std::cout << std::endl;
# else
	}
# endif
	CppADUnknownError( (i_var + n_var) == Rec->TotNumVar() );

	if( VectorSto != CppADNull )
		delete [] VectorSto;

	return;
}

} // END CppAD namespace

# undef CppADForJacSweepTrace

# endif
