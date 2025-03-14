# ifndef CPPAD_LOCAL_PLAY_PLAYER_HPP
# define CPPAD_LOCAL_PLAY_PLAYER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/addr_enum.hpp>
# include <cppad/local/play/sequential_iterator.hpp>
# include <cppad/local/play/subgraph_iterator.hpp>
# include <cppad/local/play/dyn_player.hpp>
# include <cppad/local/play/random_setup.hpp>
# include <cppad/local/atom_state.hpp>
# include <cppad/local/is_pod.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file player.hpp
File used to define the player class.
*/

/*!
Class used to store and play back an operation sequence recording.

\tparam Base
These were AD< Base > operations when recorded. Operations during playback
are done using the type Base .
*/

// random_itr_info
struct random_itr_info_t {
   //
   // *_op2arg
   // index in arg_vec_ corresonding to the first argument for each operator
   pod_vector<unsigned  short> short_op2arg;
   pod_vector<addr_t>          addr_t_op2arg;
   pod_vector<size_t>          size_t_op2arg;
   /*
   *_op2var
   Index of the result variable for each operator. If the operator has
   no results, this is not defined. The invalid index num_var_rec_ is used
   when NDEBUG is not defined. If the operator has more than one result, this
   is the primary result; i.e., the last result. Auxillary results are only
   used by the current operator and not used by other operators.
   */
   pod_vector<unsigned  short> short_op2var;
   pod_vector<addr_t>          addr_t_op2var;
   pod_vector<size_t>          size_t_op2var;
   /*
   *_var2op
   Mapping from primary variable index to corresponding operator index.
   This is used to traverse sub-graphs of the operation sequence.
   This value is valid (invalid) for primary (auxiliary) variables.
   */
   pod_vector<unsigned  short> short_var2op;
   pod_vector<addr_t>          addr_t_var2op;
   pod_vector<size_t>          size_t_var2op;
   //
   // swap
   void swap( random_itr_info_t& other )
   {  //
      short_op2arg.swap( other.short_op2arg );
      short_op2var.swap( other.short_op2var );
      short_var2op.swap( other.short_var2op );
      //
      addr_t_op2arg.swap( other.addr_t_op2arg );
      addr_t_op2var.swap( other.addr_t_op2var );
      addr_t_var2op.swap( other.addr_t_var2op );
      //
      size_t_op2arg.swap( other.size_t_op2arg );
      size_t_op2var.swap( other.size_t_op2var );
      size_t_var2op.swap( other.size_t_var2op );
   }
   //
   // clear
   void clear(void)
   {  //
      short_op2arg.clear();
      short_op2var.clear();
      short_var2op.clear();
      //
      addr_t_op2arg.clear();
      addr_t_op2var.clear();
      addr_t_var2op.clear();
      //
      size_t_op2arg.clear();
      size_t_op2var.clear();
      size_t_var2op.clear();
   }
   //
   // size
   size_t size(void) const
   {  //
      CPPAD_ASSERT_UNKNOWN( short_op2arg.size()  == short_op2var.size() );
      CPPAD_ASSERT_UNKNOWN( addr_t_op2arg.size() == addr_t_op2var.size() );
      CPPAD_ASSERT_UNKNOWN( size_t_op2arg.size() == size_t_op2var.size() );
      //
      size_t short_num_arg  = short_op2arg.size();
      size_t int_num_arg    = addr_t_op2arg.size();
      size_t size_t_num_arg = size_t_op2arg.size();
      //
      size_t short_num_var  = short_var2op.size();
      size_t int_num_var    = addr_t_var2op.size();
      size_t size_t_num_var = size_t_var2op.size();
      //
      // result
      size_t result = 0;
      //
      // result
      if( short_num_arg != 0 )
      {  CPPAD_ASSERT_UNKNOWN( int_num_arg    == 0 );
         CPPAD_ASSERT_UNKNOWN( size_t_num_arg == 0 );
         //
         result = 2 * short_num_arg + short_num_var;
      }
      else
         CPPAD_ASSERT_UNKNOWN( short_num_var == 0 );
      //
      // result
      if( int_num_arg != 0 )
      {  CPPAD_ASSERT_UNKNOWN( short_num_arg  == 0 );
         CPPAD_ASSERT_UNKNOWN( size_t_num_arg == 0 );
         //
         result = 2 * int_num_arg + int_num_var;
      }
      else
         CPPAD_ASSERT_UNKNOWN( int_num_var == 0 );
      //
      // result
      if( size_t_num_arg != 0 )
      {  CPPAD_ASSERT_UNKNOWN( short_num_arg  == 0 );
         CPPAD_ASSERT_UNKNOWN( int_num_arg    == 0 );
         //
         result = 2 * size_t_num_arg + size_t_num_var;
      }
      else
         CPPAD_ASSERT_UNKNOWN( size_t_num_var == 0 );
      //
      return result;
   }
};

template <class Base>
class player {
public:
   //
   // value_t
   typedef Base value_t;
private:
   //
   // friend
   // player<Base> must be a friend of player< AD<Base> > for base2ad to work
   template <class AnotherBase> friend class player;
   // ----------------------------------------------------------------------
   //
   // dyn_play_
   dyn_player<Base> dyn_play_;
   //
   // num_var_rec_
   // Number of variables in the recording.
   size_t num_var_rec_;

   // num_var_load_rec_
   // number of vecad load opeations in the reconding
   size_t num_var_load_rec_;

   // num_var_vecad_rec_
   // Number of VecAD vectors in the recording
   size_t num_var_vecad_rec_;

   // op_vec_
   // The operators in the recording.
   pod_vector<opcode_t> op_vec_;

   // arg_vec_
   // The operation argument indices in the recording
   pod_vector<addr_t> arg_vec_;

   // text_vec_
   // Character strings ('\\0' terminated) in the recording.
   pod_vector<char> text_vec_;

   // var_vecad_ind_
       // The VecAD indices in the recording.
   pod_vector<addr_t> var_vecad_ind_;
   //
   // random_itr_info_
   // Information needed to use member functions that begin with random_
   // and for using const_subgraph_iterator.
   random_itr_info_t random_itr_info_;
   //
public:
   //
   /// default constructor
   // set all scalars to zero to avoid valgraind warning when ani assignment
   // occures before values get set.
   player(void)
   : num_var_rec_(0)
   , num_var_load_rec_(0)
   , num_var_vecad_rec_(0)
   { }
   //
   // move semantics constructor
   // (none of the default constructor values matter to the destructor)
   player(player& play)
   {  swap(play);  }
   //
   // destructor
   ~player(void)
   { }
   //
   // address_type
   // type used for addressing iterators for this player
   play::addr_enum address_type(void) const
   {
      // required
      size_t required = 0;
      required = std::max(required, num_var_rec_   );  // number variables
      required = std::max(required, op_vec_.size()  ); // number operators
      required = std::max(required, arg_vec_.size() ); // number arguments
      //
      // unsigned short
      if( required <= std::numeric_limits<unsigned short>::max() )
         return play::unsigned_short_enum;
      //
      // addr_t
      if( required <= std::numeric_limits<addr_t>::max() )
         return play::addr_t_enum;
      //
      // unsigned size_t
      CPPAD_ASSERT_UNKNOWN(
         required <= std::numeric_limits<size_t>::max()
      );
      return play::size_t_enum;
   }
   //
   // get_recording
   /*!
   Moving an operation sequence from a recorder to this player

   \param rec
   the object that was used to record the operation sequence. After this
   operation, the state of the recording is no longer defined. For example,
   the pod_vector member variables in this have been swapped with rec.

   \param n_ind
   the number of independent variables (only used for error checking
   when NDEBUG is not defined).

   \par
   Use an assert to check that the length of the following vectors is
   less than the maximum possible value for addr_t; i.e., that an index
   in these vectors can be represented using the type addr_t:
   op_vec_, var_vecad_ind_, arg_vec_, test_vec_, par_all_, text_vec_,
   dyn_par_arg_.
   */
   void get_recording(recorder<Base>& rec, size_t n_ind)
   {
# ifndef NDEBUG
      size_t addr_t_max = size_t( std::numeric_limits<addr_t>::max() );
# endif
      //
      // dyn_play_
      dyn_play_.get_recording( rec.dyn_record_ );

      // size_t values
      num_var_rec_        = rec.num_var_rec_;
      num_var_load_rec_   = rec.num_var_load_rec_;

      // op_vec_
      op_vec_.swap(rec.op_vec_);
      CPPAD_ASSERT_UNKNOWN(op_vec_.size() < addr_t_max );

      // arg_vec_
      arg_vec_.swap(rec.arg_vec_);
      CPPAD_ASSERT_UNKNOWN(arg_vec_.size()    < addr_t_max );

      // text_rec_
      text_vec_.swap(rec.text_vec_);
      CPPAD_ASSERT_UNKNOWN(text_vec_.size() < addr_t_max );

      // var_vecad_ind_
          var_vecad_ind_.swap(rec.var_vecad_ind_);
      CPPAD_ASSERT_UNKNOWN(var_vecad_ind_.size() < addr_t_max );

      // num_var_vecad_rec_
      num_var_vecad_rec_ = 0;
      {  // var_vecad_ind_ contains size of each VecAD followed by
         // the parameter indices used to inialize it.
         size_t i = 0;
         while( i < var_vecad_ind_.size() )
         {  num_var_vecad_rec_++;
            i += size_t( var_vecad_ind_[i] ) + 1;
         }
         CPPAD_ASSERT_UNKNOWN( i == var_vecad_ind_.size() );
      }

      // random access information
      clear_random();

      // some checks
      check_inv_op(n_ind);
      check_variable_dag();
   }
   //
   // check_inv_op
# ifdef NDEBUG
   void check_inv_op(size_t n_ind) const
   {  return; }
# else
   void check_inv_op(size_t n_ind) const
   {  play::const_sequential_iterator itr = begin();
      op_code_var   op;
      const addr_t* op_arg;
      size_t        var_index;
      itr.op_info(op, op_arg, var_index);
      CPPAD_ASSERT_UNKNOWN( op == BeginOp );
      size_t i_op = 0;
      while( op != EndOp )
      {  // start at second operator
         (++itr).op_info(op, op_arg, var_index);
         ++i_op;
         CPPAD_ASSERT_UNKNOWN( (op == InvOp) == (i_op <= n_ind) );
      }
      return;
   }
# endif
   //
   // check_variable_dag
# ifdef NDEBUG
   void check_variable_dag(void) const
   {  return; }
# else
   void check_variable_dag(void) const
   {  play::const_sequential_iterator itr = begin();
      op_code_var   op;
      const addr_t* op_arg;
      size_t        var_index;
      itr.op_info(op, op_arg, var_index);
      CPPAD_ASSERT_UNKNOWN( op == BeginOp );
      //
      addr_t arg_var_bound = 0;
      while( op != EndOp )
      {  (++itr).op_info(op, op_arg, var_index);
         switch(op)
         {
            // cases where nothing to do
            case BeginOp:
            case EndOp:
            case EqppOp:
            case InvOp:
            case LdpOp:
            case LeppOp:
            case LtppOp:
            case NeppOp:
            case ParOp:
            case AFunOp:
            case FunapOp:
            case FunrpOp:
            case FunrvOp:
            case StppOp:
            break;

            // only first argument is a variable
            case AbsOp:
            case AcosOp:
            case AcoshOp:
            case AsinOp:
            case AsinhOp:
            case AtanOp:
            case AtanhOp:
            case CosOp:
            case CoshOp:
            case DivvpOp:
            case ErfOp:
            case ErfcOp:
            case ExpOp:
            case Expm1Op:
            case LevpOp:
            case LogOp:
            case Log1pOp:
            case LtvpOp:
            case NegOp:
            case PowvpOp:
            case SignOp:
            case SinOp:
            case SinhOp:
            case SqrtOp:
            case SubvpOp:
            case TanOp:
            case TanhOp:
            case FunavOp:
            case ZmulvpOp:
            CPPAD_ASSERT_UNKNOWN(op_arg[0] <= arg_var_bound );
            break;

            // only second argument is a variable
            case AddpvOp:
            case DisOp:
            case DivpvOp:
            case EqpvOp:
            case LdvOp:
            case LepvOp:
            case LtpvOp:
            case MulpvOp:
            case NepvOp:
            case PowpvOp:
            case StvpOp:
            case SubpvOp:
            case ZmulpvOp:
            CPPAD_ASSERT_UNKNOWN(op_arg[1] <= arg_var_bound );
            break;

            // only first and second arguments are variables
            case AddvvOp:
            case DivvvOp:
            case EqvvOp:
            case LevvOp:
            case LtvvOp:
            case MulvvOp:
            case NevvOp:
            case PowvvOp:
            case SubvvOp:
            case ZmulvvOp:
            CPPAD_ASSERT_UNKNOWN(op_arg[0] <= arg_var_bound );
            CPPAD_ASSERT_UNKNOWN(op_arg[1] <= arg_var_bound );
            break;

            // StpvOp
            case StpvOp:
            CPPAD_ASSERT_UNKNOWN(op_arg[2] <= arg_var_bound );
            break;

            // StvvOp
            case StvvOp:
            CPPAD_ASSERT_UNKNOWN(op_arg[1] <= arg_var_bound );
            CPPAD_ASSERT_UNKNOWN(op_arg[2] <= arg_var_bound );
            break;

            // CSumOp
            case CSumOp:
            {  CPPAD_ASSERT_UNKNOWN( 5 < op_arg[2] );
               for(addr_t j = 5; j < op_arg[2]; j++)
                  CPPAD_ASSERT_UNKNOWN(op_arg[j] <= arg_var_bound);
            }
            itr.correct_before_increment();
            break;

            // CExpOp
            case CExpOp:
            if( op_arg[1] & 1 )
               CPPAD_ASSERT_UNKNOWN( op_arg[2] <= arg_var_bound);
            if( op_arg[1] & 2 )
               CPPAD_ASSERT_UNKNOWN( op_arg[3] <= arg_var_bound);
            if( op_arg[1] & 4 )
               CPPAD_ASSERT_UNKNOWN( op_arg[4] <= arg_var_bound);
            if( op_arg[1] & 8 )
               CPPAD_ASSERT_UNKNOWN( op_arg[5] <= arg_var_bound);
            break;

            // PriOp
            case PriOp:
            if( op_arg[0] & 1 )
               CPPAD_ASSERT_UNKNOWN( op_arg[1] <= arg_var_bound);
            if( op_arg[0] & 2 )
               CPPAD_ASSERT_UNKNOWN( op_arg[3] <= arg_var_bound);
            break;

            // CSkipOp
            case CSkipOp:
            if( op_arg[1] & 1 )
               CPPAD_ASSERT_UNKNOWN( op_arg[2] <= arg_var_bound);
            if( op_arg[1] & 2 )
               CPPAD_ASSERT_UNKNOWN( op_arg[3] <= arg_var_bound);
            itr.correct_before_increment();
            break;

            default:
            CPPAD_ASSERT_UNKNOWN(false);
            break;


         }
         if( NumRes(op) > 0 )
         {  if( var_index > 0 )
            {  CPPAD_ASSERT_UNKNOWN(size_t(arg_var_bound) < var_index);
            }
            else
            {  CPPAD_ASSERT_UNKNOWN(size_t(arg_var_bound) == var_index);
            }
            //
            arg_var_bound = addr_t(var_index);
         }
      }
      return;
   }
# endif
   //
   // operator=
   // move semantics assignment
   void operator=(player&& play)
   {  swap(play); }
   //
   // operator=
   void operator=(const player& play)
   {
      //
      // dyn_play_
      dyn_play_           = play.dyn_play_;
      //
      // size_t objects
      num_var_rec_        = play.num_var_rec_;
      num_var_load_rec_   = play.num_var_load_rec_;
      num_var_vecad_rec_  = play.num_var_vecad_rec_;
      //
      // pod_vectors
      op_vec_             = play.op_vec_;
      arg_vec_            = play.arg_vec_;
      text_vec_           = play.text_vec_;
      var_vecad_ind_      = play.var_vecad_ind_;
      //
      // random_itr_info_
      random_itr_info_    = play.random_itr_info_;
   }
   //
   // base2ad
   // Create a player< AD<Base> > from this player<Base>
   player< AD<Base> > base2ad(void) const
   {  player< AD<Base> > play;
      //
      // dyn_play_
      play.dyn_play_            = dyn_play_.base2ad();
      //
      // size_t objects
      play.num_var_rec_        = num_var_rec_;
      play.num_var_load_rec_   = num_var_load_rec_;
      play.num_var_vecad_rec_  = num_var_vecad_rec_;
      //
      // pod_vectors
      play.op_vec_             = op_vec_;
      play.arg_vec_            = arg_vec_;
      play.text_vec_           = text_vec_;
      play.var_vecad_ind_      = var_vecad_ind_;
      //
      // random_itr_info_
      play.random_itr_info_    = random_itr_info_;
      //
      return play;
   }
   //
   // swap
   /// used for move semantics version of ADFun assignment operation
   void swap(player& other)
   {  //
      // dyn_play_
      dyn_play_.swap( other.dyn_play_ );
      //
      // size_t objects
      std::swap(num_var_rec_,        other.num_var_rec_);
      std::swap(num_var_load_rec_,   other.num_var_load_rec_);
      std::swap(num_var_vecad_rec_,  other.num_var_vecad_rec_);
      //
      // pod_vectors
      op_vec_.swap(             other.op_vec_);
      arg_vec_.swap(            other.arg_vec_);
      text_vec_.swap(           other.text_vec_);
      var_vecad_ind_.swap(  other.var_vecad_ind_);
      //
      // random_itr_info_
      random_itr_info_.swap(    other.random_itr_info_);
   }
   //
   // setup_random
   /// Enable use of const_subgraph_iterator and member functions that begin
   // with random_(no work if already setup).
   void setup_random(unsigned short& not_used)
   {  play::random_setup(
         num_var_rec_                        ,
         op_vec_                             ,
         arg_vec_                            ,
         &random_itr_info_.short_op2arg      ,
         &random_itr_info_.short_op2var      ,
         &random_itr_info_.short_var2op
      );
   }
   void setup_random(addr_t& not_used)
   {  play::random_setup(
         num_var_rec_                        ,
         op_vec_                             ,
         arg_vec_                            ,
         &random_itr_info_.addr_t_op2arg     ,
         &random_itr_info_.addr_t_op2var     ,
         &random_itr_info_.addr_t_var2op
      );
   }
# if ! CPPAD_IS_SAME_TAPE_ADDR_TYPE_SIZE_T
   void setup_random(size_t& not_used)
   {  play::random_setup(
         num_var_rec_                        ,
         op_vec_                             ,
         arg_vec_                            ,
         &random_itr_info_.size_t_op2arg     ,
         &random_itr_info_.size_t_op2var     ,
         &random_itr_info_.size_t_var2op
      );
   }
# endif
   //
   // clear_
   /// Free memory used for functions that begin with random_
   /// and random iterators and subgraph iterators
   void clear_random(void)
   {  random_itr_info_.clear();
      CPPAD_ASSERT_UNKNOWN( random_itr_info_.size() == 0  );
   }
   //
   // par_all
       pod_vector_maybe<Base>& par_all(void)
   {  return dyn_play_.par_all(); }
   const pod_vector_maybe<Base>& par_all(void) const
   {  return dyn_play_.par_all(); }
   //
   // par_is_dyn
   const pod_vector<bool>& par_is_dyn(void) const
   {  return dyn_play_.par_is_dyn(); }
   //
   // dyn2par_index
   const pod_vector<addr_t>& dyn2par_index(void) const
   {  return dyn_play_.dyn2par_index(); }
   //
   // dyn_par_op
   const pod_vector<opcode_t>& dyn_par_op(void) const
   {  return dyn_play_.dyn_par_op(); }
   //
   // dyn_par_arg
   const pod_vector<addr_t>& dyn_par_arg(void) const
   {  return dyn_play_.dyn_par_arg(); }
   //
   // GetOp
   /*!
   \brief
   fetch an operator from the recording.

   \return
   the i-th operator in the recording.

   \param i
   the index of the operator in recording
   */
   op_code_var GetOp (size_t i) const
   {  return op_code_var(op_vec_[i]); }
   //
   // GetVecInd
   /*!
   \brief
   Fetch a VecAD index from the recording.

   \return
   the i-th VecAD index in the recording.

   \param i
   the index of the VecAD index in recording
   */
   size_t GetVecInd (size_t i) const
   {  return size_t( var_vecad_ind_[i] ); }
   //
   // par_one
   Base par_one(size_t i) const
   {  return dyn_play_.par_one(i);  }
   //
   // par_ptr
   const Base* par_ptr(void) const
   {  return dyn_play_.par_ptr(); }
   //
   // GetTxt
   /*!
   \brief
   Fetch a '\\0' terminated string from the recording.

   \return
   the beginning of the string.

   \param i
   the index where the string begins.
   */
   const char *GetTxt(size_t i) const
   {  CPPAD_ASSERT_UNKNOWN(i < text_vec_.size() );
      return text_vec_.data() + i;
   }
   //
   // n_dyn_independent
   size_t n_dyn_independent(void) const
   {  return dyn_play_.n_dyn_independent(); }
   //
   // num_dynamic_par
   size_t num_dynamic_par(void) const
   {  return dyn_play_.num_dynamic_par(); }
   //
   // num_dynamic_arg
   size_t num_dynamic_arg(void) const
   {  return dyn_play_.num_dynamic_arg(); }
   //
   // num_var_rec
   size_t num_var_rec(void) const
   {  return num_var_rec_; }
   //
   // num_var_load_rec
   size_t num_var_load_rec(void) const
   {  return num_var_load_rec_; }
   //
   // num_op_rec
   size_t num_op_rec(void) const
   {  return op_vec_.size(); }
   //
   // all_var_rec_ind_rec
   size_t num_var_vecad_ind_rec(void) const
   {  return var_vecad_ind_.size(); }
   //
   // num_var_vecad_rec
   size_t num_var_vecad_rec(void) const
   {  return num_var_vecad_rec_; }
   //
   // num_op_arg_rec
   size_t num_op_arg_rec(void) const
   {  return arg_vec_.size(); }
   //
   // num_par_rec
   size_t num_par_rec(void) const
   {  return dyn_play_.num_par_rec(); }
   //
   // num_text_rec
   size_t num_text_rec(void) const
   {  return text_vec_.size(); }
   //
   // size_op_seq
   // A measure of amount of memory used to store
   // the operation sequence, just lengths, not capacities.
   // In user api as f.size_op_seq(); see the file fun_property.omh.
   size_t size_op_seq(void) const
   {  return 0
         + dyn_play_.size_op_seq()
         + op_vec_.size()            * sizeof(opcode_t)
         + arg_vec_.size()           * sizeof(addr_t)
         + text_vec_.size()          * sizeof(char)
         + var_vecad_ind_.size() * sizeof(addr_t)
      ;
   }
   // size_random
   // A measure of amount of memory used for random access routine
   // In user api as f.size_random(); see the file fun_property.omh.
   size_t size_random(void) const
   {  return random_itr_info_.size(); }
   //
   // begin
   /// const sequential iterator begin
   play::const_sequential_iterator begin(void) const
   {  size_t op_index = 0;
      size_t num_var  = num_var_rec_;
      return play::const_sequential_iterator(
         num_var, &op_vec_, &arg_vec_, op_index
      );
   }
   //
   // end
   play::const_sequential_iterator end(void) const
   {  size_t op_index = op_vec_.size() - 1;
      size_t num_var  = num_var_rec_;
      return play::const_sequential_iterator(
         num_var, &op_vec_, &arg_vec_, op_index
      );
   }
   //
   // begin_subgraph
   play::const_subgraph_iterator<addr_t>  begin_subgraph(
      const play::const_random_iterator<addr_t>& random_itr ,
      const pod_vector<addr_t>*                  subgraph   ) const
   {  size_t subgraph_index = 0;
      return play::const_subgraph_iterator<addr_t>(
         random_itr,
         subgraph,
         subgraph_index
      );
   }
   //
   // end_subgraph
   template <class Addr>
   play::const_subgraph_iterator<Addr>  end_subgraph(
      const play::const_random_iterator<Addr>&   random_itr ,
      const pod_vector<addr_t>*                  subgraph   ) const
   {  size_t subgraph_index = subgraph->size() - 1;
      return play::const_subgraph_iterator<Addr>(
         random_itr,
         subgraph,
         subgraph_index
      );
   }
   //
   /// const random iterator
   play::const_random_iterator<unsigned short>
   get_random(unsigned short& not_used) const
   {  return play::const_random_iterator<unsigned short>(
         op_vec_                             ,
         arg_vec_                            ,
         &random_itr_info_.short_op2arg      ,
         &random_itr_info_.short_op2var      ,
         &random_itr_info_.short_var2op
      );
   }
   play::const_random_iterator<addr_t>
   get_random(addr_t& not_used) const
   {  return play::const_random_iterator<addr_t>(
         op_vec_                             ,
         arg_vec_                            ,
         &random_itr_info_.addr_t_op2arg     ,
         &random_itr_info_.addr_t_op2var     ,
         &random_itr_info_.addr_t_var2op
      );
   }
# if ! CPPAD_IS_SAME_TAPE_ADDR_TYPE_SIZE_T
   play::const_random_iterator<size_t>
   get_random(size_t& not_used)
   {  return play::const_random_iterator<size_t>(
         op_vec_                             ,
         arg_vec_                            ,
         &random_itr_info_.size_t_op2arg     ,
         &random_itr_info_.size_t_op2var     ,
         &random_itr_info_.size_t_var2op
      );
   }
# endif
};

} } // END_CPPAD_lOCAL_NAMESPACE
# endif
