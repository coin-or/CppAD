# ifndef CppADADForwardIncluded
# define CppADADForwardIncluded

// BEGIN SHORT COPYRIGHT
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
// END SHORT COPYRIGHT

/*
$begin ADForward$$
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
$mindex ADForward derivative second order Taylor coefficient$$

$table
$bold Syntax$$ $cnext
$syntax%size_t ADForward(
	bool                   %print%,
	size_t                 %d%,
	size_t                 %numvar%,
	TapeRec<%Base%>       *%Rec%,
	size_t                 %J%,
	Base                  *%Taylor%,
)%$$
$tend

$fend 20$$

$head Return Value$$
The return value is equal to the number of
$syntax%AD<%Base%>%$$ comparison operations have a different result
from when the information in $italic Rec$$ was recorded.


$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^m \rightarrow B^n
\] $$

$head print$$
If $italic print$$ is false,
suppress the output that is otherwise generated 
by the PripOp and PrivOp instructions.


$head d$$
Given the $th d-1$$ order Taylor coefficients matrix for all the variables,
and the $th d$$ order Taylor coefficients for all the independent variables,
$code ADForward$$ computes the $th d$$ order Taylor coefficients 
for all the other variables.


$head numvar$$
is the number of rows in the matrix $italic Taylor$$.
It must also be equal to $syntax%%Rec%->TotNumVar()%$$.


$head J$$
Is the number of columns in the coefficient matrix $italic Taylor$$.
This must be greater than or equal $latex d+1$$.


$head On Input$$

$subhead Independent Variables and Operators$$
The independent variable records come first.
For $latex i = 1, \ldots , m$$ and $latex j = 0 , \ldots , d$$,
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
	the operator with index zero must be a $code InvOp$$
$tend

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = m+1, \ldots , numvar-1$$,
$latex j = 0 , \ldots , d-1$$,
and $latex k = m+1, \ldots ,$$ $syntax%%Rec%->NumOp() - 1%$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%i% * %J% + %j%]%$$      $cnext 
	$th j$$ coefficient for independent variable with index $italic i$$     
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	any operator except for $code InvOp$$ 
$tend

$head On Output$$

$subhead Rec$$
None of the values stored in $italic Rec$$ are modified.

$subhead Independent Variables$$
For $latex i = 1, \ldots , m$$ and $latex j = 0 , \ldots , J-1$$,
$syntax%%Taylor%[%i% * %J% + %j%]%$$ is not modified.


$subhead Other Variables$$
For $latex i = m+1, \ldots , numvar-1$$ and $latex j \neq d$$,
$syntax%%Taylor%[%i% * %J% + %j%]%$$ is not modified.
For $latex i = m+1, \ldots , numvar-1$$, 
$syntax%%Taylor%[%i% * %J% + %d%]%$$ is set equal to the
$th d$$ order Taylor coefficient for the variable with index $italic i$$.


$end
------------------------------------------------------------------------------
*/
# define CppADForwardTrace 0

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
size_t ADForward(
	bool                  print,
	size_t                d,
	size_t                numvar,
	TapeRec<Base>        *Rec,
	size_t                J,
	Base                 *Taylor
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
	const Base       *P;
	const Base       *X;
	const Base       *Y;

	Base             *Z;
	Base           *Tmp;

	size_t            i;
	size_t          len;


	// The status is only computed when d is zero and
	static size_t compareCount;
	if( d == 0 )
		compareCount = 0;

	// if this is an order zero calculation, initialize vector indices
	size_t *VectorInd;
	bool   *VectorSto;
	i = Rec->NumVecInd();
	if( i > 0 )
	{	VectorInd = new size_t[i];
		VectorSto = new bool[i];
		while(i--)
		{	VectorInd[i] = Rec->GetVecInd(i);
			VectorSto[i] = false;
		}
	}
	else
	{	VectorInd = CppADNull;
		VectorSto = CppADNull;
	}


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
		Z      = Taylor + i_var * J;

		// rest of information depends on the case

		switch( op )
		{
			case AbsOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );
			X   = Taylor + ind[0] * J;
			ForAbsOp(d, Z, X);
			break;
			// -------------------------------------------------

			case AddvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			ForAddvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			P = Rec->GetPar( ind[0] );
			Y = Taylor + ind[1] * J;
			ForAddpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			P = Rec->GetPar( ind[1] );
			ForAddvpOp(d, Z, X, P);
			break;
			// -------------------------------------------------

			case AcosOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// acos(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = Taylor + (i_var+1) * J;
			X   = Taylor + ind[0] * J;
			ForAcosOp(d, Z, Tmp, X);
			break;
			// -------------------------------------------------

			case AsinOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// asin(x) and sqrt(1 - x * x) are computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = Taylor + (i_var+1) * J;
			X   = Taylor + ind[0] * J;
			ForAsinOp(d, Z, Tmp, X);
			break;
			// -------------------------------------------------

			case AtanOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// atan(x) and 1 + x * x must be computed in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = Taylor + (i_var+1) * J;
			X   = Taylor + ind[0] * J;
			ForAtanOp(d, Z, Tmp, X);
			break;
			// -------------------------------------------------

			case CEpppOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError(0); // this instruction is not used
			break;


			// ---------------------------------------------------

			case CEppvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			CppADUnknownError( ind[2] < i_var );
			Y = Taylor + ind[2] * J;
			Z[d] = CondExp( 
				*(Rec->GetPar( ind[0] )),
				d == 0 ? *(Rec->GetPar( ind[1] )) : Base(0),
				Y[d]
			);
			break;

			// ---------------------------------------------------

			case CEpvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			CppADUnknownError( ind[1] < i_var );
			X = Taylor + ind[1] * J;
			Z[d] = CondExp( 
				*(Rec->GetPar( ind[0] )),
				X[d],
				d == 0 ? *(Rec->GetPar( ind[2] )) : Base(0)
			);
			break;

			// ---------------------------------------------------

			case CEpvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			CppADUnknownError( ind[1] < i_var );
			CppADUnknownError( ind[2] < i_var );
			X = Taylor + ind[1] * J;
			Y = Taylor + ind[2] * J;
			Z[d] = CondExp( 
				*(Rec->GetPar( ind[0] )),
				X[d],
				Y[d]
			);
			break;

			// ---------------------------------------------------

			case CEvppOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			CppADUnknownError( ind[0] < i_var );
			Z[d] = CondExp(
				*(Taylor + ind[0] * J),
				d == 0 ? *(Rec->GetPar( ind[1] )) : Base(0),
				d == 0 ? *(Rec->GetPar( ind[2] )) : Base(0)
			);
			break;


			// ---------------------------------------------------

			case CEvpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[2] < i_var );
			Y = Taylor + ind[2] * J;
			Z[d] = CondExp( 
				*(Taylor + ind[0] * J), 
				d == 0 ? *(Rec->GetPar( ind[1] )) : Base(0),
				Y[d]
			);
			break;

			// ---------------------------------------------------

			case CEvvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			X = Taylor + ind[1] * J;
			Z[d] = CondExp( 
				*(Taylor + ind[0] * J), 
				X[d],
				d == 0 ? *(Rec->GetPar( ind[2] )) : Base(0)
			);
			break;

			// ---------------------------------------------------

			case CEvvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			CppADUnknownError( ind[2] < i_var );
			X = Taylor + ind[1] * J;
			Y = Taylor + ind[2] * J;
			Z[d] = CondExp( 
				*(Taylor + ind[0] * J), 
				X[d],
				Y[d]
			);
			break;

			// ---------------------------------------------------

			case CosOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// cosine and sine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in variable record
			Tmp = Taylor + (i_var+1) * J;
			X   = Taylor + ind[0] * J;
			ForSinCos(d, Tmp, Z, X);
			break;
			// -------------------------------------------------

			case DisOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			if( d == 0 ) 
			{	ind    = Rec->GetInd(n_ind, i_ind);
				CppADUnknownError( ind[0] < i_var );
				X   = Taylor + ind[0] * J;
				Z[0] = ADDiscrete<Base>::Eval(ind[1], X[0]);
			}
			else	Z[d] = Base(0);
			break;
			// -------------------------------------------------

			case DivvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			ForDivvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = Taylor + ind[1] * J;
			P = Rec->GetPar( ind[0] );
			ForDivpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			P = Rec->GetPar( ind[1] );
			X = Taylor + ind[0] * J;
			ForDivvpOp(d, Z, X, P);
			break;

			// -------------------------------------------------
			case EqfppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			if( d == 0 )
			{	X            = Rec->GetPar( ind[0] );
				Y            = Rec->GetPar( ind[1] );
				compareCount += (X[0] == Y[0]);
			}
			break;

			// -------------------------------------------------
			case EqtppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			if( d == 0 )
			{	X            = Rec->GetPar( ind[0] );
				Y            = Rec->GetPar( ind[1] );
				compareCount += (X[0] != Y[0]);
			}
			break;

			// -------------------------------------------------
			case EqfpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	P            = Rec->GetPar( ind[0] );
				Y            = Taylor + ind[1] * J;
				compareCount += (*P == Y[0]);
			}
			break;

			// -------------------------------------------------
			case EqtpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	P            = Rec->GetPar( ind[0] );
				Y            = Taylor + ind[1] * J;
				compareCount += (*P != Y[0]);
			}
			break;

			// -------------------------------------------------
			case EqfvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				P            = Rec->GetPar( ind[1] );
				compareCount += (X[0] == *P);
			}
			break;

			// -------------------------------------------------
			case EqtvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				P            = Rec->GetPar( ind[1] );
				compareCount += (X[0] != *P);
			}
			break;

			// -------------------------------------------------
			case EqfvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				Y            = Taylor + ind[1] * J;
				compareCount += (X[0] == Y[0]);
			}
			break;

			// -------------------------------------------------
			case EqtvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				Y            = Taylor + ind[1] * J;
				compareCount += (X[0] != Y[0]);
			}
			break;

			// -------------------------------------------------
			case ExpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			ForExpOp(d, Z, X);
			break;
			// -------------------------------------------------
			case LdpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			
			CppADUnknownError( ind[0] > 0 );
			CppADUnknownError( ind[0] < Rec->NumVecInd() );
			CppADUnknownError( VectorInd != CppADNull );
			CppADUnknownError( VectorSto != CppADNull );

			if( d == 0 )
			{	i   = ind[1];
				CppADUnknownError( 
					i < VectorInd[ind[0] - 1] 
				);
				CppADUnknownError( 
					i + ind[0] < Rec->NumVecInd() 
				);

				if( VectorSto[ i + ind[0] ] )
				{	i   = VectorInd[ i + ind[0] ];
					Rec->ReplaceInd(i_ind + 2, i);
					CppADUnknownError(i > 0 );
				}
				else
				{	i   = VectorInd[ i + ind[0] ];
					Rec->ReplaceInd(i_ind + 2, 0);
					Z[d] = *(Rec->GetPar(i));
					i    = 0;
				}
			}
			else	i = ind[2];
			if( i > 0 )
			{	CppADUnknownError( i < i_var );
				Y     = Taylor + i * J;
				Z[d]  = Y[d];
			}
			break;
			// -------------------------------------------------
			case LdvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );
			
			CppADUnknownError( ind[0] > 0 );
			CppADUnknownError( ind[0] < Rec->NumVecInd() );
			CppADUnknownError( VectorInd != CppADNull );
			CppADUnknownError( VectorSto != CppADNull );

			if( d == 0 )
			{
				X   = Taylor + ind[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ ind[0] - 1 ];
				CppADUsageError( 
					i < len,
					"VecAD index value >= vector length"
				);
				CppADUnknownError( 
					i + ind[0] < Rec->NumVecInd() 
				);

				if( VectorSto[ i + ind[0] ] )
				{	i   = VectorInd[ i + ind[0] ];
					Rec->ReplaceInd(i_ind + 2, i);
					CppADUnknownError(i > 0 );
				}
				else
				{	i   = VectorInd[ i + ind[0] ];
					Rec->ReplaceInd(i_ind + 2, 0);
					Z[d] = *(Rec->GetPar(i));
					i    = 0;
				}
			}
			else	i = ind[2];
			if( i > 0 )
			{	CppADUnknownError( i < i_var );
				Y     = Taylor + i * J;
				Z[d]  = Y[d];
			}
			break;
			// -------------------------------------------------

			case InvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 0 );
			break;

			// -------------------------------------------------
			case LefppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			if( d == 0 )
			{	X            = Rec->GetPar( ind[0] );
				Y            = Rec->GetPar( ind[1] );
				compareCount += LessThanOrZero(X[0] - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LetppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			if( d == 0 )
			{	X            = Rec->GetPar( ind[0] );
				Y            = Rec->GetPar( ind[1] );
				compareCount += ! LessThanOrZero(X[0] - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LefpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	P            = Rec->GetPar( ind[0] );
				Y            = Taylor + ind[1] * J;
				compareCount += LessThanOrZero(*P - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LetpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	P            = Rec->GetPar( ind[0] );
				Y            = Taylor + ind[1] * J;
				compareCount += ! LessThanOrZero(*P - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LefvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				P            = Rec->GetPar( ind[1] );
				compareCount += LessThanOrZero(X[0] - *P);
			}
			break;

			// -------------------------------------------------
			case LetvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				P            = Rec->GetPar( ind[1] );
				compareCount += ! LessThanOrZero(X[0] - *P);
			}
			break;

			// -------------------------------------------------
			case LefvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				Y            = Taylor + ind[1] * J;
				compareCount += LessThanOrZero(X[0] - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LetvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				Y            = Taylor + ind[1] * J;
				compareCount += ! LessThanOrZero(X[0] - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LogOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			ForLogOp(d, Z, X);
			break;

			// -------------------------------------------------
			case LtfppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			if( d == 0 )
			{	X            = Rec->GetPar( ind[0] );
				Y            = Rec->GetPar( ind[1] );
				compareCount += LessThanZero(X[0] - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LttppOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			if( d == 0 )
			{	X            = Rec->GetPar( ind[0] );
				Y            = Rec->GetPar( ind[1] );
				compareCount += ! LessThanZero(X[0] - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LtfpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	P            = Rec->GetPar( ind[0] );
				Y            = Taylor + ind[1] * J;
				compareCount += LessThanZero(*P - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LttpvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	P            = Rec->GetPar( ind[0] );
				Y            = Taylor + ind[1] * J;
				compareCount += ! LessThanZero(*P - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LtfvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				P            = Rec->GetPar( ind[1] );
				compareCount += LessThanZero(X[0] - *P);
			}
			break;

			// -------------------------------------------------
			case LttvpOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				P            = Rec->GetPar( ind[1] );
				compareCount += ! LessThanZero(X[0] - *P);
			}
			break;

			// -------------------------------------------------
			case LtfvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				Y            = Taylor + ind[1] * J;
				compareCount += LessThanZero(X[0] - Y[0]);
			}
			break;

			// -------------------------------------------------
			case LttvvOp:
			CppADUnknownError( n_var == 0 );
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );
			if( d == 0 )
			{	X            = Taylor + ind[0] * J;
				Y            = Taylor + ind[1] * J;
				compareCount += ! LessThanZero(X[0] - Y[0]);
			}
			break;
			// -------------------------------------------------

			case MulvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			ForMulvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			Y = Taylor + ind[1] * J;
			P = Rec->GetPar( ind[0] );
			ForMulpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			P = Rec->GetPar( ind[1] );
			ForMulvpOp(d, Z, X, P);
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

			P = Rec->GetPar( ind[0] );
			if( d == 0 )
				Z[d] = *P;
			else	Z[d] = Base(0); 
			break;
			// -------------------------------------------------

			case PripOp:
			CppADUnknownError( n_var == 1);
			if( print & (d == 0) )
			{	n_ind  = 2;
				CppADUnknownError( ind[0] < Rec->NumTxt() );
				std::cout << Rec->GetTxt(ind[0]);
				std::cout << *(Rec->GetPar(ind[1]));
			}
			break;
			// -------------------------------------------------

			case PrivOp:
			CppADUnknownError( n_var == 1);
			if( print & (d == 0) )
			{	n_ind  = 2;
				CppADUnknownError( ind[0] < Rec->NumTxt() );
				CppADUnknownError( ind[1] < i_var );

				X      = Taylor + ind[1] * J;
				std::cout << Rec->GetTxt(ind[0]);
				std::cout << X[0];
			}
			break;
			// -------------------------------------------------

			case SinOp:
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			// sine and cosine must come in pairs
			CppADUnknownError( n_var == 2);
			CppADUnknownError( (i_var+1) < numvar  );

			// use Tmp for data stored in second variable
			Tmp = Taylor + (i_var+1) * J;
			X   = Taylor + ind[0] * J;
			ForSinCos(d, Z, Tmp, X);
			break;
			// -------------------------------------------------

			case SqrtOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 1 );
			CppADUnknownError( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			ForSqrtOp(d, Z, X);
			break;
			// -------------------------------------------------

			case StppOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			if( d == 0 )
			{	CppADUnknownError( VectorInd != CppADNull );
				CppADUnknownError( VectorSto != CppADNull );
				CppADUnknownError( ind[0] < Rec->NumVecInd() );

				i   = ind[1];
				CppADUnknownError(i < VectorInd[ind[0] - 1]);
				CppADUnknownError( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = i_var;
				VectorSto[ i + ind[0] ] = true;

				Z[d] = *( Rec->GetPar( ind[2] ) );
			}
			else	Z[d] = Base(0);
			break;
			// -------------------------------------------------

			case StpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			if( d == 0 )
			{	CppADUnknownError( VectorInd != CppADNull );
				CppADUnknownError( VectorSto != CppADNull );
				CppADUnknownError( ind[0] < Rec->NumVecInd() );
				CppADUnknownError( ind[2] < i_var );

				i   = ind[1];
				CppADUnknownError(i < VectorInd[ind[0] - 1]);
				CppADUnknownError( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = i_var;
				VectorSto[ i + ind[0] ] = true;
			}
			Y    = Taylor + ind[2] * J;
			Z[d] = Y[d];
			break;
			// -------------------------------------------------

			case StvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			if( d == 0 )
			{	CppADUnknownError( VectorInd != CppADNull );
				CppADUnknownError( VectorSto != CppADNull );
				CppADUnknownError( ind[0] < Rec->NumVecInd() );
				CppADUnknownError( ind[1] < i_var );

				X   = Taylor + ind[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ ind[0] - 1 ];
				CppADUsageError( 
					i < len,
					"VecAD index value >= vector length"
				);
				CppADUnknownError( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = i_var;
				VectorSto[ i + ind[0] ] = true;

				Z[d] = *( Rec->GetPar( ind[2] ) );
			}
			else	Z[d] = Base(0);
			break;
			// -------------------------------------------------

			case StvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 3 );

			if( d == 0 )
			{	CppADUnknownError( VectorInd != CppADNull );
				CppADUnknownError( VectorSto != CppADNull );
				CppADUnknownError( ind[0] < Rec->NumVecInd() );
				CppADUnknownError( ind[1] < i_var );
				CppADUnknownError( ind[2] < i_var );

				X   = Taylor + ind[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ ind[0] - 1 ];
				CppADUsageError( 
					i < len,
					"VecAD index value >= vector length"
				);
				CppADUnknownError( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = i_var;
				VectorSto[ i + ind[0] ] = true;
			}
			Y    = Taylor + ind[2] * J;
			Z[d] = Y[d];
			break;
			// -------------------------------------------------

			case SubvvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );
			CppADUnknownError( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			ForSubvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case SubpvOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[1] < i_var );

			P = Rec->GetPar( ind[0] );
			Y = Taylor + ind[1] * J;
			ForSubpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CppADUnknownError( n_var == 1);
			CppADUnknownError( n_ind == 2 );
			CppADUnknownError( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			P = Rec->GetPar( ind[1] );
			ForSubvpOp(d, Z, X, P);
			break;
			// -------------------------------------------------

			default:
			CppADUnknownError(0);
		}
# if CppADForwardTrace
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			ind,
			d + 1, 
			Z, 
			0, 
			(Base *) CppADNull
		);
	}
	std::cout << std::endl;
# else
	}
# endif
	CppADUnknownError( (i_var + n_var) == Rec->TotNumVar() );
	if( VectorInd != CppADNull )
		delete [] VectorInd;
	if( VectorSto != CppADNull )
		delete [] VectorSto;

	return compareCount;
}

} // END CppAD namespace

# undef CppADForwardTrace

# endif
