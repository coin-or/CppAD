# ifndef CppADAllocMatIncluded
# define CppADAllocMatIncluded

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
$begin AllocMat$$
$spell
	nr
	nc
	Alloc
	Mat
$$

$section Matrix Allocation Routine$$

$table 
$bold Syntax$$
$cnext $syntax%AllocMat<%Type%> %M%(int %nr%, int %nc%)%$$
$rnext
$cnext $syntax%%Type%* %M%[int %i%]%$$
$rnext
$cnext $syntax%(%Type%**) %M%$$
$tend

$fend 25$$

$head Description$$
The $code AllocMat$$ template class 
can be used to allocate and automatically free matrices
with an arbitrary type.
$syntax%

AllocMat<%Type%> %M%(int %nr%, int %nc%)
%$$
This syntax allocates memory for an $italic nr$$
by $italic nc$$ matrix 
with elements of the specified type. The arguments
$italic nr$$ and $italic nc$$ must be greater than or equal to zero.
$syntax%

%Type%* %M%[int %i%]
%$$
This syntax returns the $th i$$ row of the matrix $italic M$$.
The value $italic i$$ must be between zero and $syntax%%nr%-1%$$
where $italic nr$$ is the value in the constructor for $italic M$$.
$syntax%

(%Type%**) %M%
%$$
This syntax returns a vector of length $italic nr$$
each element of which points to a vector of length $italic nc$$ where
the elements of the matrix $italic M$$ are stored.
Here the values $italic nr$$ and $italic nc$$ are those
used in the constructor for $italic M$$.

$children%
	Adolc/AllocMat.cpp
%$$

$head Example$$
The file
$xref/AllocMat.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

# include <cassert>

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
class AllocMat 
{
public:
	// constructor
	AllocMat(int nr_, int nc_)  : nr(nr_), nc(nc_)
	{	int i;
		vec = new Type  [nr * nc];
		mat = new Type* [nr];
		for(i = 0; i < nr; i++)
			mat[i] = vec + i * nc;
	}

	// destructor
	~AllocMat()
	{	delete [] mat;
		delete [] vec;
	}

	Type *operator[](int i)
	{	assert( i < nr );
		assert( 0 <= i );
		return mat[i];
	}

	// pointer to the memory
	operator Type** ()
	{	return mat; }

private:
	const int nr;
	const int nc;

	Type **mat;
	Type  *vec;
};

} // END CppAD namespace

# endif
