// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin exp_2_rev2.cpp}

exp_2: Verify Second Order Reverse Sweep
########################################

{xrst_spell_off}
{xrst_code cpp} */
# include <cstddef>                 // define size_t
# include <cmath>                   // prototype for fabs
extern bool exp_2_for0(double *v0); // computes zero order forward sweep
extern bool exp_2_for1(double *v1); // computes first order forward sweep
bool exp_2_rev2(void)
{  bool ok = true;

   // set the value of v0[j], v1[j] for j = 1 , ... , 5
   double v0[6], v1[6];
   ok &= exp_2_for0(v0);
   ok &= exp_2_for1(v1);

   // initial all partial derivatives as zero
   double f_v0[6], f_v1[6];
   size_t j;
   for(j = 0; j < 6; j++)
   {  f_v0[j] = 0.;
      f_v1[j] = 0.;
   }

   // set partial derivative for f_5
   f_v1[5] = 1.;
   ok &= std::fabs( f_v1[5] - 1. ) <= 1e-10; // partial f_5 w.r.t v_5^1

   // f_4 = f_5( v_1^0 , ... , v_4^1 , v_2^0 + v_4^0 , v_2^1 + v_4^1 )
   f_v0[2] += f_v0[5] * 1.;
   f_v0[4] += f_v0[5] * 1.;
   f_v1[2] += f_v1[5] * 1.;
   f_v1[4] += f_v1[5] * 1.;
   ok &= std::fabs( f_v0[2] - 0. ) <= 1e-10; // partial f_4 w.r.t. v_2^0
   ok &= std::fabs( f_v0[4] - 0. ) <= 1e-10; // partial f_4 w.r.t. v_4^0
   ok &= std::fabs( f_v1[2] - 1. ) <= 1e-10; // partial f_4 w.r.t. v_2^1
   ok &= std::fabs( f_v1[4] - 1. ) <= 1e-10; // partial f_4 w.r.t. v_4^1

   // f_3 = f_4( v_1^0 , ... , v_3^1, v_3^0 / 2 , v_3^1 / 2 )
   f_v0[3] += f_v0[4] / 2.;
   f_v1[3] += f_v1[4] / 2.;
   ok &= std::fabs( f_v0[3] - 0.  ) <= 1e-10; // partial f_3 w.r.t. v_3^0
   ok &= std::fabs( f_v1[3] - 0.5 ) <= 1e-10; // partial f_3 w.r.t. v_3^1

   // f_2 = f_3(  v_1^0 , ... , v_2^1, v_1^0 * v_1^0 , 2 * v_1^0 * v_1^1 )
   f_v0[1] += f_v0[3] * 2. * v0[1];
   f_v0[1] += f_v1[3] * 2. * v1[1];
   f_v1[1] += f_v1[3] * 2. * v0[1];
   ok &= std::fabs( f_v0[1] - 1.  ) <= 1e-10; // partial f_2 w.r.t. v_1^0
   ok &= std::fabs( f_v1[1] - 0.5 ) <= 1e-10; // partial f_2 w.r.t. v_1^1

   // f_1 = f_2( v_1^0 , v_1^1 , 1 + v_1^0 , v_1^1 )
   f_v0[1] += f_v0[2] * 1.;
   f_v1[1] += f_v1[2] * 1.;
   ok &= std::fabs( f_v0[1] - 1. ) <= 1e-10; // partial f_1 w.r.t. v_1^0
   ok &= std::fabs( f_v1[1] - 1.5) <= 1e-10; // partial f_1 w.r.t. v_1^1

   return ok;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end exp_2_rev2.cpp}
*/
