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

$section AD Class Objects$$

$index AD, object$$

$head Purpose$$
The sections listed below describe the operations 
that are available to 
$xref/glossary/AD Object/AD objects/$$.
The corresponding
$xref/AD/AD Operation Sequence/AD operation sequence/$$
can be
$xref/glossary/Tape State/Recording/recorded/1/$$
and then transferred to an $xref/ADFun/$$ object.
The ADFun object can be used to
evaluate the function and derivatives corresponding to the
AD sequence of operations.

$head AD Operation Sequence$$
The sections listed below define the available AD operations. 
An AD operation becomes part of an AD operation sequence,
if its result is an
$xref/glossary/AD Object/AD object/$$ 
and the corresponding  
$xref/glossary/Tape State/Recording/tape state is Recording/1/$$.
For example, if both $italic x$$ and $italic y$$
have type $syntax%AD<double>%$$, the 
$xref/Add/$$ operation
$syntax%
	%x% + %y%
%$$
is defined as part $xref/ADValued/$$ section and its
result is an AD object.
Thus,
if the corresponding tape is recording,
this operation becomes part of the AD operation sequence
that is stored and transferred to the corresponding $xref/ADFun/$$ object.
In contrast, the result of the $xref/Compare/$$ operation
$syntax%
	%x% < %y%
%$$
is defined as part of the $xref/BoolValued/$$ section but 
has type $code bool$$ and hence is not an AD object.
If one executes the following code
$syntax%
	if( %x% < %y% )
		%y% = cos(%x%);
	else	%y% = sin(%x%); 
%$$
The choice may depend on the value of the 
$xref/glossary/Independent Variable/independent variables/$$
but only the choice made while the tape is recording
is transferred to the corresponding $code ADFun$$ object.

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
