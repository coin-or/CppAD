// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin openmp_setup_ad.cpp$$
$spell
	num
	openmp
	CppAD
$$

$section Setup for OpenMP AD$$

$index openmp, setup AD$$
$index AD, setup OpenMP$$
$index setup, OpenMP AD$$
$index thread, setup OpenMP AD$$

$head Syntax$$
$codei%setup_ad(%num_threads%)%$$

$head Purpose$$
This Routine preforms the setup operations for 
using $code AD<double>$$ in an OpenMP multi-threading environment.

$head Restrictions$$
Execution must be in sequential mode when this routine is called; i.e.,
not $cref/in_parallel/ta_in_parallel/$$.

$head num_thread$$
This argument has prototype
$codei%
	size %num_threads%
%$$
It specifies the number of OpenMP threads for subsequent execution.

$head Source Code$$
$code
$verbatim%multi_thread/openmp/setup_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <omp.h>
# include <cppad/cppad.hpp>

namespace {

	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return static_cast<bool> ( omp_in_parallel() ); }

	// used to inform CppAD of the current thread number
	size_t thread_num(void)
	{	return static_cast<size_t>( omp_get_thread_num() ); } 
}

void setup_ad(size_t num_threads)
{
	// Set the number of OpenMP threads
	omp_set_dynamic(0); // turn off dynamic thread adjustment
	omp_set_num_threads( int( num_threads ) );

	// Setup for using AD<double> in parallel mode
	CppAD::thread_alloc::parallel_setup(num_threads, in_parallel, thread_num);
	CppAD::parallel_ad<double>();
}
// END PROGRAM
