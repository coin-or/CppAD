# ifndef CPPAD_LOCAL_IS_POD_HPP
# define CPPAD_LOCAL_IS_POD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin is_pod dev}
{xrst_spell
   nullptr
}

The is_pod Template Function
############################

Default Definition
******************
The default template definition is that

   ``is_pod`` < *Type* >()

is false for all types.

Fundamental Types
*****************
This file specializes ``is_pod`` < *Type* > to be true where *Type*
is any of the c++11 fundamental types that hold data; i.e.,
``void`` and ``nullptr_t`` are excluded.

Other Type
**********
You can inform CppAD that a particular *Type* is plain old data by
defining

| |tab| ``namespace CppAD`` { ``namespace local`` {
| |tab| |tab| ``template <> inline bool is_pod<`` *Type* >( ``void`` ) { ``return true`` ; }
| |tab| } }

{xrst_end is_pod}
*/
namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
   //
   template <class T> inline bool is_pod(void) { return false; }
   // bool
   template <> inline bool is_pod<bool>(void)                   {return true;}
   // short
   template <> inline bool is_pod<short int>(void)              {return true;}
   template <> inline bool is_pod<unsigned short int>(void)     {return true;}
   // int
   template <> inline bool is_pod<int>(void)                    {return true;}
   template <> inline bool is_pod<unsigned int>(void)           {return true;}
   // long
   template <> inline bool is_pod<long int>(void)               {return true;}
   template <> inline bool is_pod<unsigned long int>(void)      {return true;}
   // long long
   template <> inline bool is_pod<long long int>(void)          {return true;}
   template <> inline bool is_pod<unsigned long long int>(void) {return true;}
   // Character types
   template <> inline bool is_pod<char>(void)                   {return true;}
   template <> inline bool is_pod<signed char>(void)            {return true;}
   template <> inline bool is_pod<unsigned char>(void)          {return true;}
   template <> inline bool is_pod<wchar_t>(void)                {return true;}
   template <> inline bool is_pod<char16_t>(void)               {return true;}
   template <> inline bool is_pod<char32_t>(void)               {return true;}
   // floating point types
   template <> inline bool is_pod<float>(void)                  {return true;}
   template <> inline bool is_pod<double>(void)                 {return true;}
   template <> inline bool is_pod<long double>(void)            {return true;}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
