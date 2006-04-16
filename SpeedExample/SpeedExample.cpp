/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
/*
$begin SpeedExample.cpp$$
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
$codep
	double: c[*] = a[*] + b[*]
	size = 10  rate = 14,122,236
	size = 20  rate = 7,157,515
	size = 30  rate = 4,972,500
	size = 40  rate = 3,887,214
	size = 50  rate = 3,123,086
	size = 60  rate = 2,685,214
	size = 70  rate = 2,314,737
	size = 80  rate = 2,032,124
	size = 90  rate = 1,814,145
	size = 100 rate = 1,657,828
$$

$end
*/
