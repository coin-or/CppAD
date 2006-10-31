# ifndef CppADSpeedTestIncluded
# define CppADSpeedTestIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin SpeedTest$$
$spell
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

$section Run One Speed Test$$

$head Syntax$$

$code # include <CppAD/SpeedTest.h>$$
$pre
$$
$syntax%SpeedTest(%Test%, %first%, %inc%, %last%)%$$


$head Purpose$$
The $code SpeedTest$$ function executes a speed test
for various sized problems
and reports the results on standard output; i.e. $code std::cout$$.
The size of each test problem is included in its report
(unless $italic first$$ is equal to $italic last$$).

$head Repeat Factor$$
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
The file $code SpeedTest.h$$ contains the 
$code SpeedTest$$ function.
This file is included by $code CppAD/CppAD.h$$
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
	speed_example/speed_example.cpp
%$$
The section $xref/SpeedExample.cpp/$$ contains an example usage
of $code SpeedTest$$.

$end
-----------------------------------------------------------------------
*/
// BEGIN PROGRAM


# include <cstddef>
# include <ctime>
# include <cmath>
# include <string>
# include <iostream>
# include <iomanip>

// For an unknown reason, cannot move other includes (using Sun's CC compiler)
# include <CppAD/local/cppad_error.hpp>
# include <CppAD/SpeedTest.h>

namespace CppAD { // BEGIN CppAD namespace

inline double SpeedTestSecond(void)
{	return (double) clock() / (double) CLOCKS_PER_SEC;
}
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

	CppADUsageError( 
		inc != 0 && first != 0 && last != 0,
		"inc, first, or last is zero in call to SpeedTest"
	);
	CppADUsageError( 
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
		s0     = SpeedTestSecond();
		s1     = SpeedTestSecond();
		while( s1 - s0 < 1. )
		{	repeat = 2 * repeat;
			s0     = SpeedTestSecond();
			name   = Test(size, repeat);
			s1     = SpeedTestSecond();
		}
		rate = (size_t)(.5 + repeat / (s1 - s0));
		

		if( size == first && name != "" )
			cout << name << endl;

		if( first != last )
		{
# ifdef _MSC_VER        // convert to integer to avoid warning in this case
			std::cout << "size = "  << int(size);
# else
			std::cout << "size = "  << size;
# endif
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
# ifdef _MSC_VER        // convert to integer to avoid warning in this case
			std::cout << int(digit);
# else
			std::cout << digit;
# endif
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
