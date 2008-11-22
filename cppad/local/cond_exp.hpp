# ifndef CPPAD_COND_EXP_INCLUDED
# define CPPAD_COND_EXP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin CondExp$$
$spell
	Atan2
	CondExp
	Taylor
	std
	Cpp
	namespace
	inline
	const
	abs
	Op
	bool
	Lt
	Le
	Eq
	Ge
	Gt
$$

$index conditional, expression$$
$index expression, conditional$$
$index assign, conditional$$

$section AD Conditional Expressions$$

$head Syntax$$
$syntax%%result% = CondExp%Op%(%left%, %right%, %trueCase%, %falseCase%)%$$


$head Purpose$$
Record, 
as part of an AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$,
a the conditional result of the form 
$syntax%
	if( %left% %op% %right% )
		%result% = %trueCase%
	else	%result% = %falseCase%
%$$
The notation $italic Op$$ and $italic op$$ 
above have the following correspondence: 
$table
$italic Op$$ 
	$pre  $$ $cnext $code Lt$$
	$pre  $$ $cnext $code Le$$
	$pre  $$ $cnext $code Eq$$
	$pre  $$ $cnext $code Ge$$
	$pre  $$ $cnext $code Gt$$
$rnext
$italic op$$ 
	$cnext $code <$$
	$cnext $code <=$$
	$cnext $code ==$$
	$cnext $code >=$$
	$cnext $code >$$
$tend
If $italic f$$ is the $xref/ADFun/$$ object corresponding to the
AD operation sequence,
the choice in an AD conditional expression is made each time
$xref/Forward//f.Forward/$$ is used to evaluate the zero order Taylor
coefficients with new values for the 
$cref/independent variables/glossary/Tape/Independent Variable/$$.
This is in contrast to the $xref/Compare//AD comparison operators/$$
which are boolean valued and not included in the AD operation sequence. 

$head Op$$
In the syntax above, $italic Op$$ represents one of the following
two characters: $code Lt$$, $code Le$$, $code Eq$$, $code Ge$$, $code Gt$$. 
As in the table above,
$italic Op$$ determines the comparison operator $italic op$$.

$head Type$$
These functions are defined in the CppAD namespace for arguments of
$italic Type$$ is $code float$$ , $code double$$, or any type of the form
$syntax%AD<%Base%>%$$.
(Note that all four arguments must have the same type.)
 
$head left$$
The argument $italic left$$ has prototype
$syntax%
	const %Type% &%left%
%$$
It specifies the value for the left side of the comparison operator.
 
$head right$$
The argument $italic right$$ has prototype
$syntax%
	const %Type% &%right%
%$$
It specifies the value for the right side of the comparison operator.

$head trueCase$$
The argument $italic trueCase$$ has prototype
$syntax%
	const %Type% &%trueCase%
%$$
It specifies the return value if the result of the comparison is true.

$head falseCase$$
The argument $italic falseCase$$ has prototype
$syntax%
	const %Type% &%falseCase%
%$$
It specifies the return value if the result of the comparison is false.

$head result$$
The $italic result$$ has prototype
$syntax%
	%Type% &%falseCase%
%$$


$head CondExp$$
Previous versions of CppAD used 
$syntax%
	CondExp(%flag%, %trueCase%, %falseCase%)
%$$
for the same meaning as 
$syntax%
	CondExpGt(%flag%, %Type%(0), %trueCase%, %falseCase%)
%$$
Use of $code CondExp$$ is deprecated, but continues to be supported.

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.


$head Example$$

$head Test$$
$children%
	example/cond_exp.cpp
%$$
The file
$xref/CondExp.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$head Atan2$$
The following implementation of the
AD $xref/atan2/$$ function is a more complex
example of using conditional expressions:
$code
$verbatim%cppad/local/atan2.hpp%0%BEGIN CondExp%// END CondExp%$$
$$


$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// ------------ CondExpOp(cop, left, right, trueCase, falseCase) --------------
// CompareType and ResultType are different for the forward and reverse
// sparese calculations.
template <class CompareType, class ResultType>
CPPAD_INLINE ResultType CondExpTemplate( 
	enum  CompareOp            cop ,
	const CompareType        &left ,
	const CompareType       &right , 
	const ResultType     &trueCase , 
	const ResultType    &falseCase )
{	ResultType returnValue;
	switch( cop )
	{
		case CompareLt:
		if( left < right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareLe:
		if( left <= right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareEq:
		if( left == right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareGe:
		if( left >= right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareGt:
		if( left > right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		default:
		CPPAD_ASSERT_UNKNOWN(0);
		returnValue = trueCase;
	}
	return returnValue;
}

inline float CondExpOp( 
	enum CompareOp     cop ,
	const float      &left ,
	const float     &right , 
	const float  &trueCase , 
	const float &falseCase )
{	return CondExpTemplate(cop, left, right, trueCase, falseCase);
}

inline double CondExpOp( 
	enum CompareOp     cop ,
	const double      &left ,
	const double     &right , 
	const double  &trueCase , 
	const double &falseCase )
{	return CondExpTemplate(cop, left, right, trueCase, falseCase);
}

template <class Base>
CPPAD_INLINE AD<Base> CondExpOp(
	enum  CompareOp cop       ,
	const AD<Base> &left      , 
	const AD<Base> &right     , 
	const AD<Base> &trueCase  , 
	const AD<Base> &falseCase )
{
	AD<Base> returnValue;
	CPPAD_ASSERT_UNKNOWN( Parameter(returnValue) );

	// check first case where do not need to tape
	if( IdenticalPar(left) & IdenticalPar(right) )
	{	switch( cop )
		{
			case CompareLt:
			if( left.value_ < right.value_ )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareLe:
			if( left.value_ <= right.value_ )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareEq:
			if( left.value_ == right.value_ )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareGe:
			if( left.value_ >= right.value_ )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareGt:
			if( left.value_ > right.value_ )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			default:
			CPPAD_ASSERT_UNKNOWN(0);
			returnValue = trueCase;
		}
		return returnValue;
	}

	// must use CondExp incase Base is an AD type and recording
	returnValue.value_ = CondExpOp(cop, 
		left.value_, right.value_, trueCase.value_, falseCase.value_);

	ADTape<Base> *tape = CPPAD_NULL;
	if( Variable(left) )
		tape = left.tape_this();
	if( Variable(right) )
		tape = right.tape_this();
	if( Variable(trueCase) )
		tape = trueCase.tape_this();
	if( Variable(falseCase) )
		tape = falseCase.tape_this();

	// add this operation to the tape
	if( tape != CPPAD_NULL ) 
		tape->RecordCondExp(cop, 
			returnValue, left, right, trueCase, falseCase);

	return returnValue;
}

// --- RecordCondExp(cop, returnValue, left, right, trueCase, falseCase) -----

template <class Base>
void ADTape<Base>::RecordCondExp(
	enum CompareOp  cop         ,
	AD<Base>       &returnValue ,
	const AD<Base> &left        ,
	const AD<Base> &right       ,
	const AD<Base> &trueCase    ,
	const AD<Base> &falseCase   )
{	size_t   ind0, ind1, ind2, ind3, ind4, ind5;
	size_t   returnValue_taddr;

	// taddr_ of this variable
	CPPAD_ASSERT_UNKNOWN( NumVar(CExpOp) == 1 );
	returnValue_taddr = Rec_.PutOp(CExpOp);

	// ind[0] = cop
	ind0 = size_t( cop );

	// ind[1] = base 2 representaion of the value
	// [Var(left), Var(right), Var(trueCase), Var(falseCase)]
	ind1 = 0;

	// Make sure returnValue is in the list of variables and set its taddr
	if( Parameter(returnValue) )
		returnValue.make_variable(id_, returnValue_taddr );
	else	returnValue.taddr_ = returnValue_taddr;

	// ind[2] = left address
	if( Parameter(left) )
		ind2 = Rec_.PutPar(left.value_);
	else
	{	ind1 += 1;
		ind2 = left.taddr_;	
	}

	// ind[3] = right address
	if( Parameter(right) )
		ind3 = Rec_.PutPar(right.value_);
	else
	{	ind1 += 2;
		ind3 = right.taddr_;	
	}

	// ind[4] = trueCase address
	if( Parameter(trueCase) )
		ind4 = Rec_.PutPar(trueCase.value_);
	else
	{	ind1 += 4;
		ind4 = trueCase.taddr_;	
	}

	// ind[5] =  falseCase address
	if( Parameter(falseCase) )
		ind5 = Rec_.PutPar(falseCase.value_);
	else
	{	ind1 += 8;
		ind5 = falseCase.taddr_;	
	}

	CPPAD_ASSERT_UNKNOWN( NumInd(CExpOp) == 6 );
	CPPAD_ASSERT_UNKNOWN( ind1 > 0 );
	Rec_.PutInd(ind0, ind1, ind2, ind3, ind4, ind5);

	// check that returnValue is a dependent variable
	CPPAD_ASSERT_UNKNOWN( Variable(returnValue) );
}

// ------------ CondExpOp(left, right, trueCase, falseCase) ----------------

# define CPPAD_COND_EXP(Name)                                              \
	template <class Base>                                              \
	CPPAD_INLINE AD<Base> CondExp##Name(                                     \
		const AD<Base> &left      ,                                \
		const AD<Base> &right     ,                                \
		const AD<Base> &trueCase  ,                                \
		const AD<Base> &falseCase )                                \
	{                                                                  \
		return CondExpOp(Compare##Name,                            \
			left, right, trueCase, falseCase);                 \
	}

// AD<Base>
CPPAD_COND_EXP(Lt)
CPPAD_COND_EXP(Le)
CPPAD_COND_EXP(Eq)
CPPAD_COND_EXP(Ge)
CPPAD_COND_EXP(Gt)
template <class Base>
CPPAD_INLINE AD<Base> CondExp(
	const AD<Base> &flag      , 
	const AD<Base> &trueCase  ,
	const AD<Base> &falseCase )
{	
	return CondExpOp(CompareGt, flag, AD<Base>(0), trueCase, falseCase);
}

# undef CPPAD_COND_EXP
# define CPPAD_COND_EXP(Name, Op, Type)                             \
	inline Type CondExp##Name(                                  \
		const Type &left      ,                             \
		const Type &right     ,                             \
		const Type &trueCase  ,                             \
		const Type &falseCase )                             \
	{	Type returnValue;                                   \
		if( left Op right )                                 \
			returnValue = trueCase;                     \
		else	returnValue = falseCase;                    \
		return returnValue;                                 \
	}

// float
CPPAD_COND_EXP(Lt,  <, float)
CPPAD_COND_EXP(Le, <=, float)
CPPAD_COND_EXP(Eq, ==, float)
CPPAD_COND_EXP(Ge, >=, float)
CPPAD_COND_EXP(Gt,  >, float)
inline float CondExp(
	const float &flag      , 
	const float &trueCase  ,
	const float &falseCase )
{	
	return CondExpGt(flag, float(0), trueCase, falseCase);
}

// double
CPPAD_COND_EXP(Lt,  <, double)
CPPAD_COND_EXP(Le, <=, double)
CPPAD_COND_EXP(Eq, ==, double)
CPPAD_COND_EXP(Ge, >=, double)
CPPAD_COND_EXP(Gt,  >, double)
inline double CondExp(
	const double &flag      , 
	const double &trueCase  ,
	const double &falseCase )
{	
	return CondExpGt(flag, 0., trueCase, falseCase);
}

# undef CPPAD_COND_EXP

} // END CppAD namespace

# endif 
