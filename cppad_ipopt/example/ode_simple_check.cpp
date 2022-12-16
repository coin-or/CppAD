// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "ode_run.hpp"
# include "ode_simple.hpp"
# include "ode_check.hpp"

bool ode_simple_check(void)
{  bool ok = true;
   bool retape;
   size_t i;

   // solution vector
   NumberVector x;

   // number of time grid intervals between measurement values
   SizeVector N(Nz + 1);
   N[0] = 0;
   for(i = 1; i <= Nz; i++)
      N[i] = 4;

   for(i = 0; i < 2; i++)
   {  retape = bool(i);
      ipopt_ode_case<FG_simple>(retape, N, x);
      ok &= ode_check(N, x);
   }

   return ok;
}
