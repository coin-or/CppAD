// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin pthread_get_started.cpp}
{xrst_spell
   posix
}

{xrst_template ,
   example/multi_thread/get_started.xrst
   @Name@    , Posix
   @####@    , #####
   @DEFAULT@ , USE_DEFAULT_ADFUN_CONSTRUCTOR
}

{xrst_end pthread_get_started.cpp}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <pthread.h>

# define USE_DEFAULT_ADFUN_CONSTRUCTOR 1

namespace {
   //
   // d_vector, ad_vector, fun_vector
   typedef CPPAD_TESTVECTOR(double)                 d_vector;
   typedef CPPAD_TESTVECTOR( CppAD::AD<double> )    ad_vector;
   typedef CPPAD_TESTVECTOR( CppAD::ADFun<double> ) fun_vector;
   typedef CPPAD_TESTVECTOR( pthread_t )            pthread_vector;
   //
   // thread_info, thread_info_vector
   typedef struct {
      size_t                thread_num;
      CppAD::ADFun<double>* f_ptr;
      size_t                j_begin;
      size_t                j_end;
      const d_vector*       x_ptr;
      d_vector*             Jac_ptr;
      bool                  ok;
   } thread_info;
   typedef CPPAD_TESTVECTOR( thread_info ) thread_info_vector;
   //
   // thread_specific_key_
   pthread_key_t thread_specific_key_;
   //
   // thread_specific_destructor
   void thread_specific_destructor(void* thread_num_vptr)
   {  return; }
   //
   // sequential_execution_
   bool sequential_execution_ = true;
   //
   // in_parallel
   bool in_parallel(void)
   {  return ! sequential_execution_; }
   //
   // thread_number
   size_t thread_number(void)
   {  // get thread specific information
      void*   thread_num_vptr = pthread_getspecific(thread_specific_key_);
      size_t* thread_num_ptr  = static_cast<size_t*>(thread_num_vptr);
      size_t  thread_num      = *thread_num_ptr;
      return thread_num;
   }
   //
   // partial
   double partial(
      CppAD::ADFun<double>& f, size_t j, const d_vector& x
   )
   {  size_t nx = x.size();
      d_vector dx(nx), dy(1);
      for(size_t k = 0; k < nx; ++k)
         dx[k] = 0.0;
      dx[j] = 1.0;
      f.Forward(0, x);
      dy = f.Forward(1, dx);
      return dy[0];
   }
   //
   // run_one_thread
   void* run_one_thread(void* thread_info_vptr)
   {  //
      // thread_num, f_ptr, j_begin, j_end, x_ptr, Jac_ptr
      thread_info* thread_info_ptr =
         static_cast<thread_info*>(thread_info_vptr);
      size_t                thread_num = thread_info_ptr->thread_num;
      CppAD::ADFun<double>* f_ptr      = thread_info_ptr->f_ptr;
      size_t                j_begin    = thread_info_ptr->j_begin;
      size_t                j_end      = thread_info_ptr->j_end;
      const d_vector*       x_ptr      = thread_info_ptr->x_ptr;
      d_vector*             Jac_ptr    = thread_info_ptr->Jac_ptr;
      //
      // f, Jac, ok
      CppAD::ADFun<double>& f   = *f_ptr;
      d_vector&             Jac = *Jac_ptr;
      bool&                 ok  = thread_info_ptr->ok;
      //
      // rc
      int rc;
      //
      // phtread_setspecific, ok
      // This sets up the thread_number function for this thread.
      if( thread_num != 0 )
      {  rc  = pthread_setspecific(thread_specific_key_, &thread_num);
         ok &= rc == 0;
         ok &= thread_number() == thread_num;
      }
      //
      // f
      // This will cause an assert if USE_DEFAULT_ADFUN_CONSTRUCTOR is 0.
      // (The assert uses the thread_number function for this thread.)
      f.capacity_order(0);
      //
      // Jac
      for(size_t j = j_begin; j < j_end; ++j)
         Jac[j] = partial(f, j, *x_ptr);
      //
      return nullptr;
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
      x[j] = 1.0 + 1.0 / double(j+1);
   //
   // Jac
   d_vector Jac(nx);
   //
   // n_per_thread, n_extra
   size_t n_per_thread = nx / num_threads;
   size_t n_extra      = nx % num_threads;
   //
   // thread_info_vec
   thread_info_vector thread_info_vec(num_threads);
   size_t j_begin = 0;
   size_t j_end;
   for(size_t thread_num = 0; thread_num < num_threads; ++thread_num)
   {  j_end = j_begin + n_per_thread;
      if( thread_num < n_extra )
         ++j_end;
      //
      thread_info_vec[thread_num].thread_num = thread_num;
      thread_info_vec[thread_num].f_ptr      = &f_thread[thread_num];
      thread_info_vec[thread_num].j_begin    = j_begin;
      thread_info_vec[thread_num].j_end      = j_end;
      thread_info_vec[thread_num].x_ptr      = &x;
      thread_info_vec[thread_num].Jac_ptr    = &Jac;
      thread_info_vec[thread_num].ok         = true;
      //
      j_begin = j_end;
   }
   ok &= j_end == nx;
   //
   // rc
   int rc;
   //
   // thread_specific_key_, ok
   rc  = pthread_key_create(&thread_specific_key_, thread_specific_destructor);
   ok &= rc == 0;

   //
   // thread_setspecific, ok
   // must be set for this thread before calling parall_setup or parallel_ad
   rc = pthread_setspecific(
         thread_specific_key_, &thread_info_vec[0].thread_num
   );
   ok &= rc == 0;
   ok &= thread_number() == 0;
   //
   // parallel_setup
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
   // ptread_vec
   pthread_vector pthread_vec(num_threads - 1);
   //
   // sequential_execution_, ok
   sequential_execution_ = false;
   ok &= in_parallel();
   //
   // Jac, pthread_vec, ok
   // Launch num_threads - 1 posix threads
   for(size_t thread_num = 1; thread_num < num_threads; ++thread_num)
   {  pthread_attr_t* no_attr = nullptr;
      rc = pthread_create(
         &pthread_vec[thread_num-1],
         no_attr,
         run_one_thread,
         &thread_info_vec[thread_num]
      );
      ok &= rc == 0;
   }
   {  // run master thread's indices
      size_t thread_num = 0;
      run_one_thread(&thread_info_vec[thread_num]);
   }
   // wait for other threads to finish
   for(size_t thread_num = 1; thread_num < num_threads; ++thread_num)
   {  void* no_status = nullptr;
      rc  = pthread_join(pthread_vec[thread_num-1], &no_status);
      ok &= rc == 0;
   }
   //
   // sequential_execution_, ok
   sequential_execution_ = true;
   CppAD::thread_alloc::parallel_setup(1, nullptr, nullptr);
   ok &= ! in_parallel();
   //
   // hold_memory, ok
   // free memory for other threads before this (the master thread)
   ok &= thread_number() == 0;
   CppAD::thread_alloc::hold_memory(false);
   for(size_t thread_num = 1; thread_num < num_threads; ++thread_num)
   {  ok &= thread_info_vec[thread_num].ok;
      CppAD::thread_alloc::free_available(thread_num);
   }
   ok &= thread_info_vec[0].ok;
   CppAD::thread_alloc::free_available(0);
   //
   // ok
   for(size_t j = 0; j < nx; ++j)
   {  //
      // check
      double check = 1.0;
      for(size_t k = 0; k < nx; ++k)
         if(k != j)
            check *= x[k];
      //
      // ok
      ok &= CppAD::NearEqual(Jac[j], check, eps99, eps99);
   }
   return ok;
}
// END C++
