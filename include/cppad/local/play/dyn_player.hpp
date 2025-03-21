# ifndef CPPAD_LOCAL_PLAY_DYN_PLAYER_HPP
# define CPPAD_LOCAL_PLAY_DYN_PLAYER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/pod_vector.hpp>

/*
------------------------------------------------------------------------------
{xrst_begin dyn_player dev}

Class That Records a Dynamic Parameter Operation Sequence
#########################################################

dyn_play
********
We use dyn_play to denote a dynamic player create with one of these
constructors.
{xrst_literal ,
   // BEGIN_CLASS , // END_CLASS
   // BEGIN_DYN_PLAY , // END_DYN_PLAY
}


check_dynamic_dag
*****************
This checks that the dynamic parameter operation sequence is an acyclic graph.
{xrst_literal
   // BEGIN_CHECK_DYNAMIC_DAG
   // END_CHECK_DYNAMIC_DAG
}

Assignment
**********
{xrst_literal ,
   // BEGIN_MOVE_ASSIGNMENT , // END_MOVE_ASSIGNMENT
   // BEGIN_COPY_ASSIGNMENT , // END_COPY_ASSIGNMENT
}

swap
****
This switches the contents of the two players.
{xrst_literal
   // BEGIN_SWAP
   // END_SWAP
}

base2ad
*******
This returns a AD<Base> player corresponding with the same operation sequence.
{xrst_literal
   // BEGIN_BASE2AD
   // END_BASE2AD
}

get_recording
*************
This transfers a dynamic parameter recording to a dynamic parameter player.
{xrst_literal
   // BEGIN_GET_RECORDING
   // END_GET_RECORDING
}

dyn_rec
=======
The contents of *dyn_rec* are modified in an unspecified way by this operation.

par_all
*******
This vector holds all the parameter values (constant and dynamic parameters).
{xrst_literal
   // BEGIN_PAR_ALL
   // END_PAR_ALL
}

par_is_dyn
**********
This vector identifies which parameters are dynamic.
{xrst_literal
   // BEGIN_PAR_IS_DYN
   // END_PAR_IS_DYN
}

dyn2par_index
*************
This vector maps the dynamic parameter index to the
corresponding index in the vector of all the parameters.
{xrst_literal
   // BEGIN_DYN2PAR_INDEX
   // END_DYN2PAR_INDEX
}

par_ptr
*******
{xrst_literal
   // BEGIN_PAR_PTR
   // END_PAR_PTR
}

par_one
*******
{xrst_literal
   // BEGIN_PAR_ONE
   // END_PAR_ONE
}

par_value
=========
is the value of one of the parameters.

par_ptr
=======
is a raw pointer to all of the parameters

n_dyn_independent
*****************
is the number of independent dynamic parameters
{xrst_literal
   // BEGIN_N_DYN_INDEPENDENT
   // END_N_DYN_INDEPENDENT
}

num_dynamic_par
***************
is the number of dynamic parameters
{xrst_literal
   // BEGIN_NUM_DYNAMIC_PAR
   // END_NUM_DYNAMIC_PAR
}

num_dynamic_arg
***************
is the length of the dynamic parameter argument vector
{xrst_literal
   // BEGIN_NUM_DYNAMIC_ARG
   // END_NUM_DYNAMIC_ARG
}

size_op_seq
***********
This is a measure of how may bytes are needed to store the operation sequence.
Just lengths (not capacities) are used for this computation.
This used to compute :ref:`fun_property@size_op_seq` for an ADFun object.
{xrst_literal
   // BEGIN_SIZE_OP_SEQ
   // END_SIZE_OP_SEQ
}

{xrst_end dyn_player}
*/

// BEGIN_CPPAD_LOCAL_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local {
template <class Base> class dyn_player {
   // END_CLASS
   //
   // friend
   template <class AnotherBase> friend class dyn_player;
private:
   //
   // n_dyn_independent_
   // Number of dynamic parameters in the recording
   size_t n_dyn_independent_;
   //
   // par_all_;
   // Vector containing all the parameters in the recording.
   // Use pod_vector_maybe because Base may not be plain old data.
   pod_vector_maybe<Base> par_all_;
   //
   // par_is_dyn_
   // Which elements of par_all_ are dynamic parameters
   // (same size are par_all_)
   pod_vector<bool> par_is_dyn_;
   //
   // dyn2par_index_
   // mapping from dynamic parameter index to parameter index
   // dyn2par_index_.size() equal to number of dynamic parameters
   // dyn2par_index_[j] < dyn2par_index_[j+1]
   pod_vector<addr_t> dyn2par_index_;
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
   // set all scalars to zero to avoid valgraind warning when ani assignment
   // occures before values get set.
   // BEGIN_DYN_PLAY
   // dyn_player<Base> dyn_play
   // dyn_player<Base> dyn_play(play)
   dyn_player(void)
   : n_dyn_independent_(0)
   { }
   dyn_player(dyn_player& dyn_play)
   {  swap(dyn_play);  }
   // END_DYN_PLAY
   //
   // destructor
   ~dyn_player(void)
   { }
   //
   // check_dynamic_dag
   // Check dynamic parameter graph to make sure arguments have value less
   // than or equal to the previously created dynamic parameter.
   // This is the directed acyclic graph condition (DAG).
# ifdef NDEBUG
   void check_dynamic_dag(void) const
   {  return; }
# else
   // BEGIN_CHECK_DYNAMIC_DAG
   void check_dynamic_dag(void) const
   // END_CHECK_DYNAMIC_DAG
   {  // number of dynamic parameters
      size_t num_dyn = dyn_par_op_.size();
      //
      size_t i_arg = 0; // initialize dynamic parameter argument index
      for(size_t i_dyn = 0; i_dyn < num_dyn; ++i_dyn)
      {  // i_par is parameter index
         addr_t i_par = dyn2par_index_[i_dyn];
         CPPAD_ASSERT_UNKNOWN( par_is_dyn_[i_par] );
         //
         // operator for this dynamic parameter
         op_code_dyn op = op_code_dyn( dyn_par_op_[i_dyn] );
         //
         // number of arguments for this dynamic parameter
         size_t n_arg       = num_arg_dyn(op);
         if( op == atom_dyn )
         {  size_t n = size_t( dyn_par_arg_[i_arg + 2] );
            size_t m = size_t( dyn_par_arg_[i_arg + 3] );
            n_arg    = 6 + n + m;
            CPPAD_ASSERT_UNKNOWN(
               n_arg == size_t( dyn_par_arg_[i_arg + 5 + n + m] )
            );
            for(size_t i = 5; i < n - 1; ++i)
               CPPAD_ASSERT_UNKNOWN( dyn_par_arg_[i_arg + i] <  i_par );
# ifndef NDEBUG
            for(size_t i = 5+n; i < 5+n+m; ++i)
            {  addr_t j_par = dyn_par_arg_[i_arg + i];
               CPPAD_ASSERT_UNKNOWN( (j_par == 0) || (j_par >= i_par) );
            }
# endif
         }
         else
         {  size_t num_non_par = num_non_par_arg_dyn(op);
            for(size_t i = num_non_par; i < n_arg; ++i)
               CPPAD_ASSERT_UNKNOWN( dyn_par_arg_[i_arg + i] < i_par);
         }
         //
         // next dynamic parameter
         i_arg += n_arg;
      }
      return;
   }
# endif
   // BEGIN_MOVE_ASSIGNMENT
   // dyn_play_1 = dyn_play_2
   void operator=(dyn_player&& dyn_play)
   // END_MOVE_ASSIGNMENT
   {  swap(dyn_play); }
   //
   // BEGIN_COPY_ASSIGNMENT
   // dyn_play_1 = dyn_play_2
   void operator=(const dyn_player& dyn_play)
   // END_COPY_ASSIGNMENT
   {
      // size_t objects
      n_dyn_independent_  = dyn_play.n_dyn_independent_;
      //
      // pod_vectors
      par_is_dyn_         = dyn_play.par_is_dyn_;
      dyn2par_index_      = dyn_play.dyn2par_index_;
      dyn_par_op_         = dyn_play.dyn_par_op_;
      dyn_par_arg_        = dyn_play.dyn_par_arg_;
      //
      // pod_maybe_vectors
      par_all_            = dyn_play.par_all_;
   }
   //
   // BEGIN_BASE2AD
   // ad_dyn_play = dyn_play
   dyn_player< AD<Base> > base2ad(void) const
   // END_BASE2AD
   {  //
      // dyn_play
      dyn_player< AD<Base> > dyn_play;
      //
      // size_t objects
      dyn_play.n_dyn_independent_  = n_dyn_independent_;
      //
      // pod_vectors
      dyn_play.par_is_dyn_         = par_is_dyn_;
      dyn_play.dyn2par_index_      = dyn2par_index_;
      dyn_play.dyn_par_op_         = dyn_par_op_;
      dyn_play.dyn_par_arg_        = dyn_par_arg_;
      //
      // pod_maybe_vector< AD<Base> > = pod_maybe_vector<Base>
      dyn_play.par_all_.resize( par_all_.size() );
      for(size_t i = 0; i < par_all_.size(); ++i)
         dyn_play.par_all_[i] = par_all_[i];
      //
      return dyn_play;
   }
   // BEGIN_SWAP
   // dyn_play_1.swap( dyn_play_2 )
   void swap(dyn_player& other)
   // END_SWAP
   {  // size_t objects
      std::swap(n_dyn_independent_,  other.n_dyn_independent_);
      //
      // pod_vectors
      par_is_dyn_.swap(         other.par_is_dyn_);
      dyn2par_index_.swap(      other.dyn2par_index_);
      dyn_par_op_.swap(         other.dyn_par_op_);
      dyn_par_arg_.swap(        other.dyn_par_arg_);
      //
      // pod_maybe_vectors
      par_all_.swap(    other.par_all_);
   }
   //
   // BEGIN_GET_RECORDING
   void get_recording(dyn_recorder<Base>& dyn_rec)
   // END_GET_RECORDING
   {  //
# ifndef NDEBUG
      size_t addr_t_max = size_t( std::numeric_limits<addr_t>::max() );
      CPPAD_ASSERT_UNKNOWN( dyn_rec.par_is_dyn_.size() < addr_t_max );
      CPPAD_ASSERT_UNKNOWN( dyn_rec.dyn_par_op_.size() < addr_t_max );
      CPPAD_ASSERT_UNKNOWN( dyn_rec.dyn_par_arg_.size() < addr_t_max );
      CPPAD_ASSERT_UNKNOWN( dyn_rec.par_all_.size() < addr_t_max );
# endif

      // size_t values
      n_dyn_independent_ = dyn_rec.n_dyn_independent_;
      //
      // pod_vectors
      par_is_dyn_.swap( dyn_rec.par_is_dyn_ );
      dyn_par_op_.swap( dyn_rec.dyn_par_op_ );
      dyn_par_arg_.swap( dyn_rec.dyn_par_arg_ );
      //
      // pod_maybe
      par_all_.swap( dyn_rec.par_all_ );
      //
      // dyn2par_index_
      dyn2par_index_.resize( dyn_par_op_.size() );
      size_t i_dyn = 0;
      for(size_t i_par = 0; i_par < par_all_.size(); ++i_par)
      {  if( par_is_dyn_[i_par] )
         {  dyn2par_index_[i_dyn] = addr_t( i_par );
            ++i_dyn;
         }
      }
      CPPAD_ASSERT_UNKNOWN( i_dyn == dyn2par_index_.size() );
      //
      // check_dyanmic_dag
      check_dynamic_dag();
   }
   //
   // BEGIN_PAR_ALL
   // par_all = dyn_play.par_all()
   pod_vector_maybe<Base>& par_all(void)
   // END_PAR_ALL
   {  return par_all_; }
   const pod_vector_maybe<Base>& par_all(void) const
   {  return par_all_; }
   //
   // BEGIN_PAR_IS_DYN
   // par_is_dyn = dyn_play.par_is_dyn()
   const pod_vector<bool>& par_is_dyn(void) const
   // END_PAR_IS_DYN
   {  return par_is_dyn_; }
   //
   // BEGIN_DYN2PAR_INDEX
   // dyn2par_index = dyn_play.dyn2par_index()
   const pod_vector<addr_t>& dyn2par_index(void) const
   // END_DYN2PAR_INDEX
   {  return dyn2par_index_; }
   //
   // BEGIN_DYN_OP_ALL
   // dyn_par_op = dyn_play.dyn_par_op()
   const pod_vector<opcode_t>& dyn_par_op(void) const
   // END_DYN_OP_ALL
   {  return dyn_par_op_; }
   //
   // BEGIN_DYN_ARG_ALL
   // dyn_par_arg = dyn_play.dyn_par_arg()
   const pod_vector<addr_t>& dyn_par_arg(void) const
   {  return dyn_par_arg_; }
   // END_DYN_ARG_ALL
   //
   // BEGIN_PAR_ONE
   // par_one = dyn_play.par_one(i)
   Base par_one(size_t i) const
   // END_PAR_ONE
   {  return par_all_[i]; }
   //
   // BEGIN_PAR_PTR
   // par_ptr = dyn_play.par_ptr()
   const Base* par_ptr(void) const
   // END_PAR_PTR
   {  return par_all_.data(); }
   //
   // BEGIN_N_DYN_INDEPENDENT
   // n_dyn_independent
   size_t n_dyn_independent(void) const
   // END_N_DYN_INDEPENDENT
   {  return n_dyn_independent_; }
   //
   // BEGIN_NUM_DYNAMIC_PAR
   // num_dynamic_par = dy_play.num_dynamic_par()
   size_t num_dynamic_par(void) const
   // END_NUM_DYNAMIC_PAR
   {  return dyn_par_op_.size(); }
   //
   // BEGIN_NUM_DYNAMIC_ARG
   // num_dynamic_arg = dyn_play.num_dyamic_arg()
   size_t num_dynamic_arg(void) const
   // END_NUM_DYNAMIC_ARG
   {  return dyn_par_arg_.size(); }
   //
   // BEGIN_NUM_PAR_REC
   // num_par_all = dyn_play.rec()
   size_t num_par_all(void) const
   // END_NUM_PAR_REC
   {  return par_all_.size(); }
   //
   // BEGIN_SIZE_OP_SEQ
   // size_op_seq = dyn_play.size_op_seq()
   size_t size_op_seq(void) const
   // END_SIZE_OP_SEQ
   {  return 0
         + par_all_.size()       * sizeof(Base)
         + par_is_dyn_.size()    * sizeof(bool)
         + dyn2par_index_.size() * sizeof(addr_t)
         + dyn_par_op_.size()    * sizeof(opcode_t)
         + dyn_par_arg_.size()   * sizeof(addr_t)
      ;
   }
};

} } // END_CPPAD_lOCAL_NAMESPACE
# endif
