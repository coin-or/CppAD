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
# endif

/*
$begin sparse_rc$$
$spell
	CppAD
	const
	nnz
	cppad
	hpp
	rc
$$
$section Row and Column Sparsity Patterns$$

$head Under Construction$$
This class is under construction and not yet appropriation for public use.

$head Syntax$$
$codei%# include <cppad/utility/sparse_rc.hpp>
%$$
$codei%sparse_rc %pattern%<%SizeVector%>(%row_in%, %col_in%)
%$$
$codei%sparse_rc %pattern%<%SizeVector%>(%other%)
%$$
$codei%const %SizeVector%& %row%( %pattern%.row() )
%$$
$codei%const %SizeVector%& %col%( %pattern%.col() )
%$$

$head SizeVector$$
We use $icode SizeVector$$ to denote $cref SimpleVector$$ class
$cref/with elements of type/SimpleVector/Elements of Specified Type/$$
$code size$$.

$head nnz$$
We use the notation $icode nnz$$ to denote the number of
possibly non-zero entries in the pattern; i.e., $icode%row_in%.size()%$$.

$head row_in$$
This argument has prototype
$codei%
	const %SizeVector%& %row_in%
%$$
For $icode%k% = 0, %...%, %nnz%-1%$$,
$icode%row_in%[%k%]%$$ is the row index for the $th k$$ possibly
non-zero entry in the sparsity pattern.


$head col_in$$
This argument has prototype
$codei%
	const %SizeVector%& %col_in%
%$$
It must have the same size as $icode row_in$$.
For $icode%k% = 0, %...%, %nnz%-1%$$,
$icode%col_in%[%k%]%$$ is the column index for the $th k$$ possibly
non-zero entry in the sparsity pattern.

$head pattern$$
This object is $code const$$ except during its constructor.

$head other$$
This argument has prototype
$codei%
	const sparse_rc& %other%
%$$
The sparsity pattern $icode pattern$$ will be the same as $icode other$$.

$head row$$
This is vector is equal to $icode row_in$$ in the constructor for
$icode pattern$$.

$head col$$
This is vector is equal to $icode col_in$$ in the constructor for
$icode pattern$$.

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
# include <cppad/core/cppad_assert.hpp> // for CPPAD_ASSERT

namespace CppAD { // BEGIN CPPAD_NAMESPACE

/// sparsity pattern for a matrix with indices of type size_t
template <class SizeVector>
class sparse_rc {
private:
	/// row_[k] is the row index for the k-th possibly non-zero entry
	const SizeVector row_;
	/// col_[k] is the column index for the k-th possibly non-zero entry
	const SizeVector col_;
public:
	/// Constructor from row and column vectors
	sparse_rc(const SizeVector& row_in, const SizeVector& col_in)
	: row_(row_in), col_(col_in)
	{	CPPAD_ASSERT_KNOWN(
			row_in.size() == col_in.size(),
			"CppAD::sparse_rc constructor: row and col size are not equal"
		);
	}
	/// Constructor from another sparsity pattern
	sparse_rc(const sparse_rc& other)
	: row_( other.row_ ), col_( other.col_ )
	{ }
	/// row indices
	const SizeVector& row(void) const
	{	return row_; }
	/// column indices
	const SizeVector& col(void) const
	{	return col_; }
};

} // END_CPPAD_NAMESPACE
