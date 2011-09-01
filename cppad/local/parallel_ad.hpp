/* $Id$ */
# ifndef CPPAD_PARALLEL_AD_INCLUDED
# define CPPAD_PARALLEL_AD_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin parallel_ad$$
$spell
	CppAD
	num
	isnan
	std
$$

$section Enable AD Calculations During Parallel Mode$$

$head Syntax$$
$codei%parallel_ad<%Base%>()%$$

$head Purpose$$
By default, for each $codei%AD<%Base%>%$$ class there is only one 
tape that records $cref/AD of Base/glossary/AD of Base/$$ operations.
This tape is a global variable and hence it cannot be used
by multiple threads at the same time. 
The $cref/parallel_setup/ta_parallel_setup/$$ function informs CppAD of the
maximum number of threads that can be active in parallel mode.
It is still necessary to inform each $codei%AD<%Base%>%$$ type 
before using it in using $cref/parallel/omp_in_parallel/$$ execution mode
(and $code parallel_ad$$ does this).

$head Restriction$$
This function must be called before any $codei%AD<%Base>%$$ objects are used
in $cref/parallel/ta_in_parallel/$$ mode.

$head isnan$$
This routine has the side effect of calling
$codei%
	%b% = isnan(%s%)
%$$
where $icode s$$ has type $icode%Base%$$, $codei%AD<%Base%>%$$, and
$codei%std::complex<double>%$$.

$head Example$$
The files $cref openmp_ad.cpp$$ and $cref pthread_ad.cpp$$
contain examples and tests that use this function.   

$end
-----------------------------------------------------------------------------
*/

# include <cppad/local/std_set.hpp>

// BEGIN CppAD namespace
namespace CppAD {

/*!
Enable parallel execution mode with <code>AD<Base></code> by initializing 
static variables that my be used.
*/

template <class Base>
void parallel_ad(void)
{	CPPAD_ASSERT_KNOWN( 
		! thread_alloc::in_parallel() ,
		"parallel_ad must be called before entering parallel execution mode."
	);

	// ensure statics in following functions are initialized
	elapsed_seconds();
	ErrorHandler::Current();
	isnan( std::complex<double>(0.) );
	NumArg(BeginOp);
	one_element_std_set<size_t>();
	two_element_std_set<size_t>();

	// the sparse_pack class has member functions with static data
	sparse_pack sp;
	sp.resize(1, 1);       // so can call add_element
	sp.add_element(0, 0);  // has static data
	sp.begin(0);           // so can call next_element
	sp.next_element();     // has static data
	sp.clear(0);           // has static data


	// statics that depend on the value of Base
	AD<Base>::id_handle(0);
	AD<Base>::tape_handle(0);	
	discrete<Base>::List();
	erf_template( Base(0.) );
	erf_template( AD<Base>(0.) );
	isnan( Base(0.) );
	isnan( AD<Base>(0.) );

}

} // END CppAD namespace

# endif
