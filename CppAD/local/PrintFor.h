# ifndef CppADPrintIncluded
# define CppADPrintIncluded

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
$begin PrintFor$$
$spell
	std
	cout
	const
$$

$index print, forward$$
$index forward, print$$

$section Printing Values During Forward Mode Calculations$$ 

$table
$bold Syntax$$ $cnext
$syntax%void PrintFor(const char *%text%, const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
This form of printing values is delayed until 
$xref/Forward//$$ mode is called with order equal to zero; 
i.e., when the value of the independent variables is changed.
This is can be helpful for understanding why tape evaluations
have trouble, for example, if the result of a tape calculation
is the IEEE code for not a number $code Nan$$.
The text specified by $italic text$$, 
followed by the value specified by $italic x$$,
are is printed to $code std::cout$$. 
$pre

$$
The $xref/Output/$$ section describes the normal 
printing of values; i.e., printing when the corresponding
code is executed.


$head Example$$
$children%
	PrintFor/PrintFor.cpp
%$$
The program
$xref/PrintFor.cpp/$$
is an example and a test of this operation.
The output of this program
states the conditions for passing and failing the test.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { // Begin CppAD namespace

template <class Base>
void PrintFor(const char *text, const AD<Base> &x)
{ 	if( AD<Base>::Tape()->State() == Recording )
	{	if( Parameter(x) )
			AD<Base>::Tape()->RecordPripOp(text, x.value);
		else	AD<Base>::Tape()->RecordPrivOp(text, x.taddr);
	}
}

} // END CppAD namespace

# endif
