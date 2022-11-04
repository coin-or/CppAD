# ifndef CPPAD_CORE_TESTVECTOR_HPP
# define CPPAD_CORE_TESTVECTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin testvector$$
$spell
    CppAD
    cmake
    testvector
    cppad
    Eigen
    ifdef
    hpp
    std
    endif
    ublas
$$


$section Using The CppAD Test Vector Template Class$$

$head Syntax$$
$codei%CPPAD_TESTVECTOR(%Scalar%)
%$$

$head Choice$$
The user can choose, during the install procedure,
which template class to use in the examples and tests; see below.
This shows that any
$cref/simple vector/SimpleVector/$$ class can be used in place of
$codei%
    CPPAD_TESTVECTOR(%Type%)
%$$
When writing their own code,
users can choose a specific simple vector they prefer; for example,
$codei%
    CppAD::vector<%Type%>
%$$


$head CppAD::vector$$
If in the $cref/cmake command/cmake/CMake Command/$$
you specify $cref cppad_testvector$$ to be $code cppad$$,
$codei%
# define CPPAD_TESTVECTOR(%Scalar%) CppAD::vector< %Scalar% >
%$$

$subhead CPPAD_CPPADVECTOR, Deprecated 2022-06-22$$
This symbol is 1 (0) if the definition above
is used (is not used) for $code CPPAD_TESTVECTOR$$.

$head std::vector$$
If in the cmake command
you specify $icode cppad_testvector$$ to be $code std$$,
$codei%
# define CPPAD_TESTVECTOR(%Scalar%) std::vector< %Scalar% >
%$$

$subhead CPPAD_STDVECTOR, Deprecated 2022-06-22$$
This symbol is 1 (0) if the definition above
is used (is not used) for $code CPPAD_TESTVECTOR$$.

$head boost::numeric::ublas::vector$$
If in the cmake command
you specify $icode cppad_testvector$$ to be $code boost$$,
$codei%
# define CPPAD_TESTVECTOR(%Scalar%) boost::numeric::ublas::vector< %Scalar% >
%$$

$subhead CPPAD_BOOSTVECTOR, Deprecated 2022-06-22$$
This symbol is 1 (0) if the definition above
is used (is not used) for $code CPPAD_TESTVECTOR$$.

$head CppAD::eigen_vector$$
If in the cmake command
you specify $icode cppad_testvector$$ to be $code eigen$$,
$codei%
# define CPPAD_TESTVECTOR(%Scalar%) CppAD::eigen_vector< %Scalar% >
%$$
see $cref/eigen_vector/cppad_eigen.hpp/eigen_vector/$$.
In this case CppAD will use the Eigen vector
for many of its examples and tests.

$subhead CPPAD_EIGENVECTOR, Deprecated 2022-06-22$$
This symbol is 1 (0) if the definition above
is used (is not used) for $code CPPAD_TESTVECTOR$$.

$end
------------------------------------------------------------------------
*/
# if CPPAD_CPPADVECTOR
# define CPPAD_TESTVECTOR(Scalar) CppAD::vector< Scalar >
# endif
//
# if CPPAD_STDVECTOR
# include <vector>
# define CPPAD_TESTVECTOR(Scalar) std::vector< Scalar >
# endif
//
# if CPPAD_BOOSTVECTOR
# include <boost/numeric/ublas/vector.hpp>
# define CPPAD_TESTVECTOR(Scalar) boost::numeric::ublas::vector< Scalar >
# endif
//
# if CPPAD_EIGENVECTOR
# include <cppad/example/cppad_eigen.hpp>
# define CPPAD_TESTVECTOR(Scalar) CppAD::eigen_vector< Scalar >
# endif
//
# endif
