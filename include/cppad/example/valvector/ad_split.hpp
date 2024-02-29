# ifndef CPPAD_EXAMPLE_VALVECTOR_AD_SPLIT_HPP
# define CPPAD_EXAMPLE_VALVECTOR_AD_SPLIT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/example/valvector/class.hpp>
# include <cppad/cppad.hpp>
/*
{xrst_begin valvector_ad_split}
{xrst_spell
   asplit
   valvectors
}

Split A valvector Into a Vector of valvector
############################################
Split a valvector into a vector of valvectors each with size one.

Syntax
******
| valvector_ad_split *asplit*
| *asplit( *ax* , *ay_vec* )


ax
**
This CppAD::AD<valvector> is ``const`` and passed by reference.
The size *ax*.size()  must be equal to one or *ay_vec* .size() .

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
*/
class valvector_ad_split_atom : public CppAD::atomic_four<valvector> {
public:
   //
   // ctor
   valvector_ad_split_atom(const std::string& name) :
   CppAD::atomic_four<valvector>(name)
   { }
private:
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // scalar_type
   typedef valvector::scalar_type scalar_type;
   // ------------------------------------------------------------------------
   // for_type
   bool for_type(
      size_t                                     call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
   {
      assert( call_id == 0 );       // default value
      assert( type_x.size() == 1 ); // n
      //
      // type_y
      size_t m = type_y.size();
      for(size_t i = 0; i < m; ++i)
         type_y[i] = type_x[0];
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
      // ok
      bool ok = true;
      //
      // q, m
      size_t q = order_up + 1;
      size_t m = taylor_y.size() / q;
      //
# ifndef NDEBUG
      size_t n = taylor_x.size() / q;
      assert( call_id == 0 );
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
      //
      return ok;
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
      // ok
      bool ok = true;
      //
      // q, m
      size_t q = order_up + 1;
      size_t m = ataylor_y.size() / q;
      //
# ifndef NDEBUG
      size_t n = ataylor_x.size() / q;
      assert( call_id == 0 );
      assert( n == 1 );
      assert( m == select_y.size() );
# endif
      //
      // ay_k
      CppAD::vector<ad_valvector> ay_k(m);
      CppAD::vector<ad_valvector> ax_k(1);
      //
      // ataylor_y
      for(size_t k = order_low; k < q; ++k)
      {  ax_k[0] = ataylor_x[k];
         (*this)(call_id, ax_k, ay_k);
         {  for(size_t i = 0; i < m; ++i)
               ataylor_y[i * q + k] = ay_k[i];
         }
      }
      //
      return ok;
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
      // ok
      bool ok = true;
      //
      // q, m
      size_t q = order_up + 1;
      size_t m = taylor_y.size() / q;
      //
# ifndef NDEBUG
      size_t n = taylor_x.size() / q;
      assert( call_id == 0 );
      assert( n == 1 );
      assert( 1 == select_x.size() );
# endif
      if( ! select_x[0] )
         return ok;
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
      //
      return ok;
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
      // ok
      bool ok = true;
      //
      // m
      size_t m = select_y.size();
      size_t n = select_x.size();
      //
      assert( call_id == 0 );
      assert( n == 1 );
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
      return ok;
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
      // ok
      bool ok = true;
      //
      // n
      size_t n = select_x.size();
      //
      assert( call_id == 0 );
      assert( n == 1 );
      //
      // pattern_out
      size_t nnz = 0;
      pattern_out.resize(n, n, nnz);
      //
      return ok;
   }
};

class valvector_ad_split {
private:
   typedef CppAD::AD<valvector> ad_valvector;
   valvector_ad_split_atom      atomic_fun_;
public:
   valvector_ad_split(void) : atomic_fun_("valvector_ad_split_atom")
   { }
   template <class ADVector>
   void operator()(const ad_valvector&  ax, ADVector& ay)
   {
      size_t call_id = 0;
      ADVector ax_vec(1);
      ax_vec[0] = ax;
      atomic_fun_(call_id, ax_vec, ay);
      return;
   }
};

# endif
