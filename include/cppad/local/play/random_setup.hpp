# ifndef CPPAD_LOCAL_PLAY_RANDOM_SETUP_HPP
# define CPPAD_LOCAL_PLAY_RANDOM_SETUP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file random_setup.hpp
*/

/*!
Set up random access to a player object.

\tparam Addr
An integer type capable of representing the largest value in the vectors
var_arg, op2arg_index, op2var_index, var2op_index.

\param num_var
num_var is the number of variables in this operation sequence.

\param var_op
The mapping
<code>op = op_code_var[ var_op[op_index] ]</code>
maps from operator index op_index to the operator op.

\param var_arg
is a vector of all the arguments for all the operators.
The mapping op2arg_index will map from operator indices
to index in this vector.

\param op2arg_index
On input, op2arg_index is either the empty vector
(or contains the proper result from a previous call to random_setup).
Upon return it maps each operator index to the index in op_var_arg of its
first argument for the operator.

\param op2var_index
On input, op2var_index is either the empty vector
(or contains the proper result from a previous call to random_setup).
Upon return it maps each operator index to the primary (last)
result for the operator. If there are no results for the operator,
the return value map value is not specified.

\param var2op_index
On input, var2op_index is either the empty vector
(or contains the proper result from a previous call to random_setup).
Upon return it maps each primary variable index to the corresponding
operator index. The value of the map is only specified for primary variable
indices.
*/
template <class Addr>
void random_setup(
   size_t                                    num_var    ,
   const pod_vector<opcode_t>&               var_op     ,
   const pod_vector<addr_t>&                 var_arg    ,
   pod_vector<Addr>*                         op2arg_index ,
   pod_vector<Addr>*                         op2var_index ,
   pod_vector<Addr>*                         var2op_index )
{
   if( op2arg_index->size() != 0 )
   {  CPPAD_ASSERT_UNKNOWN( op2arg_index->size() == var_op.size() );
      CPPAD_ASSERT_UNKNOWN( op2var_index->size() == var_op.size() );
      CPPAD_ASSERT_UNKNOWN( var2op_index->size() == num_var        );
      return;
   }
   CPPAD_ASSERT_UNKNOWN( op2var_index->size() == 0         );
   CPPAD_ASSERT_UNKNOWN( op2var_index->size() == 0         );
   CPPAD_ASSERT_UNKNOWN( var2op_index->size() == 0         );
   CPPAD_ASSERT_UNKNOWN( op_code_var( var_op[0] ) == BeginOp );
   CPPAD_ASSERT_NARG_NRES(BeginOp, 1, 1);
   //
   size_t num_op     = var_op.size();
   size_t  var_index = 0;
   size_t  arg_index = 0;
   //
   op2arg_index->resize( num_op );
   op2var_index->resize( num_op );
   var2op_index->resize( num_var  );
# ifndef NDEBUG
   // value of var2op for auxiliary variables is num_op (invalid)
   for(size_t i_var = 0; i_var < num_var; ++i_var)
      (*var2op_index)[i_var] = Addr( num_op );
   // value of op2var is num_var (invalid) when NumRes(op) = 0
   for(size_t i_op = 0; i_op < num_op; ++i_op)
      (*op2var_index)[i_op] = Addr( num_var );
# endif
   for(size_t i_op = 0; i_op < num_op; ++i_op)
   {  op_code_var op          = op_code_var( var_op[i_op] );
      //
      // index of first argument for this operator
      (*op2arg_index)[i_op]   = Addr( arg_index );
      arg_index            += NumArg(op);
      //
      // index of first result for next operator
      var_index  += NumRes(op);
      if( NumRes(op) > 0 )
      {  // index of last (primary) result for this operator
         (*op2var_index)[i_op] = Addr( var_index - 1 );
         //
         // mapping from primary variable to its operator
         (*var2op_index)[var_index - 1] = Addr( i_op );
      }
      // CSumOp
      if( op == CSumOp )
      {  CPPAD_ASSERT_UNKNOWN( NumArg(CSumOp) == 0 );
         //
         // pointer to first argument for this operator
         const addr_t* op_arg = var_arg.data() + arg_index;
         //
         // The actual number of arguments for this operator is
         // op_arg[4] + 1
         // Correct index of first argument for next operator
         arg_index += size_t(op_arg[4] + 1);
      }
      //
      // CSkip
      if( op == CSkipOp )
      {  CPPAD_ASSERT_UNKNOWN( NumArg(CSumOp) == 0 );
         //
         // pointer to first argument for this operator
         const addr_t* op_arg = var_arg.data() + arg_index;
         //
         // The actual number of arguments for this operator is
         // 7 + op_arg[4] + op_arg[5].
         // Correct index of first argument for next operator.
         arg_index += size_t(7 + op_arg[4] + op_arg[5]);
      }
   }
}

} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
