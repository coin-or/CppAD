# ifndef  CPPAD_LOCAL_VAL_GRAPH_SUMMATION_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_SUMMATION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/rev_depend.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
------------------------------------------------------------------------------
{xrst_begin_parent val_summation dev}
{xrst_spell
   csum
   dep
   neg
}

Combine Multiple sum Operators into a csum Operator
###################################################

Prototype
*********
{xrst_literal
   // BEGIN_SUMMATION
   // END_SUMMATION
}


Discussion
**********
We refer to an add, sub, neg, or csum operator as a sum operation.
If the result for one of these operations is only used by a sum operator,
the two operations can be replaced by a single csum operator.
This process is repeated until the result is used by some other operation
or the result is a dependent variable.

Limitation
**********
The process above terminates before reaching a csum operator in the
original tape. To get full optimization in this case, csum operators
should first be converted to add and sub operations.

Changes
*******
Only the following values, for this tape, are guaranteed to be same:
#. The number of independent values :ref:`val_tape@n_ind` .
#. The size of the dependent vector :ref:`dep_vec.size() <val_tape@dep_vec>` .

Example
*******
The file :ref:`val_summation_xam.cpp-name` is an example and test using
tape.summation().

Contents
********
{xrst_toc_table
   val_graph/summation_xam.cpp
}

{xrst_end val_summation}
-----------------------------------------------------------------------------
{xrst_begin val_csum_info dev}
{xrst_spell
   struct
}

Information for a Cumulative Summation
######################################

{xrst_code cpp} */
struct csum_info_t {
   bool  first_done;  // is first operator argument included in lists
   bool  second_done; // is second operator argument included in lists
   std::list<addr_t> add_list; // list of value indices to add
   std::list<addr_t> sub_list; // list of value indices to subtract
   // ctor
   csum_info_t() : first_done(false), second_done(false) { }
};
/* {xrst_code}
{xrst_end val_csum_info}
------------------------------------------------------------------------------
{xrst_begin val_replace_csum_op dev}
{xrst_spell
   neg
}

Replace An Operator with a Cumulative Summation
###############################################

Prototype
*********
{xrst_literal
   // BEGIN_REPLACE_CSUM_OP
   // END_REPLACE_CSUM_OP
}

Replacement Operator Use
************************
This will replace an operator use in the tape.
The :ref:`set_op2arg_index <val_op2arg_index@set>` must have been
executed on this tape before a replacement can be done.

res_index
*********
is the value index of the result for this operator.

op_index
********
is the index of the operator that we are replacing
This operator must have just one result with index res_index.

csum_info
*********
Is the cumulative summation information for the operator
that we are replacing.
The idea here is that there are multiple add, sub, and neg operators
that become dead code when this replacement is done.

{xrst_end val_replace_csum_op}
*/
//
// BEGIN_REPLACE_CSUM_OP
template <class Value>
void tape_t<Value>::replace_csum_op(
   addr_t       res_index ,
   addr_t       i_op      ,
   csum_info_t& csum_info )
// END_REPLACE_CSUM_OP
{
   //
   // This is a replace
   //
   // op_enum_vec_
   op_enum_vec_[i_op] = uint8_t( csum_op_enum );
   //
   // arg_index_vec_
   op2arg_index_[i_op] = addr_t( arg_vec_.size() );
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
   for(itr = csum_info.add_list.begin(); itr != csum_info.add_list.end(); ++itr)
   {  CPPAD_ASSERT_UNKNOWN( *itr < res_index );
      arg_vec_.push_back( *itr );
   }
   //
   // arg_vec_: subtraction variables
   for(itr = csum_info.sub_list.begin(); itr != csum_info.sub_list.end(); ++itr)
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
// ---------------------------------------------------------------------------
// BEGIN_SUMMATION
template <class Value>
void tape_t<Value>::summation(void)
// END_SUMMATION
{  //
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
   // information for all the cumulative summation operations
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
   // cat_list
   auto cat_list = [] (std::list<addr_t>& des, std::list<addr_t>& src)
   {  std::list<addr_t>::iterator itr_pos         = des.end();
      std::list<addr_t>::const_iterator itr_begin = src.begin();
      std::list<addr_t>::const_iterator itr_end   = src.end();
      des.insert(itr_pos, itr_begin, itr_end);
      return;
   };
   //
   // set_op2arg_index
   // This is necessary before calling replace_csum_op.
   set_op2arg_index();
   //
   // val_use_case, vec_last_load
   Vector<addr_t> val_use_case, vec_last_load;
   rev_depend(val_use_case, vec_last_load);
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
      const base_op_t<Value>* op_ptr_i    = op_itr.op_ptr();
      addr_t                  res_index_i = op_itr.res_index();
      addr_t                  arg_index_i = op_itr.arg_index();
      //
      // op_enum_i, n_arg
      op_enum_t op_enum_i  = op_ptr_i->op_enum();
      addr_t    n_arg      = op_ptr_i->n_arg(arg_index_i, arg_vec_);
      //
      // use_i, sum_i
      bool sum_i   = sum_op( op_enum_i );
      bool use_i   = false;
      if( sum_i )
      {
# ifndef NDEBUG
         addr_t n_res = op_ptr_i->n_res(arg_index_i, arg_vec_);
         CPPAD_ASSERT_UNKNOWN( n_res == 1 );
# endif
         use_i = 0 != val_use_case[res_index_i];
      }
      if( use_i & sum_i )
      {  // i_op is one of neg, add, or sub
         //
         // op_arg
         op_arg.resize(n_arg);
         for(addr_t i = 0; i < n_arg; ++i)
            op_arg[i] = arg_vec_[arg_index_i + i];
         //
         // op_arg_equal_i
         bool op_arg_equal_i = false;
         if( op_ptr_i->is_binary() )
            op_arg_equal_i = op_arg[0] == op_arg[1];
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
               {  CPPAD_ASSERT_UNKNOWN( ! op_arg_equal_i );
                  csum_info_i.add_list.push_back(op_arg[0]);
               }
               if( ! csum_info_i.second_done )
               {  CPPAD_ASSERT_UNKNOWN( ! op_arg_equal_i );
                  csum_info_i.add_list.push_back(op_arg[1]);
               }
               break;
               //
               case sub_op_enum:
               if( ! csum_info_i.first_done )
               {  CPPAD_ASSERT_UNKNOWN( ! op_arg_equal_i );
                  csum_info_i.add_list.push_back(op_arg[0]);
               }
               if( ! csum_info_i.second_done )
               {  CPPAD_ASSERT_UNKNOWN( ! op_arg_equal_i );
                  csum_info_i.sub_list.push_back(op_arg[1]);
               }
               break;
            }
            csum_info_i.first_done  = true;
            csum_info_i.second_done = true;
         }
         //
         if( val_use_case[res_index_i] == n_op() )
         {  // i_op is a dependent variable or used more than once
            //
            if( is_csum_i )
            {  replace_csum_op(res_index_i, i_op, csum_map[i_op]);
               csum_map.erase( i_op );
            }
         }
         else
         {  //
            // j_op
            // i_op result is only used by the j_op operator
            addr_t j_op = val_use_case[res_index_i];
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
               {  replace_csum_op(res_index_i, i_op,  csum_map[i_op]);
                  csum_map.erase( i_op );
               }
            }
            else
            {  // The only use of i_op result is in a summation operator
               //
               // csum_map[i_op]
               if( ! is_csum_i )
               {  csum_info_t csum_info;
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
                     // no use is adding and subtracting the same argument
                     case sub_op_enum:
                     if( ! op_arg_equal_i )
                     {  csum_info.add_list.push_back(op_arg[0]);
                        csum_info.sub_list.push_back(op_arg[1]);
                     }
                     break;
                  }
                  csum_map[i_op] = csum_info;
               }
               //
               // csum_info_i
               csum_info_t& csum_info_i = csum_map[i_op];
               //
               // second_operand
               bool second_operand = false;
               bool op_arg_equal_j = false;
               if( (op_enum_j == add_op_enum) || (op_enum_j == sub_op_enum) )
               {  addr_t          arg_index_j = op2arg_index_[j_op];
                  addr_t          right_index = arg_vec_[arg_index_j + 1];
                  addr_t          left_index  = arg_vec_[arg_index_j + 0];
                  second_operand  = right_index == res_index_i;
                  op_arg_equal_j  = right_index == left_index;
                  CPPAD_ASSERT_UNKNOWN(
                     left_index == res_index_i || right_index == res_index_i
                  );
               }
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
                  if( op_arg_equal_j )
                  {  cat_list(csum_info_j.add_list, csum_info_i.add_list);
                     cat_list(csum_info_j.sub_list, csum_info_i.sub_list);
                  }
                  splice_list(csum_info_j.add_list, csum_info_i.add_list);
                  splice_list(csum_info_j.sub_list, csum_info_i.sub_list);
                  break;
                  //
                  case sub_op_enum:
                  if( second_operand )
                  {  if( ! op_arg_equal_j )
                     {  splice_list(csum_info_j.add_list, csum_info_i.sub_list);
                        splice_list(csum_info_j.sub_list, csum_info_i.add_list);
                     }
                  }
                  else if( op_arg_equal_j )
                  {  cat_list(csum_info_j.add_list, csum_info_i.add_list);
                     cat_list(csum_info_j.sub_list, csum_info_i.sub_list);
                     splice_list(csum_info_j.add_list, csum_info_i.add_list);
                     splice_list(csum_info_j.sub_list, csum_info_i.sub_list);
                  }
                  else
                  {  splice_list(csum_info_j.add_list, csum_info_i.add_list);
                     splice_list(csum_info_j.sub_list, csum_info_i.sub_list);
                  }
                  break;
               }
               if( op_arg_equal_j )
               {  csum_info_j.first_done  = true;
                  csum_info_j.second_done = true;
               }
               else if( second_operand )
                  csum_info_j.second_done = true;
               else
                  csum_info_j.first_done = true;
            }
         }
      }
   }
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   std::cout << "summation:  inuse = " << final_inuse - initial_inuse << "\n";
# endif
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
