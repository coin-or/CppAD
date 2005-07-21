# ifndef CppADDiscreteIncluded
# define CppADDiscreteIncluded

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
$begin Discrete$$
$spell
	std
	Eq
	Cpp
	const
	inline
	Geq
$$

$section User Defined Discrete Functions$$

$table
$bold Syntax$$
$cnext $syntax%CppADCreateDiscrete(%Base%, %FunName%)%$$ 
$rnext
$cnext $syntax%AD<%Base%> %FunName%(const AD<%Base%> &%w%)%$$
$rnext
$bold See Also$$
$cnext $xref/VecAD/$$
$tend


$fend 20$$

$head Description$$
The results of the $xref/Compare/$$ operations are boolean values
and hence are not recorded in the corresponding $xref/ADFun/$$ object.
One can record the results of simulated logical operations
using the discrete functions described here.

$head Definition$$
The preprocessor macro invocation
$syntax%
	CppADCreateDiscrete(%Base%, %FunName%)
%$$ 
creates a function with the following prototype
$syntax%
	inline AD<%Base%> %FunName%(const AD<%Base%> &%w%)
%$$
This function will compute its values using a user provided function
with the following prototype
$syntax%
	%Base% %FunName%(const %Base% &%w%)
%$$

$head Derivatives$$
An $xref/ADFun/$$ object will compute the value of a discrete function
using the user provided version.
All the derivatives of a discrete function are treated as zero.

$head Conditional Assignment$$
$index conditional assign$$
$index assign, conditional$$
The conditional expression function $xref/CondExp/$$
is the preferred way to do conditional assignments.
Unfortunately, this requires that the $code >$$ operator be 
defined for the base type.
If $code >$$ is not defined for the base type, you can 
use the following construction to obtain conditional assignments:
$pre

$$
Suppose that 
$italic w$$,
$italic x$$,
$italic y$$, and $italic z$$ are $code AD< std::complex<double> >$$ variables
and our computations contain a statement of the form
$codep
	if( w == 0. )
		z = x;
	else	z = y;
$$
The corresponding $xref/ADFun/$$ would only contain the
assignment that is executed. 
Further suppose that the function
$syntax%
	double EqZero(const double &%w%)
%$$
returns one if $latex w = 0$$ and zero otherwise and we use the command
$syntax%
	CppADCreateDiscrete( std::complex<double> , EqZero)
%$$
If we replace the computations above by
$codep
	z = EqZero(w) * x + (1. - EqZero(w)) * y
$$
the value of $italic z$$ will be the same as in the previous computations.
Furthermore the corresponding $code ADFun$$ will record 
this conditional assignment.
(Note that while the value of $code EqZero(w)$$ depends on $code w$$,
all of its derivatives are treated as being zero.)

$head Example$$
$children%
	Example/Discrete.cpp
%$$
The file
$xref/Discrete.cpp/$$
contains an example and a test of using discrete functions.   
It returns true if it succeeds and false otherwise.


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
	ADDiscrete(F f_) : f(f_), y_index( List()->size() )
	{	List()->push_back(this); }

	// used during the recording process
	AD<Base> Eval(const AD<Base> &x) const
	{	AD<Base> z;

		z.value = f(x.value);
		if( (AD<Base>::Tape()->State()==Recording) & Variable(x) )
		{	AD<Base>::Tape()->RecordDisOp(
				z,
				x.index,
				y_index
			);
		}
		return z;
	}

	// used to evaluate from the recording
	static Base Eval(size_t y_index, const Base &x)
	{
		CppADUnknownError(y_index < List()->size() );

		return (*List())[y_index]->f(x);
	}

private:
	const F            f;
	const size_t y_index;

	static std::vector<ADDiscrete *> *List(void)
	{	static std::vector<ADDiscrete *> list;
		return &list;
	}
		
};

} // END CppAD namespace

# endif
