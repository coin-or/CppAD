# ifndef CPPAD_SPEED_TEST_INCLUDED
# define CPPAD_SPEED_TEST_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin speed_test$$
$spell
	gettimeofday
	vec
	cppad.hpp
	Microsoft
	namespace
	std
	const
	cout
	ctime
	ifdef
	const
	endif
	cpp
$$

$index speed_test$$
$index test, speed$$

$section Run One Speed Test and Return Results$$

$head Syntax$$
$code # include <cppad/speed_test.hpp>$$
$pre
$$
$syntax%%rate_vec% = speed_test(%test%, %size_vec%, %time_min%)%$$

$head Purpose$$
The $code speed_test$$ function executes a speed test
for various sized problems
and reports the rate of execution.

$head Motivation$$
It is important to separate small calculation units
and test them individually.
This way individual changes can be tested in the context of the
routine that they are in.
On many machines, accurate timing of a very short execution 
sequences is not possible.
In addition,
there may be set up and tear down time for a test that
we do not really want included in the timing.
For this reason $code speed_test$$
automatically determines how many times to 
repeat the section of the test that we wish to time.


$head Include$$
The file $code cppad/speed_test.hpp$$ defines the 
$code speed_test$$ function.
This file is included by $code cppad/cppad.hpp$$
and it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head Vector$$
We use $italic Vector$$ to denote a 
$cref/simple vector class/SimpleVector/$$ with elements
of type $code size_t$$.

$head test$$
The $code speed_test$$ argument $italic test$$ is a function with the syntax
$syntax%
	%test%(%size%, %repeat%)
%$$
and its return value is $code void$$.

$subhead size$$
The $italic test$$ argument $italic size$$ has prototype
$syntax%
	size_t %size%
%$$
It specifies the size for this test.

$subhead repeat$$
The $italic test$$ argument $italic repeat$$ has prototype
$syntax%
	size_t %repeat%
%$$
It specifies the number of times to repeat the test.

$head size_vec$$
The $code speed_test$$ argument $italic size_vec$$ has prototype
$syntax%
	const %Vector% &%size_vec%
%$$
This vector determines the size for each of the tests problems.

$head time_min$$
The argument $italic time_min$$ has prototype
$syntax%
	double %time_min%
%$$
It specifies the minimum amount of time in seconds
that the $italic test$$ routine should take.
The $italic repeat$$ argument to $italic test$$ is increased
until this amount of execution time is reached.

$head rate_vec$$
The return value $italic rate_vec$$ has prototype
$syntax%
	%Vector% &%rate_vec%
%$$ 
We use $latex n$$ to denote its size which is the same as
the vector $italic size_vec$$.
For $latex i = 0 , \ldots , n-1$$,
$syntax%
	%rate_vec%[%i%]
%$$
is the ratio of $italic repeat$$ divided by time in seconds
for the problem with size $syntax%%size_vec%[%i%]%$$.

$head Timing$$
If your system supports the unix $code gettimeofday$$ function,
it will be used to measure time. 
Otherwise, 
time is measured by the difference in
$codep
	(double) clock() / (double) CLOCKS_PER_SEC
$$
in the context of the standard $code <ctime>$$ definitions.

$head Example$$
$children%
	speed/example/speed_test.cpp
%$$
The section $xref/speed_test.cpp/$$ contains an example and test
of $code speed_test$$.

$end
-----------------------------------------------------------------------
*/

# include <cstddef>
# include <cmath>

# include <cppad/check_simple_vector.hpp>

// undo preprocessor symbols that config.h needs to define
# ifndef CPPAD_CPPAD_INCLUDED
# include <cppad/local/preprocessor.hpp>
# endif

// use config.h to define CPPAD_GETIMEOFDAY preprocessor symbol
# include <cppad/config.h>

// Microsoft versions to not run autoconf, so CPPAD_GETTIMEOFDAY may not be
// set to proper value.
# ifdef _MSC_VER
# undef  CPPAD_GETTIMEOFDAY
# define CPPAD_GETTIMEOFDAY 0
# endif

# if CPPAD_GETTIMEOFDAY
# include <sys/time.h>
# else
# include <ctime>
# endif

# ifndef CPPAD_NULL
# define CPPAD_NULL	0
# endif

namespace CppAD { // BEGIN CppAD namespace

inline double speed_test_second(void)
{
# if CPPAD_GETTIMEOFDAY
	struct timeval value;
	gettimeofday(&value, CPPAD_NULL);
	return double(value.tv_sec) + double(value.tv_usec) * 1e-6;
# else
	return (double) clock() / (double) CLOCKS_PER_SEC;
# endif
}

// implemented as an inline so that can include in multiple link modules
// with this same file
template <class Vector>
inline Vector speed_test( 
	void test(size_t size, size_t repeat),
	Vector size_vec                      ,
	double time_min                      )
{
	// check that size_vec is a simple vector with size_t elements
	CheckSimpleVector<size_t, Vector>();

	size_t   n = size_vec.size();
	Vector rate_vec(n);
	size_t i;
	for(i = 0; i < n; i++)
	{	size_t size   = size_vec[i];
		size_t repeat = 1;
		double s0     = speed_test_second();
		double s1     = speed_test_second();
		while( s1 - s0 < time_min )
		{	repeat = 2 * repeat;
			s0     = speed_test_second();
			test(size, repeat);
			s1     = speed_test_second();
		}
		rate_vec[i] = (size_t)(.5 + repeat / (s1 - s0));
	}
	return rate_vec;
}

} // END CppAD namespace

/*
$begin SpeedTest$$
$spell
	cppad.hpp
	Microsoft
	namespace
	std
	const
	cout
	ctime
	ifdef
	const
	endif
	cpp
$$

$index SpeedTest$$
$index test, speed$$

$section Run One Speed Test and Print Results$$

$head Syntax$$

$code # include <cppad/speed_test.hpp>$$
$pre
$$
$syntax%SpeedTest(%Test%, %first%, %inc%, %last%)%$$


$head Purpose$$
The $code SpeedTest$$ function executes a speed test
for various sized problems
and reports the results on standard output; i.e. $code std::cout$$.
The size of each test problem is included in its report
(unless $italic first$$ is equal to $italic last$$).

$head Motivation$$
It is important to separate small calculation units
and test them individually.
This way individual changes can be tested in the context of the
routine that they are in.
On many machines, accurate timing of a very short execution 
sequences is not possible.
In addition,
there may be set up time for a test that
we do not really want included in the timing.
For this reason $code SpeedTest$$
automatically determines how many times to 
repeat the section of the test that we wish to time.


$head Include$$
The file $code speed_test.hpp$$ contains the 
$code SpeedTest$$ function.
This file is included by $code cppad/cppad.hpp$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head Test$$
The $code SpeedTest$$ argument $italic Test$$ is a function with the syntax
$syntax%
	%name% = %Test%(%size%, %repeat%)
%$$

$subhead size$$
The $italic Test$$ argument $italic size$$ has prototype
$syntax%
	size_t %size%
%$$
It specifies the size for this test.

$subhead repeat$$
The $italic Test$$ argument $italic repeat$$ has prototype
$syntax%
	size_t %repeat%
%$$
It specifies the number of times to repeat the test.

$subhead name$$
The $italic Test$$ result $italic name$$ has prototype
$syntax%
	std::string %name%
%$$
The results for this test are reported on $code std::cout$$
with $italic name$$ as an identifier for the test.
It is assumed that,
for the duration of this call to $code SpeedTest$$,
$italic Test$$ will always return 
the same value for $italic name$$.
If $italic name$$ is the empty string, 
no test name is reported by $code SpeedTest$$.

$head first$$
The $code SpeedTest$$ argument $italic first$$ has prototype
$syntax%
	size_t %first%
%$$
It specifies the size of the first test problem reported by this call to 
$code SpeedTest$$.
	
$head last$$
The $code SpeedTest$$ argument $italic last$$ has prototype
$syntax%
	size_t %last%
%$$
It specifies the size of the last test problem reported by this call to 
$code SpeedTest$$.

$head inc$$
The $code SpeedTest$$ argument $italic inc$$ has prototype
$syntax%
	int %inc%
%$$
It specifies the increment between problem sizes; i.e.,
all values of $italic size$$ in calls to $italic Test$$ are given by
$syntax%
	%size% = %first% + %j% * %inc%
%$$ 
where $italic j$$ is a positive integer.
The increment can be positive or negative but it cannot be zero.
The values $italic first$$, $italic last$$ and $italic inc$$ must 
satisfy the relation
$latex \[
	inc * ( last - first ) \geq 0
\] $$

$head rate$$
The value displayed in the $code rate$$ column on $code std::cout$$
is defined as the value of $italic repeat$$ divided by the 
corresponding elapsed execution time in seconds.
The elapsed execution time is measured by the difference in
$codep
	(double) clock() / (double) CLOCKS_PER_SEC
$$
in the context of the standard $code <ctime>$$ definitions.


$head Errors$$
If one of the restrictions above is violated,
the CppAD error handler is used to report the error.
You can redefine this action using the instructions in
$xref/ErrorHandler/$$

$head Example$$
$children%
	speed/example/speed_program.cpp
%$$
The section $xref/speed_program.cpp/$$ contains an example usage
of $code SpeedTest$$.

$end
-----------------------------------------------------------------------
*/
// BEGIN PROGRAM


# include <string>
# include <iostream>
# include <iomanip>
# include <cppad/local/cppad_assert.hpp>

namespace CppAD { // BEGIN CppAD namespace

inline void SpeedTestNdigit(size_t value, size_t &ndigit, size_t &pow10)
{	pow10 = 10;
	ndigit       = 1;
	while( pow10 <= value )
	{	pow10  *= 10;
		ndigit += 1;
	}
}

// implemented as an inline so that can include in multiple link modules
// with this same file
inline void SpeedTest( 
	std::string Test(size_t size, size_t repeat),
	size_t first,
	int    inc,
	size_t last
)
{

	using std::cout;
	using std::endl;

	size_t    size;
	size_t    repeat;
	size_t    rate;
	size_t    digit;
	size_t    ndigit;
	size_t    pow10;
	size_t    maxSize;
	size_t    maxSizeDigit;

	double    s0;
	double    s1;

	std::string name;

	CPPAD_ASSERT_KNOWN( 
		inc != 0 && first != 0 && last != 0,
		"inc, first, or last is zero in call to SpeedTest"
	);
	CPPAD_ASSERT_KNOWN( 
		(inc > 0 && first <= last) || (inc < 0 && first >= last),
		"SpeedTest: increment is positive and first > last or "
		"increment is negative and first < last"
	);

	// compute maxSize
	maxSize = size = first;
	while(  (inc > 0 && size <= last) || (inc < 0 && size >= last) )
	{
		if( size > maxSize )
			maxSize = size;

		// next size
		if( ((int) size) + inc > 0 )
			size += inc;
		else	size  = 0;
	}
	SpeedTestNdigit(maxSize, maxSizeDigit, pow10);

	size = first;
	while(  (inc > 0 && size <= last) || (inc < 0 && size >= last) )
	{
		repeat = 1;
		s0     = speed_test_second();
		s1     = speed_test_second();
		while( s1 - s0 < 1. )
		{	repeat = 2 * repeat;
			s0     = speed_test_second();
			name   = Test(size, repeat);
			s1     = speed_test_second();
		}
		rate = (size_t)(.5 + repeat / (s1 - s0));
		

		if( size == first && name != "" )
			cout << name << endl;

		if( first != last )
		{
			// convert int(size_t) to avoid warning on _MSC_VER sys
			std::cout << "size = "  << int(size);

			SpeedTestNdigit(size, ndigit, pow10);
			while( ndigit < maxSizeDigit )
			{	cout << " ";
				ndigit++;
			}
			cout << " ";
		}

		cout << "rate = ";
		SpeedTestNdigit(rate, ndigit, pow10);
		while( ndigit > 0 )
		{
			pow10 /= 10;
			digit  = rate / pow10;

			// convert int(size_t) to avoid warning on _MSC_VER sys
			std::cout << int(digit);

			rate    = rate % pow10;
			ndigit -= 1;

			if( (ndigit > 0) && (ndigit % 3 == 0) )
				cout << ",";
		}
		cout << endl;

		// next size
		if( ((int) size) + inc > 0 )
			size += inc;
		else	size  = 0;
	}
	return;
}

} // END CppAD namespace

// END PROGRAM
# endif
