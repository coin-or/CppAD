# ifndef CPPAD_UTILITY_SPARSE_RC_HPP
# define CPPAD_UTILITY_SPARSE_RC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_rc$$
$spell
	CppAD
	const
	nnz
	cppad
	hpp
	rc
	nr
	nc
	resize
$$
$section Row and Column Index Sparsity Patterns$$

$head Under Construction$$
This class is under construction and not yet appropriation for public use.

$head Syntax$$
$codei%# include <cppad/utility/sparse_rc.hpp>
%$$
$codei%sparse_rc<%SizeVector%>  %pattern%
%$$
$icode%resize%(%nr%, %nc%, %nnz%)
%$$
$icode%pattern%.set(%k%, %r%, %c%)
%$$
$icode%pattern%.nr()
%$$
$icode%pattern%.nc()
%$$
$icode%pattern%.nnz()
%$$
$codei%const %SizeVector%& %row%( %pattern%.row() )
%$$
$codei%const %SizeVector%& %col%( %pattern%.col() )
%$$
$icode%SizeVector% %row_major% = %pattern%.row_major()
%$$
$icode%SizeVector% %col_major% = %pattern%.col_major()
%$$

$head SizeVector$$
We use $icode SizeVector$$ to denote $cref SimpleVector$$ class
$cref/with elements of type/SimpleVector/Elements of Specified Type/$$
$code size$$.

$head pattern$$
The sparsity $icode pattern$$ is $code const$$
except during its constructor, $code resize$$, and $code set$$.


$head nr$$
This argument has prototype
$codei%
	size_t %nr%
%$$
It specifies the number of rows in the sparsity pattern.
The function call $code nr()$$ returns the value of $icode nr$$.

$head nc$$
This argument has prototype
$codei%
	size_t %nc%
%$$
It specifies the number of columns in the sparsity pattern.
The function call $code nc()$$ returns the value of $icode nc$$.

$head nnz$$
This argument has prototype
$codei%
	size_t %nnz%
%$$
It specifies the number of possibly non-zero
index pairs in the sparsity pattern.
The function call $code nnz()$$ returns the value of $icode nnz$$.

$head resize$$
The current sparsity pattern is lost and a new one is started
with the specified parameters. The elements in the $icode row$$
and $icode col$$ vectors must assigned using $code set$$.

$head k$$
This argument has type
$codei%
	size_t %k%
%$$
and must be less than $icode nnz$$.

$head r$$
This argument has type
$codei%
	size_t %r%
%$$
It specifies the value assigned to $icode%row%[%k%]%$$ and must
be less than $icode nr$$.

$head c$$
This argument has type
$codei%
	size_t %c%
%$$
It specifies the value assigned to $icode%col%[%k%]%$$ and must
be less than $icode nc$$.

$head set$$
This function sets the values
$codei%
	%row%[%k%] = %r%
	%col%[%k%] = %c%
%$$

$head row$$
This vector has size $icode nnz$$ and
$icode%row%[%k%]%$$
is the row index of the $th k$$ possibly non-zero
index pair in the sparsity pattern.

$head col$$
This vector has size $icode nnz$$ and
$icode%col%[%k%]%$$ is the column index of the $th k$$ possibly non-zero
index pair in the sparsity pattern.

$head row_major$$
This vector has size $icode nnz$$ and sorts the sparsity pattern
in row-major order. To be specific,
$codei%
	%col%[ %row_major%[%k%] ] <= %col%[ %row_major%[%k%+1] ]
%$$
and if $icode%col%[ %row_major%[%k%] ] == %col%[ %row_major%[%k%+1] ]%$$,
$codei%
	%row%[ %row_major%[%k%] ] < %row%[ %row_major%[%k%+1] ]
%$$

$head col_major$$
This vector has size $icode nnz$$ and sorts the sparsity pattern
in column-major order. To be specific,
$codei%
	%row%[ %col_major%[%k%] ] <= %row%[ %col_major%[%k%+1] ]
%$$
and if $icode%row%[ %col_major%[%k%] ] == %row%[ %col_major%[%k%+1] ]%$$,
$codei%
	%col%[ %col_major%[%k%] ] < %col%[ %col_major%[%k%+1] ]
%$$

$children%
	example/utility/sparse_rc.cpp
%$$
$head Example$$
The file $cref sparse_rc.cpp$$
contains an example and test of this class.
It returns true if it succeeds and false otherwise.

$end
*/
/*!
\file sparse_rc.hpp
A Matrix sparsity pattern class.
*/
# include <cstddef> // for size_t
# include <cppad/core/cppad_assert.hpp>  // for CPPAD_ASSERT
# include <cppad/utility/index_sort.hpp> // for row and column major ordering

namespace CppAD { // BEGIN CPPAD_NAMESPACE

/// sparsity pattern for a matrix with indices of type size_t
template <class SizeVector>
class sparse_rc {
private:
	/// number of rows in the sparsity pattern
	size_t nr_;
	/// number of columns in the sparsity pattern
	size_t nc_;
	/// number of possibly non-zero index pairs
	size_t nnz_;
	/// row_[k] is the row index for the k-th possibly non-zero entry
	SizeVector row_;
	/// col_[k] is the column index for the k-th possibly non-zero entry
	SizeVector col_;
public:
	/// default constructor
	/// Eigen vector is ambiguous for row_(0), col_(0) so use default ctor
	sparse_rc(void)
	: nr_(0), nc_(0), nnz_(0)
	{ }
	/// copy constructor
	sparse_rc(const sparse_rc& other)
	:
	nr_(other.nr_)   ,
	nc_(other.nc_)   ,
	nnz_(other.nnz_) ,
	row_(other.row_) ,
	col_(other.col_)
	{ }
	/// resize
	void resize(size_t nr, size_t nc, size_t nnz)
	{	nr_ = nr;
		nc_ = nc;
		nnz_ = nnz;
		row_.resize(nnz);
		col_.resize(nnz);
	}
	/// set a possibly non-zero indices
	void set(size_t k, size_t r, size_t c)
	{	CPPAD_ASSERT_KNOWN(
			k < nnz_,
			"The index k is not less than nnz in sparse_rc::set"
		);
		CPPAD_ASSERT_KNOWN(
			r < nr_,
			"The index r is not less than nr in sparse_rc::set"
		);
		CPPAD_ASSERT_KNOWN(
			c < nc_,
			"The index c is to not less than nc in sparse_rc::set"
		);
		row_[k] = r;
		col_[k] = c;
		//
	}
	/// nr()
	size_t nr(void) const
	{	return nr_; }
	/// nc()
	size_t nc(void) const
	{	return nc_; }
	/// nnz()
	size_t nnz(void) const
	{	return nnz_; }
	/// row indices
	const SizeVector& row(void) const
	{	return row_; }
	/// column indices
	const SizeVector& col(void) const
	{	return col_; }
	/// row-major indices
	SizeVector row_major(void) const
	{	SizeVector keys(nnz_), row_major(nnz_);
		for(size_t k = 0; k < nnz_; k++)
		{	CPPAD_ASSERT_UNKNOWN( row_[k] < nr_ );
			keys[k] = col_[k] * nr_ + row_[k];
		}
		index_sort(keys, row_major);
		return row_major;
	}
	/// column-major indices
	SizeVector col_major(void) const
	{	SizeVector keys(nnz_), col_major(nnz_);
		for(size_t k = 0; k < nnz_; k++)
		{	CPPAD_ASSERT_UNKNOWN( col_[k] < nc_ );
			keys[k] = row_[k] * nc_ + col_[k];
		}
		index_sort(keys, col_major);
		return col_major;
	}
};

} // END_CPPAD_NAMESPACE

# endif
