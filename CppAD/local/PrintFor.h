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
	std
	cout
	const
$$

$index print, forward$$
$index forward, print$$

$section Printing AD Values During Forward Mode$$ 

$table
$bold Syntax$$ $cnext
$syntax%PrintFor(%text%, %u%)%$$
$tend

$fend 20$$

$head Purpose$$
The AD object $italic u$$ may be an value in the
$xref/glossary/AD Operation Sequence/operation sequence/$$
that is transferred to an $xref/ADFun/$$ object $italic f$$ with prototype
$syntax%
	ADFun<%Base%> %f%
%$$
This object can be evaluated using $xref/Forward/$$ mode zero; i.e.,
$syntax%
	%f%.Forward(0, %x%)
%$$
with values for the
$xref/glossary/Independent Variable/independent variables/$$
(specified by the vector $italic x$$)
that are different from when the operation sequence was recorded.
The routine $code PrintFor$$ requests a printing of the value of $italic u$$ 
that corresponds to $italic x$$
when $syntax%%f%.Forward(0, %x%)%$$ is executed. 

$head text$$
The argument $italic text$$ has prototype
$syntax%
	const char *%text%
%$$
The corresponding text is written to $code std::cout$$ before the 
value of $italic u$$ that corresponds to the independent variable values.

$head u$$
The argument $italic u$$ has prototype
$syntax%
	const AD<%Base%> &%u%
%$$
The value of $italic u$$ that corresponds to $italic x$$
is written to $code std::cout$$ during the execution of 
$syntax%
	%f%.Forward(0, %x%)
%$$

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

}

# endif
