# ifndef CPPAD_LOCAL_PLAY_DYN_PLAYER_HPP
# define CPPAD_LOCAL_PLAY_DYN_PLAYER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/pod_vector.hpp>


// BEGIN_CPPAD_LOCAL_NAMESPACE
namespace CppAD { namespace local {
template <class Base> class dyn_player {
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
   // =================================================================
   // set all scalars to zero to avoid valgraind warning when ani assignment
   // occures before values get set.
   // dyn_player<Base> dyn_record
   dyn_player(void)
   : n_dyn_independent_(0)
   { }
   // move semantics constructor
   // (none of the default constructor values matter to the destructor)
   dyn_player(dyn_player& dyn_play)
   {  swap(dyn_play);  }
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
   void check_dynamic_dag(void) const
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
   // operator=
   void operator=(dyn_player&& dyn_play)
   {  swap(dyn_play); }
   //
   // operator=
   void operator=(const dyn_player& dyn_play)
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
   // base2ad
   // Create a dyn_player< AD<Base> > from this dyn_player<Base>
   dyn_player< AD<Base> > base2ad(void) const
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
   //
   // swap
   // (used for move semantics version of ADFun assignment operation)
   void swap(dyn_player& other)
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
   // get_recording
   void get_recording(dyn_recorder<Base>& dyn_rec)
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
   // par_all
   pod_vector_maybe<Base>& par_all(void)
   {  return par_all_; }
   const pod_vector_maybe<Base>& par_all(void) const
   {  return par_all_; }
   //
   // par_is_dyn
   const pod_vector<bool>& par_is_dyn(void) const
   {  return par_is_dyn_; }
   //
   // dyn2par_index_
   const pod_vector<addr_t>& dyn2par_index(void) const
   {  return dyn2par_index_; }
   //
   // dyn_par_op_
   const pod_vector<opcode_t>& dyn_par_op(void) const
   {  return dyn_par_op_; }
   //
   // dyn_par_arg
   const pod_vector<addr_t>& dyn_par_arg(void) const
   {  return dyn_par_arg_; }
   //
   // GetPar
   Base GetPar(size_t i) const
   {  return par_all_[i]; }
   //
   // GetPar
   const Base* GetPar(void) const
   {  return par_all_.data(); }
   //
   // n_dyn_independent
   size_t n_dyn_independent(void) const
   {  return n_dyn_independent_; }
   //
   // num_dynamic_par
   size_t num_dynamic_par(void) const
   {  return dyn_par_op_.size(); }
   //
   // num_dynamic_arg
   size_t num_dynamic_arg(void) const
   {  return dyn_par_arg_.size(); }
   //
   // num_par_rec
   size_t num_par_rec(void) const
   {  return par_all_.size(); }
   //
   // size_op_seq
   // A measure of amount of memory used to store
   /// the operation sequence, just lengths, not capacities.
   /// In user api as f.size_op_seq(); see the file fun_property.omh.
   size_t size_op_seq(void) const
   {  return 0
         + par_all_.size()   * sizeof(Base)
         + par_is_dyn_.size()    * sizeof(bool)
         + dyn2par_index_.size() * sizeof(addr_t)
         + dyn_par_op_.size()    * sizeof(opcode_t)
         + dyn_par_arg_.size()   * sizeof(addr_t)
      ;
   }
};

} } // END_CPPAD_lOCAL_NAMESPACE
# endif
