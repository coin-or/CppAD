// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin index_sort.cpp}

Index Sort: Example and Test
############################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end index_sort.cpp}
*/
// BEGIN C++
# include <cppad/utility/index_sort.hpp>
# include <cppad/utility/vector.hpp>
# include <valarray>
# include <vector>


namespace{
   // class that uses < to compare a pair of size_t values
   class Key {
   public:
      size_t first_;
      size_t second_;
      //
      Key(void)
      { }
      //
      Key(size_t first, size_t second)
      : first_(first), second_(second)
      { }
      //
      bool operator<(const Key& other) const
      {  if( first_ == other.first_ )
            return second_ < other.second_;
         return first_ < other.first_;
      }
   };

   template <class KeyVector, class SizeVector>
   bool vector_case(void)
   {  bool ok = true;
      size_t i, j;
      size_t first[]  =  { 4, 4, 3, 3, 2, 2, 1, 1};
      size_t second[] = { 0, 1, 0, 1, 0, 1, 0, 1};
      size_t size     = sizeof(first) / sizeof(first[0]);

      KeyVector keys(size);
      for(i = 0; i < size; i++)
         keys[i] = Key(first[i], second[i]);

      SizeVector ind(size);
      CppAD::index_sort(keys, ind);

      // check that all the indices are different
      for(i = 0; i < size; i++)
      {  for(j = 0; j < size; j++)
            ok &= (i == j) || (ind[i] != ind[j]);
      }

      // check for increasing order
      for(i = 0; i < size-1; i++)
      {  if( first[ ind[i] ] == first[ ind[i+1] ] )
            ok &= second[ ind[i] ] <= second[ ind[i+1] ];
         else
            ok &= first[ ind[i] ] < first[ ind[i+1] ];
      }

      return ok;
   }
}

bool index_sort(void)
{  bool ok = true;

   // some example simple vector template classes
   ok &= vector_case<  std::vector<Key>,  std::valarray<size_t> >();
   ok &= vector_case< std::valarray<Key>, CppAD::vector<size_t> >();
   ok &= vector_case< CppAD::vector<Key>,   std::vector<size_t> >();

   return ok;
}

// END C++
