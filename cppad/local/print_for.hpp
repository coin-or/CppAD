# ifndef CPPAD_PRINT_FOR_INCLUDED
# define CPPAD_PRINT_FOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

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

$section Printing AD Values During Forward Mode$$ 

$head Syntax$$
$syntax%PrintFor(%text%, %y%)%$$
$pre
$$
$syntax%%f%.Forward(0, %x%)%$$


$head Purpose$$
The current value of an $syntax%AD<%Base%>%$$ 
object $italic y$$ is the result of an AD of $italic Base$$ operation.
This operation may be part of the 
$xref/glossary/Operation/Sequence/operation sequence/1/$$
that is transferred to an $xref/ADFun/$$ object $italic f$$.
The $code ADFun$$ object can be evaluated at different values for the
$cref/independent variables/glossary/Tape/Independent Variable/$$.
This may result in a corresponding value for $italic y$$ 
that is different from when the operation sequence was recorded.
The routine $code PrintFor$$ requests a printing,
when $syntax%%f%.Forward(0, %x%)%$$ is executed,
of the value for $italic y$$ that corresponds to the 
independent variable values specified by $italic x$$.

$head text$$
The argument $italic text$$ has prototype
$syntax%
	const char *%text%
%$$
The corresponding text is written to $code std::cout$$ before the 
value of $italic y$$. 

$head y$$
The argument $italic y$$ has one of the following prototypes
$syntax%
	const AD<%Base%>               &%y%
	const VecAD<%Base%>::reference &%y%
%$$
The value of $italic y$$ that corresponds to $italic x$$
is written to $code std::cout$$ during the execution of 
$syntax%
	%f%.Forward(0, %x%)
%$$

$head f.Forward(0, x)$$
The objects $italic f$$, $italic x$$, and the purpose
for this operation, are documented in $xref/Forward/$$.


$head Discussion$$
This is can be helpful for understanding why tape evaluations
have trouble, for example, if the result of a tape calculation
is the IEEE code for not a number $code Nan$$.

$head Alternative$$
The $xref/Output/$$ section describes the normal 
printing of values; i.e., printing when the corresponding
code is executed.

$head Example$$
$children%
	print_for/print_for.cpp
%$$
The program
$xref/PrintFor.cpp/$$
is an example and test of this operation.
The output of this program
states the conditions for passing and failing the test.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { 
	template <class Base>
	void PrintFor(const char *text, const AD<Base> &u)
	{	ADTape<Base> *tape = AD<Base>::tape_ptr();
		CPPAD_ASSERT_KNOWN(
			tape != CPPAD_NULL,
			"PrintFor: cannot use this function because no tape"
			"\nis currently active (for this thread)."
		);

		if( Parameter(u) )
		{	CPPAD_ASSERT_UNKNOWN( NumVar(PripOp) == 0 );
			CPPAD_ASSERT_UNKNOWN( NumInd(PripOp) == 2 );
			// put operand addresses in tape
			size_t t = tape->Rec_.PutTxt(text);
			size_t p = tape->Rec_.PutPar(u.value_);
			tape->Rec_.PutInd(t, p);
			// put operator in the tape
			tape->Rec_.PutOp(PripOp);
		}
		else
		{	CPPAD_ASSERT_UNKNOWN( NumVar(PrivOp) == 0 );
			CPPAD_ASSERT_UNKNOWN( NumInd(PrivOp) == 2 );
			// put operand addresses in tape
			size_t t = tape->Rec_.PutTxt(text);
			tape->Rec_.PutInd(t, u.taddr_);
			// put operator in the tape
			tape->Rec_.PutOp(PrivOp);
		}
	}
	template <class Base>
	void PrintFor(const char *text, const VecAD_reference<Base> &u)
	{	PrintFor(text, u.ADBase()); }
}

# endif
