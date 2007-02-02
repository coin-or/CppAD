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
	namespace
	std
	Eq
	Cpp
	const
	inline
	Geq
$$

$section Discrete AD Functions$$

$head Syntax$$
$syntax%CppADCreateDiscrete(%Base%, %FunName%)%$$
$pre
$$
$syntax%%y% = %FunName%(%x%)%$$


$head Purpose$$
Record the evaluation of a discrete function as part
of an AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.
The value of a discrete function can depend on the
$xref/glossary/Independent Variable/independent variables/$$,
but its derivative is identically zero.
For example, suppose that the integer part of $italic x$$ is the 
index into an array of values. 

$head Prototypes$$
The variables above and below have the following prototypes
$syntax%
	const %Base%     &%u%
	%Base%            %v%
	const AD<%Base%> &%x%
	AD<%Base%>        %y%
%$$

$head CppADCreateDiscrete$$
The preprocessor macro invocation
$syntax%
	CppADCreateDiscrete(%Base%, %FunName%)
%$$ 
can be with in a namespace but must be outside of any routine.
It defines a function with the following syntax
$syntax%
	%y% = %FunName%(%x%)
%$$
This function will compute its values using a user defined function
with the following syntax
$syntax%
	%v% = %FunName%(%u%)
%$$

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.


$head Derivatives$$
During a zero order $xref/Forward//Forward/$$ operation,
an $xref/ADFun/$$ object will compute the value of a discrete function
using the user provided $italic Base$$ version of the routine.
All the derivatives of a discrete function will be evaluated as zero.

$head Example$$
$children%
	example/tape_index.cpp%
	example/piecewise.cpp
%$$
The file
$xref/TapeIndex.cpp/$$
contains an example and test that uses a discrete function 
to vary an array index during $cref/Forward/$$ mode calculations.
The file
$xref/Piecewise.cpp/$$
contains an example and test that uses discrete
functions for piecewise linear interpolation.


$end
------------------------------------------------------------------------------
*/

# define CppADCreateDiscrete(Base, FunName)                \
inline CppAD::AD<Base> FunName (const CppAD::AD<Base> &x)  \
{                                                          \
	static CppAD::ADDiscrete<Base> Fun(FunName);       \
                                                           \
	return Fun.Eval(x);                                \
}                                      

# include <vector>

// Begin CppAD namespace
namespace CppAD {

template <class Base>
class ADDiscrete {
	typedef Base (*F) (const Base &x);
public:
	ADDiscrete(F f_) : f(f_), y_taddr( List()->size() )
	{	List()->push_back(this); }

	// used during the recording process
	AD<Base> Eval(const AD<Base> &x) const
	{	AD<Base> z;

		z.value = f(x.value);
		if( Variable(x) )
		{	AD<Base>::Tape()->RecordDisOp(
				z,
				x.taddr,
				y_taddr
			);
		}
		return z;
	}

	// used to evaluate from the recording
	static Base Eval(size_t y_taddr, const Base &x)
	{
		CppADUnknownError(y_taddr < List()->size() );

		return (*List())[y_taddr]->f(x);
	}

private:
	const F            f;
	const size_t y_taddr;

	static std::vector<ADDiscrete *> *List(void)
	{	static std::vector<ADDiscrete *> list;
		return &list;
	}
		
};

} // END CppAD namespace

# endif
