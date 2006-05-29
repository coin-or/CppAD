# ifndef CppADPrintForIncluded
# define CppADPrintForIncluded

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
$xref/glossary/Independent Variable/independent variables/$$.
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
	PrintFor/PrintFor.cpp
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
	{ 	if( AD<Base>::Tape()->State() == Recording )
		{	if( Parameter(u) )
				AD<Base>::Tape()->RecordPripOp(text, u.value);
			else	AD<Base>::Tape()->RecordPrivOp(text, u.taddr);
		}
	}
	template <class Base>
	void PrintFor(const char *text, const VecAD_reference<Base> &u)
	{	PrintFor(text, u.ADBase()); }
}

# endif
