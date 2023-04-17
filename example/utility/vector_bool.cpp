// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin vector_bool.cpp}

CppAD::vectorBool Class: Example and Test
#########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end vector_bool.cpp}
*/
// BEGIN C++

# include <cppad/utility/vector_bool.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <sstream> // sstream and string are used to test output operation
# include <string>

bool vectorBool(void)
{  bool ok = true;
   using CppAD::vectorBool;

   // is that boolvector is
   // a simple vector class with elements of type bool
   CppAD::CheckSimpleVector< bool, vectorBool >();

   vectorBool x;          // default constructor
   ok &= (x.size() == 0);

   // resize using size_t or int
   size_t two_s = 2;
   int    two_i = 2;
   x.resize( two_s );
   x.resize( two_i );
   ok &= (x.size() == 2);

   // set element values using size_t and int
   size_t zero_s = 0;
   int    one_i  = 1;
   x[zero_s]     = false;
   x[one_i]      = true;

   vectorBool y(2);       // sizing constructor
   ok &= (y.size() == 2);

   // swap, get element values using size_t and int
   y.swap(x);
   ok &= y[zero_s] == false;
   ok &= y[one_i]  == true;

   const vectorBool z(y); // copy constructor and const element access
   ok &= (z.size() == 2);
   ok &= ( (z[0] == false) && (z[1] == true) );

   x[0] = true;           // modify, assignment changes x
   ok &= (x[0] == true);

   // resize x to zero and check that vector assignment works for both
   // size zero and mathching sizes
   x.resize(0);
   ok &= (x.size() == 0);
   ok &= (y.size() == z.size());
   //
   x = y = z;
   ok &= ( (x[0] == false) && (x[1] == true) );
   ok &= ( (y[0] == false) && (y[1] == true) );
   ok &= ( (z[0] == false) && (z[1] == true) );

   // test of push_vector
   y.push_vector(z);
   ok &= y.size() == 4;
   ok &= ( (y[0] == false) && (y[1] == true) );
   ok &= ( (y[2] == false) && (y[3] == true) );

   y[1] = false;           // element assignment to another element
   x[0] = y[1];
   ok &= (x[0] == false);

   // test of output
   std::string        correct= "01";
   std::string        str;
   std::ostringstream buf;
   buf << z;
   str = buf.str();
   ok &= (str == correct);

   // test resize(0), capacity, and clear
   size_t i = x.capacity();
   ok      &= i > 0;
   x.resize(0);
   ok      &= i == x.capacity();
   x.clear();
   ok      &= 0 == x.capacity();

   // test of push_back element
   for(i = 0; i < 100; i++)
      x.push_back( (i % 3) != 0 );
   ok &= (x.size() == 100);
   for(i = 0; i < 100; i++)
      ok &= ( x[i] == ((i % 3) != 0) );

   // initializer list constructor
   vectorBool w = { true, false, false, true };
   ok &= w.size() == 4;
   ok &= w[0] == true;
   ok &= w[1] == false;
   ok &= w[2] == false;
   ok &= w[3] == true;

   return ok;
}

// END C++
