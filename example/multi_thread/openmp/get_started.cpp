// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin openmp_get_started.cpp}
{xrst_spell
   fun fun
}

Getting Started Using OpenMP With CppAD
#######################################

in_parallel
***********
see :ref:`ta_parallel_setup@in_parallel` .

thread_number
*************
see :ref:`ta_parallel_setup@thread_num` .

ADFun Constructor
*****************
If you use the :ref:`fun_construct@Sequence Constructor` for the original
function, you will need to clear the Taylor coefficient memory associated
with the function using :ref:`capacity_order-name` ; e.g. ::

   CppAD::ADFun fun(ax, ay);
   fun.capacity_order(0);

If you do not free the Taylor coefficient memory in ``fun`` ,
the function assignments will allocate zero order Taylor coefficients for each
function in ``fun_thread`` using thread zero. Depending on what you do in
parallel mode, you may attempt to free that memory using another thread.
For example, if you change USE_DEFAULT_ADFUN_CONSTRUCTOR to 0, you will
get the message::

   Attempt to return memory for a different thread while in parallel mode

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end openmp_get_started.cpp}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <omp.h>

# define USE_DEFAULT_ADFUN_CONSTRUCTOR 1

namespace {
   //
   // d_vector, ad_vector, fun_vector
   typedef CPPAD_TESTVECTOR(double)                  d_vector;
   typedef CPPAD_TESTVECTOR( CppAD::AD<double> )    ad_vector;
   typedef CPPAD_TESTVECTOR( CppAD::ADFun<double> ) fun_vector;
   //
   // in_parallel
   bool in_parallel(void)
   {  return omp_in_parallel() != 0;
   }
   //
   // thread_number
   size_t thread_number(void)
   {  return static_cast<size_t>( omp_get_thread_num() );
   }
   //
   // partial
   double partial(
      CppAD::ADFun<double>& f, size_t j, const d_vector& x
   )
   {  // f
      // This will cause an assert if USE_DEFAULT_ADFUN_CONSTRUCTOR is 0.
      f.capacity_order(0);
      //
      size_t nx = x.size();
      d_vector dx(nx), dy(1);
      for(size_t k = 0; k < nx; ++k)
         dx[k] = 0.0;
      dx[j] = 1.0;
      f.Forward(0, x);
      dy = f.Forward(1, dx);
      return dy[0];
   }
}
bool get_started(void)
{  // ok
   bool ok = true;
   //
   // eps99
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   //
   // nx, ax
   size_t nx = 10;
   ad_vector ax(nx);
   for(size_t j = 0; j < nx; ++j)
      ax[j] = 1.0;
   CppAD::Independent(ax);
   //
   // fun
   ad_vector  ay(1);
   ay[0] = ax[0];
   for(size_t j = 1; j < nx; ++j)
      ay[0] *= ax[j];
# if USE_DEFAULT_ADFUN_CONSTRUCTOR
   CppAD::ADFun<double> fun;
   fun.Dependent(ax, ay);
# else
   CppAD::ADFun<double> fun(ax, ay);
# endif
   //
   // num_threads, f_thread
   size_t num_threads = 4;
   fun_vector f_thread(num_threads);
   for(size_t i = 0; i < num_threads; ++i)
      f_thread[i] = fun;
   //
   // x
   d_vector x(nx);
   for(size_t j = 0; j < nx; ++j)
      ax[j] = 1.0 + 1.0 / double(j+1);
   //
   // parallel_setup
   omp_set_num_threads( int(num_threads) );
   ok &= ! in_parallel();
   CppAD::thread_alloc::parallel_setup(
      num_threads, in_parallel, thread_number
   );
   //
   // parallel_ad
   CppAD::parallel_ad<double>();
   //
   // hold_memory
   // optional and may improve speed if you do a lot of memory allocation
   CppAD::thread_alloc::hold_memory(true);
   //
   // Jac
   ad_vector Jac(nx);
   size_t j = 0;
# pragma omp parallel for
   for(j = 0; j < nx; ++j)
   {  size_t thread_num        = thread_number();
      CppAD::ADFun<double>& f  = f_thread[thread_num];
      Jac[j] = partial(f, j, x);
   }
   //
   // hold_memory
   // free memory for other threads before this (the master thread)
   CppAD::thread_alloc::parallel_setup(1, nullptr, nullptr);
   ok &= ! in_parallel();
   CppAD::thread_alloc::hold_memory(false);
   for(size_t i = 1; i < num_threads; ++i)
      CppAD::thread_alloc::free_available(i);
   CppAD::thread_alloc::free_available(0);
   //
   // j
   for(j = 0; j < nx; ++j)
   {  //
      // check
      double check = 1.0;
      for(size_t k = 0; k < nx; ++k)
         if(k != j)
            check *= x[j];
      //
      // ok
      ok &= CppAD::NearEqual(Jac[j], check, eps99, eps99);
   }
   return ok;
}
// END C++
