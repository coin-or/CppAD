# ifndef CppADUserADIncluded
# define CppADUserADIncluded

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
	CppAD/local/CopyBase.h%
	CppAD/local/CopyAD.h%
	CppAD/local/Value.h%
	CppAD/local/Integer.h%
	CppAD/local/Eq.h%
	CppAD/local/Arithmetic.h%
	CppAD/local/Compare.h%
	CppAD/local/Output.h%
	CppAD/local/PrintFor.h%
	CppAD/local/StdMathLib.h%
	CppAD/local/BoolFun.h%
	CppAD/local/NearEqualExt.h
%$$

$end
---------------------------------------------------------------------------
*/

# include <CppAD/local/Default.h>      // default constructor
# include <CppAD/local/CopyBase.h>     // construction from base type
# include <CppAD/local/CopyAD.h>       // copy constructor
# include <CppAD/local/Value.h>        // base type value
# include <CppAD/local/Integer.h>      // base type value
# include <CppAD/local/Eq.h>           // assignment operator
# include <CppAD/local/Arithmetic.h>   // arithmetic operators
# include <CppAD/local/Compare.h>      // Logical Binary operators
# include <CppAD/local/Output.h>       // output AD to a stream
# include <CppAD/local/PrintFor.h>     // print during forward mode
# include <CppAD/local/StdMathLib.h>   // standard math library functions
# include <CppAD/local/BoolFun.h>      // user defined boolean valued functions
# include <CppAD/local/NearEqualExt.h> // mix AD and base in NearEqual

# endif
