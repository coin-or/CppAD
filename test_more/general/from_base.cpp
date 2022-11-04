// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------


/*
Old FromBase example now used just for valiadation testing
*/
# include <cppad/cppad.hpp>

bool FromBase(void)
{  bool ok = true;

   using namespace CppAD;

   // construct directly form Base where Base = double
   AD<double> x(1.);
   AD<double> y = 2.;

   // construct from a type that can be converted to Base
   // where Base = AD<double>
   AD< AD<double> > X(1.);
   AD< AD<double> > Y(2);

   // check that resulting objects are parameters
   ok &= Parameter(x);
   ok &= Parameter(y);

   ok &= Parameter(X);
   ok &= Parameter(Y);

   // check values of objects
   ok &= (x == 1.);
   ok &= (X == x);

   ok &= (y == 2.);
   ok &= (Y == y);

   // user constructor through the static_cast template function
   x   = static_cast < AD<double> >( 4 );
   X   = static_cast < AD< AD<double> > >( 4 );

   ok &= (x == 4.);
   ok &= (X == x);

   return ok;
}

// END PROGRAM
