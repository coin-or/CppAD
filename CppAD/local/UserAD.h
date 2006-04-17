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
	bool
	cos
	Cpp
$$

$section AD Objects$$

$index AD, object$$

$head Purpose$$
The sections listed below describe the operations 
that are available to $xref/glossary/AD of Base/AD of Base/$$ objects.
These objects are used to
$xref/glossary/Tape State/Recording/record/1/$$
an AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.
This operation sequence can
be transferred to an $xref/ADFun/$$ object where it
can be used to evaluate the corresponding 
function and derivative values.

$childtable%
	CppAD/local/Constructor.h%
	CppAD/local/Convert.h%
	CppAD/local/Eq.h%
	CppAD/local/ADValued.h%
	CppAD/local/BoolValued.h%
	CppAD/local/VecAD.h
%$$

$end
---------------------------------------------------------------------------
*/

# include <CppAD/local/Constructor.h>
# include <CppAD/local/Convert.h>
# include <CppAD/local/Eq.h>
# include <CppAD/local/ADValued.h>
# include <CppAD/local/BoolValued.h>
# include <CppAD/local/VecAD.h>

# endif
