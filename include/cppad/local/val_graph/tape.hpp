# ifndef  CPPAD_LOCAL_VAL_GRAPH_TAPE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_TAPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/val_graph/unary_op.hpp>
# include <cppad/local/val_graph/binary_op.hpp>
# include <cppad/local/val_graph/con_op.hpp>
# include <cppad/local/val_graph/type.hpp>
// END_SORT_THIS_LINE_MINUS_1

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_graph_tape dev}
{xrst_spell
   dep
}

The Value Operator Tape
#######################

Prototype
*********
{xrst_literal
   // BEGIN_TAPE_T
   // END_TAPE_T
}

Purpose
*******
This class is used to define a function using a sequence of operators.

Value
*****
This is the type used for evaluations using the tape.


n_val
*****
{xrst_literal
   // BEGIN_N_VAL
   // END_N_VAL
}
This is the number of elements in the value vector.

n_ind
*****
{xrst_literal
   // BEGIN_N_IND
   // END_N_IND
}
This is the size of the independent vector.

arg_vec
*******
{xrst_literal
   // BEGIN_ARG_VEC
   // END_ARG_VEC
}
The elements of this vector at non-negative integers that connect
with an operator to make an operator usage.

con_vec
*******
{xrst_literal
   // BEGIN_CON_VEC
   // END_CON_VEC
}
This is a vector of constants that are loaded into the value vector.
(Constants do not depend on the independent values.)

op_vec
******
{xrst_literal
   // BEGIN_OP_VEC
   // END_OP_VEC
}
Each element of this vector corresponds to an operator usage.
The order of the vector is the order of operations.

info
****
{xrst_literal
   // BEGIN_OP_INFO_T
   // END_OP_INFO_T
}
This is the information corresponding to one operator usage.

arg_index
=========
this specifies the offset in arg_vec for this operator usage.

rex_index
=========
this specifies the offset in the value vector
of the first result for this operator usage.

op_ptr
======
this is a pointer to the operator corresponding to this
operator usage.

dep_vec
*******
{xrst_literal
   // BEGIN_DEP_VEC
   // END_DEP_VEC
}
This is the vector of dependent indices in the value vector.
The function corresponding to a tape maps the independent values
to the dependent values.

swap
****
{xrst_literal
   // BEGIN_SWAP
   // END_SWAP
}
This swaps the contents of this tape with another tape.

eval
****
{xrst_literal
   // BEGIN_EVAL
   // END_EVAL
}
Given the independent values, this routine execute the operators
in order to evaluate the rest of the value vector and the compare_false
counter.

trace
=====
If this is try, the :ref:`val_base_op@print_op` function is used
to print each of the operators.

val_vec
=======
This vector has size equal to *n_val*.
The first *n_ind* elements are inputs.
The rest of the elements are outputs.

compare_false
=============
This is the number of :ref:`val_comp_op-name` that had a false
result for their comparisons.
This is both an input and output; i.e., each false comparison
will add one to this value.

Operations on Tape
******************
{xrst_toc_table
   include/cppad/local/val_graph/record_op.hpp
   include/cppad/local/val_graph/fold_con.hpp
   include/cppad/local/val_graph/renumber.hpp
   include/cppad/local/val_graph/dead_code.hpp
}


{xrst_end val_graph_tape}
*/
// BEGIN_TAPE_T
template <class Value> class tape_t {
// END_TAPE_T
public :
   // BEGIN_OP_INFO_T
   typedef struct {
      addr_t      arg_index;    // starting index in arg_vec for an operator
      addr_t      res_index;    // starting result index in val_vec
      op_base_t<Value>* op_ptr; // pointer to this operator
   } op_info_t;
   // END_OP_INFO_T
private :
   addr_t                n_ind_;     // number of independent values
   addr_t                n_val_;     // index in val_vec of record result
   Vector<addr_t>        arg_vec_;   // index of operator arguments in val_vec
   Vector<Value>         con_vec_;   // constants used by the tape
   Vector<op_info_t>     op_vec_;    // operators that define this function
   Vector<addr_t>        dep_vec_;   // index in val_vec of dependent variables
   //
public :
   // ------------------------------------------------------------------------
   // BEGIN_N_VAL
   addr_t n_val(void) const
   {  return n_val_; }
   // END_N_VAL
   //
   // BEGIN_N_IND
   addr_t n_ind(void) const
   {  return n_ind_; }
   // END_N_IND
   //
   // BEGIN_ARG_VEC
   const Vector<addr_t>& arg_vec(void) const
   {  return arg_vec_; }
   // END_ARG_VEC
   //
   // BEGIN_CON_VEC
   const Vector<Value>& con_vec(void) const
   {  return con_vec_; }
   // END_CON_VEC
   //
   // BEGIN_OP_VEC
   const Vector<op_info_t>& op_vec(void) const
   {  return op_vec_; }
   // END_OP_VEC
   //
   // BEGIN_DEP_VEC
   const Vector<addr_t>& dep_vec(void) const
   {  return dep_vec_; }
   // END_DEP_VEC
   // ------------------------------------------------------------------------
   // BEGIN_SWAP
   void swap(tape_t& other)
   // END_SWAP
   {  std::swap( n_ind_, other.n_ind_ );
      std::swap( n_val_, other.n_val_);
      arg_vec_.swap( other.arg_vec_ );
      con_vec_.swap( other.con_vec_ );
      op_vec_.swap( other.op_vec_ );
      dep_vec_.swap( other.dep_vec_ );
   }
   // BEGIN_EVAL
   void eval(
      bool           trace         ,
      size_t&        compare_false ,
      Vector<Value>& val_vec       ) const
   // END_EVAL
   {  assert( val_vec.size() == static_cast<size_t>(n_val_) );
      //
      // trace
      if( trace )
      {  // no operators for independent variables
         std::printf("independent vector\n");
         for(addr_t res_index = 0; res_index < n_ind_; ++res_index)
         {  Value res = val_vec[res_index];
            std::printf(
               "%5d  %10.3g\n", res_index, res
            );
         }
         std::printf("operators\n");
      }
      //
      // i_op
      size_t n_op = op_vec_.size();
      for(size_t i_op = 0; i_op < n_op; ++i_op)
      {  //
         // arg_index, res_index
         const op_info_t& op_info     = op_vec_[i_op];
         op_base_t<Value>* op_ptr     = op_info.op_ptr;
         addr_t            arg_index  = op_info.arg_index;
         addr_t            res_index  = op_info.res_index;
         //
         // op_base_t<Value>::eval
         op_ptr->eval(
            trace,
            arg_index,
            arg_vec_,
            con_vec_,
            res_index,
            compare_false,
            val_vec
         );
      }
      // trace
      if( trace )
      {  // no operators for dependent variables
         std::printf("dependent vector\n");
         for(size_t i = 0; i < dep_vec_.size(); ++i)
         {  addr_t res_index = dep_vec_[i];
            Value res        = val_vec[res_index];
            std::printf(
               "%5d  %10.3g\n", res_index, res
            );
         }
         // space after end of this tape
         std::printf("\n");
      }
      return;
   }
   // ------------------------------------------------------------------------
   // functions documented in files included below
   // ------------------------------------------------------------------------
   //
   // set_ind
   addr_t set_ind(addr_t n_ind);
   //
   // record_op
   addr_t record_op(op_enum_t op_enum, const Vector<addr_t>& op_arg);
   //
   // record_con_op
   addr_t record_con_op(const Value& constant);
   //
   // record_dis_op
   addr_t record_dis_op(addr_t discrete_index, addr_t op_arg);
   //
   // record_comp_op
   addr_t record_comp_op(
      compare_enum_t compare_enum ,
      addr_t         left_index   ,
      addr_t         right_index
   );
   //
   // record_call_op
   addr_t record_call_op(
      size_t atomic_index           ,
      size_t call_id                ,
      size_t n_res                  ,
      const Vector<addr_t>& fun_arg
   );
   //
   // set_dep
   void set_dep(const Vector<addr_t>& dep_vec);
   //
   // fold_con
   void fold_con(void);
   //
   // renumber
   void renumber(void);
   //
   // dead_code
   void dead_code(bool keep_compare);
};
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# include <cppad/local/val_graph/record_op.hpp>
# include <cppad/local/val_graph/fold_con.hpp>
# include <cppad/local/val_graph/renumber.hpp>
# include <cppad/local/val_graph/dead_code.hpp>

# endif
