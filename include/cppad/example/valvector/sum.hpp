# ifndef CPPAD_EXAMPLE_VALVECTOR_SUM_HPP
# define CPPAD_EXAMPLE_VALVECTOR_SUM_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
/*
-------------------------------------------------------------------------------
{xrst_begin valvector_ad_sum}
{xrst_spell
   asum
}

Sum The Elements of and AD valvector
####################################
Sum the elements of an AD<valvector> and return the result as one AD<valvector>
with size one.

Syntax
******
| valvector_ad_sum *asum*
| *asum* ( *ax* , *ay* )

ax
**
This CppAD::AD<valvector> is ``const`` and passed by reference.

ay
**
This CppAD::AD<valvector> is passed by reference.
Its input value does not matter.
Upon return, it has size one and its element is the sum of the elements
in *ax* .

{xrst_toc_hidden
   example/valvector/ad_sum.cpp
}
Example
*******
The file :ref:`valvector_ad_sum.cpp-name` is an example and test
of this operation.

{xrst_end valvector_ad_sum}
*/
# include <cppad/example/valvector/class.hpp>
# include <cppad/cppad.hpp>
//
// valvector_atom_sum
// split and join are in same atomic function so that in AD version of reverse
// split can call join and join can call split.
class valvector_atom_sum : public CppAD::atomic_four<valvector> {
public:
   //
   // ctor
   valvector_atom_sum(const std::string& name) :
   CppAD::atomic_four<valvector>(name)
   { }
private:
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // scalar_type
   typedef valvector::scalar_type scalar_type;
   //
   // ------------------------------------------------------------------------
   // for_type
   bool for_type(
      size_t                                     call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
   {  //
      assert( call_id == 0 );
      assert( type_x.size() == 1 );
      assert( type_y.size() == 1 );
      //
      type_y[0] = type_x[0];
      //
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
      assert( call_id == 0 );
      assert( select_y.size() == 1 );
      //
      // q
      size_t q = order_up + 1;
      assert( taylor_x.size() == q);
      assert( taylor_y.size() == q);
      //
      // taylor_y
      for(size_t k = order_low; k < q; ++k)
      {  //
         taylor_y[k].resize(1);
         taylor_y[k][0] = taylor_x[k].sum();
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
      assert( call_id == 0 );
      assert( select_y.size() == 1 );
      //
      // q
      size_t q = order_up + 1;
      assert( ataylor_x.size() == q);
      assert( ataylor_y.size() == q);
      //
      //
      // ax_k, ay_k
      CppAD::vector<ad_valvector> ax_k(1);
      CppAD::vector<ad_valvector> ay_k(1);
      //
      // ataylor_y
      for(size_t k = 0; k < q; ++k)
      {  ax_k[0] = ataylor_x[k];
         (*this)(ax_k, ay_k);
         ataylor_y[k] = ay_k[0];
      }
      //
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
      assert( call_id == 0  );
      assert( select_x.size() == 1 );
      //
      // q
      size_t q = order_up + 1;
      assert( taylor_x.size() == q );
      assert( taylor_y.size() == q );
      //
      if( ! select_x[0] )
         return true;
      //
      // partial_x
      for(size_t k = 0; k < q; ++k)
      {  assert( partial_y[k].size() == 1 );
         partial_x[k] = partial_y[k];
      }
      //
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
      assert( call_id == 0  );
      assert( select_x.size() == 1 );
      //
      // q
      size_t q = order_up + 1;
      assert( ataylor_x.size() == q );
      assert( ataylor_y.size() == q );
      //
      if( ! select_x[0] )
         return true;
      //
      // apartial_x
      for(size_t k = 0; k < q; ++k)
         apartial_x[k] = apartial_y[k];
      //
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
      assert( call_id == 0 );
      assert( select_x.size() == 1 );
      assert( select_y.size() == 1 );
      //
      // m, n
      size_t m = select_y.size();
      size_t n = select_x.size();
      //
      // nnz
      size_t nnz = 0;
      if( select_x[0] && select_y[0] )
         ++nnz;
      //
      // pattern_out
      pattern_out.resize(m, n, nnz);
      if( select_x[0] && select_y[0] )
         pattern_out.set(0, 0, 0);
      //
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
      assert( call_id == 0 );
      assert( select_x.size() == 1 );
      assert( select_y.size() == 1 );
      //
      size_t n = select_x.size();
      //
      // pattern_out
      size_t nnz = 0;
      pattern_out.resize(n, n, nnz);
      //
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
      assert( call_id == 0 );
      assert( depend_x.size() == 1 );
      assert( depend_y.size() == 1 );
      //
      depend_x[0] = depend_y[0];
      //
      return true;
   }
};
// ---------------------------------------------------------------------------
// valvector_ad_sum
class valvector_ad_sum {
private:
   typedef CppAD::AD<valvector> ad_valvector;
   valvector_atom_sum           atomic_fun_;
public:
   valvector_ad_sum(void) : atomic_fun_("valvector_ad_sum")
   { }
   void operator()(const ad_valvector&  ax, ad_valvector& ay)
   {  //
      // ay
      CppAD::vector<ad_valvector> ax_vec(1), ay_vec(1);
      ax_vec[0] = ax;
      atomic_fun_(ax_vec, ay_vec);
      ay = ay_vec[0];
      return;
   }
};
# endif
