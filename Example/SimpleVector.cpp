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
$begin SimpleVector.cpp$$
$spell
	Cpp
$$

$section Simple Vector Template Class: Example and Test$$

$index vector, simple$$
$index simple, vector example$$
$index example, simple vector$$
$index test, simple vector$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/SimpleVector.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
--------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <iostream>                   // std::cout and std::endl

# include <vector>                     // std::vector
# include <valarray>                   // std::valarray
# include <CppAD/CppAD_vector.h>       // CppAD::vector
# include <CppAD/CheckSimpleVector.h>  // CppAD::CheckSimpleVector
namespace {
	template <typename Vector>
	bool Ok(void)
	{	// type corresponding to elements of Vector
		typedef typename Vector::value_type Scalar;

		bool ok = true;             // initialize testing flag

		Vector x;                   // use the default constructor
		ok &= (x.size() == 0);      // test size for an empty vector
		Vector y(2);                // use the sizing constructor
		ok &= (y.size() == 2);      // size for an vector with elements

		// non-const access to the elements of y
		size_t i;                   
		for(i = 0; i < 2; i++)
			y[i] = Scalar(i); 

		const Vector z(y);          // copy constructor
		x.resize(2);                // resize 
		x = z;                      // vector assignment

		// use the const access to the elements of x
		// and test the values of elements of x, y, z
		for(i = 0; i < 2; i++)
		{	ok &= (x[i] == Scalar(i));
			ok &= (y[i] == Scalar(i));
			ok &= (z[i] == Scalar(i));
		}
		return ok;
	}
}
bool SimpleVector (void)
{	bool ok = true;

	// use routine above to check these cases
	ok &= Ok< std::vector<double> >();
	ok &= Ok< std::valarray<float> >();
	ok &= Ok< CppAD::vector<int> >();
# ifndef _MSC_VER
	// Avoid Microsoft following compiler warning:  'size_t' : 
	// forcing value to bool 'true' or 'false' (performance warning) 
	ok &= Ok< std::vector<bool> >();
	ok &= Ok< CppAD::vector<bool> >();
# endif
	// use CheckSimpleVector for more extensive testing
	CppAD::CheckSimpleVector<double, std::vector<double>  >();
	CppAD::CheckSimpleVector<float,  std::valarray<float> >();
	CppAD::CheckSimpleVector<int,    CppAD::vector<int>   >();
	CppAD::CheckSimpleVector<bool,   std::vector<bool>    >();
	CppAD::CheckSimpleVector<bool,   CppAD::vector<bool>  >();

	return ok;
}
// END PROGRAM
