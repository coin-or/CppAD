# ifndef CPPAD_EXAMPLE_VALVECTOR_SUM_AD_HPP
# define CPPAD_EXAMPLE_VALVECTOR_SUM_AD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>
# include <cppad/example/valvector/class.hpp>
//
// sum_ad_valvector
class sum_ad_valvector : public CppAD::atomic_four<valvector> {
public:
   // scalar_type, vector_type
   typedef valvector::scalar_type scalar_type;
   typedef valvector::vector_type vector_type;
   //
   // ctor
   sum_ad_valvector(const std::string& name) :
   CppAD::atomic_four<valvector>(name)
   { }
private:
   // for_type
   bool for_type(
      size_t                                     call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
   {
      assert( call_id == 0 );       // default value
      assert( type_x.size() == 1 ); // n
      assert( type_y.size() == 1 ); // m
      //
      type_y[0] = type_x[0];
      return true;
   }
   // forward
   bool forward(
      size_t                              call_id      ,
      const CppAD::vector<bool>&          select_y     ,
      size_t                              order_low    ,
      size_t                              order_up     ,
      const CppAD::vector<valvector>&     taylor_x     ,
      CppAD::vector<valvector>&           taylor_y     ) override
   {
# ifndef NDEBUG
      size_t q = order_up + 1;
      size_t n = taylor_x.size() / q;
      size_t m = taylor_y.size() / q;
      assert( call_id == 0 );
      assert( n == 1 );
      assert( m == 1 );
      assert( m == select_y.size() );
# endif
      // ok
      bool ok = true;
      if( ! select_y[0] )
         return ok;
      //
      // order
      for(size_t order = order_low; order <= order_up; ++order)
      {  //
         // taylor_y[order]
         taylor_y[order].resize(1);
         //
         // vx, vy
         const valvector::vector_type& vx = taylor_x[order].data();
         valvector::vector_type&       vy = taylor_y[order].data();
         //
         // taylor_y[order] =  taylor_x[order]_0 + taylor_x[order]_1 + ...
         vy[0] = scalar_type(0);
         for(size_t j = 0; j < vx.size(); ++j)
            vy[0] += vx[j];
      }
      //
      return ok;
   }
};

# endif
