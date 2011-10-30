/* $Id$ */
# ifndef CPPAD_THREAD_TEAM_INCLUDED
# define CPPAD_THREAD_TEAM_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/* 
$begin thread_team.hpp$$
$spell
	cstddef
	bool
	pthread
	initializes
	hpp
	num
	CppAD
	ta
$$
$section Specifications for A Team of AD Threads$$

$index thread, AD team$$
$index AD, thread team$$
$index team, AD threads$$

$head Syntax$$
$codei%include "thread_team.hpp"
%$$
$icode%ok% = start_team(%num_threads%)
%$$
$icode%ok% = work_team(%worker%)
%$$
$icode%ok% = stop_team()
%$$

$head Purpose$$
These routines start, use, and stop a team of threads that can
be used with the CppAD type $code AD<double>$$.
These could be OpenMP, pthread, or Boost threads to name a few.

$head thread_num$$
Calls to the routines
$code start_team$$,
$code work_team$$, and
$code stop_team$$, must all have 
$cref/thread_num/ta_thread_num/$$ equal to zero 
(thread number zero is referred to as the master thread).

$head in_parallel$$
Calls to the routines
$code start_team$$,
$code work_team$$, and
$code stop_team$$, must all have 
$cref/in_parallel/ta_in_parallel/$$ equal to false; i.e.,
these calls must be made in sequential execution mode.
Execution will also be sequential when these calls return.

$head start_team$$
The argument 
$icode%num_threads% > 0%$$ specifies the number of threads in this team.
This initializes both $code AD<double>$$ and $code work_team$$
to be used with $icode num_threads$$.
If $icode%num_threads% > 1%$$,
$icode%num_threads% - 1%$$ new threads are created
and put in a waiting state until $code work_team$$ is called.

$head work_team$$
This routine may be called one or more times
between the call to $code start_team$$ and $code stop_team$$.
Each call to $code team_work$$ runs $icode num_threads$$ versions
of $icode worker$$ with the corresponding value of
$icode/thread_num/ta_thread_num/$$ different for each thread,
and between zero and $icode%num_threads% - 1%$$.

$head stop_team$$
This routine terminates all the other threads except for
thread number zero; i.e., it terminates the threads corresponding to
$codei%
	%thread_num% = 1 , ... , %num_threads%-1
%$$

$head Example Use$$
Example use of these specifications can be found in the file
$cref simple_ad.cpp$$.

$children%
	multi_thread/openmp/openmp_team.cpp%
	multi_thread/bthread/bthread_team.cpp%
	multi_thread/pthread/pthread_team.cpp
%$$
$head Example Implementation$$
Example implementations of these specifications can be found in the files:
$table
$rref openmp_team.cpp$$
$rref bthread_team.cpp$$
$rref pthread_team.cpp$$
$tend

$head Source$$
$codep */
# include <cstddef> // for size_t

extern bool start_team(size_t num_threads);
extern bool work_team(void worker(void));
extern bool stop_team(void);
/* $$
$end
*/

# endif
