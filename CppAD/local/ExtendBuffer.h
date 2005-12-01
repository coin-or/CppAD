# ifndef CppADExtendBufferIncluded
# define CppADExtendBufferIncluded

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

/*
$begin ExtendBuffer$$ $comment CppAD Developer Documentation$$
$spell
	newbuf
	newlen
	oldbuf
	CppAD
	ncopy
$$
	

$section Extends the Length of a Buffer$$

$table
$bold Syntax$$ $cnext 
$syntax%%newbuf% = ExtendBuffer(%newlen%, %ncopy%, %oldbuf%)%$$
$tend

$fend 20$$

$head Description$$
This function increases the length of the array pointed to by
$italic oldbuf$$.
If $italic ncopy$$ is zero and $italic oldbuf$$ is $code CppADNull$$,
this just allocates memory and checks for errors during the allocation.

$head newlen$$
This argument has prototype
$syntax%
	size_t %newlen%
%$$
It is the length of the array pointed to by $italic newbuf$$.

$head ncopy$$
This argument has prototype
$syntax%
	size_t %ncopy%
%$$
It is the number of elements in $italic oldbuf$$
that have important values which must be copied to $italic newbuf$$.
The value $italic ncopy$$ must be less than $italic newlen$$.

$head oldbuf$$
This argument has prototype
$syntax%
	%Type% *%oldbuf%
%$$
On input, $italic oldbuf$$ points to  $italic ncopy$$ elements of information
that must be copied to $italic newbuf$$.
The data is copied to the new buffer using the $italic Type$$
assignment operation.
If $italic oldbuf$$ is equal to $code CppADNull$$,
$italic ncopy$$ must be equal to zero.
If $italic oldbuf$$ is not equal to $code CppADNull$$,
this pointer has been deleted with 
$syntax%
	delete [] %oldbuf%
%$$ 


$head newbuf$$
The return value has prototype
$syntax%
	%Type% *%newbuf%
%$$
On output, $italic newbuf$$ has length $italic newlen$$ 
and its first $italic ncopy$$ elements contain the information
that was in $italic oldbuf$$.

$head Errors$$
If $code NDEBUG$$ is not defined and the memory cannot be allocated, 
$code CppADUsageError$$ is used to generate
a message stating that there was not sufficient memory.


$end
*/

namespace CppAD { // Begin CppAD namespace

template <typename Type>
Type * ExtendBuffer(size_t newlen, size_t ncopy, Type * oldbuf)
{
	CppADUnknownError(newlen > ncopy);
	CppADUnknownError( ncopy == 0 || oldbuf != CppADNull );

	Type *newbuf = CppADNull;
# ifdef NDEBUG
	newbuf = new Type[newlen];
# else
	try
	{	newbuf = new Type[newlen];
	}
	catch(...)
	{	CppADUsageError(0, "cannot allocate sufficient memory");
		abort();
	}
# endif
	size_t i = ncopy;
	while(i)
	{	--i;
		newbuf[i] = oldbuf[i];
	}
	if( oldbuf != CppADNull )
		delete [] oldbuf;

	return newbuf;
} 

} // End CppAD namespace

# endif
