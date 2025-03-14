// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin multi_chkpoint_one_algo app}

chkpoint_one Algorithm that Computes Square Root
################################################

Syntax
******
| *checkpoint_algo* ( *au* , *ay* )

Purpose
*******
This algorithm computes a square root using Newton's method.
It is meant to be very inefficient in order to demonstrate timing results.

au
**
This argument has prototype

   ``const`` *ADvector* & *au*

where *ADvector* is a
:ref:`simple vector class<SimpleVector-name>` with elements
of type ``AD<double>`` .
The size of *au* is three.

y_initial
=========
We use the notation

   *y_initial* = *au* [0]

for the initial value of the Newton iterate.

y_squared
=========
We use the notation

   *y_squared* = *au* [1]

for the value we are taking the square root of.

ay
**
This argument has prototype

   *ADvector* & *ay*

The size of *ay* is one and
*ay* [0] is the square root of *y_squared* .

Source
******
{xrst_literal
   // BEGIN ALGO C++
   // END ALGO C++
}

{xrst_end multi_chkpoint_one_algo}
*/
// BEGIN ALGO C++
// includes used by all source code in multi_chkpoint_one.cpp file
# include <cppad/cppad.hpp>
# include "multi_chkpoint_one.hpp"
# include "team_thread.hpp"
//
namespace {
   using CppAD::thread_alloc; // fast multi-threading memory allocator
   using CppAD::vector;       // uses thread_alloc
   //
   typedef CppAD::AD<double> a_double;

   void checkpoint_algo(const vector<a_double>& au , vector<a_double>& ay)
   {
      // extract components of argument vector
      a_double y_initial  = au[0];
      a_double y_squared  = au[1];

      // Use Newton's method to solve f(y) = y^2 = y_squared
      a_double y_itr = y_initial;
      for(size_t itr = 0; itr < 20; itr++)
      {  // solve (y - y_itr) * f'(y_itr) = y_squared - y_itr^2
         a_double fp_itr = 2.0 * y_itr;
         y_itr           = y_itr + (y_squared - y_itr * y_itr) / fp_itr;
      }

      // return the Newton approximation for f(y) = y_squared
      ay[0] = y_itr;
   }
}
// END ALGO C++

/*
{xrst_begin multi_chkpoint_one_common app}

Multi-Threaded chkpoint_one Common Information
##############################################

Purpose
*******
This source code defines the common variables that are used by
the ``multi_chkpoint_one_`` *name* functions.

Source
******
{xrst_literal
   // BEGIN COMMON C++
   // END COMMON C++
}

{xrst_end multi_chkpoint_one_common}
*/
// BEGIN COMMON C++
namespace {
   // Number of threads, set by multi_chkpoint_one_time
   // (zero means one thread with no multi-threading setup)
   size_t num_threads_ = 0;

   // We can use one checkpoint function for all threads because
   // there is no member data that gets changed during worker call.
   // This needs to stay in scope for as long as a recording will use it.
   // We cannot be in parallel mode when this object is created or deleted.
   // We use a pointer so that there is no left over memory in thread zero.
   CppAD::checkpoint<double>* a_square_root_ = nullptr;

   // structure with information for one thread
   typedef struct {
      // used by worker to compute the square root,
      // set by multi_chkpoint_one_setup
      CppAD::ADFun<double>* fun;
      //
      // value we are computing square root of, set by multi_chkpoint_one_setup
      vector<double>* y_squared;
      //
      // square root, set by worker
      vector<double>* square_root;
      //
      // false if an error occurs, true otherwise, set by worker
      bool ok;
   } work_one_t;
   //
   // Vector with information for all threads
   // (uses pointers instead of values to avoid false sharing)
   // allocated by multi_chkpoint_one_setup, freed by multi_chkpoint_one_takedown
   work_one_t* work_all_[CPPAD_MAX_NUM_THREADS];
}
// END COMMON C++
/*
-------------------------------------------------------------------------------
{xrst_begin multi_chkpoint_one_setup app}

Multi-Threaded chkpoint_one Set Up
##################################

Syntax
******
*ok* = ``multi_chkpoint_one_setup`` ( *y_squared* )

Purpose
*******
This routine splits up the computation into the individual threads.

Thread
******
It is assumed that this function is called by thread zero
and all the other threads are blocked (waiting).

y_squared
*********
This argument has prototype

   ``const vector<double>&`` *y_squared*

and its size is equal to the number of equations to solve.
It is the values that we are computing the square root of.

ok
**
This return value has prototype

   ``bool`` *ok*

If it is false,
``multi_chkpoint_one_setup`` detected an error.

Source
******
{xrst_literal
   // BEGIN SETUP C++
   // END SETUP C++
}

{xrst_end multi_chkpoint_one_setup}
*/
// BEGIN SETUP C++
namespace {
bool multi_chkpoint_one_setup(const vector<double>& y_squared)
{  size_t num_threads = std::max(num_threads_, size_t(1));
   bool   ok          = num_threads == thread_alloc::num_threads();
   ok                &= thread_alloc::thread_num() == 0;
   //
   // declare independent variable variable vector
   vector<a_double> ax(1);
   ax[0] = 2.0;
   CppAD::Independent(ax);
   //
   // argument and result for checkpoint algorithm
   vector<a_double> au(2), ay(1);
   au[0] = ax[0];                  // y_initial
   au[1] = ax[0];                  // y_squared

   // put user checkpoint function in recording
   (*a_square_root_)(au, ay);

   // f(u) = sqrt(u)
   CppAD::ADFun<double> fun(ax, ay);
   //
   // number of square roots for each thread
   size_t per_thread = (y_squared.size() + num_threads - 1) / num_threads;
   size_t y_index    = 0;
   //
   for(size_t thread_num = 0; thread_num < num_threads; thread_num++)
   {  // allocate separate memory for each thread to avoid false sharing
      size_t min_bytes(sizeof(work_one_t)), cap_bytes;
      void* v_ptr = thread_alloc::get_memory(min_bytes, cap_bytes);
      work_all_[thread_num] = static_cast<work_one_t*>(v_ptr);
      //
      // Run constructor on work_all_[thread_num]->fun
      work_all_[thread_num]->fun = new CppAD::ADFun<double>;
      //
      // Run constructor on work_all_[thread_num] vectors
      work_all_[thread_num]->y_squared = new vector<double>;
      work_all_[thread_num]->square_root = new vector<double>;
      //
      // Each worker gets a separate copy of fun. This is necessary because
      // the Taylor coefficients will be set by each thread.
      *(work_all_[thread_num]->fun) = fun;
      //
      // values we are computing square root of for this thread
      ok &=  0 == work_all_[thread_num]->y_squared->size();
      for(size_t i = 0; i < per_thread; i++)
      if( y_index < y_squared.size() )
         work_all_[thread_num]->y_squared->push_back(y_squared[y_index++]);
      //
      // set to false in case this thread's worker does not get called
      work_all_[thread_num]->ok = false;
   }
   ok &= y_index == y_squared.size();
   //
   return ok;
}
}
// END SETUP C++
/*
------------------------------------------------------------------------------
{xrst_begin multi_chkpoint_one_worker app}

Multi-Threaded chkpoint_one Worker
##################################

Purpose
*******
This routine does the computation for one thread.

Source
******
{xrst_literal
   // BEGIN WORKER C++
   // END WORKER C++
}

{xrst_end multi_chkpoint_one_worker}
*/
// BEGIN WORKER C++
namespace {
void multi_chkpoint_one_worker(void)
{  size_t thread_num  = thread_alloc::thread_num();
   size_t num_threads = std::max(num_threads_, size_t(1));
   bool   ok          = thread_num < num_threads;
   //
   vector<double> x(1), y(1);
   size_t n = work_all_[thread_num]->y_squared->size();
   work_all_[thread_num]->square_root->resize(n);
   for(size_t i = 0; i < n; i++)
   {  x[0] = (* work_all_[thread_num]->y_squared )[i];
      y    = work_all_[thread_num]->fun->Forward(0, x);
      //
      (* work_all_[thread_num]->square_root )[i] = y[0];
   }
   work_all_[thread_num]->ok             = ok;
}
}
// END WORKER C++
/*
------------------------------------------------------------------------------
{xrst_begin multi_chkpoint_one_takedown app}

Multi-Threaded chkpoint_one Take Down
#####################################

Syntax
******
*ok* = ``multi_chkpoint_one_takedown`` ( *square_root* )

Purpose
*******
This routine gathers up the results for each thread and
frees memory that was allocated by :ref:`multi_chkpoint_one_setup-name` .

Thread
******
It is assumed that this function is called by thread zero
and all the other threads are blocked (waiting).

square_root
***********
This argument has prototype

   ``vector<double>&`` *square_root*

The input value of *square_root* does not matter.
Upon return,
it has the same size and is the element by element square root of
:ref:`multi_chkpoint_one_setup@y_squared` .

ok
**
This return value has prototype

   ``bool`` *ok*

If it is false,
``multi_chkpoint_one_takedown`` detected an error.

Source
******
{xrst_literal
   // BEGIN TAKEDOWN C++
   // END TAKEDOWN C++
}

{xrst_end multi_chkpoint_one_takedown}
*/
// BEGIN TAKEDOWN C++
namespace {
bool multi_chkpoint_one_takedown(vector<double>& square_root)
{  bool ok            = true;
   ok                &= thread_alloc::thread_num() == 0;
   size_t num_threads = std::max(num_threads_, size_t(1));
   //
   // extract square roots in original order
   square_root.resize(0);
   for(size_t thread_num = 0; thread_num < num_threads; thread_num++)
   {  // results for this thread
      size_t n = work_all_[thread_num]->square_root->size();
      for(size_t i = 0; i < n; i++)
         square_root.push_back((* work_all_[thread_num]->square_root )[i]);
   }
   //
   // go down so that free memory for other threads before memory for master
   size_t thread_num = num_threads;
   while(thread_num--)
   {  // check that this tread was ok with the work it did
      ok  &= work_all_[thread_num]->ok;
      //
      // run destructor on vector object for this thread
      delete work_all_[thread_num]->y_squared;
      delete work_all_[thread_num]->square_root;
      //
      // run destructor on function object for this thread
      delete work_all_[thread_num]->fun;
      //
      // delete problem specific information
      void* v_ptr = static_cast<void*>( work_all_[thread_num] );
      thread_alloc::return_memory( v_ptr );
      //
      // Note that checkpoint object has memory connect to each thread
      // thread_alloc::inuse(thread_num) cannot be zero until it is deleted
      if( thread_num > 0 )
      {  ok &= thread_alloc::inuse(thread_num) > 0;
         //
         // return all memory that is not in use and
         // but being held for future use by this thread
         thread_alloc::free_available(thread_num);
      }
   }
   return ok;
}
}
// END TAKEDOWN C++
/*
{xrst_begin multi_chkpoint_one_run app}

Run Multi-Threaded chkpoint_one Calculation
###########################################

Syntax
******
*ok* = ``multi_chkpoint_one_run`` ( *y_squared* , *square_root* )

Thread
******
It is assumed that this function is called by thread zero
and all the other threads are blocked (waiting).

y_squared
*********
This argument has prototype

   ``const vector<double>&`` *y_squared*

and its size is equal to the number of threads.
It is the values that we are computing the square root of.

square_root
***********
This argument has prototype

   ``vector<double>&`` *square_root*

The input value of *square_root* does not matter.
Upon return,
it has the same size and
is the element by element square root of *y_squared* .

ok
**
This return value has prototype

   ``bool`` *ok*

If it is false,
``multi_chkpoint_one_run`` detected an error.

Source
******
{xrst_literal
   // BEGIN RUN C++
   // END RUN C++
}

{xrst_end multi_chkpoint_one_run}
------------------------------------------------------------------------------
*/
// BEGIN RUN C++
namespace {
bool multi_chkpoint_one_run(
   const vector<double>& y_squared  ,
   vector<double>&      square_root )
{
   bool ok = true;
   ok     &= thread_alloc::thread_num() == 0;

   // setup the work for multi-threading
   ok &= multi_chkpoint_one_setup(y_squared);

   // now do the work for each thread
   if( num_threads_ > 0 )
      team_work( multi_chkpoint_one_worker );
   else
      multi_chkpoint_one_worker();

   // combine the result for each thread and takedown the multi-threading.
   ok &= multi_chkpoint_one_takedown(square_root);

   return ok;
}
}
// END RUN C++
/*
------------------------------------------------------------------------------
{xrst_begin multi_chkpoint_one_time app}

Timing Test for Multi-Threaded chkpoint_one Calculation
#######################################################

Syntax
******

| *ok* = ``multi_chkpoint_one_time`` (
| |tab| *time_out* , *test_time* , *num_threads* , *num_solve*
| )

Thread
******
It is assumed that this function is called by thread zero in sequential
mode; i.e., not :ref:`in_parallel<ta_in_parallel-name>` .

time_out
********
This argument has prototype

   ``double&`` *time_out*

Its input value of the argument does not matter.
Upon return it is the number of wall clock seconds
used by :ref:`multi_chkpoint_one_run-name` .

test_time
*********
This argument has prototype

   ``double`` *test_time*

and is the minimum amount of wall clock time that the test should take.
The number of repeats for the test will be increased until this time
is reached.
The reported *time_out* is the total wall clock time divided by the
number of repeats.

num_threads
***********
This argument has prototype

   ``size_t`` *num_threads*

It specifies the number of threads that are available for this test.
If it is zero, the test is run without the multi-threading environment and

   1 == ``thread_alloc::num_threads`` ()

If it is non-zero, the test is run with the multi-threading and

   *num_threads* = ``thread_alloc::num_threads`` ()

num_solve
*********
This specifies the number of square roots that will be solved for.

ok
**
The return value has prototype

   ``bool`` *ok*

If it is true,
``harmonic_time`` passed the correctness test and
``multi_chkpoint_one_time`` did not detect an error.
Otherwise it is false.

{xrst_end multi_chkpoint_one_time}
*/

// BEGIN TIME C++
namespace {
   // values we are taking the square root of
   vector<double> y_squared_;

   // results of the square root calculations
   vector<double> square_root_;
   //
   void test_once(void)
   {  bool ok = multi_chkpoint_one_run(y_squared_, square_root_);
      if( ! ok )
      {  std::cerr << "multi_chkpoint_one_run: error" << std::endl;
         exit(1);
      }
      return;
   }
   //
   void test_repeat(size_t repeat)
   {  size_t i;
      for(i = 0; i < repeat; i++)
         test_once();
      return;
   }
}
// This is the only routine that is accessible outside of this file
bool multi_chkpoint_one_time(
   double& time_out, double test_time, size_t num_threads, size_t num_solve
)
{  bool ok = true;
   //
   size_t initial_inuse = thread_alloc::inuse(0);

   // number of threads, zero for no multi-threading
   num_threads_ = num_threads;

   // values we are talking the square root of
   y_squared_.resize(num_solve);
   for(size_t i_solve = 0; i_solve < num_solve; i_solve++)
      y_squared_[i_solve] = double(i_solve) + 2.0;

   // must create a_square_root_ in sequential mode
   vector<a_double> au(2), ay(1);
   au[0] = 2.0;
   au[1] = 2.0;
   a_square_root_ = new CppAD::checkpoint<double>(
      "square_root", checkpoint_algo, au, ay
   );

   // create team of threads
   ok &= thread_alloc::in_parallel() == false;
   if( num_threads > 0 )
   {  team_create(num_threads);
      ok &= num_threads == thread_alloc::num_threads();
   }
   else
   {  ok &= 1 == thread_alloc::num_threads();
   }

   // run the test case and set the time return value
   time_out = CppAD::time_test(test_repeat, test_time);

   // destroy team of threads
   if( num_threads > 0 )
      team_destroy();
   ok &= thread_alloc::in_parallel() == false;

   // must delete a_square_root_ in sequential mode
   delete a_square_root_;

   // free static variables in atomic_base class
   CppAD::atomic_base<double>::clear();

   // correctness check
   ok &= square_root_.size() == num_solve;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   for(size_t i = 0; i < num_solve; i++)
   {  double check = std::sqrt( y_squared_[i] );
      ok          &= std::fabs( square_root_[i] / check - 1.0 ) <= eps99;
   }
   //
   // free memory in CppAD vectors that are still in scope
   y_squared_.clear();
   square_root_.clear();
   au.clear();
   ay.clear();
   //
   // check that no static variables in this file are holding onto memory
   ok &= initial_inuse == thread_alloc::inuse(0);
   //
   return ok;
}
// END TIME C++
