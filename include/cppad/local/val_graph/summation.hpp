# ifndef  CPPAD_LOCAL_VAL_GRAPH_SUMMATION_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_SUMMATION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/rev_depend.hpp>
namespace CppAD { namespace local { namespace val_graph {

// csum_info
// information for one cumulative summation operator
struct csum_info_t {
   bool              first_done;  // is first op argument included below
   bool              second_done; // is second op argument included below
   std::list<addr_t> add_list;    // list of value indices to add
   std::list<addr_t> sub_list;    // list of value indices to subtract
   //
   csum_info_t() : first_done(false), second_done(false) { }
};
//
// set_op2arg_index
template <class Value>
void tape_t<Value>::set_op2arg_index(void)
{  CPPAD_ASSERT_UNKNOWN( op2arg_index_.size() == 0 );
   //
   // op2arg_indeex
   Vector<addr_t> op2arg_index( n_op() );
   op_iterator<Value> op_itr(*this, 0);
   for(addr_t i_op = 0; i_op < n_op; ++i_op)
   {   op2arg_index[i_op] = op_itr.arg_index();
      ++op_itr;
   }
   //
   // op2arg_index_
   // We needed a complete op2arg_index, before we could set op2arg_index_,
   // otherwise op_iteator would not work properly.
   op2arg_index_.swap(op2arg_index);
}
//
// op2arg_index
template <class Value>
const Vector<addr_t>& tape_t<Value>::op2arg_index(void) const
{  return op2arg_index_; }
//
// replace_csum_op
template <class Value>
void tape_t<Value>::replace_csum_op(
   addr_t       res_index ,
   addr_t       i_op      ,
   csum_info_t& csum_info )
{
   //
   // op_enum_vec_
   op_enum_vec_[i_op] = uint8_t( csum_op_enum );
   //
   // arg_index_vec_
   op2arg_index[i_op] = addr_t( arg_vec_.size() );
   //
   // n_add, arg_vec_
   addr_t n_add  = addr_t( csum_info.add_list.size() );
   arg_vec_.push_back( n_add );
   //
   // n_sub, arg_vec_
   addr_t n_sub  = addr_t( csum_info.sub_list.size() );
   arg_vec_.push_back( n_sub );
   //
   // itr
   std::list<addr_t>::const_iterator itr;
   //
   // arg_vec_: addition variables
   for(itr = csum_info.add_list.begin(); itr = csum_info.add_list.end(); ++itr)
   {  CPPAD_ASSERT_UNKNOWN( *itr < res_index );
      arg_vec_.push_back( *itr );
   }
   //
   // arg_vec_: subtraction variables
   for(itr = csum_info.sub_list.begin(); itr = csum_info.sub_list.end(); ++itr)
   {  CPPAD_ASSERT_UNKNOWN( *itr < res_index );
      arg_vec_.push_back( *itr );
   }
   //
   // n_arg, arg_vec_
   addr_t n_arg = 3 + n_add + n_sub;
   arg_vec_.push_back( n_arg );
   //
   return;
}


template <class Value>
void tape_t<Value>::summation(void)
{  // -----------------------------------------------------------------------
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   // op_arg
   Vector<addr_t> op_arg;
   //
   // csum_map
   // informaiton for all the cumulative summation operations
   std::map<addr_t, csum_info_t> csum_map;
   //
   // sum_op
   auto sum_op = [] (op_enum_t op_enum)
   {  bool result = false;
      result     |= op_enum == neg_op_enum;
      result     |= op_enum == add_op_enum;
      result     |= op_enum == sub_op_enum;
      return result;
   };
   //
   // splice_list
   auto splice_list = [] (std::list<addr_t>& des, std::list<addr_t>& src)
   {  std::list<addr_t>::iterator itr_pos = des.end();
      des.splice(itr_pos, src);
      return;
   };
   //
   // set_op2arg_index
   // This is necessary before calling replace_csum_op.
   set_op2arg_index();
   //
   // val_use_case
   Vector<addr_s> val_use_case = rev_depend();
   //
   // op_itr
   op_iterator<Value> op_itr(*this, 0);
   //
   // i_op
   for(addr_t i_op = 1; i_op < n_op(); ++i_op)
   {  //
      // op_itr
      ++op_itr; // skip index zero
      //
      // op_ptr, arg_index, res_index
      const base_op_t<Value>* op_ptr    = op_itr.op_ptr();
      addr_t                  res_index = op_itr.res_index();
      addr_t                  arg_index = op_itr.arg_index();
      //
      // op_enum_i, n_arg
      op_enum_t op_enum_i  = op_ptr->op_enum();
      addr_t    n_arg      = op_ptr->n_arg(arg_index, arg_vec_);
      //
      // use_i, sum_i
      bool use_i = 0 != val_use_case[i_op];
      bool sum_i   = sum_op( op_enum_i );
      if( use_i & sum_i )
      {  // i_op is one of neg, add, or sub
         //
         // op_arg
         op_arg.resize(n_arg);
         for(addr_t i = 0; i < n_arg; ++i)
            op_arg[i] = arg_vec_[arg_index + i];
         //
         // is_csum_i, csum_map[i_op]
         bool is_csum_i  = 0 < csum_map.count(i_op);
         if( is_csum_i )
         {  csum_info_t& csum_info_i = csum_map[i_op];
            switch(op_enum_i)
            {  //
               default:
               CPPAD_ASSERT_UNKNOWN(false);
               break;
               //
               case neg_op_enum:
               CPPAD_ASSERT_UNKNOWN( csum_info_i.first_done );
               break;
               //
               case add_op_enum:
               if( ! csum_info_i.first_done )
                  csum_info_i.add_list.push_back(op_arg[0]);
               if( ! csum_info_i.second_done )
                  csum_info_i.add_list.push_back(op_arg[1]);
               break;
               //
               case sub_op_enum:
               if( ! csum_info_i.first_done )
                  csum_info_i.add_list.push_back(op_arg[0]);
               if( ! csum_info_i.second_done )
                  csum_info_i.sub_list.push_back(op_arg[1]);
               break;
            }
            csum_info_i.first_done  = true;
            csum_info_i.second_done = true;
         }
         //
         // new_res_index
         addr_t new_res_index = 0; // invalid index in value vector
         //
         if( val_use_case[i_op] == n_op() )
         {  // i_op is a dependent variable or used more than once
            //
            if( is_csum_i )
            {  new_res_index = replace_csum_op(res_index, i_op, csum_map[i_op]);
               csum_map.erase( i_op );
            }
         }
         else
         {  //
            // j_op, second_operand
            // i_op result is and operand for the j_op operator
            addr_t j_op         = val_use_case[i_op];
            bool second_operand = j_op < 0;
            if( second_operand )
                  j_op = - j_op;
            CPPAD_ASSERT_UNNOWN( 0 < j_op && j_op < n_op() );
            //
            // op_enum_j
            op_enum_t op_enum_j = op_enum_t( op_enum_vec_[j_op] );
            //
            // sum_j
            bool sum_j = sum_op( op_enum_j );
            //
            if( ! sum_j )
            {  // The only use of i_op is not a summation operator
               if( is_csum_i )
               {  new_res_index = record_csum_op( csum_map[i_op] );
                  csum_map.erase( i_op );
               }
            }
            else
            {  // The only use of i_op resut is in a summation operator
               //
               // csum_map[i_op]
               if( ! is_csum_i )
               {  csum_info_t& csum_info;
                  switch(op_enum_i)
                  {  //
                     default:
                     CPPAD_ASSERT_UNKNOWN(false);
                     break;
                     //
                     // neg_op_enum
                     case neg_op_enum:
                     csum_info.sub_list.push_back(op_arg[0]);
                     break;
                     //
                     // add_op_enum
                     case add_op_enum:
                     csum_info.add_list.push_back(op_arg[0]);
                     csum_info.add_list.push_back(op_arg[1]);
                     break;
                     //
                     // sub_op_enum
                     case sub_op_enum:
                     csum_info.add_list.push_back(op_arg[0]);
                     csum_info.sub_list.push_back(op_arg[1]);
                     break;
                  }
                  csum_map[i_op] = csum_info;
               }
               //
               // csum_info_i
               csum_info_t& csum_info_i = csum_map[i_op];
               //
               // is_csum_j
               bool is_csum_j = 0 < csum_map.count(j_op);
               //
               // csum_map[j_op]
               if( ! is_csum_j )
                  csum_map[j_op] = csum_info_t();
               csum_info_t& csum_info_j = csum_map[j_op];
               switch( op_enum_j )
               {  //
                  default:
                  CPPAD_ASSERT_UNKNOWN(false)
                  break;
                  //
                  case neg_op_enum:
                  splice_list(csum_info_j.add_list, csum_info_i.sub_list);
                  splice_list(csum_info_j.sub_list, csum_info_i.add_list);
                  break;
                  //
                  case add_op_enum:
                  splice_list(csum_info_j.add_list, csum_info_i.add_list);
                  splice_list(csum_info_j.sub_list, csum_info_i.sub_list);
                  break;
                  //
                  case sub_op_enum:
                  if( second_operand )
                  {  splice_list(csum_info_j.add_list, csum_info_i.sub_list);
                     splice_list(csum_info_j.sub_list, csum_info_i.add_list);
                  }
                  else
                  {  splice_list(csum_info_j.add_list, csum_info_i.add_list);
                     splice_list(csum_info_j.sub_list, csum_info_i.sub_list);
                  }
                  break;
               }
               if( second_operand )
                  csum_info_j.second_done = true;
               else
                  csum_info_j.first_done = true;
            }
         }
      }
   }
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
