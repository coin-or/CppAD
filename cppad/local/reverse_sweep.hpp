# ifndef CPPAD_REVERSE_SWEEP_INCLUDED
# define CPPAD_REVERSE_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ReverseSweep$$ $comment CppAD Developer Documentation$$
$spell
	Var
	numvar
	Num
	Len
	Ind
	const
	Taylor
	CppAD
	zs
	op
	Ind
$$

$section Reverse Mode Computation of Derivatives of Taylor Coefficients$$
$mindex ReverseSweep derivative Taylor coefficient$$

$head Syntax$$
$syntax%void ReverseSweep(
	size_t %d%,
	size_t %numvar%,
	const player<%Base%> *%Rec%,
	size_t %J%,
	const Base *%Taylor%,
	size_t %K%,
	Base *%Partial%
)%$$



$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^m \rightarrow B^n
\] $$

$head G$$
Given the vector $latex v \in B^n$$ we define the function 
$latex G : B^{m \times d} \rightarrow B$$ by
$latex \[
	G(u) = \frac{1}{d !} 
		\sum_{i=1}^n v_i \left[
		\frac{ \partial^p}{ \partial t^d}
		F_i [ u^{(0)} + u^{(1)} t + \cdots + u^{(d)} t^d ]
	\right]_{t=0}
\] $$
Note that the scale factor of $latex 1 / d !$$ converts
$th d$$ order derivatives to $th d$$ order Taylor coefficients.
The routine $code ReverseSweep$$ computes all the first order partial
derivatives of $latex G$$ with respect to each of the Taylor coefficients
for the independent variables 
$latex u^{(j)}$$ for $latex j = 0 , \ldots , d$$.


$head numvar$$
is the number of rows in the matrices $italic Taylor$$ and $italic Partial$$.
It must also be equal to $syntax%%Rec%->TotNumVar()%$$.

$head J$$
Is the number of columns in the coefficient matrix $italic Taylor$$.
This must be greater than or equal $latex d+1$$.

$head K$$
Is the number of columns in the coefficient matrix $italic Partial$$.
This must be greater than or equal $latex d+1$$.

$head Taylor$$
For $latex i = 1 , \ldots , numvar - 1$$ and $latex j <= d$$
$syntax%%Taylor%[%i% * J + j]%$$ contains the 
$th j$$ order Taylor coefficients
for the variable corresponding to index $italic i$$.

$head On Input$$

$subhead Dependent Variables$$
The vector $italic v$$ is stored in the 
$th d$$ order components of the last $italic n$$ 
variables in $italic Partial$$; i.e.,
for $latex i = numvar-n , \ldots , numvar-1$$
and for $latex j < d$$,
$syntax%
	%Partial%[%i% * %K% + %j%] == 0
	%Partial%[%i% * %K% + %d%] == %v%[%numvar% - %i%]
%$$

$subhead Other Variables$$
The other variable records come before and have initial partials of zero; 
i.e., for $latex i = 1, \ldots , numvar-n-1$$ and 
$latex j \leq d$$,
$syntax%
	%Partial%[%i% * %K% + %j%] ==  0
%$$

$head On Output$$

$subhead Independent Variables$$
For $latex i = 1, \ldots , m$$, and for $latex j \leq d$$,
$syntax%%Partial%[%i% * %K% + %j%]%$$ 
is the partial of $latex G$$ with respect to $latex u_i^{(j)}$$.

$subhead Other Variables$$
For $latex i = m+1, \ldots , numvar-1$$ and $latex j \leq d$$,
$syntax%%Partial%[%i% * %K% + %j%]%$$ 
is temporary work space and its
output value is not specified.


$end
------------------------------------------------------------------------------
*/

# define CPPAD_REVERSE_SWEEP_TRACE 0

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
void ReverseSweep(
	size_t                d,
	size_t                numvar,
	const player<Base>   *Rec,
	size_t                J,
	const Base           *Taylor,
	size_t                K,
	Base                 *Partial
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        i_ind;
	size_t        n_var;
	size_t        n_ind;

	const size_t   *ind = 0;
	const Base       *P = 0;
	const Base       *Z = 0;
	const Base       *Y = 0;
	const Base       *X = 0;
	const Base       *W = 0;
	const Base       *U = 0;

	Base            *pZ = 0;
	Base            *pY = 0;
	Base            *pX = 0;
	Base            *pW = 0;
	Base            *pU = 0;

	// used by CExp operator 
	Base        *trueCase  = 0;
	Base        *falseCase = 0;
	const Base  *left      = 0;
	const Base  *right     = 0;
	const Base   zero = Base(0);

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

		// value of Z and its partials for this op
		Z   = Taylor + i_var * J;
		pZ  = Partial + i_var * K;

		// rest of informaiton depends on the case
# if CPPAD_REVERSE_SWEEP_TRACE
		n_ind = NumInd(op);
		printOp(
			std::cout, 
			Rec,
			i_var,
			op, 
			ind,
			d + 1, 
			Z, 
			d + 1, 
			pZ 
		);
# endif

		switch( op )
		{

			case AbsOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			RevAbsOp(d, Z, X, pZ, pX);
			break;
			// --------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			pX = Partial + ind[0] * K;
			pY = Partial + ind[1] * K;
			RevAddvvOp(d, pZ, pX, pY);
			break;
			// --------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			pY = Partial + ind[1] * K;
			RevAddpvOp(d, pZ, pY);
			break;
			// --------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			pX = Partial + ind[0] * K;
			RevAddvpOp(d, pZ, pX);
			break;
			// --------------------------------------------------

			case AcosOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// acos(x) and sqrt(1 - x * x) are computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );

			// use W for data stored in second record
			W  = Taylor  + (i_var+1) * J;
			pW = Partial + (i_var+1) * K;
			X    = Taylor  + ind[0] * J;
			pX   = Partial + ind[0] * K;
			RevAcosOp(d, Z, W, X, pZ, pW, pX);
			break;
			// --------------------------------------------------

			case AsinOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// atan(x) and 1 + x * x must be computed in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );

			// use W for data stored in second record
			W  = Taylor  + (i_var+1) * J;
			pW = Partial + (i_var+1) * K;
			X    = Taylor  + ind[0] * J;
			pX   = Partial + ind[0] * K;
			RevAsinOp(d, Z, W, X, pZ, pW, pX);
			break;
			// --------------------------------------------------

			case AtanOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// cosine and sine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );

			// use W for data stored in second record
			W  = Taylor  + (i_var+1) * J;
			pW = Partial + (i_var+1) * K;
			X    = Taylor  + ind[0] * J;
			pX   = Partial + ind[0] * K;
			RevAtanOp(d, Z, W, X, pZ, pW, pX);
			break;
			// -------------------------------------------------

			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 6);
			CPPAD_ASSERT_UNKNOWN( ind[1] != 0 );
			if( ind[1] & 1 )
				left = Taylor + ind[2] * J;
			else	left = Rec->GetPar(ind[2]);
			if( ind[1] & 2 )
				right = Taylor + ind[3] * J;
			else	right = Rec->GetPar(ind[3]);
			if( ind[1] & 4 )
			{	trueCase = Partial + ind[4] * K;
				trueCase[d] += CondExpOp(
					CompareOp( ind[0] ),
					*left,
					*right,
					pZ[d],
					zero
				);
			}
			if( ind[1] & 8 )
			{	falseCase = Partial + ind[5] * K;
				falseCase[d] += CondExpOp(
					CompareOp( ind[0] ),
					*left,
					*right,
					zero,
					pZ[d]
				);
			}
			break;
			// --------------------------------------------------

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
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );

			// use W for data stored in second record
			W  = Taylor  + (i_var+1) * J;
			pW = Partial + (i_var+1) * K;
			X    = Taylor  + ind[0] * J;
			pX   = Partial + ind[0] * K;
			RevTrigSinCos(d, W, Z, X, pW, pZ, pX);
			break;
			// --------------------------------------------------

			case CoshOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// cosine and sine must come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );

			// use W for data stored in second record
			W  = Taylor  + (i_var+1) * J;
			pW = Partial + (i_var+1) * K;
			X    = Taylor  + ind[0] * J;
			pX   = Partial + ind[0] * K;
			RevHypSinCos(d, W, Z, X, pW, pZ, pX);
			break;
			// --------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			break;
			// --------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			Y  = Taylor  + ind[1] * J;
			pY = Partial + ind[1] * K;
			RevDivvvOp(d, Z, X, Y, pZ, pX, pY);
			break;
			// --------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y  = Taylor  + ind[1] * J;
			pY = Partial + ind[1] * K;
			P  = Rec->GetPar( ind[0] );
			RevDivpvOp(d, Z, P, Y, pZ, pY);
			break;
			// --------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			P  = Rec->GetPar( ind[1] );
			RevDivvpOp(d, Z, X, P, pZ, pX);
			break;
			// --------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			RevExpOp(d, Z, X, pZ, pX);
			break;
			// --------------------------------------------------
			case LdpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			CPPAD_ASSERT_UNKNOWN( ind[2] < i_var );
			if( ind[2] > 0 )
			{	pY     = Partial + ind[2] * K;
				pY[d] += pZ[d];
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			CPPAD_ASSERT_UNKNOWN( ind[2] < i_var );
			if( ind[2] > 0 )
			{	pY     = Partial + ind[2] * K;
				pY[d] += pZ[d];
			}
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 0 );
			break;
			// --------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			RevLogOp(d, Z, X, pZ, pX);
			break;
			// --------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			Y  = Taylor  + ind[1] * J;
			pY = Partial + ind[1] * K;
			RevMulvvOp(d, Z, X, Y, pZ, pX, pY);
			break;
			// --------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			Y  = Taylor  + ind[1] * J;
			pY = Partial + ind[1] * K;
			P  = Rec->GetPar( ind[0] );
			RevMulpvOp(d, Z, P, Y, pZ, pY);
			break;
			// --------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			P  = Rec->GetPar( ind[1] );
			RevMulvpOp(d, Z, X, P, pZ, pX);
			break;
			// --------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 0 );
			break;
			// --------------------------------------------------

			case ParOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			break;
			// --------------------------------------------------

			case PowvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			U  = Z  + J;
			pU = pZ + K;
			W  = U  + J;
			pW = pU + K;

			// Z = exp(w)
			RevExpOp(d, Z, W, pZ, pW);

			// w = u * y
			Y  = Rec->GetPar( ind[1] );
			RevMulvpOp(d, W, U, Y, pW, pU);

			// u = log(x)
			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			RevLogOp(d, U, X, pU, pX);

			break;
			// -------------------------------------------------

			case PowpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			U  = Z  + J;
			pU = pZ + K;
			W  = U  + J;
			pW = pU + K;

			// Z = exp(w)
			RevExpOp(d, Z, W, pZ, pW);

			// w = u * y
			Y  = Taylor  + ind[1] * J;
			pY = Partial + ind[1] * K;
			RevMulpvOp(d, W, U, Y, pW, pY);

			// u = log(x)
			// x is a parameter

			break;
			// -------------------------------------------------

			case PowvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 3);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			U  = Z  + J;
			pU = pZ + K;
			W  = U  + J;
			pW = pU + K;

			// Z = exp(w)
			RevExpOp(d, Z, W, pZ, pW);

			// w = u * y
			Y  = Taylor  + ind[1] * J;
			pY = Partial + ind[1] * K;
			RevMulvvOp(d, W, U, Y, pW, pU, pY);

			// u = log(x)
			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			RevLogOp(d, U, X, pU, pX);

			break;

			// --------------------------------------------------
			case PripOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( i_ind > n_ind );
			break;
			// --------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( i_ind > n_ind );
			break;

			// -------------------------------------------------

			case SinOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// sine and cosine come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );

			// use W for data stored in cosine slot record
			W  = Taylor  + (i_var+1) * J;
			pW = Partial + (i_var+1) * K;
			X    = Taylor  + ind[0] * J;
			pX   = Partial + ind[0] * K;
			RevTrigSinCos(d, Z, W, X, pZ, pW, pX);
			break;
			// -------------------------------------------------

			case SinhOp:
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			// sine and cosine come in pairs
			CPPAD_ASSERT_UNKNOWN( n_var == 2);
			CPPAD_ASSERT_UNKNOWN( i_var < numvar - 1 );

			// use W for data stored in cosine slot record
			W  = Taylor  + (i_var+1) * J;
			pW = Partial + (i_var+1) * K;
			X    = Taylor  + ind[0] * J;
			pX   = Partial + ind[0] * K;
			RevHypSinCos(d, Z, W, X, pZ, pW, pX);
			break;
			// --------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 1 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			X  = Taylor  + ind[0] * J;
			pX = Partial + ind[0] * K;
			RevSqrtOp(d, Z, X, pZ, pX);
			break;
			// --------------------------------------------------

			case StppOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			break;
			// --------------------------------------------------

			case StpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 0);
			CPPAD_ASSERT_UNKNOWN( n_ind == 3 );
			CPPAD_ASSERT_UNKNOWN( ind[2] < i_var );
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
			CPPAD_ASSERT_UNKNOWN( ind[2] < i_var );
			break;
			// --------------------------------------------------

			case SubvvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			pX = Partial + ind[0] * K;
			pY = Partial + ind[1] * K;
			RevSubvvOp(d, pZ, pX, pY);
			break;
			// --------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[1] < i_var );

			pY = Partial + ind[1] * K;
			RevSubpvOp(d, pZ, pY);
			break;
			// --------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( n_var == 1);
			CPPAD_ASSERT_UNKNOWN( n_ind == 2 );
			CPPAD_ASSERT_UNKNOWN( ind[0] < i_var );

			pX = Partial + ind[0] * K;
			RevSubvpOp(d, pZ, pX);
			break;
			// --------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
# if CPPAD_REVERSE_SWEEP_TRACE
	std::cout << std::endl;
# endif
	CPPAD_ASSERT_UNKNOWN( i_op == 1 );
	CPPAD_ASSERT_UNKNOWN( Rec->GetOp(i_op-1) == NonOp );
	CPPAD_ASSERT_UNKNOWN( i_var == NumVar(NonOp)  );
}

} // END CppAD namespace

# undef CPPAD_REVERSE_SWEEP_TRACE

# endif
