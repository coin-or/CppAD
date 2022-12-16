// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin exp_eps_for1.cpp}

exp_eps: Verify First Order Forward Sweep
#########################################

{xrst_spell_off}
{xrst_code cpp} */
# include <cmath>                     // for fabs function
extern bool exp_eps_for0(double *v0); // computes zero order forward sweep
bool exp_eps_for1(double *v1)         // double v[8]
{  bool ok = true;
   double v0[8];

   // set the value of v0[j] for j = 1 , ... , 7
   ok &= exp_eps_for0(v0);

   v1[1] = 1.;                                      // v1 = x
   ok    &= std::fabs( v1[1] - 1. ) <= 1e-10;

   v1[2] = 1. * v1[1];                              // v2 = 1 * v1
   ok    &= std::fabs( v1[2] - 1. ) <= 1e-10;

   v1[3] = v1[2] / 1.;                              // v3 = v2 / 1
   ok    &= std::fabs( v1[3] - 1. ) <= 1e-10;

   v1[4] = v1[3];                                   // v4 = 1 + v3
   ok    &= std::fabs( v1[4] - 1. ) <= 1e-10;

   v1[5] = v1[3] * v0[1] + v0[3] * v1[1];           // v5 = v3 * v1
   ok    &= std::fabs( v1[5] - 1. ) <= 1e-10;

   v1[6] = v1[5] / 2.;                              // v6 = v5 / 2
   ok    &= std::fabs( v1[6] - 0.5 ) <= 1e-10;

   v1[7] = v1[4] + v1[6];                           // v7 = v4 + v6
   ok    &= std::fabs( v1[7] - 1.5 ) <= 1e-10;

   return ok;
}
bool exp_eps_for1(void)
{  double v1[8];
   return exp_eps_for1(v1);
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end exp_eps_for1.cpp}
*/
