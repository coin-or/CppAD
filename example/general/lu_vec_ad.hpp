# ifndef CPPAD_EXAMPLE_GENERAL_LU_VEC_AD_HPP
# define CPPAD_EXAMPLE_GENERAL_LU_VEC_AD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/cppad.hpp>

namespace CppAD {
   extern CppAD::AD<double> lu_vec_ad(
      size_t                           n,
      size_t                           m,
      CppAD::VecAD<double>             &Matrix,
      CppAD::VecAD<double>             &Rhs,
      CppAD::VecAD<double>             &Result,
      CppAD::AD<double>                &logdet
   );
}

# endif
