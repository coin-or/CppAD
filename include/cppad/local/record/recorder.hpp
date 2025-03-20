# ifndef CPPAD_LOCAL_RECORD_RECORDER_HPP
# define CPPAD_LOCAL_RECORD_RECORDER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/core/hash_code.hpp>
# include <cppad/local/pod_vector.hpp>
# include <cppad/core/ad_type.hpp>
# include <cppad/local/record/dyn_recorder.hpp>
/*
-------------------------------------------------------------------------------
{xrst_begin_parent recorder dev}
{xrst_spell
   getters
}

Class That Records Both Variable and Dynamic Parameter Operations
#################################################################

Syntax
******
| CppAD::local::recorder *record*

record
******
{xrst_literal
   // BEGIN_CLASS
   // END_CLASS
}

Base
****
is the base type for this recording; i.e., we are recording
``AD`` < *Base* > operations.

Dynamic Parameter Operations
****************************
The :ref:`dyn_recorder-name` functions can be accessed using *record* ; e.g.,

| |tab| *record* . ``set_n_dyn_independent`` ( *n_dyn_independent* )

accesses the :ref:`dyn_recorder@set_n_dyn_independent` function.

Setters
*******

set_record_compare
==================
{xrst_literal
   // BEGIN_SET_RECORD_COMPARE
   // END_SET_RECORD_COMPARE
}

set_abort_op_index
==================
{xrst_literal
   // BEGIN_SET_ABORT_OP_INDEX
   // END_SET_ABORT_OP_INDEX
}

Getters
*******

get_record_compare
==================
{xrst_literal
   // BEGIN_GET_RECORD_COMPARE
   // END_GET_RECORD_COMPARE
}

get_abort_op_index
==================
{xrst_literal
   // BEGIN_GET_ABORT_OP_INDEX
   // END_GET_ABORT_OP_INDEX
}

num_var
=======
{xrst_literal
   // BEGIN_NUM_VAR_REC
   // END_NUM_VAR_REC
}

num_var_load_rec
================
{xrst_literal
   // BEGIN_NUM_VAR_LOAD_REC
   // END_NUM_VAR_LOAD_REC
}

num_op_rec
==========
{xrst_literal
   // BEGIN_NUM_OP_REC
   // END_NUM_OP_REC
}


Memory
******
{xrst_literal
   // BEGIN_MEMORY
   // END_MEMORY
}


Contents
********
{xrst_toc_table after
   include/cppad/local/record/put_var_vecad.hpp
   include/cppad/local/record/put_dyn_atomic.hpp
   include/cppad/local/record/put_var_atomic.hpp
   include/cppad/local/record/cond_exp.hpp
   include/cppad/local/record/comp_op.hpp
   include/cppad/local/record/dyn_recorder.hpp
}



{xrst_end recorder}
-------------------------------------------------------------------------------
*/
// BEGIN_CPPAD_LOCAL_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local {
template <class Base> class recorder {
   // END_CLASS
   //
   friend class player<Base>;
   //
private:
   //
   // dyn_record_
   dyn_recorder<Base> dyn_record_;
   //
   // record_compare
   // are comparison operators being recorded
   bool record_compare_;
   //
   // abort_op_index_
   // operator index at which to abort recording with an error
   // (do not abort when zero)
   size_t abort_op_index_;
   //
   // num_var_
       // Number of variables in the recording.
   size_t num_var_;
       //
   // num_var_load_rec_
   // Number vecad load operations (LdpOp or LdvOp) currently in recording.
   size_t num_var_load_rec_;
   //
   // var_op_
   // The operators in the recording.
   pod_vector<opcode_t> var_op_;
   //
   // var_vecad_ind_
       // The VecAD indices in the recording.
   pod_vector<addr_t> var_vecad_ind_;
   //
   // var_arg_
   // The argument indices in the recording
   pod_vector<addr_t> var_arg_;
   //
   // var_text_
   // Character strings ('\\0' terminated) in the recording.
   pod_vector<char> var_text_;
   //
public:
   //
   // Constructor
   recorder(void)
   : record_compare_(true)
   , abort_op_index_(0)
   , num_var_(0)
   , num_var_load_rec_(0)
   { }
   //
   // Destructor
   ~recorder(void)
   { }
   // ------------------------------------------------------------------------
   // Parameter Operations
   // ------------------------------------------------------------------------
   // set_n_dyn_independent
 void set_n_dyn_independent(size_t n_dyn_independent)
   {  dyn_record_.set_n_dyn_independent(n_dyn_independent); }
   //
   // n_dyn_independent
 size_t n_dyn_independent(void) const
   {  return dyn_record_.n_dyn_independent(); }
   //
   // put_dyn_atomic
   template <class VectorAD>
   void put_dyn_atomic(
      tape_id_t                   tape_id    ,
      size_t                      atom_index ,
      size_t                      call_id    ,
      const vector<ad_type_enum>& type_x     ,
      const vector<ad_type_enum>& type_y     ,
      const VectorAD&             ax         ,
      VectorAD&                   ay
   )
   {  dyn_record_.put_dyn_atomic(
         tape_id, atom_index, call_id, type_x, type_y, ax, ay
      );
   }
   //
   // par_all
       const pod_vector_maybe<Base>& par_all(void) const
   {  return dyn_record_.par_all(); }
   //
   // put_con_par
   addr_t put_con_par(const Base &par)
   {   return dyn_record_.put_con_par(par); }
   //
   addr_t put_dyn_par(const Base &par, op_code_dyn op)
   {  return dyn_record_.put_dyn_par(par, op); }
   addr_t put_dyn_par( const Base &par, op_code_dyn op, addr_t a0)
   {  return dyn_record_.put_dyn_par(par, op, a0); }
   addr_t put_dyn_par( const Base &par, op_code_dyn op, addr_t a0, addr_t a1)
   {  return dyn_record_.put_dyn_par(par, op, a0, a1); }
   //
   // put_dyn_cond_exp
   addr_t put_dyn_cond_exp(const Base &par, CompareOp cop,
      addr_t left, addr_t right, addr_t if_true, addr_t if_false
   )
   {  return dyn_record_.put_dyn_cond_exp(
         par, cop, left, right, if_true, if_false
      );
   }
   // put_dyn_arg_vec
   void put_dyn_arg_vec(const pod_vector<addr_t>& arg_vec)
   {  dyn_record_.put_dyn_arg_vec(arg_vec); }
   // ------------------------------------------------------------------------
   //
   // BEGIN_SET_RECORD_COMPARE
   // recorder.set_record_compare(record_compare)
   void set_record_compare(bool record_compare)
   // END_SET_RECORD_COMPARE
   {  record_compare_ = record_compare; }
   //
   // BEGIN_SET_ABORT_OP_INDEX
   // recorder.set_abort_op_index(abort_op_index)
   void set_abort_op_index(size_t abort_op_index)
   // END_SET_ABORT_OP_INDEX
   {  abort_op_index_ = abort_op_index; }
   //
   // BEGIN_GET_RECORD_COMPARE
   // record_compare = recorder.get_record_compare()
   bool get_record_compare(void) const
   // END_GET_RECORD_COMPARE
   {  return record_compare_; }
   //
   // BEGIN_GET_ABORT_OP_INDEX
   // abort_op_index = recorder.get_abort_op_index()
   size_t get_abort_op_index(void) const
   // END_GET_ABORT_OP_INDEX
   {  return abort_op_index_; }
   //
   // BEGIN_NUM_VAR_REC
   /// num_var = recorder.num_var()
   size_t num_var(void) const
   // END_NUM_VAR_REC
   {  return num_var_; }
   //
   // BEGIN_NUM_VAR_LOAD_REC
   // num_var_load_rec = recorder.num_var_load_rec()
   size_t num_var_load_rec(void) const
   // END_NUM_VAR_LOAD_REC
   {  return num_var_load_rec_; }
   //
   // BEGIN_NUM_OP_REC
   // num_op_rec = recorder.num_op_rec()
   size_t num_op_rec(void) const
   // END_NUM_OP_REC
   {  return  var_op_.size(); }
   //
   // BEGIN_MEMORY
   // memory = recorder.memory()
   size_t Memory(void) const
   // END_MEMORY
   {  return 0
         + dyn_record_.Memory()
         + var_op_.capacity()             * sizeof(opcode_t)
         + var_vecad_ind_.capacity()  * sizeof(addr_t)
         + var_arg_.capacity()            * sizeof(addr_t)
         + var_text_.capacity()           * sizeof(char)
      ;
   }
   //
   // PutOp
   addr_t PutOp(op_code_var op);
   //
   // PutArg
   void PutArg(addr_t a0);
   void PutArg(addr_t a0, addr_t a1);
   void PutArg(addr_t a0, addr_t a1, addr_t a2);
   void PutArg(addr_t a0, addr_t a1, addr_t a2, addr_t a3);
   void PutArg(addr_t a0, addr_t a1, addr_t a2, addr_t a3, addr_t a4);
   void PutArg(
      addr_t a0, addr_t a1, addr_t a2, addr_t a3, addr_t a4, addr_t a5
   );
   //
   // PutLoadOp
   addr_t PutLoadOp(op_code_var op);
   //
   // ReserveArg
   size_t ReserveArg(size_t n_arg);
   //
   // ReplaceArg
   void ReplaceArg(size_t i_arg, addr_t value);
   //
   // PutTxt
   addr_t PutTxt(const char *text);
   //
   // put_var_vecad_ind
   addr_t put_var_vecad_ind(addr_t vec_ind);
   //
   // put_var_vecad
   addr_t put_var_vecad(size_t length, const pod_vector<addr_t>& taddr);
   //
   // put_var_atomic
   template <class VectorAD>
   void put_var_atomic(
      tape_id_t                   tape_id    ,
      size_t                      atom_index ,
      size_t                      call_id    ,
      const vector<ad_type_enum>& type_x     ,
      const vector<ad_type_enum>& type_y     ,
      const VectorAD&             ax         ,
      VectorAD&                   ay
   );

   /// record a variable or dynamic parameter conditional expression
   void cond_exp(
      tape_id_t       tape_id     ,
      enum CompareOp  cop         ,
      AD<Base>       &result      ,
      const AD<Base> &left        ,
      const AD<Base> &right       ,
      const AD<Base> &if_true     ,
      const AD<Base> &if_false
   );

   /// record a comparison operators for varialbes or just dynamic parameters
   void comp_eq(
      bool                        var_left     ,
      bool                        var_right    ,
      bool                        dyn_left     ,
      bool                        dyn_right    ,
      const AD<Base>&             aleft        ,
      const AD<Base>&             aright       ,
      bool                        result
   );
   void comp_le(
      bool                        var_left     ,
      bool                        var_right    ,
      bool                        dyn_left     ,
      bool                        dyn_right    ,
      const AD<Base>&             aleft        ,
      const AD<Base>&             aright       ,
      bool                        result
   );
   void comp_lt(
      bool                        var_left     ,
      bool                        var_right    ,
      bool                        dyn_left     ,
      bool                        dyn_right    ,
      const AD<Base>&             aleft        ,
      const AD<Base>&             aright       ,
      bool                        result
   );

};
/*
------------------------------------------------------------------------------
{xrst_begin var_put_op dev}
{xrst_spell
   ldv
   ldp
}

Put Next Operator in the Variable Operation Sequence
####################################################

Prototype
*********
{xrst_literal
   // BEGIN_PUT_OP
   // END_PUT_OP
}

Description
***********
This sets the op code for the next operation in this recording.
This call must be followed by putting the corresponding
argument in the recording.

op
**
is the op code corresponding to the operation that is being recorded.
The LdpOp and LdvOp operators are special cases and must use the
:ref:`var_put_load_op-name` function.

var_index
*********
is the index of the primary (last) variable
corresponding to the result of this operation.
The number of variables corresponding to the operation is given by
``NumRes`` ( *op* ) .
With each call to PutOp or PutLoadOp,
*var_index* increases by the number of variables corresponding to the call.
This index starts at zero after the default constructor.

{xrst_end var_put_op}
*/
// BEGIN_PUT_OP
// var_index = record.PutOp(op)
template <class Base> addr_t recorder<Base>::PutOp(op_code_var op)
// END_PUT_OP
{  size_t i    = var_op_.extend(1);
   CPPAD_ASSERT_KNOWN(
      (abort_op_index_ == 0) || (abort_op_index_ != i),
      "Operator index equals abort_op_index in Independent"
   );
   var_op_[i]  = static_cast<opcode_t>(op);
   CPPAD_ASSERT_UNKNOWN( var_op_.size() == i + 1 );
   CPPAD_ASSERT_UNKNOWN( (op != LdpOp) && (op != LdvOp) );

   // first operator should be a BeginOp and NumRes( BeginOp ) > 0
   num_var_ += NumRes(op);
   CPPAD_ASSERT_UNKNOWN( num_var_ > 0 );

   // index of last variable corresponding to this operation
   // (if NumRes(op) > 0)
   CPPAD_ASSERT_KNOWN(
      (size_t) std::numeric_limits<addr_t>::max() >= num_var_ - 1,
      "cppad_tape_addr_type maximum value has been exceeded"
   )

   return static_cast<addr_t>( num_var_ - 1 );
}
/*
-------------------------------------------------------------------------------
{xrst_begin var_put_arg dev}
{xrst_spell
   etc
}

Put Operator Arguments in the Variable Operation Sequence
#########################################################

Syntax
******
| *recorder* . ``PutArg`` ( *a0* )
| *recorder* . ``PutArg`` ( *a0*  , *a1* )
| ...
| *recorder* . ``PutArg`` ( *a0*  , *a1* , *a2* , *a3* , *a4* , *a5* )

Description
***********
Places the values passed to ``PutArg`` at the end of the
current operation argument indices for the recording;
*a0* comes before *a1* etc.
The number of the operation argument indices starts at zero
after the default constructor.
The proper number of operation arguments
corresponding to the operation code *op* is given by
``NumArg`` ( *op* ) (except for the CSumOP and CSkipOp operators).

a0
**
is the first argument to place at the end of the operator argument vector.

a1
**
if present, is places after *a0* at the end of the operator argument vector.

a2
**
if present, is places after *a1* at the end of the operator argument vector.

a3
**
if present, is places after *a2* at the end of the operator argument vector.

a4
**
if present, is places after *a3* at the end of the operator argument vector.

a5
**
if present, is places after *a4* at the end of the operator argument vector.

Prototype
*********
{xrst_literal ,
   // BEGIN_PUT_ARG_0 , END_PUT_ARG_0
   // BEGIN_PUT_ARG_1 , END_PUT_ARG_1
   // BEGIN_PUT_ARG_2 , END_PUT_ARG_2
   // BEGIN_PUT_ARG_3 , END_PUT_ARG_3
   // BEGIN_PUT_ARG_4 , END_PUT_ARG_4
   // BEGIN_PUT_ARG_5 , END_PUT_ARG_5
}
{xrst_end var_put_arg}
*/
// BEGIN_PUT_ARG_0
template <class Base> void recorder<Base>::PutArg(addr_t a0)
// END_PUT_ARG_0
{
   size_t i      =  var_arg_.extend(1);
   var_arg_[i]   =  a0;
   CPPAD_ASSERT_UNKNOWN( var_arg_.size()    == i + 1 );
}
// BEGIN_PUT_ARG_1
template <class Base> void recorder<Base>::PutArg(addr_t a0, addr_t a1)
// END_PUT_ARG_1
{
   size_t i      =  var_arg_.extend(2);
   var_arg_[i++] =  a0;
   var_arg_[i]   =  a1;
   CPPAD_ASSERT_UNKNOWN( var_arg_.size()    == i + 1 );
}
// BEGIN_PUT_ARG_2
template <class Base> void recorder<Base>::PutArg(
   addr_t a0, addr_t a1, addr_t a2
)
// END_PUT_ARG_2
{
   size_t i      =  var_arg_.extend(3);
   var_arg_[i++] =  a0;
   var_arg_[i++] =  a1;
   var_arg_[i]   =  a2;
   CPPAD_ASSERT_UNKNOWN( var_arg_.size()    == i + 1 );
}
// BEGIN_PUT_ARG_3
template <class Base> void recorder<Base>::PutArg(
   addr_t a0, addr_t a1, addr_t a2, addr_t a3
)
// END_PUT_ARG_3
{
   size_t i      =  var_arg_.extend(4);
   var_arg_[i++] =  a0;
   var_arg_[i++] =  a1;
   var_arg_[i++] =  a2;
   var_arg_[i]   =  a3;
   CPPAD_ASSERT_UNKNOWN( var_arg_.size()    == i + 1 );

}
// BEGIN_PUT_ARG_4
template <class Base> void recorder<Base>::PutArg(
   addr_t a0, addr_t a1, addr_t a2, addr_t a3, addr_t a4
)
// END_PUT_ARG_4
{
   size_t i      =  var_arg_.extend(5);
   var_arg_[i++] =  a0;
   var_arg_[i++] =  a1;
   var_arg_[i++] =  a2;
   var_arg_[i++] =  a3;
   var_arg_[i]   =  a4;
   CPPAD_ASSERT_UNKNOWN( var_arg_.size()    == i + 1 );

}
// BEGIN_PUT_ARG_5
template <class Base> void recorder<Base>::PutArg(
   addr_t a0, addr_t a1, addr_t a2, addr_t a3, addr_t a4, addr_t a5
)
// END_PUT_ARG_5
{
   size_t i      =  var_arg_.extend(6);
   var_arg_[i++] =  a0;
   var_arg_[i++] =  a1;
   var_arg_[i++] =  a2;
   var_arg_[i++] =  a3;
   var_arg_[i++] =  a4;
   var_arg_[i]   =  a5;
   CPPAD_ASSERT_UNKNOWN( var_arg_.size()    == i + 1 );
}
/*
-------------------------------------------------------------------------------
{xrst_begin var_put_load_op dev}
{xrst_spell
   ldv
   ldp
}

Put Next LdpOp or LdvOp Operator in Operation Sequence
######################################################

Prototype
*********
{xrst_literal
   // BEGIN_PUT_LOAD_OP
   // END_PUT_LOAD_OP
}

Description
***********
This sets the op code for a load instruction.
This call must be followed by putting the corresponding
argument indices in the recording.

op
**
Is the op code corresponding to the operation that is being
recorded (which must be LdpOp or LdvOp).

var_index
*********
The return value is the index of the variable
corresponding to the result of this operation  must be one.
With each call to PutLoadOp or PutOp,
the return index increases by the number of variables corresponding
to this call to the call.
This index starts at zero after the default constructor.

num_var_load_rec
****************
The return value for ``num_var_load_rec()``
increases by one after each call to this function.

{xrst_end var_put_load_op}
*/
// BEGIN_PUT_LOAD_OP
// var_index = recorder.PutLoadOp(op)
template <class Base> addr_t recorder<Base>::PutLoadOp(op_code_var op)
// END_PUT_LOAD_OP
{  size_t i    = var_op_.extend(1);
   CPPAD_ASSERT_KNOWN(
      (abort_op_index_ == 0) || (abort_op_index_ != i),
      "This is the abort operator index specified by "
      "Independent(x, abort_op_index)."
   );
   var_op_[i]  = op;
   CPPAD_ASSERT_UNKNOWN( var_op_.size() == i + 1 );
   CPPAD_ASSERT_UNKNOWN( (op == LdpOp) || (op == LdvOp) );

   // first operator should be a BeginOp and NumRes( BeginOp ) > 0
   num_var_ += NumRes(op);
   CPPAD_ASSERT_UNKNOWN( num_var_ > 0 );

   // count this vecad load operation
   num_var_load_rec_++;

   // index of last variable corresponding to this operation
   // (if NumRes(op) > 0)
   CPPAD_ASSERT_KNOWN(
      (size_t) std::numeric_limits<addr_t>::max() >= num_var_ - 1,
      "cppad_tape_addr_type maximum value has been exceeded"
   )
   return static_cast<addr_t>( num_var_ - 1 );
}
/*
-------------------------------------------------------------------------------
{xrst_begin var_reserve_arg dev}

Reserve Space for Variable Recording Arguments, Delay Placing Values There
##########################################################################

Prototype
*********
{xrst_literal
   // BEGIN_RESERVE_ARG
   // END_RESERVE_ARG
}

n_arg
*****
number of arguments to reserve space for

arg_index
*********
is the index in the argument vector corresponding to the
first of the arguments being reserved.

{xrst_end var_reserve_arg}
*/
// BEGIN_RESERVE_ARG
// arg_index = recorder.ReserveArg(n_arg)
template <class Base> size_t recorder<Base>::ReserveArg(size_t n_arg)
// END_RESERVE_ARG
{
   size_t i      =  var_arg_.extend(n_arg);
   CPPAD_ASSERT_UNKNOWN( var_arg_.size()    == i + n_arg );
   return i;
}
/*
-------------------------------------------------------------------------------
{xrst_begin var_replace_arg dev}

Replace an Argument Value in the Variable Recording
###################################################

Prototype
*********
{xrst_literal
   // BEGIN_REPLACE_ARG
   // END_REPLACE_ARG
}

Purpose
*******
This is intended to be used to replace reserved values.

arg_index
*********
is the index, in argument vector, for the value that is replaced.

value
*****
is the new value for the argument with the specified index.

{xrst_end var_replace_arg}
*/
// BEGIN_REPLACE_ARG
// recorder.ReplaceArg(arg_index, value)
template <class Base> void recorder<Base>::ReplaceArg(
   size_t arg_index, addr_t value
)
// END_REPLACE_ARG
{  var_arg_[arg_index] =  value; }
// --------------------------------------------------------------------------
/*
{xrst_begin var_put_txt dev}

Put a Character String in the Text for This Variable Recording
##############################################################

Prototype
*********
{xrst_literal
   // BEGIN_PUT_TXT
   // END_PUT_TXT
}

text
****
is a ``\\0`` terminated character string that is to be put in the
vector of characters corresponding to this recording.
The terminator ``\\0`` is included.

txt_index
*********
is the offset with in the text vector for this recording at which
the character string starts.

{xrst_end var_put_txt}
*/
// BEGIN_PUT_TXT
// txt_index = recorder.PutTxt(text)
template <class Base> addr_t recorder<Base>::PutTxt(const char *text)
// END_PUT_TXT
{
   // determine length of the text including terminating '\0'
   size_t n = 0;
   while( text[n] != '\0' )
      n++;
   CPPAD_ASSERT_UNKNOWN( n <= 1000 );
   n++;
   CPPAD_ASSERT_UNKNOWN( text[n-1] == '\0' );

   // copy text including terminating '\0'
   size_t i = var_text_.extend(n);
   size_t j;
   for(j = 0; j < n; j++)
      var_text_[i + j] = text[j];
   CPPAD_ASSERT_UNKNOWN( var_text_.size() == i + n );

   CPPAD_ASSERT_KNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >= i,
      "cppad_tape_addr_type maximum value has been exceeded"
   );
   //
   return static_cast<addr_t>( i );
}

} } // END_CPPAD_LOCAL_NAMESPACE

// ----------------------------------------------------------------------------
// member function implementations
# include <cppad/local/record/put_var_vecad.hpp>
# include <cppad/local/record/put_var_atomic.hpp>
# include <cppad/local/record/cond_exp.hpp>
# include <cppad/local/record/comp_op.hpp>

# endif
