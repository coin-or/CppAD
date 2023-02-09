# ifndef  CPPAD_LOCAL_VAL_GRAPH_VAL2FUN_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_VAL2FUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// --------------------------------------------------------------------------
/*
{xrst_begin val2fun_graph dev}

Create an ADFun Object Corresponding to a Value Graph
#####################################################

Syntax
******

| |tab| ``ADFun`` < *Base* > *fun*
| |tab| *fun* . ``val2fun`` ( *val_tape* , *dyn_ind* , *var_ind* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Base
****
is the type corresponding to this :ref:`adfun-name` object;
i.e., its calculations are done using the type *Base* .
It is also :ref:`val_graph_tape@Value` type for the tape.

RecBase
*******
in the prototype above, *RecBase* is the same type as *Base* .

val_tape
********
This is a :ref:`val_graph_tape-name` representation of the function.

*dyn_ind*
*********
The *i*-th element of the vector is the index in the value graph
of the *i*-th independent dynamic parameter in *fun* .

*var_ind*
*********
The *i*-th element of the vector is the index in *val_tape*
of the *i*-th independent variable in *fun* .
No two elements of *dyn_ind* or *var_ind* can have the same value.
Furthermore, the total number of elements in these two vectors
must be the number of independent variables in *val_tape* .

*fun*
*****
The input contents of *fun* do not matter.
Upon return it is an ADFun representation of the function.

Under Construction
******************
This routine is under construction and is only implemented
for a few of the possible :ref:`ADFun-name` operators.

{xrst_toc_hidden
   val_graph/val2fun_xam.cpp
}
Examples
********
The file :ref:`val_val2fun_xam.cpp-name`
is an example an test of this conversion.

{xrst_end val2fun_graph}
*/

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/call_atomic.hpp>
# include <cppad/local/pod_vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void ADFun<Base, RecBase>::val2fun(
   const local::val_graph::tape_t<Base>& val_tape  ,
   const CppAD::vector<size_t>&          dyn_ind   ,
   const CppAD::vector<size_t>&          var_ind   )
// END_PROTOTYPE
{  //
   // vector
   using CppAD::vector;
   //
   // op_info_t
   typedef typename local::val_graph::tape_t<Base>::op_info_t op_info_t;
   //
   // op_base_t, op_enum_t
   using local::val_graph::op_base_t;
   using local::val_graph::op_enum_t;
   //
   // val_op_vec, val_arg_vec, val_con_vec, val_dep_vec
   const vector<op_info_t>& val_op_vec = val_tape.op_vec();
   const vector<addr_t>&    val_arg_vec = val_tape.arg_vec();
   const vector<Base>&      val_con_vec = val_tape.con_vec();
   const vector<addr_t>&    val_dep_vec = val_tape.dep_vec();
   //
   // nan
   Base nan = CppAD::numeric_limits<Base>::quiet_NaN();
   //
   // par_addr
   // a temporary parameter index
   addr_t par_addr;
   //
   // var_addr
   // a temporary variable index
   addr_t var_addr;
   //
   // tmp_addr
   // a parameter or variable temporary index
   addr_t tmp_addr;
   //
# ifndef NDEBUG
   // val_n_ind
   // size of the independent value vector
   size_t val_n_ind = val_tape.n_ind();
# endif
   //
   // dyn_n_ind
   // number of independent dynamc parameters
   size_t dyn_n_ind = dyn_ind.size();
   //
   // var_n_ind
   // number of independent varibles
   size_t var_n_ind = var_ind.size();
   //
   // val_n_op
   size_t val_n_op = val_op_vec.size();
   //
   CPPAD_ASSERT_KNOWN( dyn_n_ind + var_n_ind == val_n_ind,
      "val2fun: The number of independent variables and dynamic parameters\n"
      "is not equal to the size of the independent value vector"
   );
   //
   // n_val
   size_t n_val = val_tape.n_val();
   //
   // val_ad_type
   vector<ad_type_enum> val_ad_type(n_val);
   for(size_t i = 0; i < n_val; ++i)
      val_ad_type[i] = number_ad_type_enum; // invalid
   //
   // val_index2con
   // After fold_con, all the constants that get used are op_con results.
   vector<Base> val_index2con(n_val);
   for(size_t i = 0; i < n_val; ++i)
      val_index2con[i] = nan;
   for(size_t i_op = 0; i_op < val_op_vec.size(); ++i_op)
   {  op_enum_t op_enum = val_op_vec[i_op].op_ptr->op_enum();
      if( op_enum == local::val_graph::con_op_enum )
      {  addr_t res_index = val_op_vec[i_op].res_index;
         addr_t con_index = val_arg_vec[ val_op_vec[i_op].arg_index ];
         val_index2con[res_index] = val_con_vec[con_index];
      }
   }
   //
   // val2fun_index
   // mapping from value index to index in the AD function object.
   // The meaning of this index depends on its ad_type.
   vector<addr_t> val2fun_index(n_val);
   for(size_t i = 0; i < n_val; ++i)
      val2fun_index[i] = std::numeric_limits<addr_t>::max(); // invalid
   //
   // rec
   // start a functon recording
   local::recorder<Base> rec;
   CPPAD_ASSERT_UNKNOWN( rec.num_op_rec() == 0 );
   rec.set_num_dynamic_ind(dyn_n_ind);
   rec.set_abort_op_index(0);
   rec.set_record_compare(false);
   //
   // parameter
# ifndef NDEBUG
   const local::pod_vector_maybe<Base>& parameter( rec.all_par_vec());
   CPPAD_ASSERT_UNKNOWN( parameter.size() == 0 );
# endif
   //
   // rec
   // initialize with the value nan at index nan
   par_addr = rec.put_con_par(nan);
   CPPAD_ASSERT_UNKNOWN( par_addr == 0 );
   CPPAD_ASSERT_UNKNOWN( isnan( parameter[par_addr] ) );
   //
   // rec
   // Place the variable with index 0 in the tape
   CPPAD_ASSERT_NARG_NRES(local::BeginOp, 1, 1);
   rec.PutOp(local::BeginOp);
   rec.PutArg(0); // parameter argumnet is the nan above
   //
   // rec, val_ad_type, val2fun_index
   // put the independent value vector in the function recording

   for(size_t i = 0; i < dyn_n_ind; ++i)
   {  CPPAD_ASSERT_KNOWN( dyn_ind[i] < val_n_ind,
         "val2fun: number of independent values is <= dyn_ind[i]"
      );
      CPPAD_ASSERT_KNOWN( val_ad_type[ dyn_ind[i] ] == number_ad_type_enum,
         "val2fun: dep_ind[i] == dep_ind[j] for some i and j"
      );
      val_ad_type[ dyn_ind[i] ]  = dynamic_enum;
      par_addr                    = rec.put_dyn_par(nan, local::ind_dyn);
      val2fun_index[ dyn_ind[i] ] = par_addr;
      CPPAD_ASSERT_UNKNOWN( isnan( parameter[par_addr] ) );
   }

   for(size_t i = 0; i < var_n_ind; ++i)
   {  CPPAD_ASSERT_KNOWN( var_ind[i] < val_n_ind,
         "val2fun: number of independent values is <= var_ind[i]"
      );
      CPPAD_ASSERT_KNOWN( val_ad_type[ var_ind[i] ] == number_ad_type_enum,
         "val2fun: var_ind[i] == dep_ind[j] for some i and j\n"
         "or var_ind[i] == var_ind[j] for some i and j"
      );
      val_ad_type[ var_ind[i] ]   = variable_enum;
      var_addr                    = rec.PutOp( local::InvOp );
      val2fun_index[ var_ind[i] ] = var_addr;
   }
   //
   // ind_taddr_
   // address of the independent variables on variable tape
   ind_taddr_.resize(var_n_ind);
   for(size_t i = 0; i < var_n_ind; ++i)
      ind_taddr_[i] = i + 1;
   //
   // ad_type_x, ad_type_y, fun_arg, select_y
   vector<ad_type_enum> ad_type_x, ad_type_y;
   vector<addr_t>       fun_arg;
   vector<bool>         select_y;
   vector<Base>         con_x;
   vector< AD<Base> >   ax, ay;
   //
   // op_index
   for(size_t op_index = 0; op_index < val_n_op; ++op_index)
   {  //
      // is_unary, is_binary, arg_index, res_index, n_arg, op_enum
      const op_info_t& op_info   = val_op_vec[op_index];
      op_base_t<Base>* op_ptr    = op_info.op_ptr;
      bool             is_unary  = op_ptr->is_unary();
      bool             is_binary = op_ptr->is_binary();
      addr_t           arg_index = op_info.arg_index;
      addr_t           res_index = op_info.res_index;
      addr_t           n_arg     = op_ptr->n_arg(arg_index, val_arg_vec);
      op_enum_t        op_enum   = op_ptr->op_enum();
      //
      // rec, val_ad_type, val2fun_index
      if( is_unary || is_binary )
      {  //
         // ad_type_x, val_ad_type
         ad_type_x.resize(n_arg);
         fun_arg.resize(n_arg);
         ad_type_enum res_ad_type = constant_enum;
         for(addr_t i = 0; i < n_arg; ++i)
         {  ad_type_x[i] = val_ad_type[ val_arg_vec[arg_index + i] ];
            fun_arg[i]   = val2fun_index[ val_arg_vec[arg_index + i] ];
            CPPAD_ASSERT_UNKNOWN( ad_type_x[i] < number_ad_type_enum );
            res_ad_type = std::max(res_ad_type, ad_type_x[i] );
         }
         val_ad_type[res_index] = res_ad_type;
         CPPAD_ASSERT_KNOWN( constant_enum < res_ad_type,
            "val2fun: must first call fold_con"
         );
         //
         // rec, val2fun_index
         switch( op_enum )
         {  //
            // default
            default:
            CPPAD_ASSERT_KNOWN( op_enum > local::val_graph::number_op_enum,
               "val_graph::val2fun: op_enum is not yet implemented"
            );
            tmp_addr = 0; // to avoid compiler warning
            break;
            // ----------------------------------------------------------------
            // add_op_enum
            case local::val_graph::add_op_enum:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            if( val_ad_type[res_index] == dynamic_enum )
            {  tmp_addr = rec.put_dyn_par(
                  nan, local::add_dyn, fun_arg[0], fun_arg[1]
               );
               CPPAD_ASSERT_UNKNOWN( isnan( parameter[tmp_addr] ) );
            }
            else
            {  if( ad_type_x[0] < variable_enum )
               {  CPPAD_ASSERT_UNKNOWN( ad_type_x[1] == variable_enum );
                  tmp_addr = rec.PutOp(local::AddpvOp);
               }
               else if( ad_type_x[1] < variable_enum )
               {  CPPAD_ASSERT_UNKNOWN( ad_type_x[0] == variable_enum );
                  tmp_addr = rec.PutOp(local::AddpvOp);
                  std::swap(fun_arg[0], fun_arg[1]);
               }
               else
               {  CPPAD_ASSERT_UNKNOWN( ad_type_x[1] == variable_enum );
                  CPPAD_ASSERT_UNKNOWN( ad_type_x[0] == variable_enum );
                  tmp_addr = rec.PutOp(local::AddvvOp);
               }
               rec.PutArg(fun_arg[0], fun_arg[1]);
            }
            break;
            // ----------------------------------------------------------------
            // sub_op_enum
            case local::val_graph::sub_op_enum:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            if( val_ad_type[res_index] == dynamic_enum )
            {  tmp_addr = rec.put_dyn_par(
                  nan, local::sub_dyn, fun_arg[0], fun_arg[1]
               );
               CPPAD_ASSERT_UNKNOWN( isnan( parameter[tmp_addr] ) );
            }
            else
            {  if( ad_type_x[0] < variable_enum )
               {  CPPAD_ASSERT_UNKNOWN( ad_type_x[1] == variable_enum );
                  tmp_addr = rec.PutOp(local::SubpvOp);
               }
               else if( ad_type_x[1] < variable_enum )
               {  CPPAD_ASSERT_UNKNOWN( ad_type_x[0] == variable_enum );
                  tmp_addr = rec.PutOp(local::SubvpOp);
               }
               else
               {  CPPAD_ASSERT_UNKNOWN( ad_type_x[1] == variable_enum );
                  CPPAD_ASSERT_UNKNOWN( ad_type_x[0] == variable_enum );
                  tmp_addr = rec.PutOp(local::SubvvOp);
               }
               rec.PutArg(fun_arg[0], fun_arg[1]);
            }
            break;
            // ----------------------------------------------------------------
         }
         val2fun_index[res_index] = tmp_addr;
      }
      else switch( op_enum )
      {  // ! ( is_unary || is_binary)
         //
         // default
         default:
         CPPAD_ASSERT_KNOWN( op_enum > local::val_graph::number_op_enum,
            "val_graph::val2fun: op_enum is not yet implemented"
         );
         // ------------------------------------------------------------------
         // dis_op
         // rec, val_ad_type, val2fun_index
         case local::val_graph::dis_op_enum:
         fun_arg.resize(1);
         CPPAD_ASSERT_UNKNOWN( n_arg = 2 );
         {  addr_t dynamic_index  = val_arg_vec[arg_index + 0];
            addr_t val_index      = val_arg_vec[arg_index + 1];
            fun_arg[0]            = val2fun_index[val_index];
            ad_type_enum  ad_type = val_ad_type[val_index];
            CPPAD_ASSERT_KNOWN( constant_enum < ad_type,
               "val2fun: must first call fold_con"
            );
            if( ad_type == dynamic_enum )
            {  tmp_addr = rec.put_dyn_par(
                  nan, local::dis_dyn, dynamic_index, fun_arg[0]
               );
            }
            else
            {  CPPAD_ASSERT_UNKNOWN( ad_type == variable_enum );
               tmp_addr = rec.PutOp(local::DisOp);
               rec.PutArg( dynamic_index );
               rec.PutArg( fun_arg[0] );
            }
            val_ad_type[res_index]   = ad_type;
            val2fun_index[res_index] = tmp_addr;
         }
         break;
         // ------------------------------------------------------------------
         // con_op
         // rec, val_ad_type, val2fun_index
         case local::val_graph::con_op_enum:
         CPPAD_ASSERT_UNKNOWN( n_arg = 1 );
         {  const Base& constant = val_con_vec[arg_index];
            par_addr = rec.put_con_par(constant);
            CPPAD_ASSERT_UNKNOWN(
               CppAD::isnan(constant) || parameter[par_addr] == constant
            );
            //
            val_ad_type[res_index]   = constant_enum;
            val2fun_index[res_index] = par_addr;
         }
         break;
         // -------------------------------------------------------------------
         // comp_op
         // rec
         case local::val_graph::comp_op_enum:
         {  //
            // compare, left_index, right_index
            addr_t compare          = val_arg_vec[arg_index + 0];
            addr_t left_index       = val_arg_vec[arg_index + 1];
            addr_t right_index      = val_arg_vec[arg_index + 2];
            //
            // var_left, var_right, dyn_left, dyn_right
            bool var_left  = val_ad_type[left_index]  == variable_enum;
            bool var_right = val_ad_type[right_index] == variable_enum;
            bool dyn_left  = val_ad_type[left_index]  == dynamic_enum;
            bool dyn_right = val_ad_type[right_index] == dynamic_enum;
            //
            // ax
            ax.resize(2);
            //
            if( var_left | dyn_left )
               ax[0].taddr_ = val2fun_index[left_index];
            else
               ax[0].value_ = val_index2con[left_index];
            //
            if( var_right | dyn_right )
               ax[1].taddr_ = val2fun_index[right_index];
            else
               ax[1].value_ = val_index2con[right_index];
            //
            // res, compare_enum
            using local::val_graph::compare_enum_t;
            compare_enum_t compare_enum = compare_enum_t( compare );
            bool res;
            switch(compare_enum)
            {  //
               default:
               CPPAD_ASSERT_UNKNOWN(false);
               break;
               //
               case local::val_graph::compare_eq_enum:
               res = true;
               rec.comp_eq(
                  var_left, var_right, dyn_left, dyn_right, ax[0], ax[1], res
               );
               break;
               //
               case local::val_graph::compare_le_enum:
               res = true;
               rec.comp_le(
                  var_left, var_right, dyn_left, dyn_right, ax[0], ax[1], res
               );
               break;
               //
               case local::val_graph::compare_lt_enum:
               res = true;
               rec.comp_lt(
                  var_left, var_right, dyn_left, dyn_right, ax[0], ax[1], res
               );
               break;
               //
               case local::val_graph::compare_ne_enum:
               res = false;
               rec.comp_eq(
                  var_left, var_right, dyn_left, dyn_right, ax[0], ax[1], res
               );
               break;
               //
               case local::val_graph::compare_no_enum:
               break;
            }
         }
         break;
         // -------------------------------------------------------------------
         // call_op
         // rec, val_ad_type, val2fun_index
         case local::val_graph::call_op_enum:
         {  //
            // atomic_index, call_id
            addr_t n_res        =  val_arg_vec[arg_index + 1] ;
            size_t atomic_index = size_t( val_arg_vec[arg_index + 2] );
            size_t call_id      = size_t( val_arg_vec[arg_index + 3] );
            //
            // ad_type_x, con_x
            ad_type_x.resize(n_arg - 4);
            con_x.resize(n_arg - 4);
            for(addr_t i = 4; i < n_arg; ++i)
            {  addr_t val_index =  val_arg_vec[arg_index + i];
               ad_type_x[i-4]   = val_ad_type[ val_index ];
               con_x[i-4]       = val_index2con[ val_index ];
            }
            //
            // ad_type_y
            ad_type_y.resize(n_res);
            local::val_graph::call_atomic_for_type<Base>(
               con_x, ad_type_x, ad_type_y, atomic_index, call_id
            );
            //
            // record_dynamic, record_variable
            bool record_dynamic  = false;
            bool record_variable = false;
            for(addr_t i = 0; i < n_res; ++i)
            {  CPPAD_ASSERT_UNKNOWN( ad_type_y[i] <= variable_enum );
               record_dynamic  |= ad_type_y[i] == dynamic_enum;
               record_variable |= ad_type_y[i] == variable_enum;
            }
            // tape_id
            // tape_id is zero because not a true recording
            tape_id_t tape_id = 0;
            //
            // ax, ay
            if( record_dynamic || record_variable )
            {  //
               // ax
               ax.resize(n_arg - 4);
               for(addr_t j = 4; j < n_arg; ++j)
               {  addr_t val_index = val_arg_vec[arg_index + j];
                  ax[j-4].taddr_   = val2fun_index[val_index];
                  ax[j-4].value_   = val_index2con[val_index];
               }
               // ay
               ay.resize(n_res);
               for(addr_t j = 0; j < n_res; ++j)
               {  ay[j].taddr_     = 0; // not used
                  ay[j].value_     = val_index2con[res_index + j];
               }
            }
            if( record_dynamic ) rec.put_dyn_atomic(
               tape_id, atomic_index, call_id, ad_type_x, ad_type_y, ax, ay
            );
            if( record_variable ) rec.put_var_atomic(
               tape_id, atomic_index, call_id, ad_type_x, ad_type_y, ax, ay
            );
            //
            // val2fun_index, val_ad_type
            for(addr_t i = 0; i < n_res; ++i)
            {  val2fun_index[res_index + i] = ay[i].taddr_;
               val_ad_type[res_index + i]   = ad_type_y[i];
            }
         }
         break;
      }
   }
   // dep_taddr_, rec
   // address of the dependent variables on variable tape
   dep_taddr_.resize( val_dep_vec.size() );
   for(size_t i = 0; i < val_dep_vec.size(); ++i)
   {  addr_t val_index     = val_dep_vec[i];
      ad_type_enum ad_type = val_ad_type[val_index];
      addr_t fun_index     = val2fun_index[val_index];
      if( ad_type < variable_enum )
      {  // see RecordParOp(const AD<Base>& y)
         rec.PutArg( fun_index );
         fun_index = rec.PutOp(local::ParOp);
      }
      dep_taddr_[i] = fun_index;
   }
   // rec
   rec.PutOp(local::EndOp);
   //
   //
   // ----------------------------------------------------------------------
   // End recording, set private member data except for
   // ----------------------------------------------------------------------
   //
   // bool values in this object except check_for_nan_
   has_been_optimized_        = false;
   //
   // size_t values in this object
   compare_change_count_      = 1;
   compare_change_number_     = 0;
   compare_change_op_index_   = 0;
   num_order_taylor_          = 0;
   cap_order_taylor_          = 0;
   num_direction_taylor_      = 0;
   num_var_tape_              = rec.num_var_rec();
   //
   // taylor_
   taylor_.resize(0);
   //
   // cskip_op_
   cskip_op_.resize( rec.num_op_rec() );
   //
   // load_op2var_
   load_op2var_.resize( rec.num_var_load_rec() );
   //
   // play_
   // Now that each dependent variable has a place in the recording,
   // and there is a EndOp at the end of the record, we can transfer the
   // recording to the player and and erase the recording.
   play_.get_recording(rec, var_n_ind);
   //
   // ind_taddr_
   // Note that play_ has been set, we can use it to check operators
   CPPAD_ASSERT_UNKNOWN( var_n_ind < num_var_tape_);
   for(size_t j = 0; j < var_n_ind; j++)
   {  CPPAD_ASSERT_UNKNOWN( play_.GetOp(j+1) == local::InvOp );
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] = j+1 );
   }
   //
   // for_jac_sparse_pack_, for_jac_sparse_set_
   for_jac_sparse_pack_.resize(0, 0);
   for_jac_sparse_set_.resize(0,0);
   //
   // resize subgraph_info_
   subgraph_info_.resize(
      ind_taddr_.size(),   // n_dep
      dep_taddr_.size(),   // n_ind
      play_.num_op_rec(),  // n_op
      play_.num_var_rec()  // n_var
   );
   //
   // set the function name
   function_name_ = "";
   //
   //
   return;
}

} // END_CPPAD_NAMESPACE
// --------------------------------------------------------------------------
# endif
