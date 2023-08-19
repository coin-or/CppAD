// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin num_limits.cpp}

Numeric Limits: Example and Test
################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end num_limits.cpp}
*/
// BEGIN C++

# ifdef _MSC_VER
// Supress Microsoft compiler warning about possible loss of precision,
// in the constructors (when converting to std::complex<float>)
//    Float one = 1
//    Float two = 2
// 1 and 2 are small enough so no loss of precision when converting to float.
# pragma warning(disable:4244)
# endif

# include <cppad/cppad.hpp>
# include <complex>

namespace {
   typedef CppAD::AD<double> Float;
   //
   // -----------------------------------------------------------------
   bool check_epsilon(void)
   {  bool ok    = true;
      Float eps   = CppAD::numeric_limits<Float>::epsilon();
      Float eps2  = eps / 2.0;
      Float check = 1.0 + eps;
      ok         &= 1.0 !=  check;
      check       = 1.0 + eps2;
      ok         &= 1.0 == check;
      return ok;
   }
   // -----------------------------------------------------------------
   bool check_min(void)
   {  bool ok     = true;
      Float min   = CppAD::numeric_limits<Float>::min();
      Float eps   = CppAD::numeric_limits<Float>::epsilon();
      //
      Float match = (min / 100.) * 100.;
      ok         &= fabs(match / min - 1.0)  > 3.0 * eps;
      //
      match       = (min * 100.) / 100.;
      ok         &= fabs(match / min - 1.0)  < 3.0 * eps;
      return ok;
   }
   // -----------------------------------------------------------------
   bool check_max(void)
   {  bool ok     = true;
      Float max   = CppAD::numeric_limits<Float>::max();
      Float eps   = CppAD::numeric_limits<Float>::epsilon();
      //
      Float match = (max * 100.) / 100.;
      ok         &= fabs(match / max - 1.0) > 3.0 * eps;
      //
      match       = (max / 100.) * 100.;
      ok         &= fabs(match / max - 1.0) < 3.0 * eps;
      return ok;
   }
   // -----------------------------------------------------------------
   bool check_nan(void)
   {  bool ok     = true;
      Float nan   = CppAD::numeric_limits<Float>::quiet_NaN();
      ok         &= nan != nan;
      return ok;
   }
   // -----------------------------------------------------------------
   bool check_infinity(void)
   {  bool ok    = true;
      Float inf  = CppAD::numeric_limits<Float>::infinity();
      Float hun  = Float(100);
      Float tmp  = Float(0);
      tmp        = inf + hun;
      ok        &= inf == tmp;
      tmp        = inf - inf;
      ok        &= CppAD::isnan( tmp );
      return ok;
   }
   // -----------------------------------------------------------------
   bool check_digits10(void)
   {  bool ok     = true;
      Float neg_log_eps =
         - log10( CppAD::numeric_limits<Float>::epsilon() );
      int ceil_neg_log_eps =
         Integer( neg_log_eps );
      ok &= ceil_neg_log_eps == CppAD::numeric_limits<Float>::digits10;
      return ok;
   }
   // -----------------------------------------------------------------
   bool check_max_digits10(void)
   {  bool ok          = true;
      int max_digits10 = CppAD::numeric_limits<Float>::max_digits10;
      Float pi         = 4.0 * atan( Float(1.0) );
      //
      std::stringstream ss;
      ss << std::setprecision( max_digits10 ) << pi;
      //
      Float check = Float( std::atof( ss.str().c_str() ) );
      ok         &= pi == check;
      return ok;
   }
}

bool num_limits(void)
{  bool ok = true;

   ok &= check_epsilon();
   ok &= check_min();
   ok &= check_max();
   ok &= check_nan();
   ok &= check_infinity();
   ok &= check_digits10();
   ok &= check_max_digits10();

   return ok;
}
// END C++
