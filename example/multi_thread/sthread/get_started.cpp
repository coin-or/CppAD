// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sthread_get_started.cpp}

{xrst_template ,
   example/multi_thread/template/get_started.xrst

   title: Getting Started Using @Name@ Threads With CppAD
   start source code after: // <space> BEGIN_C++
   end source code before:  // <space> END_C++

   @Name@    , Standard
   @####@    , ########
   @DEFAULT@ , USE_DEFAULT_ADFUN_CONSTRUCTOR
}

{xrst_end sthread_get_started.cpp}
------------------------------------------------------------------------------
*/
// BEGIN_C++
# include <thread>
# include <mutex>
# include <cppad/cppad.hpp>

# define USE_DEFAULT_ADFUN_CONSTRUCTOR 1

namespace {
   //
   // d_vector, ad_vector, fun_vector, sthread_vector
   typedef CPPAD_TESTVECTOR(double)                  d_vector;
   typedef CPPAD_TESTVECTOR( CppAD::AD<double> )    ad_vector;
   typedef CPPAD_TESTVECTOR( CppAD::ADFun<double> ) fun_vector;
   typedef CPPAD_TESTVECTOR( std::thread* )         sthread_vector;
   //
   // std::vector<bool> does not support the data method; see
   // https://en.cppreference.com/w/cpp/container/vector_bool
   // 'Does not necessarily store its elements as a contiguous array.'
   typedef CppAD::vector<bool>                      b_vector;
   //
   // thread_id2num_
   std::map<std::thread::id, size_t> thread_id2num_;
   //
   // begin_thread_mutex_
   std::mutex begin_thread_mutex_;
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
   {  std::thread::id thread_id = std::this_thread::get_id();
      return thread_id2num_.at( thread_id );
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
   void run_one_thread(
      size_t                thread_num     ,
      CppAD::ADFun<double>* f_ptr          ,
      size_t                j_begin        ,
      size_t                j_end          ,
      const d_vector*       x_ptr          ,
      d_vector*             Jac_ptr        ,
      bool*                 ok_ptr         )
   {  //
      // begin_thread_mutex_
      begin_thread_mutex_.lock();
      begin_thread_mutex_.unlock();
      //
      // f, x, Jac, ok
      CppAD::ADFun<double>& f   = *f_ptr;
      const d_vector&       x   = *x_ptr;
      d_vector&             Jac = *Jac_ptr;
      bool&                 ok  = *ok_ptr;
      //
      // ok
      ok &= thread_number() == thread_num;
      //
      // f
      // This will cause an assert if Taylor coefficients were allocated
      // by a different thread.
      f.capacity_order(0);
      //
      // Jac
      for(size_t j = j_begin; j < j_end; ++j)
         Jac[j] = partial(f, j, x);
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
   // This allocates memory for first order Taylor coefficients using thread 0.
   // An assert will occur at f.capacity_order(0) in run_one_thread when
   // it is called by a different thread.
   CppAD::ADFun<double> fun(ax, ay);
# endif
   //
   // num_threads, f_thread, ok_thread
   size_t num_threads = 4;
   fun_vector f_thread(num_threads);
   b_vector   ok_thread(num_threads);
   for(size_t thread_num = 0; thread_num < num_threads; ++thread_num)
   {  f_thread[thread_num] = fun;
      ok_thread[thread_num] = true;
   }
   //
   // x
   d_vector x(nx);
   for(size_t j = 0; j < nx; ++j)
      x[j] = 1.0 + 1.0 / double(j+1);
   //
   // thread_id2num_
   // must setup for this thread before calling parallel_setup
   thread_id2num_.clear();
   std::thread::id thread_id  = std::this_thread::get_id();
   size_t          thread_num = 0;
   thread_id2num_[thread_id]  = thread_num;
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
   // thread_ptr
   sthread_vector thread_ptr(num_threads - 1);
   //
   // Jac
   d_vector Jac(nx);
   //
   // n_per_thread, n_extra
   size_t n_per_thread = nx / num_threads;
   size_t n_extra      = nx % num_threads;
   //
   // sequential_execution_
   sequential_execution_ = false;
   ok &= in_parallel();
   //
   // begin_thread_mutex_
   begin_thread_mutex_.lock();
   //
   // Jac
   // Launch num_threads - 1 threads
   size_t j_begin = n_per_thread;
   size_t j_end;
   for(thread_num = 1; thread_num < num_threads; ++thread_num)
   {  j_end = j_begin + n_per_thread;
      if( thread_num <= n_extra )
         ++j_end;
      CppAD::ADFun<double>* f_ptr  = &f_thread[thread_num];
      bool*                 ok_ptr = &ok_thread[thread_num];
      thread_ptr[thread_num-1] = new std::thread(
         run_one_thread, thread_num, f_ptr, j_begin, j_end, &x, &Jac, ok_ptr
      );
      //
      // thread_id2num_
      thread_id2num_[ thread_ptr[thread_num-1]->get_id() ] = thread_num;
      //
      j_begin = j_end;
   }
   //
   // begin_thread_mutex_
   begin_thread_mutex_.unlock();
   //
   ok &= j_end == nx;
   {  // run master thread's indices
      thread_num        = 0;
      j_begin           = 0;
      j_end             = j_begin + n_per_thread;
      CppAD::ADFun<double, double>* f_ptr  = &f_thread[thread_num];
      bool*                         ok_ptr = &ok_thread[thread_num];
      run_one_thread(thread_num, f_ptr, j_begin, j_end, &x, &Jac, ok_ptr);
   }
   // wait for other threads to finish
   for(thread_num = 1; thread_num < num_threads; ++thread_num)
   {  thread_ptr[thread_num-1]->join();
      delete thread_ptr[thread_num-1];
   }
   //
   // sequential_execution_
   sequential_execution_ = true;
   CppAD::thread_alloc::parallel_setup(1, nullptr, nullptr);
   ok &= ! in_parallel();
   //
   // hold_memory
   // free memory for other threads before this (the master thread)
   ok &= thread_number() == 0;
   CppAD::thread_alloc::hold_memory(false);
   for(thread_num = 1; thread_num < num_threads; ++thread_num)
   {  CppAD::thread_alloc::free_available(thread_num);
      ok &= ok_thread[thread_num];
   }
   ok &= ok_thread[0];
   CppAD::thread_alloc::free_available(0);
   //
   // j
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
// END_C++
