// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
Check comparison operators between AD< AD<Base> > and Base, int
*/
# include <cppad/cppad.hpp>

namespace {
   // ---------------------------------------------------------------------
   bool compare_parameter(bool optimize)
   {  bool ok = true;
      using CppAD::AD;
      //
      // f
      CPPAD_TESTVECTOR( AD<double> ) ax(1), ap(1), ay(1);
      ax[0] = 1.0;
      ap[0] = 3.0;
      CppAD::Independent( ax, ap );
      if( ap[0] < 2.0 )
         ay[0] = ax[0] + ax[0];
      else
         ay[0] = ax[0] * ax[0];
      CppAD::ADFun<double> f(ax, ay);
      if( optimize )
         f.optimize();
      //
      // x, y, ok
      CPPAD_TESTVECTOR(double) x(1), y(1);
      x[0] = 4.0;
      y    = f.Forward(0, x);
      ok  &= y[0] == x[0] * x[0];
      ok &= f.compare_change_number() == 0;
      //
      // p, ok
      CPPAD_TESTVECTOR(double) p(1);
      p[0] = 1.0;
      f.new_dynamic(p);
      y    = f.Forward(0, x);
      ok  &= y[0] == x[0] * x[0];
      ok &= f.compare_change_number() == 1;
      //
      return ok;
   }
   // ---------------------------------------------------------------------
   template <class Type>
   bool compare_type(void)
   {  bool ok = true;
      using CppAD::AD;

      Type      middle = 4;
      AD<double> three = 3;
      AD<double> four  = 4;
      AD<double> five  = 5;

      // AD<double> > Type
      ok &= ! (three  >  middle);
      ok &= ! (four   >  middle);
      ok &=   (five   >  middle);
      // Type > AD<double>
      ok &=   (middle >  three );
      ok &= ! (middle >  four  );
      ok &= ! (middle >  five  );

      // AD<double> >= Type
      ok &= ! (three  >= middle);
      ok &=   (four   >= middle);
      ok &=   (five   >= middle);
      // Type > AD<double>
      ok &=   (middle >= three );
      ok &=   (middle >= four  );
      ok &= ! (middle >= five  );

      // AD<double> < Type
      ok &=   (three  <  middle);
      ok &= ! (four   <  middle);
      ok &= ! (five   <  middle);
      // Type > AD<double>
      ok &= ! (middle <  three );
      ok &= ! (middle <  four  );
      ok &=   (middle <  five  );

      // AD<double> <= Type
      ok &=   (three  <= middle);
      ok &=   (four   <= middle);
      ok &= ! (five   <= middle);
      // Type > AD<double>
      ok &= ! (middle <= three );
      ok &=   (middle <= four  );
      ok &=   (middle <= five  );

      return ok;
   }
}
bool Compare(void)
{  bool ok = true;
   ok  &= compare_type<int>();
   ok  &= compare_type<double>();
   ok  &= compare_type< CppAD::AD<double> >();
   ok  &= compare_parameter(true);
   ok  &= compare_parameter(false);
   return ok;
}
