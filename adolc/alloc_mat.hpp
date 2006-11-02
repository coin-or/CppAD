# ifndef CppAD_ALLOC_MAT_INCLUDED
# define CppAD_ALLOC_MAT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin AllocMat$$
$spell
	nr
	nc
	Alloc
	Mat
$$

$section Matrix Allocation Routine$$

$head Syntax$$
$syntax%AllocMat<%Type%> %M%(%nr%, %nc%)%$$
$pre
$$
$syntax%%p% = %M%[%i%]%$$
$pre
$$
$syntax%(%Type%**) %M%$$


$head Description$$
The $code AllocMat$$ template class 
can be used to allocate and automatically free matrices
with an arbitrary type.

$head Constructor$$
$syntax%
	AllocMat<%Type%> %M%(%nr%, %nc%)
%$$
allocates memory for an $italic nr$$ by $italic nc$$ matrix 
with elements of the specified type. 
The arguments $italic nr$$ and $italic nc$$ have prototype
$syntax%
	int %nr%
	int %nc%
%$$
and must be greater than or equal to zero.

$head Row Access$$
The syntax
$syntax%
	%p% = %M%[%i%]
%$$
returns a point to the $th i$$ row of the matrix $italic M$$.
The argument $italic i$$ has prototype
$syntax%
	int %i%
%$$
and must have value $italic i$$ must be between zero and $syntax%%nr%-1%$$
where $italic nr$$ is the value in the constructor for $italic M$$
(or else a standard assert is generated).
The return value has prototype
$syntax%
	%Type% *%p%
%$$
 
$head Matrix Access$$
The syntax
$syntax%
	(%Type%**) %M%
%$$
returns a vector of length $italic nr$$
each element of which points to a vector of length $italic nc$$ where
the elements of the matrix $italic M$$ are stored.
Here the values $italic nr$$ and $italic nc$$ are those
used in the constructor for $italic M$$.

$children%
	adolc/alloc_mat.cpp
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
