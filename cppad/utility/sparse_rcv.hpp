# ifndef CPPAD_UTILITY_SPARSE_RCV_HPP
# define CPPAD_UTILITY_SPARSE_RCV_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------
$begin sparse_rcv$$
$spell
	CppAD
	nr
	nc
	const
	var
	nnz
	cppad
	hpp
	rcv
	rc
$$
$section Sparse Matrix Row, Column, Value Representation$$

$head Under Construction$$
This class is under construction and not yet appropriation for public use.

$head Syntax$$
$codei%# include <cppad/utility/sparse_rcv.hpp>
%$$
$codei%sparse_rcv<%SizeVector%, %ValueVector%>  %matrix%(%pattern%)
%$$
$icode%matrix%.set(%k%, %v%)
%$$
$icode%nr% = %matrix%.nr()
%$$
$icode%nc% = %matrix%.nc()
%$$
$icode%nnz% = %matrix%.nnz()
%$$
$codei%const %SizeVector%& %row%( %matrix%.row() )
%$$
$codei%const %SizeVector%& %col%( %matrix%.col() )
%$$
$codei%const %ValueVector%& %val%( %matrix%.val() )
%$$

$head SizeVector$$
We use $cref/SizeVector/sparse_rc/SizeVector/$$ to denote the
$cref SimpleVector$$ class corresponding to $icode pattern$$.

$head ValueVector$$
We use $icode ValueVector$$ to denote the
$cref SimpleVector$$ class corresponding to $icode val$$.

$head pattern$$
This argument has prototype
$codei%
	const sparse_rc<%SizeVector%>& %pattern%
%$$
It specifies the possibly non-zero entries in the $icode matrix$$.

$head matrix$$
This is a sparse matrix object with the sparsity specified by $icode pattern$$.
It is $code const$$ for all the operations except for
the constructor and $code set$$.

$head nr$$
This return value has prototype
$codei%
	size_t %nr%
%$$
and is the number of rows in $icode matrix$$.

$head nc$$
This argument and return value has prototype
$codei%
	size_t %nc%
%$$
and is the number of columns in $icode matrix$$.

$head nnz$$
We use the notation $icode nnz$$ to denote the number of
possibly non-zero entries in $icode matrix$$.

$head k$$
This argument has type
$codei%
	size_t %k%
%$$
and must be less than $icode nnz$$.

$head v$$
This argument has type
$codei%
	%ValueVector%::value_type
%$$
It specifies the value assigned to $icode%val%[%k%]%$$.

$head set$$
This function sets the value
$codei%
	%val%[%k%] = %v%
%$$

$head row$$
This vector has size $icode nnz$$ and
$icode%row%[%k%]%$$
is the row index of the $th k$$ possibly non-zero
element in $icode matrix$$.

$head col$$
This vector has size $icode nnz$$ and
$icode%col[%k%]%$$ is the column index of the $th k$$ possibly non-zero
element in $icode matrix$$

$head val$$
This vector has size $icode nnz$$ and
$icode%val[%k%]%$$ is value of the $th k$$ possibly non-zero entry
in the sparse matrix.

$children%
	example/utility/sparse_rcv.cpp
%$$
$head Example$$
The file $cref sparse_rcv.cpp$$
contains an example and test of this class.
It returns true if it succeeds and false otherwise.

$end
*/
/*!
\file sparse_rcv.hpp
A sparse matrix class.
*/
# include <cppad/utility/sparse_rc.hpp>

namespace CppAD { // BEGIN CPPAD_NAMESPACE

/// Sparse matrices with elements of type Scalar
template <class SizeVector, class ValueVector>
class sparse_rcv {
private:
	/// sparsity pattern
	const sparse_rc<SizeVector> pattern_;
	/// value_type
	typedef typename ValueVector::value_type value_type;
	/// val_[k] is the value for the k-th possibly non-zero entry in the matrix
	ValueVector    val_;
public:
	// ------------------------------------------------------------------------
	/// constructor
	sparse_rcv(const sparse_rc<SizeVector>& pattern )
	:
	pattern_(pattern)    ,
	val_(pattern_.nnz())
	{ }
	// ------------------------------------------------------------------------
	void set(size_t k, const value_type& v)
	{	CPPAD_ASSERT_KNOWN(
			pattern_.nnz(),
			"The index k is not less than nnz in sparse_rcv::set"
		);
		val_[k] = v;
	}
	size_t nr(void) const
	{	return pattern_.nr(); }
	size_t nc(void) const
	{	return pattern_.nc(); }
	size_t nnz(void) const
	{	return pattern_.nnz(); }
	const SizeVector& row(void) const
	{	return pattern_.row(); }
	const SizeVector& col(void) const
	{	return pattern_.col(); }
	const ValueVector& val(void) const
	{	return val_; }
};

} // END_CPPAD_NAMESPACE

# endif
