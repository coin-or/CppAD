/* -----------------------------------------------------------------------
CppAD: C++ Algorithm Differentiation Copyright (C) 2003-05 Bradley M. Bell
----------------------------------------------------------------------- */

/*
$begin SpeedTestExample$$
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

$index SpeedTest, example$$
$index example, SpeedTest$$

$section Example Use of SpeedTest$$

$head Program$$
$codep */
# include <CppAD/SpeedTest.h>
# include <string>

std::string Test(size_t size, size_t repeat)
{	// initialization section
	double *a = new double[size];
	double *b = new double[size];
	double *c = new double[size];
	size_t i  = size;;
	while(i)
	{	--i;
		a[i] = i;
		b[i] = 2 * i;
	}

	// operations we are timing
	while(repeat--)
	{	i = size;;
		while(i)
		{	--i;
			c[i] = a[i] + b[i];
		}
	}

	// return a test name that is valid for all sizes and repeats
	return "double: c[*] = a[*] + b[*]";
}
int main(void)
{
	CppAD::SpeedTest(Test, 10, 10, 100);
	return 0;
}

/* $$

$head Output$$
Executing of the program above resulted in the following
output (the rates will be different on your system):
 
$code
$verbatim%lib/SpeedTestExample.out%$$
$$

$end
*/
