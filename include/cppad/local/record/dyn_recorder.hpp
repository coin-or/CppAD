# ifndef CPPAD_LOCAL_RECORD_DYN_RECORDER_HPP
# define CPPAD_LOCAL_RECORD_DYN_RECORDER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/core/hash_code.hpp>
# include <cppad/local/pod_vector.hpp>
# include <cppad/core/ad_type.hpp>
/*
-- ----------------------------------------------------------------------------
{xrst_begin_parent dyn_recorder dev}

Class That Records a Dynamic Parameter Operation Sequence
#########################################################

dyn_record
**********
{xrst_literal ,
   // BEGIN_CLASS , // END_CLASS
   // BEGIN_DYN_RECORD , // END_DYN_RECORD
}

set_n_dyn_independent
*********************
{xrst_literal
   // BEGIN_SET_NUM_DYNAMIC_IND
   // END_SET_NUM_DYNAMIC_IND
}

n_dyn_independent
*****************
{xrst_literal
   // BEGIN_N_DYN_INDEPENDENT
   // END_N_DYN_INDEPENDENT
}

par_all
*******
{xrst_literal
   // BEGIN_PAR_ALL
   // END_PAR_ALL
}

memory
******
{xrst_literal
   // BEGIN_MEMORY
   // END_MEMORY
}


{xrst_end dyn_recorder}
*/
// BEGIN_CPPAD_LOCAL_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local {
template <class Base> class dyn_recorder {
// END_CLASS
   //
   // friend
   friend class dyn_player<Base>;
//
private:
   //
   // n_dyn_independent_
   // Number of dynamic parameters in the recording
   size_t n_dyn_independent_;
   //
   // all_dyn_vec_ind_;
   // The VecAD indices in the recording.
   pod_vector<addr_t> dyn_vecad_ind_;
   //
   // par_hash_table_
   // Hash table to reduced number of duplicate parameters in par_all_
       pod_vector<addr_t> par_hash_table_;
   //
   // par_all_;
   // Vector containing all the parameters in the recording.
   // Use pod_vector_maybe because Base may not be plain old data.
   pod_vector_maybe<Base> par_all_;
   //
   // dyn_par_is_
   // Which elements of par_all_ are dynamic parameters
   // (same size are par_all_)
   pod_vector<bool> dyn_par_is_;
   //
   // dyn_par_op_
   // operators for just the dynamic parameters in par_all_
       pod_vector<opcode_t> dyn_par_op_;
   //
   // dyn_par_arg_
   // arguments for the dynamic parameter operators
   pod_vector<addr_t> dyn_par_arg_;
//
public:
   // BEGIN_DYN_RECORD
   // dyn_recorder<Base> dyn_record
   dyn_recorder(void)
   // END_DYN_RECORD
   : n_dyn_independent_(0)
   , par_hash_table_( CPPAD_HASH_TABLE_SIZE )
   {  //
      // par_hash_table_
      // It does not matter if unitialized hash codes match but this
      // initilaization is here to avoid valgrind warnings.
      void*  ptr   = static_cast<void*>( par_hash_table_.data() );
      int    value = 0;
      size_t num   = CPPAD_HASH_TABLE_SIZE * sizeof(addr_t);
      std::memset(ptr, value, num);
   }
   //
   // Destructor
   ~dyn_recorder(void)
   { }
   //
   // BEGIN_SET_NUM_DYNAMIC_IND
   // dyn_record.set_n_dyn_independent(n_dyn_independent)
   void set_n_dyn_independent(size_t n_dyn_independent)
   // END_SET_NUM_DYNAMIC_IND
   {  n_dyn_independent_ = n_dyn_independent; }
   //
   // BEGIN_N_DYN_INDEPENDENT
   // n_dyn_independent = dyn_record.get_num_dynamic_ind()
   size_t get_num_dynamic_ind(void) const
   // END_N_DYN_INDEPENDENT
   {  return n_dyn_independent_; }
   //
   // put_dyn_par
   addr_t put_dyn_par(
      const Base &par, op_code_dyn op
   );
   addr_t put_dyn_par(
      const Base &par, op_code_dyn op, addr_t a0
   );
   addr_t put_dyn_par(
      const Base &par, op_code_dyn op, addr_t a0, addr_t a1
   );
   //
   // put_dyn_cond_exp
   addr_t put_dyn_cond_exp(const Base &par, CompareOp cop,
      addr_t left, addr_t right, addr_t if_true, addr_t if_false
   );
   //
   // put_dyn_arg_vec
   void put_dyn_arg_vec(const pod_vector<addr_t>& arg);
   //
   // put_con_par
   addr_t put_con_par(const Base &par);
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
   );
   //
   // BEGIN_PAR_ALL
   // par_all = dyn_record.par_all()
   const pod_vector_maybe<Base>& par_all(void) const
   {  return par_all_; }
   // END_PAR_ALL
   //
   // BEGIN_MEMORY
   /// memory = dyn_record.Memory()
   size_t Memory(void) const
   // END_MEMORY
   {  return 0
         + dyn_vecad_ind_.capacity() * sizeof(addr_t)
         + par_hash_table_.capacity()    * sizeof(addr_t)
         + par_all_.capacity()       * sizeof(Base)
         + dyn_par_is_.capacity()        * sizeof(bool)
         + dyn_par_op_.capacity()        * sizeof(opcode_t)
         + dyn_par_arg_.capacity()       * sizeof(addr_t)
      ;
   }

};
/*
------------------------------------------------------------------------------
{xrst_begin put_dyn_par dev}

Put a Dynamic Parameter at End of Parameter Vector
##################################################

Syntax
******
| *par_index* = *dyn_record* . ``put_dyn_par`` ( *par* , *op* )
| *par_index* = *dyn_record* . ``put_dyn_par`` ( *par* , *op* , *a0* )
| *par_index* = *dyn_record* . ``put_dyn_par`` ( *par* , *op* , *a0* , *a1* )

par
***
is value of dynamic parameter to be placed at the end of the
parameter vector.

op
**
is the operator for this dynamic parameter.

a0
**
is the first argument for this operation.
It must be present if :ref:`num_arg_dyn@n_arg` for this
operator is greater than zero.

a1
**
is the second argument for this operation.
It must be present if :ref:`num_arg_dyn@n_arg` for this
operator is greater than one.

par_index
*********
is the index of this dynamic parameter in the vector of all parameters.

Prototype
*********
{xrst_literal ,
   // BEGIN_PUT_DYN_PAR_0 , // END_PUT_DYN_PAR_0
   // BEGIN_PUT_DYN_PAR_1 , // END_PUT_DYN_PAR_1
   // BEGIN_PUT_DYN_PAR_2 , // END_PUT_DYN_PAR_2
}

{xrst_end put_dyn_par}
*/
// BEGIN_PUT_DYN_PAR_0
template <class Base> addr_t dyn_recorder<Base>::put_dyn_par(
   const Base &par, op_code_dyn op
)
// END_PUT_DYN_PAR_0
{
   CPPAD_ASSERT_UNKNOWN(
      op == ind_dyn || op == result_dyn || op == atom_dyn
   );
   CPPAD_ASSERT_UNKNOWN( num_arg_dyn(op) == 0 );
   par_all_.push_back( par );
   dyn_par_is_.push_back(true);
   dyn_par_op_.push_back( opcode_t(op) );
   return static_cast<addr_t>( par_all_.size() - 1 );
}
// BEGIN_PUT_DYN_PAR_1
template <class Base> addr_t dyn_recorder<Base>::put_dyn_par(
   const Base &par, op_code_dyn op, addr_t a0
)
// END_PUT_DYN_PAR_1
{
   CPPAD_ASSERT_UNKNOWN( num_arg_dyn(op) == 1 );
   par_all_.push_back( par );
   dyn_par_is_.push_back(true);
   dyn_par_op_.push_back( opcode_t(op) );
   dyn_par_arg_.push_back(a0);
   return static_cast<addr_t>( par_all_.size() - 1 );
}
// BEGIN_PUT_DYN_PAR_2
template <class Base> addr_t dyn_recorder<Base>::put_dyn_par(
   const Base &par, op_code_dyn op, addr_t a0, addr_t a1
)
// END_PUT_DYN_PAR_2
{
   CPPAD_ASSERT_UNKNOWN( num_arg_dyn(op) == 2 );
   par_all_.push_back( par );
   dyn_par_is_.push_back(true);
   dyn_par_op_.push_back( opcode_t(op) );
   dyn_par_arg_.push_back(a0);
   dyn_par_arg_.push_back(a1);
   return static_cast<addr_t>( par_all_.size() - 1 );
}
/*
------------------------------------------------------------------------------
{xrst_begin put_dyn_cond_exp dev}

Put a Conditional Expression Dynamic Parameter at End of Parameter Vector
#########################################################################

Syntax
******
| *par_index* = *dyn_record* . ``put_dyn_cond_exp`` (
|     *par* , *cop* , *left* , *right* , *if_true* , *if_false*
| )

par
***
is value of dynamic parameter to be placed at the end of the
parameter vector.

cop
***
is the operator comparison operator; i.e., Lt, Le, Eq, Ge, Gt, or Ne.

left
****
is the left argument in conditional expression (which is a parameter).

right
*****
is the right argument in conditional expression (which is a parameter).

if_true
*******
is the if_true argument in conditional expression (which is a parameter).

if_false
********
is the if_false argument in conditional expression (which is a parameter).

par_index
*********
is the index of this dynamic parameter in the vector of all parameters.

Prototype
*********
{xrst_literal
   // BEGIN_PUT_DYN_COND_EXP
   // END_PUT_DYN_COND_EXP
}

{xrst_end put_dyn_cond_exp}
*/
// BEGIN_PUT_DYN_COND_EXP
template <class Base> addr_t dyn_recorder<Base>::put_dyn_cond_exp(
   const Base &par,
   CompareOp   cop,
   addr_t      left,
   addr_t      right,
   addr_t      if_true,
   addr_t      if_false
)
// END_PUT_DYN_COND_EXP
{
   CPPAD_ASSERT_UNKNOWN( num_arg_dyn(cond_exp_dyn) == 5 );
   addr_t ret = addr_t( par_all_.size() );
   par_all_.push_back( par );
   dyn_par_is_.push_back(true);
   dyn_par_op_.push_back( opcode_t(cond_exp_dyn) );
   dyn_par_arg_.push_back( addr_t(cop) );
   dyn_par_arg_.push_back(left);
   dyn_par_arg_.push_back(right);
   dyn_par_arg_.push_back(if_true);
   dyn_par_arg_.push_back(if_false);
   return ret;
}
/*
------------------------------------------------------------------------------
{xrst_begin put_dyn_arg_vec dev}

Put a Vector of Arguments at End of Dynamic Parameter Argument Vector
#####################################################################

Prototype
*********
{xrst_literal
   // BEGIN_PUT_DYN_ARG_VEC
   // END_PUT_DYN_ARG_VEC
}

arg_vec
*******
is the vector of values to be added at the end of the
dynamic parameter operator argument vector.

{xrst_end put_dyn_arg_vec}
*/
// BEGIN_PUT_DYN_ARG_VEC
// dyn_record.put_dyn_arg_vec(arg_vec)
template <class Base>
void dyn_recorder<Base>::put_dyn_arg_vec(const pod_vector<addr_t>& arg_vec)
// END_PUT_DYN_ARG_VEC
{  for(size_t i = 0; i < arg_vec.size(); ++i)
      dyn_par_arg_.push_back( arg_vec[i] );
}
/*
------------------------------------------------------------------------------
{xrst_begin put_con_par dev}

Find or Add a Constant Parameter to Current Parameter Vector
############################################################

Prototype
*********
{xrst_literal
   // BEGIN_PUT_CON_PAR
   // END_PUT_CON_PAR
}

par
***
is the parameter to be found or placed in the vector of parameters.

par_index
*********
is the index in the parameter vector corresponding to this parameter value.
This value is not necessarily placed at the end of the vector
(because values that are identically equal may be reused).


{xrst_end put_con_par}
*/
// BEGIN_PUT_CON_PAR
// par_index = dyn_record.put_con_par(par)
template <class Base> addr_t dyn_recorder<Base>::put_con_par(const Base &par)
// END_PUT_CON_PAR
{
# ifndef NDEBUG
   // index zero is used to signify that a value is not a parameter;
   // i.e., it is a variable.
   if( par_all_.size() == 0 )
      CPPAD_ASSERT_UNKNOWN( CppAD::isnan(par) );
# endif
   // ---------------------------------------------------------------------
   // check for a match with a previous parameter
   //
   // get hash code for this value
   size_t code  = static_cast<size_t>( hash_code(par) );

   // current index in par_all_ corresponding to this hash code
   size_t index = static_cast<size_t>( par_hash_table_[code] );

   // check if the old parameter matches the new one
   if( (0 < index) && (index < par_all_.size()) )
   {  if( ! dyn_par_is_[index] )
         if( IdenticalEqualCon(par_all_[index], par) )
            return static_cast<addr_t>( index );
   }
   // ---------------------------------------------------------------------
   // put paramerter in par_all_ and replace hash entry for this codee
   //
   index = par_all_.size();
   par_all_.push_back( par );
   dyn_par_is_.push_back(false);
   //
   // change the hash table for this code to point to new value
   par_hash_table_[code] = static_cast<addr_t>( index );
   //
   // return the parameter index
   CPPAD_ASSERT_KNOWN(
      static_cast<size_t>( std::numeric_limits<addr_t>::max() ) >= index,
      "cppad_tape_addr_type maximum value has been exceeded"
   )
   return static_cast<addr_t>( index );
}

} } // END_CPPAD_LOCAL_NAMESPACE

// ----------------------------------------------------------------------------
// member function implementations
# include <cppad/local/record/put_dyn_atomic.hpp>

# endif
