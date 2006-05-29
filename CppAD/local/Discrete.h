# ifndef CppADDiscreteIncluded
# define CppADDiscreteIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
	Example/Discrete.cpp%
	Example/Piecewise.cpp
%$$
The file
$xref/Discrete.cpp/$$
contains an example and test of using discrete functions.   
It returns true if it succeeds and false otherwise.
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
		if( (AD<Base>::Tape()->State()==Recording) & Variable(x) )
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
