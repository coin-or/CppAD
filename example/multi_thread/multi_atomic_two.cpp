// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin multi_atomic_two_user app}

Defines a atomic_two Operation that Computes Square Root
########################################################

Syntax
******
| ``atomic_user`` *a_square_root*
| *a_square_root* ( *au* , *ay* )

Purpose
*******
This atomic function operation computes a square root using Newton's method.
It is meant to be very inefficient in order to demonstrate timing results.

au
**
This argument has prototype

   ``const`` *ADvector* & *au*

where *ADvector* is a
:ref:`simple vector class<SimpleVector-name>` with elements
of type ``AD<double>`` .
The size of *au* is three.

num_itr
=======
We use the notation

   *num_itr* = ``size_t`` ( ``Integer`` ( *au* [0] ) )

for the number of Newton iterations in the computation of the square root
function.  The component *au* [0] must be a
:ref:`glossary@Parameter` .

y_initial
=========
We use the notation

   *y_initial* = *au* [1]

for the initial value of the Newton iterate.

y_squared
=========
We use the notation

   *y_squared* = *au* [2]

for the value we are taking the square root of.

ay
**
This argument has prototype

   *ADvector* & *ay*

The size of *ay* is one and
*ay* [0] is the square root of *y_squared* .

Limitations
***********
Only zero order forward mode is implements for the
``atomic_user`` class.

Source
******
{xrst_literal
   // BEGIN USER C++
   // END USER C++
}

{xrst_end multi_atomic_two_user}
*/
// BEGIN USER C++
// includes used by all source code in multi_atomic_two.cpp file
# include <cppad/cppad.hpp>
# include "multi_atomic_two.hpp"
# include "team_thread.hpp"
//
namespace {
using CppAD::thread_alloc; // fast multi-threading memory allocator
using CppAD::vector;       // uses thread_alloc

class atomic_user : public CppAD::atomic_base<double> {
public:
   // ctor
   atomic_user(void)
   : CppAD::atomic_base<double>("atomic_square_root")
   { }
private:
   // forward mode routine called by CppAD
   bool forward(
      size_t                   p   ,
      size_t                   q   ,
      const vector<bool>&      vu  ,
      vector<bool>&            vy  ,
      const vector<double>&    tu  ,
      vector<double>&          ty  ) override
   {
# ifndef NDEBUG
      size_t n = tu.size() / (q + 1);
      size_t m = ty.size() / (q + 1);
      assert( n == 3 );
      assert( m == 1 );
# endif
      // only implementing zero order forward for this example
      if( q != 0 )
         return false;

      // extract components of argument vector
      size_t num_itr    = size_t( tu[0] );
      double y_initial  = tu[1];
      double y_squared  = tu[2];

      // check for setting variable information
      if( vu.size() > 0 )
      {  if( vu[0] )
            return false;
         vy[0] = vu[1] || vu[2];
      }

      // Use Newton's method to solve f(y) = y^2 = y_squared
      double y_itr = y_initial;
      for(size_t itr = 0; itr < num_itr; itr++)
      {  // solve (y - y_itr) * f'(y_itr) = y_squared - y_itr^2
         double fp_itr = 2.0 * y_itr;
         y_itr         = y_itr + (y_squared - y_itr * y_itr) / fp_itr;
      }

      // return the Newton approximation for f(y) = y_squared
      ty[0] = y_itr;
      return true;
   }
};
}
// END USER C++
/*
{xrst_begin multi_atomic_two_common app}

Multi-Threaded atomic_two Common Information
############################################

Purpose
*******
This source code defines the common variables that are used by
the ``multi_atomic_two_`` *name* functions.

Source
******
{xrst_literal
   // BEGIN COMMON C++
   // END COMMON C++
}

{xrst_end multi_atomic_two_common}
*/
// BEGIN COMMON C++
namespace {
   // Number of threads, set by multi_atomic_two_time
   // (zero means one thread with no multi-threading setup)
   size_t num_threads_ = 0;

   // Number of Newton iterations, set by multi_atomic_two_time
   size_t num_itr_;

   // We can use one atomic_atomic function for all threads because
   // there is no member data that gets changed during worker call.
   // This needs to stay in scope for as long as a recording will use it.
   // We cannot be in parallel mode when this object is created or deleted.
   // We use a pointer so that there is no left over memory in thread zero.
   atomic_user* a_square_root_ = 0;

   // structure with information for one thread
   typedef struct {
      // used by worker to compute the square root, set by multi_atomic_two_setup
      CppAD::ADFun<double>* fun;
      //
      // value we are computing square root of, set by multi_atomic_two_setup
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
   // allocated by multi_atomic_two_setup, freed by multi_atomic_two_takedown
   work_one_t* work_all_[CPPAD_MAX_NUM_THREADS];
}
// END COMMON C++
/*
-------------------------------------------------------------------------------
{xrst_begin multi_atomic_two_setup app}

Multi-Threaded atomic_two Set Up
################################

Syntax
******
*ok* = ``multi_atomic_two_setup`` ( *y_squared* )

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
``multi_atomic_two_setup`` detected an error.

Source
******
{xrst_literal
   // BEGIN SETUP C++
   // END SETUP C++
}

{xrst_end multi_atomic_two_setup}
*/
// BEGIN SETUP C++
namespace {
bool multi_atomic_two_setup(const vector<double>& y_squared)
{  using CppAD::AD;
   size_t num_threads = std::max(num_threads_, size_t(1));
   bool   ok          = num_threads == thread_alloc::num_threads();
   ok                &= thread_alloc::thread_num() == 0;
   //
   // declare independent variable variable vector
   vector< AD<double> > ax(1);
   ax[0] = 2.0;
   CppAD::Independent(ax);
   //
   // argument and result for atomic function
   vector< AD<double> > au(3), ay(1);
   au[0] = AD<double>( num_itr_ ); // num_itr
   au[1] = ax[0];                  // y_initial
   au[2] = ax[0];                  // y_squared
   // put atomic function operation in recording
   (*a_square_root_)(au, ay);
   //
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
{xrst_begin multi_atomic_two_worker app}

Multi-Threaded atomic_two Worker
################################

Purpose
*******
This routine does the computation for one thread.

Source
******
{xrst_literal
   // BEGIN WORKER C++
   // END WORKER C++
}

{xrst_end multi_atomic_two_worker}
*/
// BEGIN WORKER C++
namespace {
void multi_atomic_two_worker(void)
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
{xrst_begin multi_atomic_two_takedown app}

Multi-Threaded atomic_two Take Down
###################################

Syntax
******
*ok* = ``multi_atomic_two_takedown`` ( *square_root* )

Purpose
*******
This routine gathers up the results for each thread and
frees memory that was allocated by :ref:`multi_atomic_two_setup-name` .

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
:ref:`multi_atomic_two_setup@y_squared` .

ok
**
This return value has prototype

   ``bool`` *ok*

If it is false,
``multi_atomic_two_takedown`` detected an error.

Source
******
{xrst_literal
   // BEGIN TAKEDOWN C++
   // END TAKEDOWN C++
}

{xrst_end multi_atomic_two_takedown}
*/
// BEGIN TAKEDOWN C++
namespace {
bool multi_atomic_two_takedown(vector<double>& square_root)
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
      // check that there is no longer any memory inuse by this thread
      if( thread_num > 0 )
      {  ok &= 0 == thread_alloc::inuse(thread_num);
         //
         // return all memory being held for future use by this thread
         thread_alloc::free_available(thread_num);
      }
   }
   return ok;
}
}
// END TAKEDOWN C++
/*
{xrst_begin multi_atomic_two_run app}

Run Multi-Threaded atomic_two Calculation
#########################################

Syntax
******
*ok* = ``multi_atomic_two_run`` ( *y_squared* , *square_root* )

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
``multi_atomic_two_run`` detected an error.

Source
******
{xrst_literal
   // BEGIN RUN C++
   // END RUN C++
}

{xrst_end multi_atomic_two_run}
------------------------------------------------------------------------------
*/
// BEGIN RUN C++
namespace {
bool multi_atomic_two_run(
   const vector<double>& y_squared  ,
   vector<double>&      square_root )
{
   bool ok = true;
   ok     &= thread_alloc::thread_num() == 0;

   // setup the work for multi-threading
   ok &= multi_atomic_two_setup(y_squared);

   // now do the work for each thread
   if( num_threads_ > 0 )
      team_work( multi_atomic_two_worker );
   else
      multi_atomic_two_worker();

   // combine the result for each thread and takedown the multi-threading.
   ok &= multi_atomic_two_takedown(square_root);

   return ok;
}
}
// END RUN C++
/*
------------------------------------------------------------------------------
{xrst_begin multi_atomic_two_time app}

Timing Test for Multi-Threaded atomic_two Calculation
#####################################################

Syntax
******

| *ok* = ``multi_atomic_two_time`` (
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
used by :ref:`multi_atomic_two_run-name` .

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
``multi_atomic_two_time`` did not detect an error.
Otherwise it is false.

{xrst_end multi_atomic_two_time}
*/

// BEGIN TIME C++
namespace {
   // values we are taking the square root of
   vector<double> y_squared_;

   // results of the square root calculations
   vector<double> square_root_;
   //
   void test_once(void)
   {  bool ok = multi_atomic_two_run(y_squared_, square_root_);
      if( ! ok )
      {  std::cerr << "multi_atomic_two_run: error" << std::endl;
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
bool multi_atomic_two_time(
   double& time_out, double test_time, size_t num_threads, size_t num_solve
)
{  bool ok = true;
   //
   size_t initial_inuse = thread_alloc::inuse(0);

   // number of threads, zero for no multi-threading
   num_threads_ = num_threads;

   // number of Newton iterations
   num_itr_ = 20;

   // values we are talking the square root of
   y_squared_.resize(num_solve);
   for(size_t i_solve = 0; i_solve < num_solve; i_solve++)
      y_squared_[i_solve] = double(i_solve) + 2.0;

   // must create a_square_root_ sequential mode
   a_square_root_ = new atomic_user;

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
   y_squared_.clear();
   square_root_.clear();
   //
   // check that no static variables in this file are holding onto memory
   ok &= initial_inuse == thread_alloc::inuse(0);
   //
   return ok;
}
// END TIME C++
