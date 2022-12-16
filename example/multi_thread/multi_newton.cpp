// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin multi_newton_common}

Common Variables use by Multi-Threaded Newton Method
####################################################

Purpose
*******
This source code defined the common
include files, defines, and variables that are used by the
multi-newton method.

Source
******
{xrst_literal
   // BEGIN COMMON C++
   // END COMMON C++
}

{xrst_end multi_newton_common}
*/
// BEGIN COMMON C++
# include <cppad/cppad.hpp>
# include <cppad/utility/time_test.hpp>
# include <cmath>
# include <cstring>
# include "multi_newton.hpp"
# include "team_thread.hpp"
# define USE_THREAD_ALLOC_FOR_WORK_ALL 1

namespace {
   using CppAD::thread_alloc; // fast multi-threadeding memory allocator
   using CppAD::vector;       // uses thread_alloc

   // number of threads, set by multi_newton_time.
   size_t num_threads_ = 0;

   // function we are finding zeros of, set by multi_newton_time
   void (*fun_)(double x, double& f, double& df) = 0;

   // convergence criteria, set by multi_newton_setup
   double epsilon_ = 0.;

   // maximum number of iterations, set by  multi_newton_setup
   size_t max_itr_ = 0;

   // length for all sub-intervals
   double sub_length_ = 0.;

   // structure with information for one thread
   typedef struct {
      // number of sub intervals (worker input)
      size_t num_sub;
      // beginning of interval (worker input)
      double xlow;
      // end of interval (worker input)
      double xup;
      // vector of zero candidates (worker output)
      // after call to multi_newton_setup:    x.size() == 0
      // after call to multi_newton_work:     x.size() is number of zeros
      // after call to multi_newton_takedown: x.size() == 0
      vector<double> x;
      // false if an error occurs, true otherwise (worker output)
      bool   ok;
   } work_one_t;
   // vector with information for all threads
   // after call to multi_newton_setup:    work_all.size() == num_threads
   // after call to multi_newton_takedown: work_all.size() == 0
   // (use pointers instead of values to avoid false sharing)
   vector<work_one_t*> work_all_;
}
// END COMMON C++
/*
-------------------------------------------------------------------------------
{xrst_begin multi_newton_setup}
{xrst_spell
   xlow
   xup
}

Set Up Multi-Threaded Newton Method
###################################

Syntax
******

| *ok* = ``multi_newton_setup`` (
| |tab| *num_sub* , *xlow* , *xup* , *epsilon* , *max_itr* , *num_threads*
| )

Purpose
*******
These routine does the setup for splitting finding all the zeros in an
interval into separate sub-intervals, one for each thread.

Thread
******
It is assumed that this function is called by thread zero,
and all the other threads are blocked (waiting).

num_sub
*******
See *num_sub* in :ref:`multi_newton_run<multi_newton_run@num_sub>` .

xlow
****
See *xlow* in :ref:`multi_newton_run<multi_newton_run@xlow>` .

xup
***
See *xup* in :ref:`multi_newton_run<multi_newton_run@xup>` .

epsilon
*******
See *epsilon* in :ref:`multi_newton_run<multi_newton_run@epsilon>` .

max_itr
*******
See *max_itr* in :ref:`multi_newton_run<multi_newton_run@max_itr>` .

num_threads
***********
See *num_threads* in
:ref:`multi_newton_run<multi_newton_run@num_threads>` .

Source
******
{xrst_literal
   // BEGIN SETUP C++
   // END SETUP C++
}

{xrst_end multi_newton_setup}
*/
// BEGIN SETUP C++
namespace {
bool multi_newton_setup(
   size_t num_sub                              ,
   double xlow                                 ,
   double xup                                  ,
   double epsilon                              ,
   size_t max_itr                              ,
   size_t num_threads                          )
{
   num_threads  = std::max(num_threads_, size_t(1));
   bool ok      = num_threads == thread_alloc::num_threads();
   ok          &= thread_alloc::thread_num() == 0;

   // inputs that are same for all threads
   epsilon_ = epsilon;
   max_itr_ = max_itr;

   // resize the work vector to accomidate the number of threads
   ok &= work_all_.size() == 0;
   work_all_.resize(num_threads);

   // length of each sub interval
   sub_length_ = (xup - xlow) / double(num_sub);

   // determine values that are specific to each thread
   size_t num_min   = num_sub / num_threads; // minimum num_sub
   size_t num_more  = num_sub % num_threads; // number that have one more
   size_t sum_num   = 0;  // sum with respect to thread of num_sub
   size_t thread_num, num_sub_thread;
   for(thread_num = 0; thread_num < num_threads; thread_num++)
   {
# if  USE_THREAD_ALLOC_FOR_WORK_ALL
      // allocate separate memory for this thread to avoid false sharing
      size_t min_bytes(sizeof(work_one_t)), cap_bytes;
      void* v_ptr = thread_alloc::get_memory(min_bytes, cap_bytes);
      work_all_[thread_num] = static_cast<work_one_t*>(v_ptr);

      // thread_alloc is a raw memory allocator; i.e., it does not call
      // the constructor for the objects it creates. The vector
      // class requires it's constructor to be called so we do it here
      new(& (work_all_[thread_num]->x) ) vector<double>();
# else
      work_all_[thread_num] = new work_one_t;
# endif

      // number of sub-intervalse for this thread
      if( thread_num < num_more  )
         num_sub_thread = num_min + 1;
      else
         num_sub_thread = num_min;

      // when thread_num == 0, xlow_thread == xlow
      double xlow_thread = xlow + double(sum_num) * sub_length_;

      // when thread_num == num_threads - 1, xup_thread = xup
      double xup_thread =
         xlow + double(sum_num + num_sub_thread) * sub_length_;
      if( thread_num == num_threads - 1 )
         xup_thread = xup;

      // update sum_num for next time through loop
      sum_num += num_sub_thread;

      // input information specific to this thread
      work_all_[thread_num]->num_sub = num_sub_thread;
      work_all_[thread_num]->xlow    = xlow_thread;
      work_all_[thread_num]->xup     = xup_thread;
      ok &= work_all_[thread_num]->x.size() == 0;

      // in case this thread does not get called
      work_all_[thread_num]->ok = false;
   }
   ok &= sum_num == num_sub;
   return ok;
}
}
// END SETUP C++
/*
------------------------------------------------------------------------------
{xrst_begin multi_newton_worker}
{xrst_spell
   xlow
   xup
}

Do One Thread's Work for Multi-Threaded Newton Method
#####################################################

Syntax
******
``multi_newton_worker`` ()

Purpose
*******
This function finds all the zeros in the interval [ *low* , *up*  ] .

low
***
This is the value of the :ref:`multi_newton_common-name` information

   *low* = ``work_all_`` [ *thread_num* ] ``->xlow``

up
**
This is the value of the :ref:`multi_newton_common-name` information

   *up* = ``work_all_`` [ *thread_num* ] ``->xup``

thread_num
**********
This is the number for the current thread; see
:ref:`thread_num<ta_thread_num-name>` .

Source
******
{xrst_literal
   // BEGIN WORKER C++
   // END WORKER C++
}

{xrst_end multi_newton_worker}
*/
// BEGIN WORKER C++
namespace {
void multi_newton_worker(void)
{
   // Split [xlow, xup] into num_sub intervales and
   // look for one zero in each sub-interval.
   size_t thread_num    = thread_alloc::thread_num();
   size_t num_threads   = std::max(num_threads_, size_t(1));
   bool   ok            = thread_num < num_threads;
   size_t num_sub       = work_all_[thread_num]->num_sub;
   double xlow          = work_all_[thread_num]->xlow;
   double xup           = work_all_[thread_num]->xup;
   vector<double>& x    = work_all_[thread_num]->x;

   // check arguments
   ok &= max_itr_ > 0;
   ok &= num_sub > 0;
   ok &= xlow < xup;
   ok &= x.size() == 0;

   // check for special case where there is nothing for this thread to do
   if( num_sub == 0 )
   {  work_all_[thread_num]->ok = ok;
      return;
   }

   // check for a zero on each sub-interval
   size_t i;
   double xlast = xlow - 2.0 * sub_length_; // over sub_length_ away from x_low
   double flast = 2.0 * epsilon_;           // any value > epsilon_ would do
   for(i = 0; i < num_sub; i++)
   {
      // note that when i == 0, xlow_i == xlow (exactly)
      double xlow_i = xlow + double(i) * sub_length_;

      // note that when i == num_sub - 1, xup_i = xup (exactly)
      double xup_i  = xup  - double(num_sub - i - 1) * sub_length_;

      // initial point for Newton iterations
      double xcur = (xup_i + xlow_i) / 2.;

      // Newton iterations
      bool more_itr = true;
      size_t itr    = 0;
      // initialize these values to avoid MSC C++ warning
      double fcur=0.0, dfcur=0.0;
      while( more_itr )
      {  fun_(xcur, fcur, dfcur);

         // check end of iterations
         if( fabs(fcur) <= epsilon_ )
            more_itr = false;
         if( (xcur == xlow_i ) & (fcur * dfcur > 0.) )
            more_itr = false;
         if( (xcur == xup_i)   & (fcur * dfcur < 0.) )
            more_itr = false;

         // next Newton iterate
         if( more_itr )
         {  xcur = xcur - fcur / dfcur;
            // keep in bounds
            xcur = std::max(xcur, xlow_i);
            xcur = std::min(xcur, xup_i);

            more_itr = ++itr < max_itr_;
         }
      }
      if( fabs( fcur ) <= epsilon_ )
      {  // check for case where xcur is lower bound for this
         // sub-interval and upper bound for previous sub-interval
         if( fabs(xcur - xlast) >= sub_length_ )
         {  x.push_back( xcur );
            xlast = xcur;
            flast = fcur;
         }
         else if( fabs(fcur) < fabs(flast) )
         {  x[ x.size() - 1] = xcur;
            xlast            = xcur;
            flast            = fcur;
         }
      }
   }
   work_all_[thread_num]->ok = ok;
}
}
// END WORKER C++
/*
-------------------------------------------------------------------------------
{xrst_begin multi_newton_takedown}
{xrst_spell
   xout
}

Take Down Multi-threaded Newton Method
######################################

Syntax
******

   *ok* = ``harmonic_takedown`` ( *xout* )

Purpose
*******
This routine does the takedown for splitting the Newton method into
sub-intervals.

Thread
******
It is assumed that this function is called by thread zero,
and all the other threads have completed their work and are blocked (waiting).

xout
****
See :ref:`multi_newton_run<multi_newton_run@xout>` .

Source
******
{xrst_literal
   // BEGIN TAKEDOWN C++
   // END TAKEDOWN C++
}

{xrst_end multi_newton_takedown}
*/
// BEGIN TAKEDOWN C++
namespace {
bool multi_newton_takedown(vector<double>& xout)
{  // number of threads in the calculation
   size_t num_threads  = std::max(num_threads_, size_t(1));

   // remove duplicates and points that are not solutions
   xout.resize(0);
   bool   ok = true;
   ok       &= thread_alloc::thread_num() == 0;

   // initialize as more that sub_length_ / 2 from any possible solution
   double xlast = - sub_length_;
   for(size_t thread_num = 0; thread_num < num_threads; thread_num++)
   {  vector<double>& x = work_all_[thread_num]->x;

      size_t i;
      for(i = 0; i < x.size(); i++)
      {  // check for case where this point is lower limit for this
         // thread and upper limit for previous thread
         if( fabs(x[i] - xlast) >= sub_length_ )
         {  xout.push_back( x[i] );
            xlast = x[i];
         }
         else
         {  double fcur, flast, df;
            fun_(x[i],   fcur, df);
            fun_(xlast, flast, df);
            if( fabs(fcur) < fabs(flast) )
            {  xout[ xout.size() - 1] = x[i];
               xlast                  = x[i];
            }
         }
      }
      // check that this thread was ok with the work it did
      ok &= work_all_[thread_num]->ok;
   }

   // go down so free memory for other threads before memory for master
   size_t thread_num = num_threads;
   while(thread_num--)
   {
# if USE_THREAD_ALLOC_FOR_WORK_ALL
      // call the destructor for vector destructor
      work_all_[thread_num]->x.~vector<double>();
      // delete the raw memory allocation
      void* v_ptr = static_cast<void*>( work_all_[thread_num] );
      thread_alloc::return_memory( v_ptr );
# else
      delete work_all_[thread_num];
# endif
      // Note that xout corresponds to memroy that is inuse by master
      // (so we can only chech have freed all their memory).
      if( thread_num > 0 )
      {  // check that there is no longer any memory inuse by this thread
         ok &= thread_alloc::inuse(thread_num) == 0;
         // return all memory being held for future use by this thread
         thread_alloc::free_available(thread_num);
      }
   }
   // now we are done with the work_all_ vector so free its memory
   // (because it is a static variable)
   work_all_.clear();

   return ok;
}
}
// END TAKEDOWN C++
/*
------------------------------------------------------------------------------
{xrst_begin multi_newton_run}
{xrst_spell
   df
   xlow
   xout
   xup
}

A Multi-Threaded Newton's Method
################################

Syntax
******

| ``ok`` = ``multi_newton_run`` ( *xout* ,
| |tab| ``fun`` , ``num_sub`` , ``xlow`` , ``xup`` , ``epsilon`` , ``max_itr`` , ``num_threads``
| )

Purpose
*******
Multi-threaded determination of the argument values :math:`x`,
in the interval :math:`[a, b]` (where :math:`a < b`),
such that :math:`f(x) = 0`.

Thread
******
It is assumed that this function is called by thread zero,
and all the other threads are blocked (waiting).

Method
******
For :math:`i = 0 , \ldots , n`,
we define the *i*-th grid point :math:`g_i` by

.. math::

   g_i = a \frac{n - i}{n} +  b \frac{i}{n}

For :math:`i = 0 , \ldots , n-1`,
we define the *i*-th sub-interval of :math:`[a, b]` by

.. math::

   I_i = [ g_i , g_{i+1} ]

Newton's method is applied starting
at the center of each of the sub-intervals :math:`I_i` for
:math:`i = 0 , \ldots , n-1`
and at most one zero is found for each sub-interval.

ok
**
The return value *ok* has prototype

   ``bool`` *ok*

If an error occurs, it is false, otherwise it is true.

xout
****
The argument *xout* has the prototype

   ``vector<double>&`` *xout*

The input size and value of the elements of *xout* do not matter.
Upon return from ``multi_newton`` ,
the size of *xout* is less than or equal
the number of sub-intervals :math:`n` and

.. math::

   | f( xout[i] ) | \leq epsilon

for each valid index 0 <= ``i`` < ``xout`` . *size* () .
Two :math:`x` solutions are considered equal (and joined as one) if
the absolute difference between the solutions is less than
:math:`(b - a) / n`.

fun
***
The argument *fun* has prototype

   ``void`` *fun* ( ``double`` *x* , ``double&`` *f* , ``double&`` *df* )

This function must evaluate :math:`f(x)`,
and its derivative :math:`f^{(1)} (x)`,
using the syntax

   *fun* ( *x* , *f* , *df* )

where the arguments to *fun* have the prototypes

| |tab| ``double`` *x*
| |tab| ``double&`` *f*
| |tab| ``double&`` *df*

.
The input values of *f* and *df* do not matter.
Upon return they are :math:`f(x)` and :math:`f^{(1)} (x)` respectively.

num_sub
*******
The argument *num_sub* has prototype

   ``size_t`` *num_sub*

It specifies the number of sub-intervals; i.e., :math:`n`.

xlow
****
The argument *xlow* has prototype

   ``double`` *xlow*

It specifies the lower limit for the entire search interval; i.e., :math:`a`.

xup
***
The argument *xup* has prototype

   ``double`` *xup*

It specifies the upper limit for the entire search interval; i.e., :math:`b`.

epsilon
*******
The argument *epsilon* has prototype

   ``double`` *epsilon*

It specifies the convergence criteria for Newton's method in terms
of how small the function value must be.

max_itr
*******
The argument *max_itr* has prototype

   ``size_t`` *max_itr*

It specifies the maximum number of iterations of Newton's method to try
before giving up on convergence (on each sub-interval).

num_threads
***********
This argument has prototype

   ``size_t`` *num_threads*

It specifies the number of threads that are available for this test.
If it is zero, the test is run without the multi-threading environment.

Source
******
{xrst_literal
   // BEGIN SOLVE C++
   // END SOLVE C++
}

{xrst_end multi_newton_run}
---------------------------------------------------------------------------
*/
// BEGIN SOLVE C++
namespace {
bool multi_newton_run(
   vector<double>& xout                       ,
   void fun(double x, double& f, double& df)  ,
   size_t num_sub                             ,
   double xlow                                ,
   double xup                                 ,
   double epsilon                             ,
   size_t max_itr                             ,
   size_t num_threads                         )
{
   bool ok = true;
   ok     &= thread_alloc::thread_num() == 0;

   // setup the work for num_threads threads
   ok &= multi_newton_setup(
      num_sub, xlow, xup, epsilon, max_itr, num_threads
   );

   // now do the work for each thread
   if( num_threads > 0 )
      team_work( multi_newton_worker );
   else
      multi_newton_worker();

   // now combine the results for all the threads
   ok &= multi_newton_takedown(xout);

   return ok;
}
}
// END SOLVE C++
/*
-----------------------------------------------------------------------------
{xrst_begin multi_newton_time}
.

Timing Test of Multi-Threaded Newton Method
###########################################

Syntax
******

| *ok* = ``multi_newton_time`` ( *time_out* , *test_time* , *num_threads* ,
| |tab| *num_zero* , *num_sub* , *num_sum* , *use_ad*
| )

Purpose
*******
Runs correctness and timing test for a multi-threaded Newton method.
This test uses Newton's method to determine all the zeros of the sine
function on an interval.
CppAD, or hand coded derivatives,
can be used to calculate the derivatives used by Newton's method.
The calculation can be done in parallel on the different sub-intervals.
In addition, the calculation can be done without multi-threading.

Thread
******
It is assumed that this function is called by thread zero in sequential
mode; i.e., not :ref:`in_parallel<ta_in_parallel-name>` .

ok
**
This return value has prototype

   ``bool`` *ok*

If it is true,
``multi_newton_time`` passed the correctness test.
Otherwise it is false.

time_out
********
This argument has prototype

   ``double&`` *time_out*

The input value of the argument does not matter.
Upon return it is the number of wall clock seconds required for
the multi-threaded Newton method can compute all the zeros.

test_time
*********
Is the minimum amount of wall clock time that the test should take.
The number of repeats for the test will be increased until this time
is reached.
The reported *time_out* is the total wall clock time divided by the
number of repeats.

num_threads
***********
This argument has prototype

   ``size_t`` *num_threads*

It specifies the number of threads that
are available for this test.
If it is zero, the test is run without multi-threading and

   1 == ``thread_alloc::num_threads`` ()

when ``multi_newton_time`` is called.
If it is non-zero, the test is run with multi-threading and

   *num_threads* == ``thread_alloc::num_threads`` ()

when ``multi_newton_time`` is called.

num_zero
********
This argument has prototype

   ``size_t`` *num_zero*

and it must be greater than one.
It specifies the actual number of zeros in the test function
:math:`\sin(x)`.
To be specific, ``multi_newton_time`` will attempt to determine
all of the values of :math:`x` for which :math:`\sin(x) = 0` and
:math:`x` is in the interval

   [ 0 , ( *num_zero* ``- 1`` ) * *pi*  ]

.

num_sub
*******
This argument has prototype

   ``size_t`` *num_sub*

It specifies the number of sub-intervals to divide the total interval into.
It must be greater than *num_zero*
(so that the correctness test can check we have found all the zeros).

num_sum
*******
This argument has prototype

   ``size_t`` *num_sum*

and must be greater than zero.
The actual function used by the Newton method is

.. math::

   f(x) = \frac{1}{n} \sum_{i=1}^{n} \sin (x)

where :math:`n` is equal to *num_sum* .
Larger values of *num_sum* simulate a case where the
evaluation of the function :math:`f(x)` takes more time.

use_ad
******
This argument has prototype

   ``bool`` *user_ad*

If *use_ad* is ``true`` ,
then derivatives will be computed using CppAD.
Note that this derivative computation includes
re-taping the function for each
value of :math:`x` (even though re-taping is not necessary).

If *use_ad* is ``false`` ,
derivatives will be computed using a hand coded routine.

Source
******
{xrst_literal
   // BEGIN TIME C++
   // END TIME C++
}

{xrst_end multi_newton_time}
*/
// BEGIN TIME C++

namespace { // empty namespace

   // values correspond to arguments in previous call to multi_newton_time
   size_t num_zero_;   // number of zeros of f(x) in the total interval
   size_t num_sub_;    // number of sub-intervals to split calculation into
   size_t num_sum_;    // larger values make f(x) take longer to calculate

   // value of xout corresponding to most recent call to test_once
   vector<double> xout_;

   // A version of the sine function that can be made as slow as we like
   template <class Float>
   Float f_eval(Float x)
   {  Float sum = 0.;
      size_t i;
      for(i = 0; i < num_sum_; i++)
         sum += sin(x);

      return sum / Float(num_sum_);
   }

   // Direct calculation of derivative with same number of floating point
   // operations as for f_eval.
   double df_direct(double x)
   {  double sum = 0.;
      size_t i;
      for(i = 0; i < num_sum_; i++)
         sum += cos(x);

      return sum / double(num_sum_);
   }

   // AD calculation of detivative
   void fun_ad(double x, double& f, double& df)
   {  using CppAD::AD;

      // use vector because it uses fast multi-threaded memory alloc
      vector< AD<double> > X(1), Y(1);
      X[0] = x;
      CppAD::Independent(X);
      Y[0] = f_eval(X[0]);
      CppAD::ADFun<double> F(X, Y);
      vector<double> dx(1), dy(1);
      dx[0] = 1.;
      dy    = F.Forward(1, dx);
      f     = Value( Y[0] );
      df    = dy[0];
      return;
   }

   // evaulate the function and its derivative
   void fun_no(double x, double& f, double& df)
   {  f  = f_eval(x);
      df = df_direct(x);
      return;
   }


   // Run computation of all the zeros once
   void test_once(void)
   {  if(  num_zero_ == 0 )
      {  std::cerr << "multi_newton_time: num_zero == 0" << std::endl;
         exit(1);
      }
      double pi      = 4. * std::atan(1.);
      double xlow    = 0.;
      double xup     = double(num_zero_ - 1) * pi;
      double eps     =
         xup * 100. * CppAD::numeric_limits<double>::epsilon();
      size_t max_itr = 20;

      // note that fun_ is set to fun_ad or fun_no by multi_newton_time
      bool ok = multi_newton_run(
         xout_       ,
         fun_        ,
         num_sub_    ,
         xlow        ,
         xup         ,
         eps         ,
         max_itr     ,
         num_threads_
      );
      if( ! ok )
      {  std::cerr << "multi_newton: error" << std::endl;
         exit(1);
      }
      return;
   }

   // Repeat computation of all the zeros a specied number of times
   void test_repeat(size_t repeat)
   {  size_t i;
      for(i = 0; i < repeat; i++)
         test_once();
      return;
   }
} // end empty namespace


// This is the only routine that is accessible outside of this file
bool multi_newton_time(
   double& time_out      ,
   double  test_time     ,
   size_t  num_threads   ,
   size_t  num_zero      ,
   size_t  num_sub       ,
   size_t  num_sum       ,
   bool    use_ad
)
{
   bool ok = true;
   ok     &= thread_alloc::thread_num() == 0;
   ok     &= num_sub > num_zero;

   // Set local namespace environment variables
   num_threads_  = num_threads;
   if( use_ad )
      fun_ = fun_ad;
   else
      fun_ = fun_no;
   //
   num_zero_     = num_zero;
   num_sub_      = num_sub;
   num_sum_      = num_sum;

   // create team of threads
   ok &= thread_alloc::in_parallel() == false;
   if( num_threads > 0 )
   {  team_create(num_threads);
      ok &= num_threads == thread_alloc::num_threads();
   }
   else
   {  ok &= 1 == thread_alloc::num_threads();
   }

   // run the test case and set time return value
   time_out = CppAD::time_test(test_repeat, test_time);

   // destroy team of threads
   if( num_threads > 0 )
      team_destroy();
   ok &= thread_alloc::in_parallel() == false;
   //
   // correctness check
   double pi      = 4. * std::atan(1.);
   double xup     = double(num_zero_ - 1) * pi;
   double eps     = xup * 100. * CppAD::numeric_limits<double>::epsilon();
   ok        &= (xout_.size() == num_zero);
   size_t i   = 0;
   for(i = 0; i < xout_.size(); i++)
      ok &= std::fabs( xout_[i] - pi * double(i)) <= 2 * eps;

   // xout_ is a static variable, so clear it to free its memory
   xout_.clear();

   // return correctness check result
   return  ok;
}
// END TIME C++
