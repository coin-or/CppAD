# ifndef CPPAD_FORWARD0SWEEP_INCLUDED
# define CPPAD_FORWARD0SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

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


$head numvar$$
is the number of rows in the matrix $italic Taylor$$.
It must also be equal to $syntax%%Rec%->TotNumVar()%$$.


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
# ifdef NDEBUG
# define CPPAD_GET_PAR(i) (P_0 + i)
# else
# define CPPAD_GET_PAR(i) Rec->GetPar(i)
# endif

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
size_t forward0sweep(
	bool                  print,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                J,
	Base                 *Taylor
)
{
	// some constants
	Base one(1);

	size_t        numop;
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        i_ind;
	size_t        n_var;
	size_t        n_ind;

	const size_t   *ind = 0;
	const size_t *ind_0 = 0;
	const Base     *P_0 = 0;
	const Base       *P = 0;
	const Base       *X = 0;
	const Base       *Y = 0;

	// used by CExp operator (left and right also used by Com operator)
	const Base  *left = 0, *right = 0, *trueCase = 0, *falseCase = 0;

	// used by Com operator
	bool result;

	Base             *Z = 0;
	Base             *W = 0;
	Base             *U = 0;

	size_t            i;
	size_t          len;


	// initialize the comparision operator (ComOp) counter
	size_t compareCount = 0;

	// if this is an order zero calculation, initialize vector indices
	size_t *VectorInd = CPPAD_NULL;  // address for each element
	bool   *VectorVar = CPPAD_NULL;  // is element a variable
	i = Rec->NumVecInd();
	if( i > 0 )
	{	VectorInd = CPPAD_TRACK_NEW_VEC(i, VectorInd);
		VectorVar = CPPAD_TRACK_NEW_VEC(i, VectorVar);
		while(i--)
		{	VectorInd[i] = Rec->GetVecInd(i);
			VectorVar[i] = false;
		}
	}


	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->TotNumVar() == numvar );

	// set the number of operators
	numop = Rec->NumOp();

	// skip the NonOp at the beginning of the recording
	i_op  = 0;
	i_var = 0;
	i_ind = 0;
	op    = Rec->GetOp(i_op);
	n_var = 1;
	n_ind = 0;
	ind_0 = Rec->GetInd(n_ind, i_ind);
	if( Rec->NumPar() > 0 )
		P_0   = Rec->GetPar(0);
	ind   = ind_0;

	CPPAD_ASSERT_UNKNOWN( op == NonOp );
	while(++i_op < numop)
	{
		// check previous n_var and n_ind
		CPPAD_ASSERT_UNKNOWN( n_var == NumVar(op) );
		CPPAD_ASSERT_UNKNOWN( n_ind == NumInd(op) );

		// increment for previous op
		i_var += n_var;
		ind   += n_ind;

		// this op
		op     = Rec->GetOp(i_op);

		// value of z for this op
		Z      = Taylor + i_var * J;

		// rest of information depends on the case

		switch( op )
		{
			case AbsOp:
			n_var = 1;
			n_ind = 1;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			X   = Taylor + ind[0] * J;
			if( LessThanZero( X[0] ) )
				Z[0] = - X[0];
			else	Z[0] = X[0];
			break;
			// -------------------------------------------------

			case AddvvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			Z[0] = X[0] + Y[0];
			break;
			// -------------------------------------------------

			case AddpvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			P = CPPAD_GET_PAR(ind[0]);
			Y = Taylor + ind[1] * J;
			Z[0] = P[0] + Y[0];
			break;
			// -------------------------------------------------

			case AddvpOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			P = CPPAD_GET_PAR(ind[1]);
			Z[0] = X[0] + P[0];
			break;
			// -------------------------------------------------

			case AcosOp:
			// variables: acos(x),  sqrt(1 - x * x) 
			n_ind = 1;
			n_var = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + ind[0] * J;
			W[0] = sqrt( one - X[0] * X[0] );
			Z[0] = acos( X[0] );
			break;
			// -------------------------------------------------

			case AsinOp:
			// variables: asin(x),  sqrt(1 - x * x) 
			n_ind = 1;
			n_var = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + ind[0] * J;
			W[0] = sqrt( one - X[0] * X[0] );
			Z[0] = asin( X[0] );
			break;
			// -------------------------------------------------

			case AtanOp:
			// variables: atan(x),  1 + x * x 
			n_ind = 1;
			n_var = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + ind[0] * J;
			W[0] = one + X[0] * X[0];
			Z[0] = atan( X[0] );
			break;
			// -------------------------------------------------

			case CExpOp:
			n_var = 1;
			n_ind = 6;
			CPPAD_ASSERT_UNKNOWN( ind[1] != 0 );
			if( ind[1] & 1 )
				left = Taylor + ind[2] * J;
			else	left = CPPAD_GET_PAR(ind[2]);
			if( ind[1] & 2 )
				right = Taylor + ind[3] * J;
			else	right = CPPAD_GET_PAR(ind[3]);
			// d == 0
			{	if( ind[1] & 4 )
					trueCase = Taylor + ind[4] * J;
				else	trueCase = CPPAD_GET_PAR(ind[4]);
				if( ind[1] & 8 )
					falseCase = Taylor + ind[5] * J;
				else	falseCase = CPPAD_GET_PAR(ind[5]);
			}
			Z[0] = CondExpOp(
				CompareOp( ind[0] ),
				*left,
				*right,
				*trueCase,
				*falseCase
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			n_var = 0;
			n_ind = 4;
			CPPAD_ASSERT_UNKNOWN( ind[1] > 1 );
			// d == 0
			{	if( ind[1] & 1 )
					result = true;
				else	result = false;
				if( ind[1] & 2 )
					left = Taylor + ind[2] * J;
				else	left = CPPAD_GET_PAR(ind[2]);
				if( ind[1] & 4 )
					right = Taylor + ind[3] * J;
				else	right = CPPAD_GET_PAR(ind[3]);
				switch( CompareOp( ind[0] ) )
				{	case CompareLt:
					compareCount += ( result != 
					LessThanZero(*left - *right) );
					break;

					case CompareLe:
					compareCount += ( result !=
					LessThanOrZero(*left - *right) );
					break;

					case CompareEq:
					compareCount += ( result != 
					(*left == *right) );
					break;

					case CompareGe:
					compareCount += ( result !=
					GreaterThanOrZero(*left - *right) );
					break;

					case CompareGt:
					compareCount += ( result != 
					GreaterThanZero(*left - *right) );
					break;

					case CompareNe:
					compareCount += ( result != 
					(*left != *right) );
					break;

					default:
					CPPAD_ASSERT_UNKNOWN(0);
				}
			}
			break;
			// ---------------------------------------------------

			case CosOp:
			// variables: cos(x), sin(x)
			n_ind = 1;
			n_var = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + ind[0] * J;
			W[0] = sin( X[0] );
			Z[0] = cos( X[0] );
			break;
			// ---------------------------------------------------

			case CoshOp:
			// variables: cosh(x), sinh(x)
			n_ind = 1;
			n_var = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + ind[0] * J;
			W[0] = sinh( X[0] );
			Z[0] = cosh( X[0] );
			break;
			// -------------------------------------------------

			case DisOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			// d == 0
			{	X   = Taylor + ind[0] * J;
				Z[0] = ADDiscrete<Base>::Eval(ind[1], X[0]);
			}
			break;
			// -------------------------------------------------

			case DivvvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			Z[0] = X[0] / Y[0];
			break;
			// -------------------------------------------------

			case DivpvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y = Taylor + ind[1] * J;
			P = CPPAD_GET_PAR(ind[0]);
			Z[0] = P[0] / Y[0];
			break;
			// -------------------------------------------------

			case DivvpOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			P = CPPAD_GET_PAR(ind[1]);
			X = Taylor + ind[0] * J;
			Z[0] = X[0] / P[0];
			break;
			// -------------------------------------------------

			case ExpOp:
			n_var = 1;
			n_ind = 1;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			Z[0] = exp( X[0] );
			break;
			// -------------------------------------------------

			case InvOp:
			n_var = 1;
			n_ind = 0;
			break;
			// -------------------------------------------------

			case LdpOp:
			n_var = 1;
			n_ind = 3;
			
			CPPAD_ASSERT_UNKNOWN( ind[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );

			// d == 0
			{	i   = ind[1];
				CPPAD_ASSERT_UNKNOWN( 
					i < VectorInd[ind[0] - 1] 
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + ind[0] < Rec->NumVecInd() 
				);

				if( VectorVar[ i + ind[0] ] )
				{	i     = VectorInd[ i + ind[0] ];
					i_ind = ind - ind_0;
					Rec->ReplaceInd(i_ind + 2, i);
					CPPAD_ASSERT_UNKNOWN(i > 0 );
					CPPAD_ASSERT_UNKNOWN( i < i_var );
					Y     = Taylor + i * J;
					Z[0]  = Y[0];
				}
				else
				{	i     = VectorInd[ i + ind[0] ];
					i_ind = ind - ind_0;
					Rec->ReplaceInd(i_ind + 2, 0);
					Z[0] = *(CPPAD_GET_PAR(i));
					i    = 0;
				}
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			n_var = 1;
			n_ind = 3;
			
			CPPAD_ASSERT_UNKNOWN( ind[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );

			// d == 0
			{
				X   = Taylor + ind[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ ind[0] - 1 ];
				CPPAD_ASSERT_KNOWN( 
					i < len,
					"VecAD index value >= vector length"
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + ind[0] < Rec->NumVecInd() 
				);

				if( VectorVar[ i + ind[0] ] )
				{	i     = VectorInd[ i + ind[0] ];
					i_ind = ind - ind_0;
					Rec->ReplaceInd(i_ind + 2, i);
					CPPAD_ASSERT_UNKNOWN(i > 0 );
					CPPAD_ASSERT_UNKNOWN( i < i_var );
					Y     = Taylor + i * J;
					Z[0]  = Y[0];
				}
				else
				{	i     = VectorInd[ i + ind[0] ];
					i_ind = ind - ind_0;
					Rec->ReplaceInd(i_ind + 2, 0);
					Z[0] = *(CPPAD_GET_PAR(i));
					i    = 0;
				}
			}
			break;
			// -------------------------------------------------

			case LogOp:
			n_var = 1;
			n_ind = 1;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			Z[0] = log( X[0] );
			break;
			// -------------------------------------------------

			case MulvvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			Z[0] = X[0] * Y[0];
			break;
			// -------------------------------------------------

			case MulpvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y = Taylor + ind[1] * J;
			P = CPPAD_GET_PAR(ind[0]);
			Z[0] = P[0] * Y[0];
			break;
			// -------------------------------------------------

			case MulvpOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			P = CPPAD_GET_PAR(ind[1]);
			Z[0] = X[0] * P[0];
			break;
			// -------------------------------------------------

			case NonOp:
			n_var = 1;
			n_ind = 0;
			break;
			// -------------------------------------------------

			case ParOp:
			n_var = 1;
			n_ind = 1;

			P = CPPAD_GET_PAR(ind[0]);
			Z[0] = *P;
			break;
			// -------------------------------------------------

			case PowvpOp:
			// variables: pow(x, y), log(x), y * log(x)
			n_var = 3;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var);
			U = Z + J;
			W = U + J;

			// u = log(x)
			X = Taylor + ind[0] * J;
			U[0] = log( X[0] );

			// w = u * y
			Y = CPPAD_GET_PAR(ind[1]);
			W[0] = U[0] * Y[0];

			// z = exp(w)
			// zero order case exactly same as Base type operation
			// d == 0
			Z[0] = pow(X[0], Y[0]);

			break;
			// -------------------------------------------------

			case PowpvOp:
			// variables: pow(x, y), log(x), y * log(x)
			n_var = 3;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var);
			U = Z + J;
			W = U + J;

			// u = log(x)
			X = CPPAD_GET_PAR(ind[0]);
			U[0] = log(X[0]);

			// w = u * y
			Y = Taylor + ind[1] * J;
			W[0] = U[0] * Y[0];

			// z = exp(w)
			// zero order case exactly same as Base type operation
			Z[0] = pow(X[0], Y[0]);

			break;
			// -------------------------------------------------

			case PowvvOp:
			// variables: pow(x, y), log(x), y * log(x)
			n_var = 3;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var);
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var);
			U = Z + J;
			W = U + J;

			// u = log(x)
			X = Taylor + ind[0] * J;
			U[0] = log( X[0] );

			// w = u * y
			Y = Taylor + ind[1] * J;
			W[0] = U[0] * Y[0];

			// z = exp(w)
			// zero order case exactly same as Base type operation
			Z[0] = pow(X[0], Y[0]);

			break;
			// -------------------------------------------------

			case PripOp:
			n_var = 0;
			n_ind = 2;
			if( print )
			{	CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumTxt() );
				std::cout << Rec->GetTxt(ind[0]);
				std::cout << *(CPPAD_GET_PAR(ind[1]));
			}
			break;
			// -------------------------------------------------

			case PrivOp:
			n_var = 0;
			n_ind = 2;
			if( print )
			{	CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumTxt() );
				CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

				X      = Taylor + ind[1] * J;
				std::cout << Rec->GetTxt(ind[0]);
				std::cout << X[0];
			}
			break;
			// -------------------------------------------------

			case SinOp:
			// variables: sin(x), cos(x)
			n_ind = 1;
			n_var = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in second variable
			W = Taylor + (i_var+1) * J;
			X = Taylor + ind[0] * J;
			W[0] = cos( X[0] );
			Z[0] = sin( X[0] );
			break;
			// -------------------------------------------------

			case SinhOp:
			// variables: sinh(x), cosh(x)
			n_ind = 1;
			n_var = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in second variable
			W = Taylor + (i_var+1) * J;
			X = Taylor + ind[0] * J;
			W[0] = cosh( X[0] );
			Z[0] = sinh( X[0] );
			break;
			// -------------------------------------------------

			case SqrtOp:
			n_var = 1;
			n_ind = 1;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			Z[0] = sqrt( X[0] );
			break;
			// -------------------------------------------------

			case StppOp:
			n_var = 0;
			n_ind = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );

				i   = ind[1];
				CPPAD_ASSERT_UNKNOWN(i < VectorInd[ind[0] - 1]);
				CPPAD_ASSERT_UNKNOWN( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = ind[2];
				VectorVar[ i + ind[0] ] = false;

				Z[0] = *( CPPAD_GET_PAR(ind[2]) );
			}
			break;
			// -------------------------------------------------

			case StpvOp:
			n_var = 0;
			n_ind = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );
				CPPAD_ASSERT_UNKNOWN( ind[2] < i_var );

				i   = ind[1];
				CPPAD_ASSERT_UNKNOWN(i < VectorInd[ind[0] - 1]);
				CPPAD_ASSERT_UNKNOWN( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = ind[2];
				VectorVar[ i + ind[0] ] = true;
			}
			break;
			// -------------------------------------------------

			case StvpOp:
			n_var = 0;
			n_ind = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );
				CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

				X   = Taylor + ind[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ ind[0] - 1 ];
				CPPAD_ASSERT_KNOWN( 
					i < len,
					"VecAD index value >= vector length"
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = ind[2];
				VectorVar[ i + ind[0] ] = false;

				Z[0] = *( CPPAD_GET_PAR(ind[2]) );
			}
			break;
			// -------------------------------------------------

			case StvvOp:
			n_var = 0;
			n_ind = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( ind[0] < Rec->NumVecInd() );
				CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );
				CPPAD_ASSERT_UNKNOWN( ind[2] < i_var );

				X   = Taylor + ind[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ ind[0] - 1 ];
				CPPAD_ASSERT_KNOWN( 
					i < len,
					"VecAD index value >= vector length"
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + ind[0] < Rec->NumVecInd() 
				);
				VectorInd[ i + ind[0] ] = ind[2];
				VectorVar[ i + ind[0] ] = true;
			}
			break;
			// -------------------------------------------------

			case SubvvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X = Taylor + ind[0] * J;
			Y = Taylor + ind[1] * J;
			Z[0] = X[0] - Y[0];
			break;
			// -------------------------------------------------

			case SubpvOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			P = CPPAD_GET_PAR(ind[0]);
			Y = Taylor + ind[1] * J;
			Z[0] = P[0] - Y[0];
			break;
			// -------------------------------------------------

			case SubvpOp:
			n_var = 1;
			n_ind = 2;
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X = Taylor + ind[0] * J;
			P = CPPAD_GET_PAR(ind[1]);
			Z[0] = X[0] - P[0];
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
		// check not past last index value
		CPPAD_ASSERT_UNKNOWN( ind + n_ind <= ind_0 + Rec->NumInd() );

# if CPPAD_FORWARD0SWEEP_TRACE
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			ind,
			d + 1, 
			Z, 
			0, 
			(Base *) CPPAD_NULL
		);
	}
	std::cout << std::endl;
# else
	}
# endif
	// check last n_var and n_ind
	CPPAD_ASSERT_UNKNOWN( n_var == NumVar(op) );
	CPPAD_ASSERT_UNKNOWN( n_ind == NumInd(op) );

	CPPAD_ASSERT_UNKNOWN( (i_var + n_var) == Rec->TotNumVar() );
	if( VectorInd != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorInd);
	if( VectorVar != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorVar);

	return compareCount;
}

} // END CppAD namespace

# undef CPPAD_FORWARD0SWEEP_TRACE

# endif
