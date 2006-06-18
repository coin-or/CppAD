# ifndef CppADCapTaylorIncluded
# define CppADCapTaylorIncluded

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
$begin capacity_taylor$$
$spell
	var
	Taylor
$$

$index Forward, capacity$$
$index capacity_taylor$$
$index capacity, Forward$$
$index memory, control$$

$section Controlling Taylor Coefficients Memory Allocation$$

$head Syntax$$
$syntax%%f%.capacity_taylor(%c%)%$$

$head Purpose$$
The Taylor coefficients calculated by Forward mode calculations
are retained in an $xref/ADFun/$$ object for subsequent use during 
$xref/Reverse/$$ mode or higher order Forward mode calculations.
This operation allow you to control that amount of memory
that is retained by an AD function object 
(for subsequent calculations).

$head f$$
The object $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$

$head c$$
The argument $italic c$$ has prototype
$syntax%
	size_t %c%
%$$
It specifies the number of Taylor coefficients that are allocated for
each variable in the AD operation sequence corresponding to $italic f$$.

$head Discussion$$
A call to $xref/ForwardAny//Forward/$$ with the syntax
$syntax%
        %y_p% = %f%.Forward(%p%, %x_p%)
%$$
uses the lower order Taylor coefficients and 
computes the $th p$$ order Taylor coefficients for all
the variables in the operation sequence corresponding to $italic f$$.
(You can determine the number of variables in the operation sequence
using the $xref/SeqProperty/size_var/size_var/$$ function.)

$subhead Pre-Allocating Memory$$
If you plan to make calls to $code Forward$$ with the maximum value of 
$italic p$$ equal to $italic q$$,
it should be faster to pre-allocate memory for these calls using
$syntax%
	%f%.capacity_taylor(%c%)
%$$
with $italic c$$ equal to $latex q + 1$$.
If you do no do this, $code Forward$$ will automatically allocate memory
and will copy the results to a larger buffer, when necessary.

$subhead Freeing Memory$$
If you no longer need the Taylor coefficients of order $italic q$$
and higher (that are stored in $italic f$$), 
you can reduce the memory allocated to $italic f$$ using
$syntax%
	%f%.capacity_taylor(%c%)
%$$
with $italic c$$ equal to $italic q$$.

$subhead Original State$$
Originally, when $italic f$$ is constructed,
there is an implicit call to $code Forward$$ with $italic p$$ equal to zero
and $italic x_p$$ equal to 
the value of the
$xref/glossary/Independent Variable/independent variables/$$ 
when the AD operation sequence was recorded.

$head Example$$
The file 
$xref/Forward.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
void ADFun<Base>::capacity_taylor(size_t c)
{	// temporary indices
	size_t i, j, p;

		// taylor_per_var,

	if( c == TaylorColDim )
		return;

	if( c == 0 )
	{	if( Taylor != CppADNull )
			CppADTrackDelVec(Taylor);
		Taylor = CppADNull;
		taylor_per_var = 0;
		return;
	}
	
	// Allocate new matrix will requested number of columns 
	size_t newlen   = c * totalNumVar;
	Base *newptr    = CppADNull;
	newptr          = CppADTrackNewVec(newlen, newptr);

	// number of columns to copy
	p = std::min(taylor_per_var, c);

	// copy the old data into the new matrix
	for(i = 0; i < totalNumVar; i++)
	{	for(j = 0; j < p; j++)
		{	newptr[i * c + j]  = Taylor[i * TaylorColDim + j];
		}
	}
	// free the old memory
	CppADTrackDelVec(Taylor);

	// use the new pointer
	Taylor         = newptr;
	TaylorColDim   = c;
	taylor_per_var = p;

	return;
}

} // END CppAD namespace
	

# endif
