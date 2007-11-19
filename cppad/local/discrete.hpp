# ifndef CPPAD_DISCRETE_INCLUDED
# define CPPAD_DISCRETE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Discrete$$
$spell
	retaping
	namespace
	std
	Eq
	Cpp
	const
	inline
	Geq
$$

$section Discrete AD Functions$$

$index discrete, AD function$$
$index function, discrete AD$$

$head Syntax$$
$syntax%CPPAD_DISCRETE_FUNCTION(%Base%, %name%)
%$$
$syntax%%v% = %name%(%u%)
%$$
$syntax%%y% = %name%(%x%)
%$$


$head Purpose$$
Record the evaluation of a discrete function as part
of an $syntax%AD<%Base%>%$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.
The value of a discrete function can depend on the
$cref/independent variables/glossary/Tape/Independent Variable/$$,
but its derivative is identically zero.
For example, suppose that the integer part of 
a $cref/variable/glossary/Variable/$$ $italic x$$ is the 
index into an array of values. 

$head Base$$
This is the 
$cref/base type/base_require/$$
corresponding to the operations sequence;
i.e., use of the $italic name$$ with arguments of type
$syntax%AD<%Base%>%$$ can be recorded in an operation sequence.

$head name$$
This is the name of the function (as it is used in the source code).
The user must provide a version of $italic name$$
where the argument has type $italic Base$$.
CppAD uses this to create a version of $italic name$$
where the argument has type $syntax%AD<%Base%>%$$.

$head u$$
The argument $italic u$$ has prototype
$syntax%
	const %Base% &%u%
%$$
It is the value at which the user provided version of $italic name$$
is to be evaluated.

$head v$$
The result $italic v$$ has prototype
$syntax%
	%Base% %v%
%$$
It is the return value for the user provided version of $italic name$$.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$
It is the value at which the CppAD provided version of $italic name$$
is to be evaluated.

$head y$$
The result $italic y$$ has prototype
$syntax%
	AD<%Base%> %v%
%$$
It is the return value for the CppAD provided version of $italic name$$.


$head Create AD Version$$
$index CPPAD_DISCRETE_FUNCTION$$
The preprocessor macro invocation
$syntax%
	CPPAD_DISCRETE_FUNCTION(%Base%, %name%)
%$$ 
defines the $syntax%AD<%Base%>%$$ version of $italic name$$.
This can be with in a namespace (not the $code CppAD$$ namespace) 
but must be outside of any routine.

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Derivatives$$
During a zero order $xref/Forward//Forward/$$ operation,
an $xref/ADFun/$$ object will compute the value of $italic name$$
using the user provided $italic Base$$ version of this routine.
All the derivatives of $italic name$$ will be evaluated as zero.

$head Example$$
$children%
	example/tape_index.cpp%
	example/interp_onetape.cpp%
	example/interp_retape.cpp
%$$
The file
$xref/TapeIndex.cpp/$$
contains an example and test that uses a discrete function 
to vary an array index during $cref/Forward/$$ mode calculations.
The file
$xref/interp_onetape.cpp/$$
contains an example and test that uses discrete
functions to avoid retaping a calculation that requires interpolation.
(The file
$xref/interp_retape.cpp/$$
shows how interpolation can be done with retaping.)

$head Deprecated$$
$index CppADCreateDiscrete, deprecated$$
$index deprecated, CppADCreateDiscrete$$
The preprocessor symbol $code CppADCreateDiscrete$$
is defined to be the same as $code CPPAD_DISCRETE_FUNCTION$$
but its use is deprecated.

$end
------------------------------------------------------------------------------
*/

# define CPPAD_DISCRETE_FUNCTION(Base, FunName)            \
inline CppAD::AD<Base> FunName (const CppAD::AD<Base> &x)  \
{                                                          \
	static CppAD::ADDiscrete<Base> Fun(FunName);       \
                                                           \
	return Fun.Eval(x);                                \
}                                      

# define CppADCreateDiscrete CPPAD_DISCRETE_FUNCTION

# include <vector>

// Begin CppAD namespace
namespace CppAD {

template <class Base>
class ADDiscrete {
	typedef Base (*F) (const Base &x);
public:
	ADDiscrete(F f) : f_(f), f_index_( List()->size() )
	{	List()->push_back(this); }

	// used during the recording process
	AD<Base> Eval(const AD<Base> &x) const
	{	AD<Base> result;

		result.value_ = f_(x.value_);
		if( Variable(x) )
		{	ADTape<Base> *tape = x.tape_this();
			CPPAD_ASSERT_UNKNOWN( NumVar(DisOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(DisOp) == 2 );

			// put operand addresses in the tape
			tape->Rec_.PutInd(x.taddr_, f_index_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(DisOp);
			// make result a variable
			result.id_ = tape->id_;

			CPPAD_ASSERT_UNKNOWN( Variable(result) );
		} 
		return result;
	}

	// used to evaluate from the recording
	static Base Eval(size_t f_index, const Base &x)
	{
		CPPAD_ASSERT_UNKNOWN(f_index < List()->size() );

		return (*List())[f_index]->f_(x);
	}

private:
	const F            f_;
	const size_t f_index_;

	static std::vector<ADDiscrete *> *List(void)
	{	static std::vector<ADDiscrete *> list;
		return &list;
	}
		
};

} // END CppAD namespace

# endif
