# ifndef  CPPAD_LOCAL_VAL_GRAPH_DEAD_CODE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_DEAD_CODE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/atomic_index.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_graph_dead_code dev}
{xrst_spell
   dep
   xam
}

Dead Code Elimination
#####################

Algorithm
*********
#. The dependent variables are marked as needed.
#. The operators are scanned in reverse if an operators result is
   needed, the corresponding arguments are marked as needed.
   The call operator has a more complicated version of this marking.
#. A forward pass is made though the operators and only the needed
   ones are included.
#. An operators result may have a new index
   because some previous results were left out.
   A mapping from the old result indices to the new result indices
   enables subsequent operators to adjust their argument indices.
#. After the forward pass, the mapping from old indices to new indices
   is used to adjust the dependent variable indices.

dep_vec
*******
This may change the indices corresponding to the dependent vector; i.e.,
:ref:`val_graph_tape@dep_vec`.

Reference
*********
`dead-code elimination <https://en.wikipedia.org/wiki/Dead-code_elimination>`_

{xrst_toc_hidden
   val_graph/dead_xam.cpp
}
Example
*******
The file :ref:`dead_xam.cpp <val_op_dead_xam.cpp-name>` is an
example and test of tape.dead_code().

{xrst_end val_graph_dead_code}
*/
template <class Value>
void tape_t<Value>::dead_code(void)
{  // -----------------------------------------------------------------------
   // Dead Code Elimination
   // https://en.wikipedia.org/wiki/Dead-code_elimination
   // -----------------------------------------------------------------------
   //
   // ident_zero
   Vector<bool> ident_zero(n_val_);
   for(size_t i = 0; i < n_val_; ++i)
      ident_zero[i] = false;
   for(size_t i_op = 0; i_op < op_vec_.size(); ++i_op)
   {  op_enum_t  op_enum = op_vec_[i_op].op_ptr->op_enum();
      if( op_enum == con_op_enum )
      {  addr_t res_index = op_vec_[i_op].res_index;
         Value& constant  = con_vec_[ arg_vec_[ op_vec_[i_op].arg_index ] ];
         ident_zero[res_index] = CppAD::IdenticalZero( constant );
      }
   }
   //
   // ident_zero_x, depend_x, depend_y
   Vector<bool> ident_zero_x, depend_x, depend_y;
   //
   // need_val_index
   Vector<bool> need_val_index(n_val_);
   for(size_t i = 0; i < n_val_; ++i)
      need_val_index[i] = false;
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      need_val_index[ dep_vec_[i] ] = true;
   //
   // need_val_index
   size_t i_op = op_vec_.size();
   while( i_op-- )
   {  // op_enum, res_index, arg_index, n_arg, n_res
      op_enum_t  op_enum = op_vec_[i_op].op_ptr->op_enum();
      addr_t res_index   = op_vec_[i_op].res_index;
      addr_t arg_index   = op_vec_[i_op].arg_index;
      addr_t n_arg = addr_t( op_vec_[i_op].op_ptr->n_arg(arg_index, arg_vec_));
      addr_t n_res = addr_t( op_vec_[i_op].op_ptr->n_res(arg_index, arg_vec_));
      //
      switch( op_enum )
      {  //
         // con_op_enum
         case con_op_enum:
         break;
         //
         // default
         default:
         CPPAD_ASSERT_UNKNOWN( n_arg == 1 || n_arg == 2 );
         CPPAD_ASSERT_UNKNOWN( n_res == 1 );
         {  //
            // need_op
            bool need_op = need_val_index[ res_index + 0];
            //
            // need_val_index
            if( need_op )
            {  for(addr_t k = 0; k < n_arg; ++k)
                  need_val_index[ arg_vec_[arg_index + k] ] = true;
            }
         }
         break;
         //
         case call_op_enum:
         {  size_t atomic_index  = size_t( arg_vec_[arg_index + 2] );
            size_t call_id       = size_t( arg_vec_[arg_index + 3] );
            //
            // v_ptr, name
            CPPAD_ASSERT_UNKNOWN( 0 < atomic_index );
            bool         set_null = false;
            size_t       type     = 0;       // result: set to avoid warning
            std::string  name;               // result:
            void*        v_ptr    = nullptr; // result: set to avoid warning
            local::atomic_index<Value>(
               set_null, atomic_index, type, &name, v_ptr
            );
            // val_graph only supports atomic_four
            CPPAD_ASSERT_UNKNOWN( type == 4 );
            //
            // ident_zero_x
            ident_zero_x.resize(n_arg - 4);
            for(addr_t i = 4; i < n_arg; ++i)
               ident_zero_x[i-4] = ident_zero[ arg_vec_[arg_index + i] ];
            //
            // depend_y
            depend_y.resize(n_res);
            for(addr_t i = 0; i < n_res; ++i)
               depend_y[i] = need_val_index[ res_index + i ];
            //
            // depend_x, ok
            depend_x.resize(n_arg - 4);
            bool ok = false;
            if( v_ptr != nullptr )
            {  atomic_four<Value>* afun =
                  reinterpret_cast< atomic_four<Value>* >(v_ptr);
               ok = afun->rev_depend(call_id, ident_zero_x, depend_x, depend_y);
               if( ! ok )
               {  // try deprecated version of this function
                  ok = afun->rev_depend(call_id, depend_x, depend_y);
               }
            }
            if( ! ok )
            {  std::string msg = name;
               if( v_ptr == nullptr )
                  msg += ": this atomic function has been deleted";
               else
                  msg += ": atomic rev_depend returned false";
               CPPAD_ASSERT_KNOWN(false, msg.c_str() );
            }
            //
            for(addr_t k = 4; k < n_arg; ++k)
               need_val_index[ arg_vec_[arg_index + k] ] = depend_x[k-4];
         }
      }
   }
   //
   // new_tape
   tape_t new_tape;
   new_tape.set_ind(n_ind_);
   //
   // new_val_index
   // include nan at index n_ind_ in val_vec
   Vector<addr_t> new_val_index( n_val_ );
   for(size_t i = 0; i <= n_ind_; ++i)
      new_val_index[i] = addr_t(i);
   for(size_t i = n_ind_ + 1; i < n_val_; ++i)
      new_val_index[i] = addr_t( n_val_ );
   //
   // op_arg, call_op_arg
   Vector<addr_t> op_arg, call_op_arg;
# ifndef NDEBUG
   // nan at index n_ind_
   assert( op_vec_[0].op_ptr->op_enum() == con_op_enum );
   assert( op_vec_[0].arg_index == 0 );
   assert( op_vec_[0].res_index == addr_t( n_ind_ ) );
   assert( arg_vec_[0] == 0 );
   assert( CppAD::isnan( con_vec_[0] ) );
# endif
   // i_op
   for(i_op = 1; i_op < op_vec_.size(); ++i_op)
   {  //
      // op_enum, arg_index, res_index
      op_enum_t op_enum   = op_vec_[i_op].op_ptr->op_enum();
      addr_t    arg_index = op_vec_[i_op].arg_index;
      addr_t    res_index = op_vec_[i_op].res_index;
      addr_t n_arg = addr_t( op_vec_[i_op].op_ptr->n_arg(arg_index, arg_vec_));
      addr_t n_res = addr_t( op_vec_[i_op].op_ptr->n_res(arg_index, arg_vec_));
      //
      // need_op
      bool need_op = false;
      for(addr_t k = 0; k < n_res; ++k)
         need_op |= need_val_index[ res_index + k];
      //
      if( need_op )
      {  //
         // new_val_index
         if( op_enum == con_op_enum )
         {  Value value = con_vec_[ arg_vec_[ arg_index ] ];
            addr_t new_res_index = new_tape.record_con_op(value);
            assert( n_res == 1 );
            new_val_index[ res_index ] = new_res_index;
         }
         else if( op_enum == call_op_enum )
         {  call_op_arg.resize( size_t(n_arg - 4) );
            for(addr_t k = 4; k < n_arg; ++k)
            {  addr_t val_index = arg_vec_[arg_index + k];
               if( need_val_index[val_index] )
                  call_op_arg[k - 4] = new_val_index[val_index];
               else
               {  // nan at index n_ind_
                  call_op_arg[k - 4] = addr_t( n_ind_ );
               }
            }
            size_t atomic_index      = size_t( arg_vec_[arg_index + 2] );
            size_t call_id     = size_t( arg_vec_[arg_index + 3] );
            addr_t new_res_index = new_tape.record_call_op(
               atomic_index, call_id, size_t(n_res), call_op_arg
            );
            for(addr_t k = 0; k < addr_t(n_res); ++k)
               new_val_index[ res_index + k ] = new_res_index + k;
         }
         else
         {  op_arg.resize(n_arg);
            for(addr_t k = 0; k < n_arg; ++k)
            {  assert( arg_vec_[arg_index + k] < res_index );
               op_arg[k] = new_val_index[ arg_vec_[arg_index + k] ];
            }
            addr_t new_res_index = new_tape.record_op(op_enum, op_arg);
            for(addr_t k = 0; k < addr_t(n_res); ++k)
               new_val_index[ res_index ] = new_res_index + k;
         }
      }
   }
   //
   // dep_vec
   Vector<addr_t> dep_vec( dep_vec_.size() );
   for(size_t k = 0; k < dep_vec_.size(); ++k)
      dep_vec[k] = new_val_index[ dep_vec_[k] ];
   new_tape.set_dep( dep_vec );
   //
   // swap
   swap(new_tape);
   //
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
