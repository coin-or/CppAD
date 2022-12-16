// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin exp_2_for2.cpp}

exp_2: Verify Second Order Forward Sweep
########################################

{xrst_spell_off}
{xrst_code cpp} */
# include <cmath>                   // prototype for fabs
extern bool exp_2_for0(double *v0); // computes zero order forward sweep
extern bool exp_2_for1(double *v1); // computes first order forward sweep
bool exp_2_for2(void)
{  bool ok = true;
   double v0[6], v1[6], v2[6];

   // set the value of v0[j], v1[j], for j = 1 , ... , 5
   ok &= exp_2_for0(v0);
   ok &= exp_2_for1(v1);

   v2[1] = 0.;                                     // v1 = x
   ok    &= std::fabs( v2[1] - 0. ) <= 1e-10;

   v2[2] = v2[1];                                  // v2 = 1 + v1
   ok    &= std::fabs( v2[2] - 0. ) <= 1e-10;

   v2[3] = 2.*(v0[1]*v2[1] + v1[1]*v1[1]);         // v3 = v1 * v1
   ok    &= std::fabs( v2[3] - 2. ) <= 1e-10;

   v2[4] = v2[3] / 2.;                             // v4 = v3 / 2
   ok    &= std::fabs( v2[4] - 1. ) <= 1e-10;

   v2[5] = v2[2] + v2[4];                          // v5 = v2 + v4
   ok    &= std::fabs( v2[5] - 1. ) <= 1e-10;

   return ok;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end exp_2_for2.cpp}
*/
