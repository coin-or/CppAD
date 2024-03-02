# ifndef CPPAD_EXAMPLE_VALVECTOR_AD_JOIN_HPP
# define CPPAD_EXAMPLE_VALVECTOR_AD_JOIN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/example/valvector/class.hpp>
# include <cppad/cppad.hpp>
/*
{xrst_begin valvector_ad_join}
{xrst_spell
   ajoin
   valvectors
}

Join a Vector of valvectors Into One valvector
##############################################
Join a vector of valvectors each with size one, into one valvector.

Under Construction
******************

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
*/
class valvector_ad_join_atom : public CppAD::atomic_four<valvector> {
public:
   //
   // ctor
   valvector_ad_join_atom(const std::string& name) :
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
      assert( type_y.size() == 1 ); // m
      //
      // type_y
      size_t n  = type_x.size();
      type_y[0] = CppAD::identical_zero_enum;
      for(size_t j = 0; j < n; ++j)
         type_y[0] = std::max( type_y[0], type_x[j] );
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
      // q, n
      size_t q = order_up + 1;
      size_t n = taylor_x.size() / q;
      //
# ifndef NDEBUG
      size_t m = taylor_y.size() / q;
      assert( call_id == 0 );
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
      size_t n = taylor_x.size() / q;
      //
# ifndef NDEBUG
      size_t m = taylor_y.size() / q;
      assert( call_id == 0 );
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
      // m, n
      size_t m = select_y.size();
      size_t n = select_x.size();
      //
      assert( call_id == 0 );
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
      assert( n == select_x.size() );
      //
      // pattern_out
      size_t nnz = 0;
      pattern_out.resize(n, n, nnz);
      //
      return ok;
   }
};

class valvector_ad_join {
private:
   typedef CppAD::AD<valvector> ad_valvector;
   valvector_ad_join_atom      atomic_fun_;
public:
   valvector_ad_join(void) : atomic_fun_("valvector_ad_join_atom")
   { }
   template <class ADVector>
   void operator()(const ADVector& ax, ad_valvector&  ay)
   {
      size_t call_id = 0;
      ADVector ay_vec(1);
      atomic_fun_(call_id, ax, ay_vec);
      ay = ay_vec[0];
      return;
   }
};

# endif
