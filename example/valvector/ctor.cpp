// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_ctor.cpp}

Example and Test of valvector Constructors
##########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_ctor.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool ctor(void)
{  // scalar_type  
   typedef valvector::scalar_type scalar_type;
   //
   // ok
   bool ok = true;
   //
   // ok
   valvector default_ctor;
   ok &= default_ctor.size() == 1;
   //
   // ok
   valvector scalar_ctor( scalar_type(1) );
   ok &= scalar_ctor.size() == 1;
   ok &= scalar_ctor[0] == scalar_type(1);
   //
   // ok
   valvector int_ctor( int(1) );
   ok &= int_ctor.size() == 1;
   ok &= int_ctor[0] == scalar_type(1);
   //
   // ok
   valvector long_int_ctor( (long int)(2) );
   ok &= long_int_ctor.size() == 1;
   ok &= long_int_ctor[0] == scalar_type(2);
   //
   // ok
   valvector double_ctor( double(3) );
   ok &= double_ctor.size() == 1;
   ok &= double_ctor[0] == scalar_type(3);
   //
   // ok
   valvector long_double_ctor( (long double)(4) );
   ok &= long_double_ctor.size() == 1;
   ok &= long_double_ctor[0] == scalar_type(4);
   //
   // ok
   valvector size_t_ctor( size_t(5) );
   ok &= size_t_ctor.size() == 1;
   ok &= size_t_ctor[0] == scalar_type(5);
   //
   // ok
   // Test implicit conversion of list elements to scalar_type
   valvector list_ctor( { 6, 7.0 } );
   ok &= list_ctor.size() == 2;
   ok &= list_ctor[0] == scalar_type(6);
   ok &= list_ctor[1] == scalar_type(7);
   //
   return ok;
}
// END C++
