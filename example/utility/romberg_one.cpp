// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin romberg_one.cpp}

One Dimensional Romberg Integration: Example and Test
#####################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end romberg_one.cpp}
*/
// BEGIN C++

# include <cppad/utility/romberg_one.hpp>
# include <cppad/utility/vector.hpp>
# include <cppad/utility/near_equal.hpp>

namespace {
   class Fun {
   private:
      const size_t degree;
   public:
      // constructor
      Fun(size_t degree_) : degree(degree_)
      { }

      // function F(x) = x^degree
      template <class Type>
      Type operator () (const Type &x)
      {  size_t i;
         Type   f = 1;
         for(i = 0; i < degree; i++)
            f *= x;
         return f;
      }
   };
}

bool RombergOne(void)
{  bool ok = true;
   size_t i;

   size_t degree = 4;
   Fun F(degree);

   // arguments to RombergOne
   double a = 0.;
   double b = 1.;
   size_t n = 4;
   size_t p;
   double r, e;

   // int_a^b F(x) dx = [ b^(degree+1) - a^(degree+1) ] / (degree+1)
   double bpow = 1.;
   double apow = 1.;
   for(i = 0; i <= degree; i++)
   {  bpow *= b;
      apow *= a;
   }
   double check = (bpow - apow) / double(degree+1);

   // step size corresponding to r
   double step = (b - a) / exp(log(2.)*double(n-1));
   // step size corresponding to error estimate
   step *= 2.;
   // step size raised to a power
   double spow = 1;

   for(p = 0; p < n; p++)
   {  spow = spow * step * step;

      r = CppAD::RombergOne(F, a, b, n, p, e);

      ok  &= e < double(degree+1) * spow;
      ok  &= CppAD::NearEqual(check, r, 0., e);
   }

   return ok;
}

// END C++
