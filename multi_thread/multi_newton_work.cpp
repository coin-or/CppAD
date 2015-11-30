// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin multi_newton_work.cpp$$
$spell
	num
$$

$section Multi-threading Newton Method Utility Routines$$
$mindex multi_newton_work setup worker combine$$
$spell
	Cpp
	xlow
	xup
	itr
	df
	xout
	df
$$


$head Syntax$$
$icode%ok% = multi_newton_setup(
	%fun%, %num_sub%, %xlow%, %xup%, %epsilon%, %max_itr%, %num_threads%
)%$$
$codei%multi_newton_worker()
%$$
$icode%ok% = multi_newton_combine(%xout%)
%$$

$head Purpose$$
These routines aid in the computation of multiple zeros of a function
$latex f: [ a, b] \rightarrow {\bf R}$$.

$head multi_newton_setup$$
Calling this functions splits up the computation of the zeros
into different sub-intervals for each thread.

$subhead fun$$
The argument $icode fun$$ has prototype
$codei%
	void %fun% (double %x%, double& %f%, double& %df%)
%$$
This argument must evaluate the function $latex f(x)$$,
and its derivative $latex f^{(1)} (x)$$,
using the syntax
$codei%
	%fun%(%x%, %f%, %df%)
%$$
where the arguments have the prototypes
$codei%
	double    %x%
	double&   %f%
	double&   %df%
%$$.
The input values of $icode f$$ and $icode df$$ do not matter.
Upon return they are $latex f(x)$$ and $latex f^{(1)} (x)$$ respectively.

$subhead num_sub$$
The argument $icode num_sub$$ has prototype
$codei%
	size_t %num_sub%
%$$
It specifies the number of sub-intervals; i.e., $latex n$$
in to split the calculation into.

$head xlow$$
The argument $icode xlow$$ has prototype
$codei%
	double %xlow%
%$$
It specifies the lower limit for the entire search; i.e., $latex a$$.

$subhead xup$$
The argument $icode xup$$ has prototype
$codei%
	double %xup%
%$$
It specifies the upper limit for the entire search; i.e., $latex b$$.

$subhead epsilon$$
The argument $icode epsilon$$ has prototype
$codei%
	double %epsilon%
%$$
It specifies the convergence criteria for Newton's method in terms
of how small the function value $latex | f(x) | \leq \varepsilon$$.

$head max_itr$$
The argument $icode max_itr$$ has prototype
$codei%
	size_t %max_itr%
%$$
It specifies the maximum number of iterations of Newton's method to try
before giving up on convergence.

$subhead num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of threads that are available for this test.
If it is zero, the test is run without multi-threading.

$head multi_newton_worker$$
Calling this function does the computation for one thread.
Following a call to $code multi_newton_setup$$,
this function should be called by each of the $icode num_threads$$ threads.

$head multi_newton_combine$$
After the $icode num_threads$$ threads have completed their
calls to $code multi_newton_worker$$,
this function call will combine the results and return the final
set of approximate zeros for $latex f(x)$$.

$subhead xout$$
The argument $icode xout$$ has the prototype
$codei%
	CppAD::vector<double>& %xout%
%$$
The input size and value of the elements of $icode xout$$ do not matter.
Upon return from $code multi_newton_combine$$,
the size of $icode xout$$ is less than or equal $latex n$$ and
$codei%
	| %f%( %xout%[%i%] ) | <= %epsilon%
%$$
for each valid index $icode i$$.
In addition, the elements of $icode xout$$ are in ascending order and
$codei%
	%xout%[i+1] - %xout%[%i%] >=  0.5 * (%xup% - %xlow%) / %num_sub%
%$$


$head Source$$
$code
$verbatim%multi_thread/multi_newton_work.cpp%0%// BEGIN C++%// END C++%1%$$
$$


$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "multi_newton_work.hpp"

# define USE_THREAD_ALLOC_FOR_WORK_ALL 1

namespace {
	using CppAD::thread_alloc;

	// This vector template class frees all memory when resized to zero.
	// In addition, its memory allocation works well during multi-threading.
	using CppAD::vector;

	// number of threads in previous call to multi_newton_setup
	size_t num_threads_ = 0;
	// convergence criteria in previous call to multi_newton_setup
	double epsilon_ = 0.;
	// maximum number of iterations in previous call to multi_newton_setup
	size_t max_itr_ = 0;
	// length for all sub-intervals
	double sub_length_ = 0.;
	// function we are finding zeros of in previous call to multi_newton_setup
	void (*fun_)(double x, double& f, double& df) = 0;

	// structure with information for one thread
	typedef struct {
		// number of sub intervals (worker input)
		size_t num_sub;
		// beginning of interval (worker input)
		double xlow;
		// end of interval (worker input)
		double xup;
		// vector of zero candidates (worker output)
		// after call to multi_newton_setup:   x.size() == 0
		// after call to multi_newton_work:    x.size() is number of zeros
		// after call to multi_newton_combine: x.size() == 0
		vector<double> x;
		// false if an error occurs, true otherwise (worker output)
		bool   ok;
	} work_one_t;
	// vector with information for all threads
	// after call to multi_newton_setup:   work_all.size() == num_threads
	// after call to multi_newton_combine: work_all.size() == 0
	// (use pointers instead of values to avoid false sharing)
	vector<work_one_t*> work_all_;
}
// -----------------------------------------------------------------------
// do the work for one thread
void multi_newton_worker(void)
{	using CppAD::vector;

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
	{	work_all_[thread_num]->ok = ok;
		return;
	}

	// check for a zero on each sub-interval
	size_t i;
	double xlast = xlow - 2 * sub_length_; // over sub_length_ away from x_low
	double flast = 2 * epsilon_;           // any value > epsilon_ would do
	for(i = 0; i < num_sub; i++)
	{
		// note that when i == 0, xlow_i == xlow (exactly)
		double xlow_i = xlow + i * sub_length_;

		// note that when i == num_sub - 1, xup_i = xup (exactly)
		double xup_i  = xup  - (num_sub - i - 1) * sub_length_;

		// initial point for Newton iterations
		double xcur = (xup_i + xlow_i) / 2.;

		// Newton iterations
		bool more_itr = true;
		size_t itr    = 0;
		// initialize these values to avoid MSC C++ warning
		double fcur=0.0, dfcur=0.0;
		while( more_itr )
		{	fun_(xcur, fcur, dfcur);

			// check end of iterations
			if( fabs(fcur) <= epsilon_ )
				more_itr = false;
			if( (xcur == xlow_i ) & (fcur * dfcur > 0.) )
				more_itr = false;
			if( (xcur == xup_i)   & (fcur * dfcur < 0.) )
				more_itr = false;

			// next Newton iterate
			if( more_itr )
			{	xcur = xcur - fcur / dfcur;
				// keep in bounds
				xcur = std::max(xcur, xlow_i);
				xcur = std::min(xcur, xup_i);

				more_itr = ++itr < max_itr_;
			}
		}
		if( fabs( fcur ) <= epsilon_ )
		{	// check for case where xcur is lower bound for this
			// sub-interval and upper bound for previous sub-interval
			if( fabs(xcur - xlast) >= sub_length_ )
			{	x.push_back( xcur );
				xlast = xcur;
				flast = fcur;
			}
			else if( fabs(fcur) < fabs(flast) )
			{	x[ x.size() - 1] = xcur;
				xlast            = xcur;
				flast            = fcur;
			}
		}
	}
	work_all_[thread_num]->ok = ok;
}
// -----------------------------------------------------------------------
// setup the work up for multiple threads
bool multi_newton_setup(
	void (fun)(double x, double& f, double& df) ,
	size_t num_sub                              ,
	double xlow                                 ,
	double xup                                  ,
	double epsilon                              ,
	size_t max_itr                              ,
	size_t num_threads                          )
{
	num_threads_ = num_threads;
	num_threads  = std::max(num_threads_, size_t(1));
	bool ok      = num_threads == thread_alloc::num_threads();

	// inputs that are same for all threads
	epsilon_ = epsilon;
	max_itr_ = max_itr;
	fun_     = fun;

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
		// the constructor for the objects it creates. The CppAD::vector
		// class requires it's constructor to be called so we do it here
		new(& (work_all_[thread_num]->x) ) vector<double>();
# else
		work_all_[thread_num] = new work_one_t;
# endif

		// number of sub-intervalse for this thread
		if( thread_num < num_more  )
			num_sub_thread = num_min + 1;
		else	num_sub_thread = num_min;

		// when thread_num == 0, xlow_thread == xlow
		double xlow_thread = xlow + sum_num * sub_length_;

		// when thread_num == num_threads - 1, xup_thread = xup
		double xup_thread = xlow + (sum_num + num_sub_thread) * sub_length_;
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
// -----------------------------------------------------------------------
// get the result of the work
bool multi_newton_combine(CppAD::vector<double>& xout)
{	// number of threads in the calculation
	size_t num_threads  = std::max(num_threads_, size_t(1));

	// remove duplicates and points that are not solutions
	xout.resize(0);
	bool   ok = true;
	size_t thread_num;

	// initialize as more that sub_lenght_ / 2 from any possible solution
	double xlast = - sub_length_;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	vector<double>& x = work_all_[thread_num]->x;

		size_t i;
		for(i = 0; i < x.size(); i++)
		{	// check for case where this point is lower limit for this
			// thread and upper limit for previous thread
			if( fabs(x[i] - xlast) >= sub_length_ )
			{	xout.push_back( x[i] );
				xlast = x[i];
			}
			else
			{	double fcur, flast, df;
				fun_(x[i],   fcur, df);
				fun_(xlast, flast, df);
				if( fabs(fcur) < fabs(flast) )
				{	xout[ xout.size() - 1] = x[i];
					xlast                  = x[i];
				}
			}
		}
		ok &= work_all_[thread_num]->ok;
	}

	// go down so free memory for other threads before memory for master
	thread_num = num_threads;
	while(thread_num--)
	{
# if USE_THREAD_ALLOC_FOR_WORK_ALL
		// call the destructor for CppAD::vector destructor
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
		{	// check that there is no longer any memory inuse by this thread
			ok &= thread_alloc::inuse(thread_num) == 0;
			// return all memory being held for future use by this thread
			thread_alloc::free_available(thread_num);
		}
	}
	// now we are done with the work_all_ vector so free its memory
	// (becasue it is a static variable)
	work_all_.clear();

	return ok;
}
// END C++
