// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
Test of Eigen Interface to CppAD Scalar Types
$end
*/
# include <cppad/example/cppad_eigen.hpp>

bool cppad_eigen(void)
{  bool ok = true;
   using CppAD::AD;
   using Eigen::Dynamic;
   using Eigen::Matrix;

   typedef Eigen::NumTraits<AD<double> >         traits;

   ok &= traits::IsComplex              == 0;
   ok &= traits::IsInteger              == 0;
   ok &= traits::IsSigned               == 1;
   ok &= traits::RequireInitialization  == 1;
   ok &= traits::ReadCost               == 1;
   ok &= traits::AddCost                == 2;
   ok &= traits::MulCost                == 2;

   ok &= traits::epsilon() ==
      std::numeric_limits<double>::epsilon();
   ok &= traits::dummy_precision() ==
      100.* std::numeric_limits<double>::epsilon();
   ok &= traits::highest() ==
      std::numeric_limits<double>::max();
   ok &= traits::lowest() ==
      std::numeric_limits<double>::min();
   ok &= std::isnan(traits::quiet_NaN());
   ok &= std::isinf(traits::infinity());

   AD<double> x = 2.0;
   ok  &= conj(x)  == x;
   ok  &= real(x)  == x;
   ok  &= imag(x)  == 0.0;
   ok  &= abs2(x)  == 4.0;

   ok  &= (!std::isinf(x));
   ok  &= (!std::isnan(x));

   x = traits::quiet_NaN();
   ok  &= std::isnan(x);

   x = traits::infinity();
   ok  &= std::isinf(x);

   // Outputing a matrix used to fail before partial specialization of
   // struct significant_decimals_default_impl in cppad_eigen.hpp.
   Matrix< AD<double>, 1, 1> X;
   X(0, 0) = AD<double>(1);
   std::stringstream stream_out;
   stream_out << X;
   ok &= "1" == stream_out.str();

   // multiplying three matrices together used to cause warning
   // before making ctor from arbitrary type to AD<Base> explicit.
   typedef CppAD::AD<double> AScalar;
   Matrix<AScalar, Dynamic, Dynamic> A(1,1), B(1,1), C(1,1), D(1,1);
   A(0,0) = 1.0;
   B(0,0) = 2.0;
   C(0,0) = 3.0;
   D      = A * B * C;
   ok    &= D(0,0) == 6.0 ;

   // Multiplying Eigen objects (such as matrices) of element types used to
   // fail before appropriate "ScalarBinaryOpTraits" was added.
   const int nn = 3;
   Matrix<double,  nn, nn> d_matrix = Matrix<double,  nn, nn>::Zero();
   Matrix<AScalar, nn, nn> a_matrix = Matrix<AScalar, nn, nn>::Zero();
   a_matrix  = a_matrix * d_matrix;
   a_matrix  = d_matrix * a_matrix;
   ok       &= a_matrix(0, 0) == AScalar(0);

   // Test multiply matrix elements of mixed types
   // (This worked before corresponding ScalarBianryOpTraits was added.)
   AScalar a_scalar = a_matrix(0,0) * d_matrix(0,0);
   ok              &= a_scalar == AScalar(0);
   a_scalar         = d_matrix(0, 0) * a_matrix(0, 0);
   ok              &= a_scalar == AScalar(0);

   return ok;
}
