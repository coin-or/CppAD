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
Check comparison operators between AD< AD<Base> > and Base, int
*/
# include <CppAD/CppAD.h>

namespace { 
	template <class Type>
	bool Compare(void)
	{	bool ok = true;
		using CppAD::AD;
	
		Type      middle = 4;
		AD<double> three = 3;
		AD<double> four  = 4;
		AD<double> five  = 5;
	
		// AD<double> > Type
		ok &= ! (three  >  middle);   
		ok &= ! (four   >  middle);   
		ok &=   (five   >  middle);   
		// Type > AD<double> 
		ok &=   (middle >  three );   
		ok &= ! (middle >  four  );   
		ok &= ! (middle >  five  );   
	
		// AD<double> >= Type
		ok &= ! (three  >= middle);   
		ok &=   (four   >= middle);   
		ok &=   (five   >= middle);   
		// Type > AD<double> 
		ok &=   (middle >= three );   
		ok &=   (middle >= four  );   
		ok &= ! (middle >= five  );   
	
		// AD<double> < Type
		ok &=   (three  <  middle);   
		ok &= ! (four   <  middle);   
		ok &= ! (five   <  middle);   
		// Type > AD<double> 
		ok &= ! (middle <  three );   
		ok &= ! (middle <  four  );   
		ok &=   (middle <  five  );   
	
		// AD<double> <= Type
		ok &=   (three  <= middle);   
		ok &=   (four   <= middle);   
		ok &= ! (five   <= middle);   
		// Type > AD<double> 
		ok &= ! (middle <= three );   
		ok &=   (middle <= four  );   
		ok &=   (middle <= five  );   
	
		return ok;
	}
}
bool Compare(void)
{	bool ok = true;
	ok     &= Compare<int>();
	ok     &= Compare<double>();
	ok     &= Compare< CppAD::AD<double> >();
	return ok;
}
