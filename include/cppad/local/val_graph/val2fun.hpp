# ifndef  CPPAD_LOCAL_VAL_GRAPH_VAL2FUN_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_VAL2FUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// --------------------------------------------------------------------------
/*
{xrst_begin val2fun_graph dev}

Create an ADFun Object Corresponding to a Value Graph
#####################################################

Syntax
******
| |tab| ``ADFun`` < *Base* > *fun*
| |tab| *fun* . ``val2fun`` ( *val_tape* , *dyn_ind* , *var_ind* )
| |tab| *fun* . ``val2fun`` ( *val_tape* , *dyn_ind* , *var_ind* , *use_val* )

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
It is also :ref:`val_tape@Value` type for the tape.

RecBase
*******
in the prototype above, *RecBase* is the same type as *Base* .

val_tape
********
This is a :ref:`val_tape-name` representation of the function.

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

use_val
*******
If this vector is present extra detection is done to convert
variables to parameters.
It has size equal to *val_tape*\ .n_val().
For each value index, that is a result for a
:ref:`val_call_op-name`, the corresponding *use_val*\ [i] is true (false)
if the result is used to compute one of the dependent variables.

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
# include <cppad/local/val_graph/op_iterator.hpp>
# include <cppad/local/val_graph/call_atomic.hpp>
# include <cppad/local/pod_vector.hpp>
# include <cppad/core/cppad_assert.hpp>

# define CPPAD_VAL2FUN_DYN_UNARY(name)                  \
   case local::val_graph::name##_op_enum:               \
   tmp_addr = rec.put_dyn_par(                          \
      nan, local::name##_dyn, fun_arg[0]                \
   );                                                   \
   break;

# define CPPAD_VAL2FUN_DYN_BINARY(name)                 \
   case local::val_graph::name##_op_enum:               \
   tmp_addr = rec.put_dyn_par(                          \
      nan, local::name##_dyn, fun_arg[0], fun_arg[1]    \
   );                                                   \
   break;

# define CPPAD_VAL2FUN_VAR_UNARY(name, Op)              \
   case local::val_graph::name##_op_enum:               \
   tmp_addr = rec.PutOp(local::Op);                     \
   rec.PutArg( fun_arg[0] );                            \
   break;

# define CPPAD_VAL2FUN_VAR_BINARY(name, Op)             \
   case local::val_graph::name##_op_enum:               \
   tmp_addr = rec.PutOp(local::Op);                     \
   break;


namespace CppAD { // BEGIN_CPPAD_NAMESPACE

template <class Base, class RecBase>
void ADFun<Base, RecBase>::val2fun(
   const local::val_graph::tape_t<Base>&                   val_tape  ,
   const CppAD::local::val_graph::Vector<size_t>&          dyn_ind   ,
   const CppAD::local::val_graph::Vector<size_t>&          var_ind   )
{  vectorBool use_val;
   val2fun(val_tape, dyn_ind, var_ind, use_val);
}

// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void ADFun<Base, RecBase>::val2fun(
   const local::val_graph::tape_t<Base>&                   val_tape  ,
   const CppAD::local::val_graph::Vector<size_t>&          dyn_ind   ,
   const CppAD::local::val_graph::Vector<size_t>&          var_ind   ,
   const CppAD::vectorBool&                                use_val   )
// END_PROTOTYPE
{  //
   // vector
   using CppAD::local::val_graph::Vector;
   //
   // base_op_t, op_enum_t, compare_enum_t
   using local::val_graph::base_op_t;
   using local::val_graph::op_enum_t;
   using local::val_graph::compare_enum_t;
   //
   // val_arg_vec, val_con_vec, val_str_vec, val_dep_vec
   const Vector<addr_t>&      val_arg_vec = val_tape.arg_vec();
   const Vector<Base>&        val_con_vec = val_tape.con_vec();
   const Vector<std::string>& val_str_vec = val_tape.str_vec();
   const Vector<addr_t>&      val_dep_vec = val_tape.dep_vec();
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
   // initialize to zero to avoid compiler warnings
   addr_t tmp_addr = 0;
   //
# ifndef NDEBUG
   // val_n_ind
   // size of the independent value vector
   size_t val_n_ind = size_t( val_tape.n_ind() );
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
   addr_t val_n_op = val_tape.n_op();
   //
   CPPAD_ASSERT_KNOWN( dyn_n_ind + var_n_ind == val_n_ind,
      "val2fun: The number of independent variables and dynamic parameters\n"
      "is not equal to the size of the independent value vector"
   );
   //
   // n_val
   addr_t n_val = val_tape.n_val();
   //
   // fun_ad_type
   Vector<ad_type_enum> fun_ad_type(n_val);
   for(addr_t i = 0; i < n_val; ++i)
      fun_ad_type[i] = number_ad_type_enum; // invalid
   //
   // val_index2con
   // After fold_con, all the constants that get used are op_con results.
   Vector<Base> val_index2con(n_val);
   for(addr_t i = 0; i < n_val; ++i)
      val_index2con[i] = nan;
   bool trace           = false;
   val_tape.eval(trace, val_index2con);
   //
   // val2fun_index
   // mapping from value index to index in the AD function object.
   // The meaning of this index depends on its ad_type.
   Vector<addr_t> val2fun_index(n_val);
   for(addr_t i = 0; i < n_val; ++i)
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
   CPPAD_ASSERT_UNKNOWN( CppAD::isnan( parameter[par_addr] ) );
   //
   // rec
   // Place the variable with index 0 in the tape
   CPPAD_ASSERT_UNKNOWN( NumArg(local::BeginOp) == 1);
   CPPAD_ASSERT_UNKNOWN( NumRes(local::BeginOp) == 1);
   rec.PutOp(local::BeginOp);
   rec.PutArg(0); // parameter argumnet is the nan above
   //
   // rec, vecad_offset
   // place the VecAD objects in the recording
   Vector<addr_t> vecad_offset;
   addr_t         offset = 1;
   for(size_t i = 0; i < val_tape.vec_initial().size(); ++i)
   {  const Vector<addr_t>& val_initial = val_tape.vec_initial()[i];
      size_t size = val_initial.size();
      local::pod_vector<addr_t>  fun_initial(size);
      for(size_t j = 0; j < size; ++j)
      {  addr_t val_index  = val_initial[j];
         const Base& value = val_index2con[val_index];
         CPPAD_ASSERT_KNOWN( ! CppAD::isnan(value),
            "val2fun: an initial value for a dynamic array is not constant"
         );
         fun_initial[j] =  rec.put_con_par( value );
      }
      rec.put_var_vecad(size, fun_initial);
      vecad_offset.push_back( offset );
      offset += addr_t(size + 1);
   }
   //
   // rec, fun_ad_type, val2fun_index
   // put the independent dynamic paraeters in the function recording
   for(size_t i = 0; i < dyn_n_ind; ++i)
   {  CPPAD_ASSERT_KNOWN( dyn_ind[i] < val_n_ind,
         "val2fun: number of independent values is <= dyn_ind[i]"
      );
      CPPAD_ASSERT_KNOWN( fun_ad_type[ dyn_ind[i] ] == number_ad_type_enum,
         "val2fun: dep_ind[i] == dep_ind[j] for some i and j"
      );
      fun_ad_type[ dyn_ind[i] ]  = dynamic_enum;
      par_addr                    = rec.put_dyn_par(nan, local::ind_dyn);
      val2fun_index[ dyn_ind[i] ] = par_addr;
      CPPAD_ASSERT_UNKNOWN( CppAD::isnan( parameter[par_addr] ) );
   }
   // put the independent variables in the function recording
   for(size_t i = 0; i < var_n_ind; ++i)
   {  CPPAD_ASSERT_KNOWN( var_ind[i] < val_n_ind,
         "val2fun: number of independent values is <= var_ind[i]"
      );
      CPPAD_ASSERT_KNOWN( fun_ad_type[ var_ind[i] ] == number_ad_type_enum,
         "val2fun: var_ind[i] == dep_ind[j] for some i and j\n"
         "or var_ind[i] == var_ind[j] for some i and j"
      );
      fun_ad_type[ var_ind[i] ]   = variable_enum;
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
   // rec_con_index
   // First constant in CppAD recording and val_graph is nan
   Vector<addr_t> rec_con_index( val_con_vec.size() );
   CPPAD_ASSERT_UNKNOWN( CppAD::isnan( val_con_vec[0] ) );
   rec_con_index[0] = 0;
   for(size_t i = 1; i < val_con_vec.size(); ++i)
      rec_con_index[i] = rec.put_con_par( val_con_vec[i] );
   //
   // zero_index
   addr_t zero_index = rec.put_con_par( Base(0.0) );
   //
   // two_sqrt_pi_index: 2 / sqrt(pi)
   addr_t two_sqrt_pi_index = rec.put_con_par(
      Base(1.0) / sqrt( atan( Base(1.0) ) )
   );
   //
   // rec_str_index
   Vector<addr_t> rec_str_index( val_str_vec.size() );
   for(size_t i = 0; i < val_str_vec.size(); ++i)
   {  const std::string& str = val_str_vec[i];
      rec_str_index[i]       = rec.PutTxt( str.c_str() );
   }
   //
   // ad_type_x, ad_type_y, fun_arg, csum_arg, select_y
   Vector<addr_t>       fun_arg, csum_arg;
   Vector<bool>         select_y;
   Vector< AD<Base> >   ax, ay;
   //
   // con_x, at_type_x, ad_type_y;
   // Use CppAD::vector because call_atomic_for_type requires it
   CppAD::vector<Base>  con_x;
   CppAD::vector<ad_type_enum> ad_type_x, ad_type_y;
   //
   // op_itr
   local::val_graph::op_iterator<Base> op_itr(val_tape, 0);
   //
   // i_op
   for(addr_t i_op = 0; i_op < val_n_op; ++i_op)
   {  //
      // op_itr
      if( 0 < i_op )
         ++op_itr;
      //
      // op_ptr, arg_index, res_index
      const base_op_t<Base>* op_ptr    = op_itr.op_ptr();
      addr_t                 arg_index = op_itr.arg_index();
      addr_t                 res_index = op_itr.res_index();
      //
      // op_enum, is_unary, is_binary, n_arg
      op_enum_t        op_enum   = op_ptr->op_enum();
      bool             is_unary  = op_ptr->is_unary();
      bool             is_binary = op_ptr->is_binary();
      addr_t           n_before  = op_ptr->n_before();
      addr_t           n_after   = op_ptr->n_after();
      addr_t           n_arg     = op_ptr->n_arg(arg_index, val_arg_vec);
      CPPAD_ASSERT_UNKNOWN( n_before + n_after <= n_arg );
      //
      // n_x
      addr_t n_x = n_arg - n_before - n_after;
      //
      // ad_type_x, fun_arg, con_x, max_ad_type
      ad_type_x.resize(n_x);
      fun_arg.resize(n_x);
      con_x.resize(n_x);
      ad_type_enum max_ad_type = constant_enum;
      for(addr_t i = 0; i < n_x; ++i)
      {  addr_t val_index     = val_arg_vec[arg_index + n_before + i];
         ad_type_x[i] = fun_ad_type[val_index];
         fun_arg[i]   = val2fun_index[val_index];
         con_x[i]     = val_index2con[val_index];
         max_ad_type  = std::max(max_ad_type, ad_type_x[i] );
      }
      /*
      CPPAD_ASSERT_KNOWN(
         n_x == 0 || constant_enum < max_ad_type,
         "val2fun: must first call fold_con"
      );
      */
      //
      // rec, fun_ad_type, val2fun_index
      if( is_unary )
      {  //
         fun_ad_type[res_index] = max_ad_type;
         CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
         //
         // rec, val2fun_index
         if( max_ad_type == dynamic_enum )
         {  switch( op_enum )
            {  //
               // default
               default:
               CPPAD_ASSERT_UNKNOWN(false);
               tmp_addr = 0; // to avoid compiler warning
               break;
               //
               // BEGIN_SORT_THIS_LINE_PLUS_1
               CPPAD_VAL2FUN_DYN_UNARY(abs);
               CPPAD_VAL2FUN_DYN_UNARY(acos);
               CPPAD_VAL2FUN_DYN_UNARY(acosh);
               CPPAD_VAL2FUN_DYN_UNARY(asin);
               CPPAD_VAL2FUN_DYN_UNARY(asinh);
               CPPAD_VAL2FUN_DYN_UNARY(atan);
               CPPAD_VAL2FUN_DYN_UNARY(atanh);
               CPPAD_VAL2FUN_DYN_UNARY(cos);
               CPPAD_VAL2FUN_DYN_UNARY(cosh);
               CPPAD_VAL2FUN_DYN_UNARY(erf);
               CPPAD_VAL2FUN_DYN_UNARY(erfc);
               CPPAD_VAL2FUN_DYN_UNARY(exp);
               CPPAD_VAL2FUN_DYN_UNARY(expm1);
               CPPAD_VAL2FUN_DYN_UNARY(log);
               CPPAD_VAL2FUN_DYN_UNARY(log1p);
               CPPAD_VAL2FUN_DYN_UNARY(neg);
               CPPAD_VAL2FUN_DYN_UNARY(sign);
               CPPAD_VAL2FUN_DYN_UNARY(sin);
               CPPAD_VAL2FUN_DYN_UNARY(sinh);
               CPPAD_VAL2FUN_DYN_UNARY(sqrt);
               CPPAD_VAL2FUN_DYN_UNARY(tan);
               // END_SORT_THIS_LINE_MINUS_1
            }
         }
         else
         {  switch( op_enum )
            {
               // default
               default:
               CPPAD_ASSERT_UNKNOWN(false);
               tmp_addr = 0; // to avoid compiler warning
               break;
               //
               // BEGIN_SORT_THIS_LINE_PLUS_1
               CPPAD_VAL2FUN_VAR_UNARY(abs,     AbsOp);
               CPPAD_VAL2FUN_VAR_UNARY(acos,    AcosOp);
               CPPAD_VAL2FUN_VAR_UNARY(acosh,   AcoshOp);
               CPPAD_VAL2FUN_VAR_UNARY(asin,    AsinOp);
               CPPAD_VAL2FUN_VAR_UNARY(asinh,   AsinhOp);
               CPPAD_VAL2FUN_VAR_UNARY(atan,    AtanOp);
               CPPAD_VAL2FUN_VAR_UNARY(atanh,   AtanhOp);
               CPPAD_VAL2FUN_VAR_UNARY(cos,     CosOp);
               CPPAD_VAL2FUN_VAR_UNARY(cosh,    CoshOp);
               CPPAD_VAL2FUN_VAR_UNARY(exp,     ExpOp);
               CPPAD_VAL2FUN_VAR_UNARY(expm1,   Expm1Op);
               CPPAD_VAL2FUN_VAR_UNARY(log,     LogOp);
               CPPAD_VAL2FUN_VAR_UNARY(log1p,   Log1pOp);
               CPPAD_VAL2FUN_VAR_UNARY(neg,     NegOp);
               CPPAD_VAL2FUN_VAR_UNARY(sign,    SignOp);
               CPPAD_VAL2FUN_VAR_UNARY(sin,     SinOp);
               CPPAD_VAL2FUN_VAR_UNARY(sinh,    SinhOp);
               CPPAD_VAL2FUN_VAR_UNARY(sqrt,    SqrtOp);
               CPPAD_VAL2FUN_VAR_UNARY(tan,     TanOp);
               CPPAD_VAL2FUN_VAR_UNARY(tanh,    TanhOp);
               // END_SORT_THIS_LINE_MINUS_1
               //
               // erf
               case local::val_graph::erf_op_enum:
               {  tmp_addr = rec.PutOp(local::ErfOp);
                  CPPAD_ASSERT_UNKNOWN( NumArg(local::ErfOp) == 3 );
                  rec.PutArg( fun_arg[0] );
                  rec.PutArg( zero_index );
                  rec.PutArg( two_sqrt_pi_index );
               }
               break;
               //
               // erfc
               case local::val_graph::erfc_op_enum:
               {  tmp_addr = rec.PutOp(local::ErfcOp);
                  CPPAD_ASSERT_UNKNOWN( NumArg(local::ErfcOp) == 3 );
                  rec.PutArg( fun_arg[0] );
                  rec.PutArg( zero_index );
                  rec.PutArg( two_sqrt_pi_index );
               }
               break;
            }
         }
         val2fun_index[res_index] = tmp_addr;
      }
      else if( is_binary )
      {  //
         fun_ad_type[res_index] = max_ad_type;
         CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
         //
         if( max_ad_type == dynamic_enum )
         {  switch(op_enum)
            {  default:
               CPPAD_ASSERT_UNKNOWN(false);
               break;
               //
               CPPAD_VAL2FUN_DYN_BINARY(add);
               CPPAD_VAL2FUN_DYN_BINARY(sub);
               CPPAD_VAL2FUN_DYN_BINARY(mul);
               CPPAD_VAL2FUN_DYN_BINARY(div);
               CPPAD_VAL2FUN_DYN_BINARY(pow);
            }
            CPPAD_ASSERT_UNKNOWN( CppAD::isnan( parameter[tmp_addr] ) );
         }
         else if(
            ad_type_x[0] == variable_enum &&
            ad_type_x[1] == variable_enum )
         {  switch(op_enum)
            {  default:
               CPPAD_ASSERT_UNKNOWN(false);
               break;
               //
               CPPAD_VAL2FUN_VAR_BINARY(add, AddvvOp);
               CPPAD_VAL2FUN_VAR_BINARY(sub, SubvvOp);
               CPPAD_VAL2FUN_VAR_BINARY(mul, MulvvOp);
               CPPAD_VAL2FUN_VAR_BINARY(div, DivvvOp);
               CPPAD_VAL2FUN_VAR_BINARY(pow, PowvvOp);
            }
            rec.PutArg(fun_arg[0], fun_arg[1]);
         }
         else if( ad_type_x[1] == variable_enum )
         {  switch(op_enum)
            {  default:
               CPPAD_ASSERT_UNKNOWN(false);
               break;
               //
               CPPAD_VAL2FUN_VAR_BINARY(add, AddpvOp);
               CPPAD_VAL2FUN_VAR_BINARY(sub, SubpvOp);
               CPPAD_VAL2FUN_VAR_BINARY(mul, MulpvOp);
               CPPAD_VAL2FUN_VAR_BINARY(div, DivpvOp);
               CPPAD_VAL2FUN_VAR_BINARY(pow, PowpvOp);
            }
            rec.PutArg(fun_arg[0], fun_arg[1]);
         }
         else
         {  CPPAD_ASSERT_UNKNOWN( ad_type_x[0] == variable_enum );
            switch(op_enum)
            {  default:
               CPPAD_ASSERT_UNKNOWN(false);
               break;
               //
               // add
               case local::val_graph::add_op_enum:
               tmp_addr = rec.PutOp(local::AddpvOp);
               std::swap(fun_arg[0], fun_arg[1]);
               break;
               //
               // mul
               case local::val_graph::mul_op_enum:
               tmp_addr = rec.PutOp(local::MulpvOp);
               std::swap(fun_arg[0], fun_arg[1]);
               break;
               //
               CPPAD_VAL2FUN_VAR_BINARY(sub, SubvpOp);
               CPPAD_VAL2FUN_VAR_BINARY(div, DivvpOp);
               CPPAD_VAL2FUN_VAR_BINARY(pow, PowvpOp);
            }
            rec.PutArg(fun_arg[0], fun_arg[1]);
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
         // rec, fun_ad_type, val2fun_index
         case local::val_graph::dis_op_enum:
         fun_arg.resize(1);
         CPPAD_ASSERT_UNKNOWN( n_arg = 2 );
         {  addr_t dynamic_index  = val_arg_vec[arg_index + 0];
            if( max_ad_type == dynamic_enum )
            {  tmp_addr = rec.put_dyn_par(
                  nan, local::dis_dyn, dynamic_index, fun_arg[0]
               );
            }
            else
            {  CPPAD_ASSERT_UNKNOWN( max_ad_type == variable_enum );
               tmp_addr = rec.PutOp(local::DisOp);
               rec.PutArg( dynamic_index );
               rec.PutArg( fun_arg[0] );
            }
            fun_ad_type[res_index]   = max_ad_type;
            val2fun_index[res_index] = tmp_addr;
         }
         break;
         // ------------------------------------------------------------------
         // con_op
         // rec, fun_ad_type, val2fun_index
         case local::val_graph::con_op_enum:
         CPPAD_ASSERT_UNKNOWN( n_arg = 1 );
         {  par_addr                 = rec_con_index[ val_arg_vec[arg_index] ];
            fun_ad_type[res_index]   = constant_enum;
            val2fun_index[res_index] = par_addr;
# ifndef NDEBUG
            const Base& constant = val_con_vec[ val_arg_vec[arg_index] ];
            CPPAD_ASSERT_UNKNOWN(
               CppAD::isnan(constant) || parameter[par_addr] == constant
            );
# endif
         }
         break;
         // -------------------------------------------------------------------
         // pri_op: rec
         case local::val_graph::pri_op_enum:
         {  //
            // before, after, flag, value
            addr_t before = rec_str_index[ val_arg_vec[arg_index + 0] ];
            addr_t after  = rec_str_index[ val_arg_vec[arg_index + 1] ];
            addr_t flag   = fun_arg[0];
            addr_t value  = fun_arg[1];
            //
            // is_var
            // base 2 representaiton of [ is_var(flag), is_var(value) ]
            addr_t is_var = 0;
            if( fun_ad_type[flag] == variable_enum )
               is_var += 1;
            if( fun_ad_type[value] == variable_enum )
               is_var += 2;
            //
            // rec
            rec.PutOp(local::PriOp);
            rec.PutArg(is_var, flag, before, value, after);
         }
         break;
         // -------------------------------------------------------------------
         // cexp_op
         case local::val_graph::cexp_op_enum:
         {  //
            // compare, compare_enum, cop
            addr_t         compare       = val_arg_vec[arg_index + 0];
            compare_enum_t compare_enum = compare_enum_t( compare );
            CompareOp      cop;
            if( compare_enum == local::val_graph::compare_eq_enum )
               cop = CompareEq;
            else if( compare_enum == local::val_graph::compare_le_enum )
               cop = CompareLe;
            else
            {  CPPAD_ASSERT_UNKNOWN(
                  compare_enum == local::val_graph::compare_lt_enum
               );
               cop = CompareLt;
            }
            if( max_ad_type == dynamic_enum )
            {  tmp_addr = rec.put_dyn_cond_exp(
                  nan, cop, fun_arg[0], fun_arg[1], fun_arg[2], fun_arg[3]
               );
            }
            else
            {  addr_t flag = 0;
               addr_t bit  = 1;
               for(size_t j = 0; j < 4; ++j)
               {  if( ad_type_x[j] == variable_enum )
                     flag |= bit;
                  bit = 2 * bit;
               }
               CPPAD_ASSERT_UNKNOWN( flag != 0 );
               rec.PutArg(
                  cop, flag, fun_arg[0], fun_arg[1], fun_arg[2], fun_arg[3]
               );
               tmp_addr = rec.PutOp(local::CExpOp);
            }
            fun_ad_type[res_index]   = max_ad_type;
            val2fun_index[res_index] = tmp_addr;
         }
         break;
         // -------------------------------------------------------------------
         // comp_op
         // rec
         case local::val_graph::comp_op_enum:
         {  //
            // compare
            addr_t compare  = val_arg_vec[arg_index + 0];
            //
            // var_left, var_right, dyn_left, dyn_right
            bool var_left  = ad_type_x[0] == variable_enum;
            bool var_right = ad_type_x[1] == variable_enum;
            bool dyn_left  = ad_type_x[0] == dynamic_enum;
            bool dyn_right = ad_type_x[1] == dynamic_enum;
            //
            // ax
            ax.resize(2);
            //
            if( var_left | dyn_left )
               ax[0].taddr_ = fun_arg[0];
            else
               ax[0].value_ = con_x[0];
            //
            if( var_right | dyn_right )
               ax[1].taddr_ = fun_arg[1];
            else
               ax[1].value_ = con_x[1];
            //
            // res, compare_enum
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
         // csum_op
         // rec, fun_ad_type, val2fun_index
         case local::val_graph::csum_op_enum:
         if( dynamic_enum <= max_ad_type )
         {  //
            // n_add, n_sub, n_arg
            addr_t n_add =  val_arg_vec[arg_index + 0];
            addr_t n_sub =  val_arg_vec[arg_index + 1];
            CPPAD_ASSERT_UNKNOWN( n_arg == 3 + n_add + n_sub );
            //
            // sum_constant
            Base sum_constant = Base(0.0);
            for(addr_t i = 0; i < n_add; ++i)
            {  if( ad_type_x[i] <= constant_enum )
                  sum_constant += con_x[i];
            }
            for(addr_t i = 0; i < n_sub; ++i)
            {  if( ad_type_x[n_add + i] <= constant_enum )
                  sum_constant -= con_x[n_add + i];
            }
            // rec, sum_constant_addr
            addr_t sum_constant_addr = rec.put_con_par(sum_constant);
            CPPAD_ASSERT_UNKNOWN(parameter[sum_constant_addr] == sum_constant);
            //
            if( max_ad_type == dynamic_enum )
            {  tmp_addr = sum_constant_addr;
               for(addr_t i = 0; i < n_x; ++i)
               {  tmp_addr = rec.put_dyn_par(
                     nan, local::add_dyn, tmp_addr, fun_arg[i]
                  );
                  CPPAD_ASSERT_UNKNOWN( CppAD::isnan( parameter[tmp_addr] ) );
               }
            }
            else if( max_ad_type == variable_enum )
            {  //
               // n_add_var, n_add_dyn
               addr_t n_add_dyn = 0;
               addr_t n_add_var = 0;
               for(addr_t i = 0; i < n_add; ++i)
               {  if( ad_type_x[i] == dynamic_enum )
                     ++n_add_dyn;
                  if( ad_type_x[i] == variable_enum )
                     ++n_add_var;
               }
               //
               // n_add_var, n_add_dyn
               addr_t n_sub_dyn = 0;
               addr_t n_sub_var = 0;
               for(addr_t i = 0; i < n_sub; ++i)
               {  if( ad_type_x[n_add + i] == dynamic_enum )
                     ++n_sub_dyn;
                  if( ad_type_x[n_add + i] == variable_enum )
                     ++n_sub_var;
               }
               //
               // csum_arg
               addr_t n_tot = 6 + n_add_var + n_sub_var + n_add_dyn + n_sub_dyn;
               csum_arg.resize(n_tot);
               csum_arg[0] = sum_constant_addr;
               csum_arg[1] = 5 + n_add_var;
               csum_arg[2] = csum_arg[1] + n_sub_var;
               csum_arg[3] = csum_arg[2] + n_add_dyn;
               csum_arg[4] = csum_arg[3] + n_sub_dyn;
               //
               addr_t i_variable = 5;
               addr_t i_dynamic  = i_variable + n_add_var + n_sub_var;
               for(addr_t i = 0; i < n_add; ++i)
               {  if( ad_type_x[i] == dynamic_enum )
                     csum_arg[i_dynamic++]  = fun_arg[i];
                  if( ad_type_x[i] == variable_enum )
                     csum_arg[i_variable++] = fun_arg[i];
               }
               for(addr_t i = 0; i < n_sub; ++i)
               {  if( ad_type_x[n_add + i] == dynamic_enum )
                     csum_arg[i_dynamic++]  = fun_arg[n_add + i];
                  if( ad_type_x[n_add + i] == variable_enum )
                     csum_arg[i_variable++] = fun_arg[n_add + i];
               }
               CPPAD_ASSERT_UNKNOWN( i_dynamic == n_tot - 1 );
               csum_arg[i_dynamic] = i_dynamic;
               //
               // rec, tmp_addr
               tmp_addr = rec.PutOp(local::CSumOp);
               for(addr_t i = 0; i < n_tot; ++i)
                  rec.PutArg( csum_arg[i] );
               CPPAD_ASSERT_UNKNOWN( local::NumRes(local::CSumOp) == 1 );
            }
            fun_ad_type[res_index]   = max_ad_type;
            val2fun_index[res_index] = tmp_addr;
         }
         break;
         // -------------------------------------------------------------------
         // vec_op
         case local::val_graph::vec_op_enum:
         // All the VecAD objects have alread been initialized
         break;
         //
         // load_op
         case local::val_graph::load_op_enum:
         {  addr_t which_vector  = val_arg_vec[arg_index + 0];
            offset               = vecad_offset[which_vector];
            addr_t load_op_index = addr_t( rec.num_var_load_rec() );
            rec.PutArg(offset, fun_arg[0], load_op_index);
            if( ad_type_x[0] < variable_enum )
               var_addr = rec.PutLoadOp(local::LdpOp);
            else
               var_addr = rec.PutLoadOp(local::LdvOp);
            //
            // The result of a load function is always a variable
            fun_ad_type[res_index]   = variable_enum;
            val2fun_index[res_index] = var_addr;
         }
         break;
         //
         // store_op
         case local::val_graph::store_op_enum:
         {  addr_t which_vector = val_arg_vec[arg_index + 0];
            offset              = vecad_offset[which_vector];
            rec.PutArg(offset, fun_arg[0], fun_arg[1] );
            if( ad_type_x[0] < variable_enum )
            {  if( ad_type_x[1] < variable_enum )
                  rec.PutOp(local::StppOp);
               else
                  rec.PutOp(local::StpvOp);
            }
            else
            {  if( ad_type_x[1] < variable_enum )
                  rec.PutOp(local::StvpOp);
               else
                  rec.PutOp(local::StvvOp);
            }
         }
         break;
         //
         // -------------------------------------------------------------------
         // call_op
         // rec, fun_ad_type, val2fun_index
         case local::val_graph::call_op_enum:
         {  //
            // atomic_index, call_id
            addr_t n_res        =  val_arg_vec[arg_index + 1];
            size_t atomic_index = size_t( val_arg_vec[arg_index + 2] );
            size_t call_id      = size_t( val_arg_vec[arg_index + 3] );
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
               ax.resize(n_x);
               for(addr_t j = 0; j < n_x; ++j)
               {  addr_t val_index = val_arg_vec[arg_index + n_before + j];
                  ax[j].taddr_   = val2fun_index[val_index];
                  ax[j].value_   = val_index2con[val_index];
               }
               // ay, ad_type_y
               ay.resize(n_res);
               for(addr_t j = 0; j < n_res; ++j)
               {  ay[j].taddr_     = 0; // not used
                  ay[j].value_     = val_index2con[res_index + j];
                  if( use_val.size() != 0 )
                  {  if( ! use_val[res_index + j] )
                     {  ay[j].value_  = nan;
                        if( ad_type_y[j] == variable_enum )
                           ad_type_y[j] = constant_enum;
                     }
                  }
               }
            }
            if( record_dynamic ) rec.put_dyn_atomic(
               tape_id, atomic_index, call_id, ad_type_x, ad_type_y, ax, ay
            );
            if( record_variable ) rec.put_var_atomic(
               tape_id, atomic_index, call_id, ad_type_x, ad_type_y, ax, ay
            );
            //
            // val2fun_index, fun_ad_type
            for(addr_t i = 0; i < n_res; ++i)
            {  val2fun_index[res_index + i] = ay[i].taddr_;
               fun_ad_type[res_index + i]   = ad_type_y[i];
            }
         }
         break;
      }
   }
   // dep_taddr_, dep_parameter, rec
   // dep_taddr_ is address of the dependent variables on variable tape
   dep_taddr_.resize( val_dep_vec.size() );
   dep_parameter_.resize( val_dep_vec.size() );
   for(size_t i = 0; i < val_dep_vec.size(); ++i)
   {  addr_t val_index     = val_dep_vec[i];
      ad_type_enum ad_type = fun_ad_type[val_index];
      addr_t fun_index     = val2fun_index[val_index];
      dep_parameter_[i]    = ad_type < variable_enum;
      if( dep_parameter_[i] )
      {  // see RecordParOp(const AD<Base>& y)
         rec.PutArg( fun_index );
         fun_index = rec.PutOp(local::ParOp);
      }
      dep_taddr_[i] = size_t( fun_index );
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

# undef CPPAD_VAL2FUN_DYN_UNARY
# undef CPPAD_VAL2FUN_DYN_BINARY
# undef CPPAD_VAL2FUN_VAR_UNARY
# undef CPPAD_VAL2FUN_VAR_BINARY

} // END_CPPAD_NAMESPACE
// --------------------------------------------------------------------------
# endif
