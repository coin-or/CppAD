# ifndef CppADUserADIncluded
# define CppADUserADIncluded

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
---------------------------------------------------------------------------

$begin AD$$
$spell
	Cpp
$$

$section Using AD<Base> Class Objects$$

$index AD<Base>, using$$

$head Description$$
The template class
$syntax%AD<%Base%>%$$ 
is defined by including 
the file $xref/CppAD//CppAD.h/$$.
The purpose of this class is to record 
$syntax%AD<%Base%>%$$ operations on a corresponding tape 
so that derivatives can be calculated.
The sections listed below describe the methods
that are available for objects in this template class.

$childtable%
	CppAD/local/Default.h%
	CppAD/local/FromBase.h%
	CppAD/local/Copy.h%
	CppAD/local/Value.h%
	CppAD/local/Integer.h%
	CppAD/local/UnaryPlus.h%
	CppAD/local/UnaryMinus.h%
	CppAD/local/BinaryOp.h%
	CppAD/local/CompEq.h%
	CppAD/local/Compare.h%
	CppAD/local/Eq.h%
	CppAD/local/Output.h%
	CppAD/local/PrintFor.h%
	CppAD/local/StdMathLib.h%
	CppAD/local/CondExp.h%
	CppAD/local/Discrete.h%
	CppAD/local/BoolFun.h%
	CppAD/local/Vec.h
%$$

$end
---------------------------------------------------------------------------
*/

# include <CppAD/local/Default.h>      // default constructor
# include <CppAD/local/FromBase.h>     // construction from base type
# include <CppAD/local/Copy.h>         // copy constructor
# include <CppAD/local/Value.h>        // base type value
# include <CppAD/local/UnaryPlus.h>    // the unary plus operator
# include <CppAD/local/UnaryMinus.h>   // the unary minus operator
# include <CppAD/local/BinaryOp.h>     // base type Binary operators
# include <CppAD/local/CompEq.h>       // computed assignment operators
# include <CppAD/local/Compare.h>      // Logical Binary operators
# include <CppAD/local/Eq.h>           // assignment operator
# include <CppAD/local/Output.h>       // output AD to a stream
# include <CppAD/local/PrintFor.h>     // print during forward mode
# include <CppAD/local/StdMathLib.h>   // standard math library functions
# include <CppAD/local/Discrete.h>     // user defined discrete functions
# include <CppAD/local/BoolFun.h>      // user defined boolean valued functions

# endif
