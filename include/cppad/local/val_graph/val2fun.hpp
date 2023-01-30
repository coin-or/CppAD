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
of the *i*-th independent dynamic parameter in *afun* .

*var_ind*
*********
The *i*-th element of the vector is the index in *val_tape*
of the *i*-th independent variable in *afun* .
No two elements of *dyn_ind* or *var_ind* can have the same value.
Furthermore, the total number of elements in these two vectors
must be the number of independent variables in *val_tape* .

*afun*
******
The input contents of *afun* do not matter.
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
The file :ref:`val2fun_xam.cpp-name`
is an example an test of this conversion.

{xrst_end val2fun_graph}
*/

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/val_graph/tape.hpp>
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
   // val_n_ind
   // size of the independent value vector
   size_t val_n_ind = val_tape.n_ind();
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
   // initialize this mapping and set its value of independent vector
   vector<ad_type_enum> val_ad_type(n_val);
   for(size_t i = 0; i < n_val; ++i)
      val_ad_type[i] = number_ad_type_enum; // invalid
   for(size_t i = 0; i < dyn_n_ind; ++i)
   {  CPPAD_ASSERT_KNOWN( dyn_ind[i] < val_n_ind,
         "val2fun: number of independent values is <= dyn_ind[i]"
      );
      CPPAD_ASSERT_KNOWN( val_ad_type[ dyn_ind[i] ] == number_ad_type_enum,
         "val2fun: dep_ind[i] == dep_ind[j] for some i and j"
      );
      val_ad_type[ dyn_ind[i] ] = dynamic_enum;
   }
   for(size_t i = 0; i < var_n_ind; ++i)
   {  CPPAD_ASSERT_KNOWN( var_ind[i] < val_n_ind,
         "val2fun: number of independent values is <= var_ind[i]"
      );
      CPPAD_ASSERT_KNOWN( val_ad_type[ var_ind[i] ] == number_ad_type_enum,
         "val2fun: var_ind[i] == dep_ind[j] for some i and j\n"
         "or var_ind[i] == var_ind[j] for some i and j"
      );
      val_ad_type[ var_ind[i] ] = variable_enum;
   }
   //
   // val_index2con_index
   // We could save space by including val_ad_type information in this vector.
   // (just give dynamics, variables, and invalid different values).
   vector<Base> val_index2con(n_val);
   for(size_t i = 0; i < n_val; ++i)
      val_index2con[i] = nan;
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
   // rec, parameter
   // initialize with the value nan at index nan
   const local::pod_vector_maybe<Base>& parameter( rec.all_par_vec());
   CPPAD_ASSERT_UNKNOWN( parameter.size() == 0 );
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
   // rec, val2fun_index
   // put the independent value vector in the function recording
   for(size_t i = 0; i < val_n_ind; ++i)
   {  if( val_ad_type[i] == dynamic_enum )
      {  par_addr         = rec.put_dyn_par(nan, local::ind_dyn);
         val2fun_index[i] = par_addr;
         CPPAD_ASSERT_UNKNOWN( isnan( parameter[par_addr] ) );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( val_ad_type[i] == variable_enum );
         var_addr         = rec.PutOp( local::InvOp );
         val2fun_index[i] = var_addr;
      }
   }
   CPPAD_ASSERT_UNKNOWN( size_t(par_addr) == dyn_n_ind );
   CPPAD_ASSERT_UNKNOWN( size_t(var_addr) == var_n_ind );
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
   vector<Base>         taylor_x, taylor_y;
   vector< AD<Base> >   ax, ay;
   //
   // op_index
   for(size_t op_index = 0; op_index < val_n_op; ++op_index)
   {  //
      // op_ptr, agr_index, res_index, n_arg, op_enum
      const op_info_t& op_info       = val_op_vec[op_index];
      op_base_t<Base>* op_ptr    = op_info.op_ptr;
      addr_t           arg_index = op_info.arg_index;
      addr_t           res_index = op_info.res_index;
      size_t           n_arg     = op_ptr->n_arg(arg_index, val_arg_vec);
      op_enum_t        op_enum   = op_ptr->op_enum();
      //
      // fun_arg, val_ad_type
      switch( op_enum )
      {  //
         // con_op
         case local::val_graph::con_op_enum:
         //
         // val_index2con
         CPPAD_ASSERT_UNKNOWN( n_arg = 1 );
         ad_type_x.resize(n_arg);
         ad_type_x[0]           = constant_enum;
         val_ad_type[res_index] = constant_enum;
         {  const Base& constant = val_con_vec[arg_index];
            par_addr = rec.put_con_par(constant);
            CPPAD_ASSERT_UNKNOWN(
               CppAD::isnan(constant) || parameter[par_addr] == constant
            );
            //
            val2fun_index[res_index] = par_addr;
            val_index2con[res_index] = constant;
         }
         break;
         //
         // default
         default:
         {  ad_type_x.resize(n_arg);
            fun_arg.resize(n_arg);
            ad_type_enum res_ad_type = constant_enum;
            for(addr_t i = 0; i < addr_t(n_arg); ++i)
            {  ad_type_x[i] = val_ad_type[ val_arg_vec[arg_index + i] ];
               fun_arg[i]   = val2fun_index[ val_arg_vec[arg_index + i] ];
               CPPAD_ASSERT_UNKNOWN( ad_type_x[i] < number_ad_type_enum );
               res_ad_type = std::max(res_ad_type, ad_type_x[i] );
            }
            val_ad_type[res_index] = res_ad_type;
         }
         CPPAD_ASSERT_UNKNOWN( constant_enum < val_ad_type[res_index] );
         break;
         //
         // call_op
         case local::val_graph::call_op_enum:
         {  //
            // atomic_index, call_id
            size_t n_res        = size_t( val_arg_vec[arg_index + 1] );
            size_t atomic_index = size_t( val_arg_vec[arg_index + 2] );
            size_t call_id      = size_t( val_arg_vec[arg_index + 3] );
            //
            // v_ptr, name
            CPPAD_ASSERT_UNKNOWN( 0 < atomic_index );
            bool         set_null = false;
            size_t       type     = 0;       // result: set to avoid warning
            std::string  name;               // result:
            void*        v_ptr    = nullptr; // result: set to avoid warning
            local::atomic_index<Base>(
               set_null, atomic_index, type, &name, v_ptr
            );
            // val_graph only supports atomic_four
            CPPAD_ASSERT_UNKNOWN( type == 4 );
            //
            // ad_type_x, taylor_x
            ad_type_x.resize(n_arg - 4);
            taylor_x.resize(n_arg - 4);
            for(addr_t i = 4; i < addr_t(n_arg); ++i)
            {  addr_t val_index =  val_arg_vec[arg_index + i];
               ad_type_x[i-4]   = val_ad_type[ val_index ];
               taylor_x[i-4]    = val_index2con[ val_index ];
            }
            //
            // ad_type_y, ok, afun
            ad_type_y.resize(n_res);
            bool               ok = false;
            atomic_four<Base>* afun = nullptr;
            if( v_ptr != nullptr )
            {  afun = reinterpret_cast< atomic_four<Base>* >(v_ptr);
               ok = afun->for_type(call_id, ad_type_x, ad_type_y);
            }
            if( ! ok )
            {  std::string msg = name;
               if( v_ptr == nullptr )
                  msg += ": this atomic function has been deleted";
               else
                  msg += ": atomic for_type returned false";
               CPPAD_ASSERT_KNOWN(false, msg.c_str() );
            }
            if( ok )
            {  // taylor_y
               select_y.resize(n_res);
               for(size_t i = 0; i < n_res; ++i)
                  select_y[i] = ad_type_y[i] == constant_enum;
               size_t order_low = 0, order_up = 0;
               taylor_y.resize(n_res);
               ok = afun->forward(
                  call_id,
                  select_y,
                  order_low,
                  order_up,
                  taylor_x,
                  taylor_y
               );
               if( ! ok )
               {  std::string msg = name;
                  msg += ": atomic forward returned false";
                  CPPAD_ASSERT_KNOWN(false, msg.c_str() );
               }
            }
            //
            // record_dynamic, record_variable
            bool record_dynamic  = false;
            bool record_variable = false;
            for(size_t i = 0; i < n_res; ++i)
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
               for(addr_t j = 4; j < addr_t(n_arg); ++j)
               {  addr_t val_index = val_arg_vec[arg_index + j];
                  ax[j-4].taddr_   = val2fun_index[val_index];
                  ax[j-4].value_   = val_index2con[val_index];
               }
               // ay
               ay.resize(n_res);
               for(addr_t j = 0; j < addr_t(n_res); ++j)
               {  ay[j].taddr_     = 0; // not used
                  ay[j].value_     = taylor_y[j];
               }
            }
            if( record_dynamic ) rec.put_dyn_atomic(
               tape_id, atomic_index, call_id, ad_type_x, ad_type_y, ax, ay
            );
            if( record_variable ) rec.put_var_atomic(
               tape_id, atomic_index, call_id, ad_type_x, ad_type_y, ax, ay
            );
            //
            // val2fun_index, val_index2con
            for(addr_t i = 0; i < addr_t(n_res); ++i)
            {  val2fun_index[res_index + i] = ay[i].taddr_;
               val_index2con[res_index + i] = taylor_y[i];
            }
         }
         break;
      }
      //
      // rec, val2fun_index, val_index2con
      switch( op_enum )
      {  //
         // call_op, con_op
         case local::val_graph::call_op_enum:
         case local::val_graph::con_op_enum:
         break;
         // ------------------------------------------------------------------
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
         val2fun_index[res_index] = tmp_addr;
         break;
         // ------------------------------------------------------------------
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
         val2fun_index[res_index] = tmp_addr;
         break;
         // ------------------------------------------------------------------
         default:
         CPPAD_ASSERT_KNOWN( op_enum > local::val_graph::number_op_enum,
            "op_enum is not yet implemented"
         );
         break;
      }
   }
   // rec
   rec.PutOp(local::EndOp);
   //
   // ----------------------------------------------------------------------
   // End recording, set private member data except for
   // ----------------------------------------------------------------------
   //
   // dep_taddr_
   // address of the dependent variables on variable tape
   dep_taddr_.resize( var_n_ind );
   for(size_t i = 0; i < var_n_ind; ++i)
   {  var_addr      = val_dep_vec[i];
      dep_taddr_[i] = val2fun_index[var_addr];
   }
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
