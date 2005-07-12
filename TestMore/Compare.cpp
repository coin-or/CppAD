// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

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
// END SHORT COPYRIGHT

/*
Check comparison operators between AD< AD<Base> > and Base, int
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Compare(void)
{	bool ok = true;
	typedef CppAD::AD< CppAD::AD<double> > ADDdouble;

	using namespace CppAD;

	AD<double> three  = 3;
	AD<double> middle = 3.5;
	AD<double> four   = 4;

	ADDdouble x = three;
	ADDdouble y = four;
	ADDdouble z = four;

	// AD<double> <op> ADDdouble
	ok &= (middle >  x);   
	ok &= (middle >= x);   
	ok &= (middle <  y);   
	ok &= (middle <= y);   
	ok &= (middle != y);   
	ok &= (four  == y);   

	// int <op> ADDdouble
	ok &= (4 >  x);   
	ok &= (3 >= x);   
	ok &= (3 <  y);   
	ok &= (3 <= y);   
	ok &= (3 != y);   
	ok &= (4 == y);

	// ADDdouble <op> AD<double>
	ok &= (x <  middle);   
	ok &= (x <= middle);   
	ok &= (y >  middle);   
	ok &= (y >= middle);   
	ok &= (y != middle);   
	ok &= (y == four);   

	// ADDdouble <op> int
	ok &= (x <  4);   
	ok &= (x <= 3);   
	ok &= (y >  3);   
	ok &= (y >= 3);   
	ok &= (y != 3);   
	ok &= (y == 4);

	return ok;
}

// END PROGRAM
