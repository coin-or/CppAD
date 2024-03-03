# ifndef CPPAD_EXAMPLE_VALVECTOR_SPLIT_JOIN_HPP
# define CPPAD_EXAMPLE_VALVECTOR_SPLIT_JOIN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
/*
-------------------------------------------------------------------------------
{xrst_begin valvector_ad_split}
{xrst_spell
   asplit
}

Split A AD valvector
####################
Split one AD<valvector> into a vector of AD<valvector> each with size one.

Syntax
******
| valvector_ad_split *asplit*
| *asplit( *ax* , *ay_vec* )

m
*
We use *m* to denote *ay_vec* .size()

ax
**
This CppAD::AD<valvector> is ``const`` and passed by reference.
The size *ax*.size()  must be equal to one or *m*

ay_vec
******
This is a :ref:`SimpleVector-name` with elements of type CppAD::AD<valvector>
and is passed by reference.
The size of *ay_vec* is not changed.
Upon return, for *i* = 0 , ... , *m* - 1 ,
the size *ay* [ *i* ].size() is one and::

   ay[i][0] = ax[i]

{xrst_toc_hidden
   example/valvector/ad_split.cpp
}
Example
*******
The file :ref:`valvector_ad_split.cpp-name` is an example and test
of this operation.

{xrst_end valvector_ad_split}
-------------------------------------------------------------------------------
{xrst_begin valvector_ad_join}
{xrst_spell
   ajoin
   valvectors
}

Join a Vector of AD valvectors
##############################
Join a vector of AD<valvector>, each with size one, into one AD<valvector>.

Syntax
******
| valvector_ad_join *ajoin*
| *ajoin( *ax_vec* , *ay* )


m
*
We use *m* to denote *ax_vec* .size() .

ax_vec
******
This is a :ref:`SimpleVector-name` with elements of type CppAD::AD<valvector> .
It is ``const`` and is passed by reference.
For *i* = 0 , ... , *m* - 1 ,
the size *ax* [ *i* ].size() is one.

ay
**
This CppAD::AD<valvector> is passed by reference and its input value
does not matter.
Upon return, its size in *m* and
for *i* = 0 , ... , *m* - 1 ::

   ay[i] = ax_vec[i][0]


{xrst_toc_hidden
   example/valvector/ad_join.cpp
}
Example
*******
The file :ref:`valvector_ad_join.cpp-name` is an example and test
of this operation.

{xrst_end valvector_ad_join}
-------------------------------------------------------------------------------
*/
# include <cppad/example/valvector/class.hpp>
# include <cppad/cppad.hpp>
//
// valvector_split_join
// split and join are in same atomic function so that in AD version of reverse
// split can call join and join can call split.
class valvector_split_join : public CppAD::atomic_four<valvector> {
public:
   //
   // ctor
   valvector_split_join(const std::string& name) :
   CppAD::atomic_four<valvector>(name)
   { }
   //
   // is_split
   static bool is_split(size_t call_id)
   {  return call_id == 1; }
   //
   // is_join
   static bool is_join(size_t call_id)
   {  return call_id == 2; }
private:
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // ------------------------------------------------------------------------
   // for_type
   bool for_type(
      size_t                                     call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
   {
      if( is_split(call_id) )
      {  //
         assert( type_x.size() == 1 ); // n
         //
         // type_y
         size_t m = type_y.size();
         for(size_t i = 0; i < m; ++i)
            type_y[i] = type_x[0];
      }
      else
      {  assert( is_join(call_id) );
         //
         assert( type_y.size() == 1 ); // m
         //
         // type_y
         size_t n  = type_x.size();
         type_y[0] = CppAD::identical_zero_enum;
         for(size_t j = 0; j < n; ++j)
            type_y[0] = std::max( type_y[0], type_x[j] );
      }
      return true;
   }
   // ------------------------------------------------------------------------
   // forward
   bool forward(
      size_t                              call_id      ,
      const CppAD::vector<bool>&          select_y     ,
      size_t                              order_low    ,
      size_t                              order_up     ,
      const CppAD::vector<valvector>&     taylor_x     ,
      CppAD::vector<valvector>&           taylor_y     ) override
   {  //
      if( is_split(call_id) )
      {  //
         // q, m
         size_t q = order_up + 1;
         size_t m = taylor_y.size() / q;
         //
# ifndef NDEBUG
         size_t n = taylor_x.size() / q;
         assert( n == 1 );
         assert( m == select_y.size() );
         for(size_t k = 0; k < q; ++k)
            assert( taylor_x[k].size() == 1 || taylor_x[k].size() == m );
# endif
         //
         // taylor_y
         for(size_t i = 0; i < m; ++i) if( select_y[i] )
         {  for(size_t k = order_low; k < q; ++k)
            {  //
               // taylor_y[i * q + k]
               taylor_y[i * q + k].resize(1);
               taylor_y[i * q + k][0] = taylor_x[k][i];
            }
         }
      }
      else
      {  assert( is_join(call_id) );
         //
         // q, n
         size_t q = order_up + 1;
         size_t n = taylor_x.size() / q;
         //
# ifndef NDEBUG
         size_t m = taylor_y.size() / q;
         assert( m == 1 );
         assert( m == select_y.size() );
         for(size_t k = 0; k < q; ++k)
            assert( taylor_x[k].size() == 1 );
# endif
         //
         // taylor_y
         if( select_y[0] )
         {  for(size_t k = order_low; k < q; ++k)
            {  //
               // taylor_y[k]
               taylor_y[k].resize(n);
               for(size_t j = 0; j < n; ++j)
                  taylor_y[k][j] = taylor_x[j * q + k][0];
            }
         }
         //
      }
      return true;
   }
   // ------------------------------------------------------------------------
   // forward
   bool forward(
      size_t                                 call_id      ,
      const CppAD::vector<bool>&             select_y     ,
      size_t                                 order_low    ,
      size_t                                 order_up     ,
      const CppAD::vector<ad_valvector>&     ataylor_x    ,
      CppAD::vector<ad_valvector>&           ataylor_y    ) override
   {  //
      if( is_split(call_id) )
      {  //
         // q, m
         size_t q = order_up + 1;
         size_t m = ataylor_y.size() / q;
         //
# ifndef NDEBUG
         size_t n = ataylor_x.size() / q;
         assert( n == 1 );
         assert( m == select_y.size() );
# endif
         //
         // ax_k, ay_k
         CppAD::vector<ad_valvector> ax_k(1);
         CppAD::vector<ad_valvector> ay_k(m);
         //
         // ataylor_y
         for(size_t k = order_low; k < q; ++k)
         {  ax_k[0] = ataylor_x[k];
            (*this)(call_id, ax_k, ay_k);
            {  for(size_t i = 0; i < m; ++i)
                  ataylor_y[i * q + k] = ay_k[i];
            }
         }
      }
      else
      {  assert( is_join(call_id) );
         //
         // q, n
         size_t q = order_up + 1;
         size_t n = ataylor_x.size() / q;
         //
# ifndef NDEBUG
         size_t m = ataylor_y.size() / q;
         assert( m == 1 );
         assert( m == select_y.size() );
# endif
         if( select_y[0] )
         {  //
            // ax_k, ay_k
            CppAD::vector<ad_valvector> ax_k(n);
            CppAD::vector<ad_valvector> ay_k(1);
            //
            // ataylor_y
            for(size_t k = order_low; k < q; ++k)
            {  for(size_t j = 0; j < n; ++j)
                  ax_k[j] = ataylor_x[j * q + k];
               (*this)(call_id, ax_k, ay_k);
               ataylor_y[k] = ay_k[0];
            }
         }
      }
      return true;
   }
   // ------------------------------------------------------------------------
   // reverse
   bool reverse(
      size_t                              call_id      ,
      const CppAD::vector<bool>&          select_x     ,
      size_t                              order_up     ,
      const CppAD::vector<valvector>&     taylor_x     ,
      const CppAD::vector<valvector>&     taylor_y     ,
      CppAD::vector<valvector>&           partial_x    ,
      const CppAD::vector<valvector>&     partial_y    ) override
   {  //
      if( is_split(call_id) )
      {  //
         // q, m
         size_t q = order_up + 1;
         size_t m = taylor_y.size() / q;
         //
# ifndef NDEBUG
         size_t n = taylor_x.size() / q;
         assert( n == 1 );
         assert( 1 == select_x.size() );
# endif
         if( ! select_x[0] )
            return true;
         //
         // partial_x
         for(size_t k = 0; k < q; ++k)
         {  partial_x[k].resize(m);
            for(size_t i = 0; i < m; ++i)
            {  //
               // partial_x[k][i]
               assert( taylor_y[i * q + k].size() == 1 );
               partial_x[k][i] = partial_y[i * q + k][0];
            }
         }
      }
      else
      {  assert( is_join(call_id) );
         //
         // q, m
         size_t q = order_up + 1;
         size_t n = taylor_x.size() / q;
         //
# ifndef NDEBUG
         size_t m = taylor_y.size() / q;
         assert( m == 1 );
         assert( n == select_x.size() );
# endif
         //
         // partial_x
         for(size_t k = 0; k < q; ++k)
         {  assert( taylor_y[k].size() == n );
            for(size_t j = 0; j < n; ++j)
            {  partial_x[j * q + k].resize(1);
               partial_x[j * q + k][0] = partial_y[k][j];
            }
         }
      }
      return true;
   }
   // ------------------------------------------------------------------------
   // reverse
   bool reverse(
      size_t                                 call_id      ,
      const CppAD::vector<bool>&             select_x     ,
      size_t                                 order_up     ,
      const CppAD::vector<ad_valvector>&     ataylor_x    ,
      const CppAD::vector<ad_valvector>&     ataylor_y    ,
      CppAD::vector<ad_valvector>&           apartial_x   ,
      const CppAD::vector<ad_valvector>&     apartial_y   ) override
   {  //
      if( is_split(call_id) )
      {  //
         // q, m
         size_t q = order_up + 1;
         size_t m = ataylor_y.size() / q;
         //
# ifndef NDEBUG
         size_t n = ataylor_x.size() / q;
         assert( n == 1 );
         assert( 1 == select_x.size() );
# endif
         if( ! select_x[0] )
            return true;
         //
         // ay_k, ax_k
         CppAD::vector<ad_valvector> ay_k(m);
         CppAD::vector<ad_valvector> ax_k(1);
         //
         // join_call_id
         size_t join_call_id = 2;
         assert( is_join(join_call_id) );
         //
         // apartial_x
         for(size_t k = 0; k < q; ++k)
         {  for(size_t i = 0; i < m; ++i)
               ay_k[i] = apartial_y[i * q + k];
            // split calls join here
            (*this)(join_call_id, ay_k, ax_k);
            apartial_x[k] = ax_k[0];
         }
      }
      else
      {  assert( is_join(call_id) );
         //
         // q, m
         size_t q = order_up + 1;
         size_t n = ataylor_x.size() / q;
         //
# ifndef NDEBUG
         size_t m = ataylor_y.size() / q;
         assert( m == 1 );
         assert( n == select_x.size() );
# endif
         //
         // ay_k, ax_k
         CppAD::vector<ad_valvector> ay_k(1);
         CppAD::vector<ad_valvector> ax_k(n);
         //
         // split_call_id
         size_t split_call_id = 1;
         assert( is_split(split_call_id) );
         //
         // apartial_x
         for(size_t k = 0; k < q; ++k)
         {  ay_k[0] = apartial_y[k];
            // Join calls split here
            (*this)(split_call_id, ay_k, ax_k);
            for(size_t j = 0; j < n; ++j)
               apartial_x[j * q + k] = ax_k[j];
         }
      }
      return true;
   }
   // ------------------------------------------------------------------------
   // jac_sparsity
   bool jac_sparsity(
      size_t                                         call_id      ,
      bool                                           dependency   ,
      const CppAD::vector<bool>&                     ident_zero_x ,
      const CppAD::vector<bool>&                     select_x     ,
      const CppAD::vector<bool>&                     select_y     ,
      CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  ) override
   {  //
      if( is_split(call_id) )
      {  //
         // m, n
         size_t m = select_y.size();
         size_t n = select_x.size();
         //
         assert( n == 1 );
         //
         // nnz
         size_t nnz = 0;
         if( select_x[0] )
         {  for(size_t i = 0; i < m; ++i)
            {  if( select_y[i] )
                  ++nnz;
            }
         }
         //
         // pattern_out
         pattern_out.resize(m, n, nnz);
         size_t k = 0;
         if( select_x[0] )
         {  for(size_t i = 0; i < m; ++i)
            {  if( select_y[i] )
                  pattern_out.set(k++, i, 0);
            }
         }
      }
      else
      {  assert( is_join(call_id) );
         //
         // m, n
         size_t m = select_y.size();
         size_t n = select_x.size();
         //
         assert( m == 1 );
         //
         // nnz
         size_t nnz = 0;
         if( select_y[0] )
         {  for(size_t j = 0; j < n; ++j)
            {  if( select_x[j] )
                  ++nnz;
            }
         }
         //
         // pattern_out
         pattern_out.resize(m, n, nnz);
         size_t k = 0;
         if( select_y[0] )
         {  for(size_t j = 0; j < n; ++j)
            {  if( select_x[j] )
                  pattern_out.set(k++, 0, j);
            }
         }
      }
      return true;
   }
   // ------------------------------------------------------------------------
   // hes_sparsity
   bool hes_sparsity(
      size_t                                         call_id      ,
      const CppAD::vector<bool>&                     ident_zero_x ,
      const CppAD::vector<bool>&                     select_x     ,
      const CppAD::vector<bool>&                     select_y     ,
      CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  ) override
   {  //
      if( is_split(call_id) )
      {  //
         // n
         size_t n = select_x.size();
         //
         assert( n == 1 );
         assert( n == select_x.size() );
         //
         // pattern_out
         size_t nnz = 0;
         pattern_out.resize(n, n, nnz);
      }
      else
      {  assert( is_join(call_id) );
         //
         // n
         size_t n = select_x.size();
         //
         assert( n == select_x.size() );
         //
         // pattern_out
         size_t nnz = 0;
         pattern_out.resize(n, n, nnz);
      }
      return true;
   }
   // ------------------------------------------------------------------------
   // rev_depend
   bool rev_depend(
      size_t                                         call_id      ,
      const CppAD::vector<bool>&                     ident_zero_x ,
      CppAD::vector<bool>&                           depend_x     ,
      const CppAD::vector<bool>&                     depend_y     ) override
   {  //
      if( is_split(call_id) )
      {  //
         // m
         size_t m = depend_y.size();
         //
# ifndef NDEBUG
         size_t n = depend_x.size();
         assert( n == 1 );
# endif
         //
         // depend_x
         depend_x[0] = false;
         for(size_t i = 0; i < m; ++i)
            depend_x[0] |= depend_y[i];
      }
      else
      {  assert( is_join(call_id) );
         //
         // n
         size_t n = depend_x.size();
         //
# ifndef NDEBUG
         size_t m = depend_y.size();
         assert( m == 1 );
# endif
         //
         // depend_x
         for(size_t j = 0; j < n; ++j)
            depend_x[j] = depend_y[0];
      }
      return true;
   }
};
// ---------------------------------------------------------------------------
// valvector_ad_split
class valvector_ad_split {
private:
   typedef CppAD::AD<valvector> ad_valvector;
   valvector_split_join         atomic_fun_;
public:
   valvector_ad_split(void) : atomic_fun_("valvector_split_join")
   { }
   template <class ADVector>
   void operator()(const ad_valvector&  ax, ADVector& ay)
   {
      size_t call_id = 1;
      assert( atomic_fun_.is_split(call_id) );
      ADVector ax_vec(1);
      ax_vec[0] = ax;
      atomic_fun_(call_id, ax_vec, ay);
      return;
   }
};
// ---------------------------------------------------------------------------
// valvector_ad_join
class valvector_ad_join {
private:
   typedef CppAD::AD<valvector> ad_valvector;
   valvector_split_join         atomic_fun_;
public:
   valvector_ad_join(void) : atomic_fun_("valvector_split_join")
   { }
   template <class ADVector>
   void operator()(const ADVector& ax, ad_valvector&  ay)
   {
      size_t call_id = 2;
      ADVector ay_vec(1);
      assert( atomic_fun_.is_join(call_id) );
      atomic_fun_(call_id, ax, ay_vec);
      ay = ay_vec[0];
      return;
   }
};
# endif
