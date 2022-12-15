// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin exp_2_for0.cpp}

exp_2: Verify Zero Order Forward Sweep
######################################

{xrst_spell_off}
{xrst_code cpp} */
# include <cmath>            // for fabs function
bool exp_2_for0(double *v0)  // double v0[6]
{  bool  ok = true;
   double x = .5;

   v0[1] = x;                                  // v1 = x
   ok  &= std::fabs( v0[1] - 0.5) < 1e-10;

   v0[2] = 1. + v0[1];                         // v2 = 1 + v1
   ok  &= std::fabs( v0[2] - 1.5) < 1e-10;

   v0[3] = v0[1] * v0[1];                      // v3 = v1 * v1
   ok  &= std::fabs( v0[3] - 0.25) < 1e-10;

   v0[4] = v0[3] / 2.;                         // v4 = v3 / 2
   ok  &= std::fabs( v0[4] - 0.125) < 1e-10;

   v0[5] = v0[2] + v0[4];                      // v5  = v2 + v4
   ok  &= std::fabs( v0[5] - 1.625) < 1e-10;

   return ok;
}
bool exp_2_for0(void)
{  double v0[6];
   return exp_2_for0(v0);
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end exp_2_for0.cpp}
*/
