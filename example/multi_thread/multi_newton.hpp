# ifndef CPPAD_EXAMPLE_MULTI_THREAD_MULTI_NEWTON_HPP
# define CPPAD_EXAMPLE_MULTI_THREAD_MULTI_NEWTON_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

bool multi_newton_time(
   double& time_out      ,
   double  test_time     ,
   size_t  num_threads   ,
   size_t  num_zero      ,
   size_t  num_sub       ,
   size_t  num_sum       ,
   bool    use_ad
);

# endif
