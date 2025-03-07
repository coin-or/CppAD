# ifndef  CPPAD_LOCAL_VAL_GRAPH_TAPE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_TAPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/op_iterator.hpp>
# include <cppad/local/val_graph/op_enum2class.hpp>
# include <cppad/local/val_graph/val_type.hpp>
# include <cppad/local/val_graph/op_iterator.hpp>

# define CPPAD_VAL_GRAPH_TAPE_TRACE 0

namespace CppAD { namespace local { namespace val_graph {

// csum_info_t: forward declare
struct csum_info_t;

/*
{xrst_begin val_tape dev}
{xrst_spell
   dep
   str
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

op_ptr
******
{xrst_literal
   // BEGIN_OP_PTR
   // END_OP_PTR
}
This is the :ref:`base_op <val_base_op-name>` corresponding to
*op_index* in this tape.

n_val
*****
{xrst_literal
   // BEGIN_N_VAL
   // END_N_VAL
}
This is the number of elements in the value vector.

n_op
****
{xrst_literal
   // BEGIN_N_OP
   // END_N_OP
}
This is the number of operators in the tape.

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
This is the vector of value constants.

str_vec
*******
{xrst_literal
   // BEGIN_STR_VEC
   // END_STR_VEC
}
This is the vector of string constants.

vec_initial
***********
{xrst_literal
   // BEGIN_VEC_INITIAL
   // END_VEC_INITIAL
}
This vector has length equal to the number of dynamic vectors.
This i-th element of this vector is the vector of initial indices
for the i-th dynamic vector; see :ref:`val_vec_op-name` .

op_enum_vec
***********
{xrst_literal
   // BEGIN_OP_ENUM_VEC
   // END_OP_ENUM_VEC
}
Each element of this vector corresponds to an operator usage.
The order of the vector is the order of operations.

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
If this is true, the :ref:`val_print_op-name` functions are used
to print the operators.

val_vec
=======
This vector has size equal to *n_val*.
The first *n_ind* elements are inputs.
The rest of the elements are outputs.

compare_false
=============
This argument is optional.
It is the number of :ref:`val_comp_op-name` that had a false
result for their comparisons.
This is both an input and output; i.e., each false comparison
will add one to this value.

Operations on Tape
******************
{xrst_comment BEGIN_SORT_THIS_LINE_PLUS_2}
{xrst_toc_table
   include/cppad/local/val_graph/compress.hpp
   include/cppad/local/val_graph/cumulative.hpp
   include/cppad/local/val_graph/dead_code.hpp
   include/cppad/local/val_graph/fold_con.hpp
   include/cppad/local/val_graph/op2arg_index.hpp
   include/cppad/local/val_graph/op_hash_table.hpp
   include/cppad/local/val_graph/op_iterator.hpp
   include/cppad/local/val_graph/option.hpp
   include/cppad/local/val_graph/record.hpp
   include/cppad/local/val_graph/record_new.hpp
   include/cppad/local/val_graph/renumber.hpp
   include/cppad/local/val_graph/rev_depend.hpp
   include/cppad/local/val_graph/summation.hpp
}
{xrst_comment END_SORT_THIS_LINE_MINUS_2}


{xrst_end val_tape}
*/
// BEGIN_TAPE_T
template <class Value> class tape_t {
// END_TAPE_T
private :
   addr_t              n_ind_;       // number of independent values
   addr_t              n_val_;       // total number of values
   Vector<addr_t>      arg_vec_;     // arguments for all operator uses
   Vector<Value>       con_vec_;     // value constants
   Vector<std::string> str_vec_;     // string constants
   Vector<addr_t>      dep_vec_;     // dependent variable indices in val_vec
   Vector<uint8_t>     op_enum_vec_; // one byte per operator enum value.
   //
   // vec_initial_
   // initial indices for all the dynamic vectors
   Vector< Vector<addr_t> > vec_initial_;
   //
   // op2arg_index_
   // Optional vector that changes how op_iterator works; e.g.,
   // this is necessary is we are using replace_csum_op with this tape.
   Vector<addr_t> op2arg_index_;
   //
   // option_map_
   std::map< std::string, std::string > option_map_;
   //
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // set by set_ind, used by set_dep
   size_t  set_ind_inuse_;
# endif
   //
public :
   // default constructor
   tape_t(void)
   {  initialize_option(); }
   //
   // BEGIN_OP_PTR
   const base_op_t<Value>* base_op_ptr(addr_t op_index) const
   {  op_enum_t op_enum = op_enum_t( op_enum_vec_[op_index] );
      return op_enum2class<Value>(op_enum);
   }
   // END_OP_PTR
   // ------------------------------------------------------------------------
   // BEGIN_N_VAL
   addr_t n_val(void) const
   {  return n_val_; }
   // END_N_VAL
   //
   // BEGIN_N_OP
   addr_t n_op(void) const
   {  return addr_t( op_enum_vec_.size() ); }
   // END_N_OP
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
   // BEGIN_STR_VEC
   const Vector<std::string>& str_vec(void) const
   {  return str_vec_; }
   // END_STR_VEC
   //
   // BEGIN_VEC_INITIAL
   const Vector< Vector<addr_t> >& vec_initial(void) const
   {  return vec_initial_; }
   // END_VEC_INITIAL
   //
   // BEGIN_OP_ENUM_VEC
   const Vector<uint8_t>& op_enum_vec(void) const
   {  return op_enum_vec_; }
   // END_OP_ENUM_VEC
   //
   // BEGIN_DEP_VEC
   const Vector<addr_t>& dep_vec(void) const
   {  return dep_vec_; }
   // END_DEP_VEC
   // ------------------------------------------------------------------------
   // BEGIN_SWAP
   void swap(tape_t& other)
   // END_SWAP
   {  // same order as declaration of member variables just below private:
      std::swap( n_ind_, other.n_ind_ );
      std::swap( n_val_, other.n_val_);
      arg_vec_.swap( other.arg_vec_ );
      con_vec_.swap( other.con_vec_ );
      str_vec_.swap( other.str_vec_ );
      dep_vec_.swap( other.dep_vec_ );
      op_enum_vec_.swap( other.op_enum_vec_ );
      vec_initial_.swap( other.vec_initial_ );
      op2arg_index_.swap( other.op2arg_index_ );
      option_map_.swap( other.option_map_ );
   }
   // eval(trace, val_vec)
   void eval(
      bool           trace         ,
      Vector<Value>& val_vec       ) const
   {  size_t                  compare_false = 0;
      eval(trace, val_vec, compare_false);
   }
   // BEGIN_EVAL
   // eval(trace, val_vec, compare_false)
   void eval(
      bool                      trace         ,
      Vector<Value>&            val_vec       ,
      size_t&                   compare_false ) const
   // END_EVAL
   {  CPPAD_ASSERT_KNOWN(
         val_vec.size() == size_t(n_val_),
         "eval: size of val_vec not equal to tape.n_val()"
      );
      using std::setw;
      using std::right;
      using std::cout;
      //
      // ind_vec_vec
      // Only the vector_op routines use this eval argument
      Vector< Vector<addr_t> > ind_vec_vec;
      //
      // trace
      if( trace )
      {  // no operators for independent variables
         std::cout << "independent vector\n";
         for(addr_t res_index = 0; res_index < n_ind_; ++res_index)
         {  Value res = val_vec[res_index];
            cout << right << setw(5) << res_index;
            cout << " " << right << setw(10) << res << "\n";
         }
         std::printf("operators\n");
      }
      //
      // op_itr, i_op
      op_iterator<Value> op_itr(*this, 0);
      for(addr_t i_op = 0; i_op < n_op(); ++i_op)
      {  //
         // op_ptr, arg_index, res_index
         const base_op_t<Value>* op_ptr     = op_itr.op_ptr();
         addr_t                  arg_index  = op_itr.arg_index();
         addr_t                  res_index  = op_itr.res_index();
         //
         // base_op_t<Value>::eval
         op_ptr->eval(
            this,
            trace,
            arg_index,
            res_index,
            val_vec,
            ind_vec_vec,
            compare_false
         );
         //
         // op_itr
         ++op_itr;
      }
      // trace
      if( trace )
      {  // no operators for dependent variables
         std::cout << "dependent vector\n";
         for(size_t i = 0; i < dep_vec_.size(); ++i)
         {  addr_t res_index = dep_vec_[i];
            Value res        = val_vec[res_index];
            cout << right << setw(5) << res_index;
            cout << " " << right << setw(10) << res << "\n";
         }
         // space after end of this tape
         std::printf("\n");
      }
      return;
   }
   // ------------------------------------------------------------------------
   // functions in record.hpp
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
      addr_t atomic_index           ,
      addr_t call_id                ,
      addr_t n_res                  ,
      const Vector<addr_t>& fun_arg
   );
   //
   // record_csum_op
   addr_t record_csum_op(
      const Vector<addr_t>& add,
      const Vector<addr_t>& sub
   );
   //
   // record_cexp_op
   addr_t record_cexp_op(
      compare_enum_t compare_enum ,
      addr_t         left         ,
      addr_t         right        ,
      addr_t         if_true      ,
      addr_t         if_false
   );
   //
   // record_pri_op
   addr_t record_pri_op(
      const std::string& before      ,
      const std::string& after       ,
      addr_t             flag_index  ,
      addr_t             value_index
   );
   //
   // record_vec_op
   addr_t record_vec_op(const Vector<addr_t>& initial);
   //
   // record_load_op
   addr_t record_load_op(
      addr_t   which_vector  ,
      addr_t   vector_index
   );
   //
   // record_store_op
   addr_t record_store_op(
      addr_t   which_vector  ,
      addr_t   vector_index  ,
      addr_t   value_index
   );
   //
   // set_dep
   void set_dep(const Vector<addr_t>& dep_vec);
   // ------------------------------------------------------------------------
   // functions in their own files
   // ------------------------------------------------------------------------
   //
   // fold_con
   void fold_con(void);
   //
   // renumber
   void renumber(void);
   //
   // rev_depend
   void rev_depend(
      Vector<addr_t>& val_use_case  ,
      Vector<addr_t>& vec_last_load
   );
   //
   // dead_code
   vectorBool dead_code(void);
   //
   // compress
   vectorBool compress(void);
   //
   // record_new
   addr_t record_new(
      tape_t&                   new_tape         ,
      Vector<addr_t>&           new_which_vec    ,
      Vector<addr_t>&           work             ,
      const Vector<addr_t>&     new_val_index    ,
      const Vector<addr_t>&     val_use_case     ,
      const base_op_t<Value>*   op_ptr           ,
      addr_t                    arg_index        ,
      addr_t                    res_index
   );
   //
   // ------------------------------------------------------------------------
   // functions in summation.hpp
   // ------------------------------------------------------------------------
   //
   // summation
   void summation(void);
   //
   // set_op2arg_index
   void set_op2arg_index();
   //
   // op2arg_index
   const Vector<addr_t>& op2arg_index(void) const;
   //
   // replace_csum_op
   void replace_csum_op(
      addr_t       res_index ,
      addr_t       i_op      ,
      csum_info_t& csum_info
   );
   // -----------------------------------------------------------------------
   // functions in cumulative.hpp
   // -----------------------------------------------------------------------
   //
   // op2csum
   void op2csum(addr_t op_index);
   //
   // ------------------------------------------------------------------------
   // functions in option.hpp
   // ------------------------------------------------------------------------
   //
   // initialize_option
   void initialize_option(void);
   //
   // set_option
   void set_option(const std::string& name, const std::string& value);
};
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/val_graph/compress.hpp>
# include <cppad/local/val_graph/cumulative.hpp>
# include <cppad/local/val_graph/dead_code.hpp>
# include <cppad/local/val_graph/fold_con.hpp>
# include <cppad/local/val_graph/op2arg_index.hpp>
# include <cppad/local/val_graph/op_hash_table.hpp>
# include <cppad/local/val_graph/option.hpp>
# include <cppad/local/val_graph/record.hpp>
# include <cppad/local/val_graph/record_new.hpp>
# include <cppad/local/val_graph/renumber.hpp>
# include <cppad/local/val_graph/summation.hpp>
// END_SORT_THIS_LINE_MINUS_1

# undef CPPAD_VAL_GRAPH_TAPE_TRACE
# endif
