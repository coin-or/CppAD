// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/cppad.hpp>
# include <set>
# include <vector>
# include <valarray>

namespace {
   template <class Scalar>
   void Case(const Scalar& x, const Scalar& y)
   {  using CppAD::CheckSimpleVector;

      CheckSimpleVector<Scalar, CppAD::vector<Scalar> > (x, y);
      CheckSimpleVector<Scalar, std::vector<Scalar>   > (x, y);
      CheckSimpleVector<Scalar, std::valarray<Scalar> > (x, y);
      typedef CPPAD_TESTVECTOR(Scalar) testvector;
      CheckSimpleVector<Scalar, testvector > (x, y);
   }
}
bool check_simple_vector(void)
{  // Unusal test in that CheckSimpleVector will abort if an error occurs
   Case(float(0), float(1));
   Case(double(0), double(1));
   //
   std::set<size_t> x, y;
   x.insert(1);
   y.insert(2);
   Case(x, y);
   //
   return true;
}
