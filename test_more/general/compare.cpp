/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Check comparison operators between AD< AD<Base> > and Base, int
*/
# include <cppad/cppad.hpp>

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
