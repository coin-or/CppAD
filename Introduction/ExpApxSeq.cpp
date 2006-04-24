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
$begin ExpApxSeq.cpp$$
$spell
	namespace
	Apx
	fstream
	ofstream
	iostream
	std
	cout
	endl
	Exp
	const
	vname
	vindex
$$
$section An Example Operation Sequence Trace$$
$codep */

# include <fstream>                  // used to write ExpApxSeq.out
double a[1], q[3], r[3], s[3], k[3]; // global variables set by ExpApxSeq
namespace { // empty namespace
	void Print(std::ofstream &file, 
	           const char *vname, size_t vindex, double v)
	{	file << vname << vindex << " = " << v;
		file << std::endl;
	}
}
void ExpApxSeq(double x, double e)
{	r[0] = s[0] = k[0] = 1.;                   // r = s = k = Type(1);

	// open the output file
	std::ofstream file( "ExpApxSeq.out" );

	a[0] = x;                                  // a = x;
	Print(file, "a", 0, a[0]);

	size_t j;
	for(j = 1; j <= 2; j++) 
	{	q[j] = r[j-1] * a[0];              // q  = a * r;
		Print(file, "q", j, q[j]);

		r[j] = q[j] / k[j-1];              // r  = q / k;
		Print(file, "r", j, r[j]);

		s[j]     = s[j-1] + r[j];          // s  = s + r;
		Print(file, "s", j, s[j]);

		k[j]     = k[j-1] + 1.;            // k  = k + Type(1);
		Print(file, "k", j, k[j]);
	}
}
/* $$
$end
*/
