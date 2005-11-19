# ifndef CppADExtendColIncluded
# define CppADExtendColIncluded

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
$begin ExtendCol$$ $comment CppAD Developer Documentation$$
$spell
	newncol
	newmat
	nrow
	oldmat
	oldncol
	CppAD
$$

$section Extends Number of Columns in a Matrix$$

$table
$bold Syntax$$ $cnext 
$syntax%%newmat% = ExtendCol(%newncol%, %nrow%, %oldncol%, %oldmat%)%$$
$tend

$fend 20$$

$head Description$$
This function increases the number of columns stored in the matrix pointed
to by $italic oldmat$$.
If $italic oldncol$$ is zero and $italic oldmat$$ is $code CppADNull$$,
this just allocates memory and checks for errors during the allocation.

$head newncol$$
This argument has prototype
$syntax%
	size_t %newncol%
%$$
It is the number of columns in the matrix pointed to by $italic newmat$$.

$head nrow$$
This argument has prototype
$syntax%
	size_t %nrow%
%$$
It is the number of rows in both the matrix pointed to by 
$italic oldmat$$ and $italic newmat$$.


$head oldncol$$
This argument has prototype
$syntax%
	size_t %oldncol%
%$$
It is the number of columns stored in the matrix $italic oldmat$$
The value $italic oldncol$$ must be less than $italic newncol$$.

$head oldmat$$
This argument has prototype
$syntax%
	%Type% *%oldmat%
%$$
On input, $italic oldmat$$ points to  
an $italic nrow$$ by $italic oldncol$$ matrix 
with $th (i,j)$$ element given by
$syntax%
	%oldmat%[%i% * %oldncol% + j]
%$$
for $latex i = 0 , \ldots , nrow-1$$ 
and $latex j = 0 , \ldots , oldncol-1$$.
If $italic oldncol$$ is equal to zero,
the value $italic oldmat$$ must be equal to $code CppADNull$$.
Otherwise, $italic oldmat$$ must point to allocated memory
and on output, this pointer has been deleted with 
$syntax%
	delete [] %oldmat%
%$$ 


$head newmat$$
The return value has prototype
$syntax%
	%Type% *%newmat%
%$$
The return value $italic newmat$$ points to  
an $italic nrow$$ by $italic newncol$$ matrix 
with $th (i,j)$$ element given by
$syntax%
	%newmat%[%i% * %newncol% + j]
%$$
for $latex i = 0 , \ldots , nrow-1$$ 
and $latex j = 0 , \ldots , newncol-1$$.
In addition, the information in $italic oldmat$$ has
been copied using the assignment
$syntax%
	%newmat%[%i% * %newncol% + j] = %oldmat%[%i% * %oldncol% + j];
%$$
for $latex i = 0 , \ldots , nrow-1$$ 
and $latex j = 0 , \ldots , oldncol-1$$.


$head Errors$$
If the memory cannot be allocated, 
$code CppADUsageError$$ is used to generate
a message stating that there was not sufficient memory.

$end
*/

namespace CppAD { // Begin CppAD namespace

template <typename Type>
Type * ExtendCol(size_t newncol, size_t nrow, size_t oldncol, Type * oldmat)
{
	CppADUnknownError(newncol > oldncol);
	CppADUnknownError(oldncol > 0 || oldmat == CppADNull);

	Type *newmat = CppADNull;
	try
	{	newmat = new Type[nrow * newncol];
	}
	catch(...)
	{	CppADUsageError(0, "cannot allocate sufficient memory");
		abort();
	}
	size_t i, j;
	for(j = 0; j < oldncol; j++)
	{	for(i = 0; i < nrow; i++)
			newmat[i * newncol + j] = oldmat[i * oldncol + j];
	}
	if( oldncol > 0 )
		delete [] oldmat;

	return newmat;
} 

} // End CppAD namespace

# endif
