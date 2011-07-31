/* $Id$ */
# ifndef CPPAD_PRINT_FOR_INCLUDED
# define CPPAD_PRINT_FOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin PrintFor$$
$spell
	VecAD
	std
	cout
	const
$$

$index print, forward mode$$
$index forward, mode print$$
$index text, output$$
$index output, text$$
$index debug, forward mode$$
$index forward, debug$$

$section Printing AD Values During Forward Mode$$ 

$head Syntax$$
$icode%f%.Forward(0, %x%)
%$$
$codei%PrintFor(%text%, %y%)
%$$
$codei%PrintFor(%text%, %y%, %z%)
%$$

$head Purpose$$
The $cref/zero order forward/ForwardZero/$$ syntax
$icode%
	f%.Forward(0, %x%)
%$$
results in a value for all of the variables in the 
$xref/glossary/Operation/Sequence/operation sequence/1/$$ corresponding
to $icode f$$.
There is a value for the $codei%AD<%Base%>%$$ object $icode y$$,
that corresponds to the value $icode x$$ for the  
$cref/independent variables/glossary/Tape/Independent Variable/$$.
Putting a $code PrintFor$$ in the operation sequence will
cause the value of $icode y$$, corresponding to $icode x$$,
to be printed during each zero order forward operation.

$head f.Forward(0, x)$$
The objects $icode f$$, $italic x$$, and the purpose
for this operation, are documented in $cref/Forward/$$.

$head text$$
The argument $icode text$$ has prototype
$codei%
	const char *%text%
%$$
The corresponding text is written to $code std::cout$$ before the 
value of $icode y$$. 

$head y$$
The argument $icode y$$ has one of the following prototypes
$codei%
	const AD<%Base%>&               %y%
	const VecAD<%Base%>::reference& %y%
%$$
The value of $icode y$$ that corresponds to $italic x$$
is written to $code std::cout$$ during the execution of 
$codei%
	%f%.Forward(0, %x%)
%$$

$head z$$
If present, the argument $icode z$$ has one of the following prototypes
$codei%
	const AD<%Base%>&               %z%
	const VecAD<%Base%>::reference& %z%
%$$
In this case
the text and value of $icode y$$ will be printed if and only if
$codei%
	%z% <= 0.
%$$.
	
$head Discussion$$
This is can be helpful for understanding why tape evaluations
have trouble, for example, if the result of a tape calculation
is the IEEE code for not a number $code Nan$$.

$head Alternative$$
The $cref/Output/$$ section describes the normal 
printing of values; i.e., printing when the corresponding
code is executed.

$head Example$$
$children%
	print_for/print_for.cpp
%$$
The program
$cref/print_for.cpp/$$
is an example and test of this operation.
The output of this program
states the conditions for passing and failing the test.

$end
------------------------------------------------------------------------------
*/

# include <cstring>

namespace CppAD { 
	template <class Base>
	void PrintFor(const char *text, const AD<Base>& y, const AD<Base>& z)
	{	CPPAD_ASSERT_NARG_NRES(PriOp, 4, 0);

		ADTape<Base> *tape = AD<Base>::tape_ptr();
		CPPAD_ASSERT_KNOWN(
			tape != CPPAD_NULL,
			"PrintFor: cannot use this function because no ADFun object"
			"\nis currently being recorded (for this thread)."
		);
		CPPAD_ASSERT_KNOWN(
			std::strlen(text) <= 1000 ,
			"PrintFor: lenght of text is greater than 1000 characters"
		);
		size_t ind0, ind1, ind2, ind3;
	
		// ind[0] = base 2 preprsentation of the value [Var(y), Var(z)]
		ind0 = 0;

		// ind[0] = address of text
		ind1 = tape->Rec_.PutTxt(text);

		// ind[2] = address for y
		if( Parameter(y) )
			ind2  = tape->Rec_.PutPar(y.value_);
		else
		{	ind0 += 1;
			ind2  = y.taddr_;
		}

		// ind[3] = address for z
		if( Parameter(z) )
			ind3  = tape->Rec_.PutPar(z.value_);
		else
		{	ind0 += 2;
			ind3  = z.taddr_;
		}

		// put the operator in the tape
		tape->Rec_.PutArg(ind0, ind1, ind2, ind3);
		tape->Rec_.PutOp(PriOp);
	}
	// Fold all other cases into the case above
	template <class Base>
	void PrintFor(const char *text, const AD<Base>& y)
	{	PrintFor(text, y, AD<Base>(0) ); }
	//
	template <class Base>
	void PrintFor(const char *text, const VecAD_reference<Base>& y)
	{	PrintFor(text, y.ADBase(), AD<Base>(0) ); }
	//
	template <class Base>
	void PrintFor(
		const char                  *text   , 
		const VecAD_reference<Base>& y      ,
		const VecAD_reference<Base>& z      )
	{	PrintFor(text, y.ADBase(), z.ADBase() ); }
	//
	template <class Base>
	void PrintFor(
		const char *text                    , 
		const VecAD_reference<Base>& y      ,
		const AD<Base>&              z      )
	{	PrintFor(text, y.ADBase(), z ); }
	//
	template <class Base>
	void PrintFor(
		const char                  *text   , 
		const AD<Base>&              y      ,
		const VecAD_reference<Base>& z      )
	{	PrintFor(text, y, z.ADBase() ); }
}

# endif
