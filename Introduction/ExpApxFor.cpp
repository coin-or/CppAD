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
$begin ExpApxFor.cpp$$
$spell
	namespace
	ExpApxFor
	const
	vname
	vindex
	iostream
	cout
	std
	endl
	vars
$$

$section An Example Forward Mode Trace$$
$codep */

# include <iostream>                        // C++ standard input/output
extern void ExpApxSeq(double x, double e);  // prototype for ExpApxSeq
extern double a[1], q[3], r[3], s[3], k[3]; // global vars set by ExpApxSeq
namespace { // empty namespace
	void Print(const char *vname, size_t vindex, double v_x )
	{	std::cout << vname << vindex << "_x = " << v_x;
		std::cout << std::endl;
	}
}
int main(void)
{	double a_x[1], q_x[3], r_x[3], s_x[3];

	// compute the global variables 
	double x = .5, e = .1;
	ExpApxSeq(x, e);

	// begin forward mode
	r_x[0] = s_x[0] = 0.;
	a_x[0] = 1.;
	Print("a", 0, a_x[0]);
	size_t j;
	for(j = 1; j <= 2; j++) 
	{	q_x[j] = r_x[j-1] * a[0] + r[j-1] * a_x[0];  // q = r * a
		Print("q", j, q_x[j]);

		r_x[j] = q_x[j] / k[j-1];                    // r = q / k
		Print("r", j, r_x[j]);

		s_x[j]     = s_x[j-1] + r_x[j];              // s = s + r
		Print("s", j, s_x[j]);
	}
	return 0;
}
/* $$
$end
*/
