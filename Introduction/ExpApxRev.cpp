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
$begin ExpApxRev.cpp$$
$spell
	namespace
	vars
	ExpApx
	const
	fname
	findex
	vname
	vindex
	iq
	ir
	cpp
	iostream
	cout
	std
	endl
	ia
	df
$$

$section An Example Reverse Mode Trace$$
$codep */
# include <iostream>                        // C++ standard input/output
extern void ExpApxSeq(double x, double e);  // prototype for ExpApxSeq
extern double a[1], q[3], r[3], s[3], k[3]; // global vars set by ExpApxSeq
namespace { // empty namespace
	void Print(const char *fname, size_t findex, 
	           const char *vname, size_t vindex, double f_v )
	{	std::cout << fname << findex << "_" 
		          << vname << vindex << " = " << f_v;
		std::cout << std::endl;
	}
}
int main(void)
{	// ordering of arguments is: 
	// a0, q0, q1, q2, r0, r1, r2, s0, s1, s2 
	// corresponding index offsets for each of the parameters
	size_t ia = 0, iq = ia+1, ir = iq+3, is = ir+3;

	// compute the global variables 
	double x = .5, e = .1;
	ExpApxSeq(x, e);

	// initial all partial derivatives as zero
	double df[10];
	size_t j;
	for(j = 0; j < 10; j++)
		df[j] = 0.;

	// f0
	df[is+2] = 1.;
	Print("f", 0, "s", 2, df[is + 2]);

	for(j = 2; j >= 1; j--) 
	{	// remove s_j = s_{j-1} + r_j from f_{6-3*j} to get f_{7-3*j}
		df[is + j-1] += df[is + j];
		df[ir + j]   += df[is + j];
		Print("f", 7-3*j, "s", j-1, df[is + j-1]);
		Print("f", 7-3*j, "r", j,   df[ir + j]);

		// remove r_j = q_j / k_{j-1} from f_{7-3*j} to get f_{8-3*j}
		df[iq + j]   += df[ir + j] / k[j-1];
		Print("f", 8-3*j, "q", j,   df[iq + j]);

		// remove q_j = r_{j-1} * a_0 from f_{8-3*j} to get f_{9-3*j}
		df[ir + j-1] += df[iq + j] * a[0];
		df[ia + 0]   += df[iq + j] * r[j-1];
		Print("f", 9-3*j, "r", j-1, df[ir + j-1]);
		Print("f", 9-3*j, "a", 0,   df[ia + 0]);
	}
	return 0;
}
/* $$
$end
*/
