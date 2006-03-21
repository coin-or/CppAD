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

$section Using The AD Template Class$$

$index AD, template class$$

$head Purpose$$
The AD template class ($syntax%AD<%Base%>%$$ class)
is defined by including 
the file $xref/CppAD//CppAD.h/$$.
The purpose of this template class is to record an
$xref/glossary/AD Operation Sequence/AD operation sequence/$$
for evaluation of the corresponding function and its derivatives.
The sections listed below describe the methods
that are available for objects in this template class.

$childtable%
	CppAD/local/Constructor.h%
	CppAD/local/Eq.h%
	CppAD/local/Convert.h%
	CppAD/local/ADValued.h%
	CppAD/local/BoolValued.h
%$$

$end
---------------------------------------------------------------------------
*/

# include <CppAD/local/Constructor.h>
# include <CppAD/local/Eq.h>
# include <CppAD/local/Convert.h>
# include <CppAD/local/ADValued.h>
# include <CppAD/local/BoolValued.h>

# endif
