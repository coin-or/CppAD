# ifndef CPPAD_LOCAL_VAL_GRAPH_CALL_ATOMIC_HPP
# define CPPAD_LOCAL_VAL_GRAPH_CALL_ATOMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/atomic_index.hpp>
# include <cppad/core/atomic/two/atomic.hpp>
# include <cppad/core/atomic/three/atomic.hpp>
# include <cppad/core/atomic/four/atomic.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
This is similar to the routins in cppad/local/sweep/call_atomic.hpp,
but it is used by val_graph instead of the sweeps.
*/
/*
{xrst_begin atomic_for_type_callback dev}

Forward Type Callback to Atomic Functions
#########################################

Prototype
*********
{xrst_literal
   // BEGIN_FORWARD
   // END_FORWARD
}

Value
*****
is the base type corresponding to the atomic function.

CppAD::vector
*************
this routine uses the CppAD::vector template class
(and not CppAD::local::val_graph::Vector) because atomic functions expect
CppAD::vector.

constant_x
**********
contains the values, in afun(ax, ay), for arguments that are constants.

type_x
******
what is the type, in the call, for each component of x.

type_y
******
the input values in this vector do not matter.
Upon return it is the type for each component of y.

atom_index
**********
is the index, in local::atomic_index, corresponding to this atomic function.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

{xrst_end atomic_for_type_callback}
*/
// BEGIN_FORWARD
template <class Value>
void call_atomic_for_type(
   const CppAD::vector<Value>&         constant_x  ,
   const CppAD::vector<ad_type_enum>&  type_x      ,
   CppAD::vector<ad_type_enum>&        type_y      ,
   size_t                              atom_index  ,
   size_t                              call_id     )
// END_FORWARD
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   //
   // type, v_ptr
   bool         set_null = false;
   size_t       type     = 0;       // set to avoid warning
   std::string  name;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<Value>(set_null, atom_index, type, &name, v_ptr);
//
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if( ok )
   {
      if( type == 2 )
      {  size_t p = 0, q = 0, nx = type_x.size(), ny = type_y.size();
         CppAD::vector<bool> vx(nx), vy(ny);
         for(size_t i = 0; i < nx; ++i)
            vx[i] = type_x[i] > constant_enum;
         const CppAD::vector<Value>& taylor_x = constant_x;
         CppAD::vector<Value> taylor_y(ny);
         atomic_base<Value>* afun =
            reinterpret_cast< atomic_base<Value>* >(v_ptr);
         afun->set_old(call_id);
         ok = afun->forward(
            p, q, vx, vy, taylor_x, taylor_y
         );
         for(size_t i = 0; i < ny; ++i)
         {  if( vy[i] )
               type_y[i] = variable_enum;
            else
               type_y[i] = constant_enum;
         }
      }
      else if( type == 3 )
      {  CPPAD_ASSERT_UNKNOWN( type == 3 );
         atomic_three<Value>* afun =
            reinterpret_cast< atomic_three<Value>* >(v_ptr);
         ok = afun->for_type(constant_x, type_x, type_y );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<Value>* afun =
            reinterpret_cast< atomic_four<Value>* >(v_ptr);
         ok = afun->for_type(call_id, type_x, type_y);
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic for_type returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  size_t p = 0, q = 0, nx = type_x.size(), ny = type_y.size();
      CppAD::vector<bool> vx(nx), vy(ny);
      for(size_t i = 0; i < nx; ++i)
         vx[i] = type_x[i] > constant_enum;
      const CppAD::vector<Value>& taylor_x = constant_x;
      CppAD::vector<Value> taylor_y(ny);
      atomic_base<Value>* afun =
         reinterpret_cast< atomic_base<Value>* >(v_ptr);
      afun->set_old(call_id);
      afun->forward(
         p, q, vx, vy, taylor_x, taylor_y
      );
      for(size_t i = 0; i < ny; ++i)
      {  if( vy[i] )
            type_y[i] = variable_enum;
         else
            type_y[i] = constant_enum;
      }
   }
   else if( type == 3 )
   {  CPPAD_ASSERT_UNKNOWN( type == 3 );
      atomic_three<Value>* afun =
         reinterpret_cast< atomic_three<Value>* >(v_ptr);
      afun->for_type(constant_x, type_x, type_y );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( type == 4 );
      atomic_four<Value>* afun =
         reinterpret_cast< atomic_four<Value>* >(v_ptr);
      afun->for_type(call_id, type_x, type_y);
   }
# endif
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
