# ifndef CPPAD_CORE_TESTVECTOR_HPP
# define CPPAD_CORE_TESTVECTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin testvector}
{xrst_spell
   ublas
}

Using The CppAD Test Vector Template Class
##########################################

Syntax
******
| ``CPPAD_TESTVECTOR`` ( *Scalar* )

Choice
******
The user can choose, during the install procedure,
which template class to use in the examples and tests; see below.
This shows that any
:ref:`simple vector<SimpleVector-name>` class can be used in place of

   ``CPPAD_TESTVECTOR`` ( *Type* )

When writing their own code,
users can choose a specific simple vector they prefer; for example,

   ``CppAD::vector<`` *Type* >

CppAD::vector
*************
If in the :ref:`cmake@CMake Command`
you specify :ref:`cppad_testvector-name` to be ``cppad`` ,

   # ``define CPPAD_TESTVECTOR`` ( *Scalar* ) ``CppAD::vector<`` *Scalar*  >

CPPAD_CPPADVECTOR, Deprecated 2022-06-22
========================================
This symbol is 1 (0) if the definition above
is used (is not used) for ``CPPAD_TESTVECTOR`` .

std::vector
***********
If in the cmake command
you specify *cppad_testvector* to be ``std`` ,

   # ``define CPPAD_TESTVECTOR`` ( *Scalar* ) ``std::vector<`` *Scalar*  >

CPPAD_STDVECTOR, Deprecated 2022-06-22
======================================
This symbol is 1 (0) if the definition above
is used (is not used) for ``CPPAD_TESTVECTOR`` .

boost::numeric::ublas::vector
*****************************
If in the cmake command
you specify *cppad_testvector* to be ``boost`` ,

   # ``define CPPAD_TESTVECTOR`` ( *Scalar* ) ``boost::numeric::ublas::vector<`` *Scalar*  >

CPPAD_BOOSTVECTOR, Deprecated 2022-06-22
========================================
This symbol is 1 (0) if the definition above
is used (is not used) for ``CPPAD_TESTVECTOR`` .

CppAD::eigen_vector
*******************
If in the cmake command
you specify *cppad_testvector* to be ``eigen`` ,

   # ``define CPPAD_TESTVECTOR`` ( *Scalar* ) ``CppAD::eigen_vector<`` *Scalar*  >

see :ref:`cppad_eigen.hpp@eigen_vector` .
In this case CppAD will use the Eigen vector
for many of its examples and tests.

CPPAD_EIGENVECTOR, Deprecated 2022-06-22
========================================
This symbol is 1 (0) if the definition above
is used (is not used) for ``CPPAD_TESTVECTOR`` .

{xrst_end testvector}
------------------------------------------------------------------------
*/
# include <cppad/configure.hpp>
#
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
