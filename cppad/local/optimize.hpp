/* $Id$ */
# ifndef CPPAD_OPTIMIZE_INCLUDED
# define CPPAD_OPTIMIZE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin optimize$$
$spell
	Taylor
	var
	CppAD
	cppad
$$

$section Optimize the Tape Corresponding to an ADFun object$$

$index optimize$$
$index tape, optimize$$
$index sequence, optimize operations$$
$index operations, optimize sequence$$
$index speed, optimize$$
$index memory, optimize$$

$head Syntax$$
$icode%f%.optimize()%$$


$head Purpose$$
The operation sequence corresponding to an $cref/ADFun/$$ object can
be very large and involve many operations; see the
size functions in $cref/seq_property/$$.
This enables one to reduce the number of operations
and thereby reduce the time and the memory required to
compute function and derivative values using an $code ADFun$$ object.

$head f$$
The object $icode f$$ has prototype
$icode%
	ADFun<%Base%> %f%
%$$

$head Improvements$$
You can see the reduction in number of variables in the operation sequence
by calling the function $cref/f.size_var()/seq_property/size_var/$$
before and after the optimization procedure.
Given that the optimization procedure takes time,
it may be faster to skip this optimize procedure and just compute
derivatives using the original operation sequence.

$subhead Testing$$
You can run the CppAD $cref/speed/speed_main/$$ tests and see
the corresponding changes in number of variables and execution time.
This will require that you specify the 
$cref/--with-Speed/InstallUnix/--with-Speed/$$ option when you 
configure CppAD.
In this case, you can run the speed tests with optimization 
using the command
$codep
	speed/cppad/cppad speed 123 optimize
$$ 
and without optimization using the command
$codep
	speed/cppad/cppad speed 123 
$$
Note that $code 123$$ is used for a random number seed and can be
replaced by any integer in the commands above.

$head Efficiency$$
The $code optimize$$ member function
may greatly reduce the number of variables 
in the operation sequence; see $cref/size_var/seq_property/size_var/$$.
If a $cref/zero order forward/ForwardZero/$$ calculation is done during
the construction of f, it will require more memory
and time that after the optimization procedure.
In addition, it will need to be redone.
For this reason, it is more efficient to use 
$codei%
	ADFun<%Base%> %f%;
	%f%.Dependent(%x%, %y%);
	%f%.optimize();
%$$
instead of
$codei%
	ADFun<%Base%> %f%(%x%, %y%)
	%f%.optimize();
%$$ 
See the discussion about
$cref/sequence constructors/FunConstruct/Sequence Constructor/$$.

$head Comparison Operators$$
Any comparison operators that are in the tape are removed by this operation.
Hence the return value of $cref/CompareChange/$$ will always be zero
for an optimized tape (even if $code NDEBUG$$ is not defined).

$head Checking Optimization$$
$index NDEBUG$$
If $code NDEBUG$$ is not defined
and $cref/f.size_taylor()/size_taylor/$$ is greater than zero,
a $cref/ForwardZero/$$ calculation is done using the optimized version
of $icode f$$ and the results are checked to see that they are
the same as before.
If they are not the same, the
$cref/ErrorHandler/$$ is called with a known error message
related to $icode%f%.optimize()%$$.

$head Example$$
$children%
	example/optimize.cpp
%$$
The file
$xref/optimize.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

/*!
\file optimize.hpp
Routines for optimizing a tape
*/

/*!
\def CPPAD_OPTIMIZE_TRACE
This value is either zero or one.
Zero is the normal operation value.
If it is one, a trace of the reverse sweep is printed.
This sweep determines which variables are connected to the 
dependent variables.
*/
# define CPPAD_OPTIMIZE_TRACE 0

CPPAD_BEGIN_NAMESPACE
/*!
Structure used by \c optimize to hold information about one variable.
This \c struct would be local inside of \c optimize, 
but the current C++ standard does not support local template parameters.
*/
struct optimize_variable {
	/// Operator for which this variable is the result, \c NumRes(op) > 0.
	/// Set by the reverse sweep at beginning of optimization.
	OpCode         op;       
	/// Pointer to first argument (child) for this operator.
	/// Set by the reverse sweep at beginning of optimization.
	const size_t*  arg;
	/*!
	Information about the parrents for this variable. 

	\li
	If \c parrent == 0, this variable has no parrents.
	\li
	If \c 0 < parrent < num_var, then \c parrent is the only parrent.
	\li
	If \c parrent == num_var, this variable has more than one parrent.

	Set by the reverse sweep at beginning of optimization.
	May be changed during the forward sweep when a variable is 
	optimized out and its \c parrent is set to zero.
	*/
	size_t         parrent; 
	/// Index of this variable in the optimized operation sequence.
	/// Set by the forward sweep at end of optimization.
	size_t         new_var;  
};

/*!
Check a unary operator for a complete match with a previous operator.

A complete match means that the result of the previous operator
can be used inplace of the result for this operator.

\param old_res
is the index in the old operation sequence for 
the variable corresponding to the result for this unary operator.

\param op
is the unary operator that we are checking a match for.
Assertion: NumArg(op) == 1 and NumRes(op) > 0.

\param old_arg
the value \c old_arg[0] is the argument for this unary operator
in the old operation sequence.
Assertion: old_arg[0] < old_res.

\param npar
is the number of paraemters corresponding to this operation sequence.

\param par
is a vector of length \a npar containing the parameters
for this operation sequence; i.e.,
given a parameter index \c i, the corresponding parameter value is
\a par[i].

\param hash_table_var
is a vector with size CPPAD_HASH_TABLE_SIZE
that maps a hash code to the corresponding 
variable index in the old operation sequence.
All the values in this table must be less than \c old_res.

\param tape
is a vector that maps a variable index, in the old operation sequence,
to the corresponding \c optimze_variable information.
For indices \c i less than \c old_res, 
the following must be true of \c tape[i]:

\li tape[i].op 
is the operator in the old operation sequence
corresponding to the old variable index \c i.
Assertion: NumRes(tape[i].op) > 0.

\li tape[i].arg 
For j < NumArg( tape[i].op ),
tape[i].arg[j] is the j-th the argument, in the old operation sequence,
corresponding to the old variable index \c i.
Assertion: tape[i].arg[j] < i.

\li \c tape[i].new_var
is the variable in the new operation sequence
corresponding to the old variable index \c i.
Assertion: tape[i].new_var < old_res.

\param code
The input value of \c code does not matter.
The output value of \c code is the hash code corresponding to
this operation in the new operation sequence.

\param new_arg
The input value of \c new_arg[0] does not matter.
The output value of \c new_arg[0] is the argument for this unary
operator in the new operation sequence.

\return
If the return value is zero,
no match was found.
If the return value is greater than zero,
it is the index of a new variable that can be used to replace the 
old variable.

\par Check Assertions
\li 
*/
template <class Base>
inline size_t optimize_unary_match(
	size_t                                         old_res        ,
	OpCode                                         op             ,
	const size_t*                                  old_arg        ,
	size_t                                         npar           ,
	const Base*                                    par            ,
	const CppAD::vector<size_t>&                   hash_table_var ,
	const CppAD::vector<struct optimize_variable>& tape           ,
	unsigned short&                                code           ,
	size_t*                                        new_arg        )
{	
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0  );
	CPPAD_ASSERT_UNKNOWN( old_arg[0] < old_res );
	new_arg[0] = tape[old_arg[0]].new_var;
	CPPAD_ASSERT_UNKNOWN( new_arg[0] < old_res );
	code = hash_code(
		op                  , 
		new_arg             ,
		npar                ,
		par
	);
	size_t  i               = hash_table_var[code];
	CPPAD_ASSERT_UNKNOWN( i < old_res );
	if( op == tape[i].op )
	{	size_t k = tape[i].arg[0];
		CPPAD_ASSERT_UNKNOWN( k < i );
		if (new_arg[0] == tape[k].new_var );
			return tape[i].new_var;
	}
	return 0;
} 

/*!
Convert a player object to an optimized recorder object

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param n
is the number of independent variables on the tape.

\param dep_taddr
On input this vector contains the indices for each of the dependent
variable values in the operation sequence corresponding to \a play.
Upon return it contains the indices for the same variables but in
the operation sequence corresponding to \a rec.

\param play
This is the operation sequence that we are optimizing.
It is essentially const, except for play back state which
changes while it plays back the operation seqeunce.

\param rec
The input contents of this recording does not matter.
Upon return, it contains an optimized verison of the
operation sequence corresponding to \a play.
*/

template <class Base>
void optimize(
	size_t                       n         ,
	CppAD::vector<size_t>&       dep_taddr ,
	player<Base>*                play      ,
	recorder<Base>*              rec       ) 
{
	// temporary indices
	size_t i, j, k;

	// temporary variables
	OpCode        op;   // current operator
	const size_t *arg;  // operator arguments
	size_t        i_var;  // index of first result for current operator

	// range and domain dimensions for F
	size_t m = dep_taddr.size();

	// number of variables in the player
	const size_t num_var = play->num_rec_var(); 

	// number of  VecAD indices 
	size_t num_vecad_ind   = play->num_rec_vecad_ind();

	// number of VecAD vectors
	size_t num_vecad_vec   = play->num_rec_vecad_vec();

	// -------------------------------------------------------------
	// data structure that maps variable index in original operation
	// sequence to corresponding operator information
	CppAD::vector<struct optimize_variable> tape(num_var);
	// -------------------------------------------------------------
	// Determine parrent value for each variable

	// initialize all variables has having no parrent
	for(i = 0; i < num_var; i++)
		tape[i].parrent = 0;

	for(j = 0; j < m; j++)
	{	// mark dependent variables as having multiple parents
		tape[ dep_taddr[j] ].parrent = num_var;
	}

	// vecad_parrent contains a flag for each VecAD object.
	// vecad maps a VecAD index (which corresponds to the beginning of the
	// VecAD object) to the vecad_parrent falg for the VecAD object.
# if CPPAD_OPTIMIZE_TRACE
	CppAD::vector<size_t> vecad_offset(num_vecad_vec);
# endif
	CppAD::vector<bool>   vecad_parrent(num_vecad_vec);
	CppAD::vector<size_t> vecad(num_vecad_ind);
	j = 0;
	for(i = 0; i < num_vecad_vec; i++)
	{	vecad_parrent[i] = false;
# if CPPAD_OPTIMIZE_TRACE
		// offset for this VecAD object
		vecad_offset[i]  = j + 1;
# endif
		// length of this VecAD
		size_t length = play->GetVecInd(j);
		// set to proper index for this VecAD
		vecad[j] = i; 
		for(k = 1; k <= length; k++)
			vecad[j+k] = num_vecad_vec; // invalid index
		// start of next VecAD
		j       += length + 1;
	}
	CPPAD_ASSERT_UNKNOWN( j == num_vecad_ind );

	// Initialize a reverse mode sweep through the operation sequence
	size_t i_op;
	play->start_reverse(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == EndOp );
	size_t mask;
# if CPPAD_OPTIMIZE_TRACE
	std::cout << std::endl;
# endif
	while(op != BeginOp)
	{	// next op
		play->next_reverse(op, arg, i_op, i_var);
		// This if is not necessary becasue last assignment
		// with this value of i_var will have NumRes(op) > 0
		if( NumRes(op) > 0 )
		{	tape[i_var].op = op;
			tape[i_var].arg = arg;
		}
# ifndef NDEBUG
		if( i_op <= n )
		{	CPPAD_ASSERT_UNKNOWN((op == InvOp) | (op == BeginOp));
		}
		else	CPPAD_ASSERT_UNKNOWN((op != InvOp) & (op != BeginOp));
# endif

# if CPPAD_OPTIMIZE_TRACE
		printOp(
			std::cout, 
			play,
			i_var,
			op, 
			arg,
			0, 
			(bool *) CPPAD_NULL,
			1, 
			& connected[i_var]
		);
# endif

		switch( op )
		{
			// Unary operator where operand is arg[0]
			case AbsOp:
			case AddvpOp:
			case AcosOp:
			case AsinOp:
			case AtanOp:
			case CosOp:
			case CoshOp:
			case DisOp:
			case DivvpOp:
			case ExpOp:
			case LogOp:
			case MulvpOp:
			case PowvpOp:
			case SinOp:
			case SinhOp:
			case SqrtOp:
			case SubvpOp:
			if( tape[i_var].parrent > 0 )
			{	if( tape[arg[0]].parrent == 0 )
					tape[arg[0]].parrent = i_var;
				else	tape[arg[0]].parrent = num_var;
			}
			break;

			// Unary operator where operand is arg[1]
			case AddpvOp:
			case DivpvOp:
			case MulpvOp:
			case SubpvOp:
			case PowpvOp:
			case PrivOp:
			if( tape[i_var].parrent > 0 )
			{	if( tape[arg[1]].parrent == 0 )
					tape[arg[1]].parrent = i_var;
				else	tape[arg[1]].parrent = num_var;
			}
			break;

			// Binary operator where operands are arg[0], arg[1]
			case AddvvOp:
			case DivvvOp:
			case MulvvOp:
			case PowvvOp:
			case SubvvOp:
			if( tape[i_var].parrent > 0 )
			{	if( tape[arg[0]].parrent == 0 )
					tape[arg[0]].parrent = i_var;
				else	tape[arg[0]].parrent = num_var;
				if( tape[arg[1]].parrent == 0 )
					tape[arg[1]].parrent = i_var;
				else	tape[arg[1]].parrent = num_var;
			}
			break;

			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
			if( tape[i_var].parrent > 0 )
			{	mask = 1;
				for(i = 2; i < 6; i++) if( arg[1] & mask )
				{	CPPAD_ASSERT_UNKNOWN( arg[i] < i_var );
					if( tape[arg[i]].parrent == 0 )
						tape[arg[i]].parrent = i_var;
					else	tape[arg[i]].parrent = num_var;
				}
				mask = mask << 1;
			}

			// Operations where there is noting to do
			case BeginOp:
			case ComOp:
			case EndOp:
			case InvOp:
			case ParOp:
			case PripOp:
			case StppOp:
			break; 

			// Load using a parameter index
			case LdpOp:
			if( tape[i_var].parrent > 0 )
			{	i = vecad[ arg[0] - 1 ];
				vecad_parrent[i] = true;
			}
			break;

			// Load using a variable index
			case LdvOp:
			if( tape[i_var].parrent > 0 )
			{	i = vecad[ arg[0] - 1 ];
				vecad_parrent[i] = true;
				if( tape[arg[1]].parrent == 0 )
					tape[arg[1]].parrent = i_var;
				else	tape[arg[1]].parrent = num_var;
			}
			break;

			// Store a variable using a parameter index
			case StpvOp:
			i = vecad[ arg[0] - 1 ];
			if( vecad_parrent[i] )
			{	if( tape[arg[2]].parrent == 0 )
					tape[arg[2]].parrent = i_var;
				else	tape[arg[2]].parrent = num_var;
			}
			break;

			// Store a variable using a variable index
			case StvvOp:
			i = vecad[ arg[0] - 1 ];
			if( vecad_parrent[i] )
			{	if( tape[arg[1]].parrent == 0 )
					tape[arg[1]].parrent = i_var;
				else	tape[arg[1]].parrent = num_var;
				if( tape[arg[2]].parrent == 0 )
					tape[arg[2]].parrent = i_var;
				else	tape[arg[2]].parrent = num_var;
			}
			break;

			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
	// values corresponding to BeginOp
	CPPAD_ASSERT_UNKNOWN( i_op == 0 && i_var == 0 && op == BeginOp );
	tape[i_var].op = op;
# if CPPAD_OPTIMIZE_TRACE
	std::cout << "VecAD information:" << std::endl;
	for(i = 0; i < num_vecad_vec; i++) 
	{	std::cout << "offset  = " << vecad_offset[i];
		std::cout << ", parrent = " << vecad_parrent[i] << std::endl;
	}
# endif
	// -------------------------------------------------------------

	// Erase all information in the recording
	rec->Erase();

	// Initilaize table mapping hash code to variable index in tape
	// as pointing to the BeginOp at the beginning of the tape
	CppAD::vector<size_t>  hash_table_var(CPPAD_HASH_TABLE_SIZE);
	for(i = 0; i < CPPAD_HASH_TABLE_SIZE; i++)
		hash_table_var[i] = 0;
	CPPAD_ASSERT_UNKNOWN( tape[0].op == BeginOp );

	// initialize mapping from old variable index to new variable index
	for(i = 0; i < num_var; i++)
		tape[i].new_var = num_var; // invalid index
	

	// initialize mapping from old VecAD index to new VecAD index
	CppAD::vector<size_t> new_vecad_ind(num_vecad_ind);
	for(i = 0; i < num_vecad_ind; i++)
		new_vecad_ind[i] = num_vecad_ind; // invalid index 

	j = 0;     // index into the old set of indices
	for(i = 0; i < num_vecad_vec; i++)
	{	// length of this VecAD
		size_t length = play->GetVecInd(j);
		if( vecad_parrent[i] )
		{	// Put this VecAD vector in new recording
			CPPAD_ASSERT_UNKNOWN(length < num_vecad_ind);
			new_vecad_ind[j] = rec->PutVecInd(length);
			for(k = 1; k <= length; k++) new_vecad_ind[j+k] =
				rec->PutVecInd(
					rec->PutPar(
						play->GetPar( 
							play->GetVecInd(j+k)
			) ) );
		}
		// start of next VecAD
		j       += length + 1;
	}
	CPPAD_ASSERT_UNKNOWN( j == num_vecad_ind );

	// start playing the operations in the forward direction
	play->start_forward(op, arg, i_op, i_var);

	// playing forward skips BeginOp at the beginning, but not EndOp at
	// the end.  Put BeginOp at beginning of recording
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
	CPPAD_ASSERT_NARG_NRES(BeginOp, 0, 1);
	tape[i_var].new_var = rec->PutOp(BeginOp);

	// temporary buffer for new argument values
	size_t new_arg[6];

	while(op != EndOp)
	{	// next op
		play->next_forward(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );

		// determine if we should keep this operator
		bool keep;
		switch( op )
		{	case ComOp:
			case PripOp:
			case PrivOp:
			keep = false;
			break;

			case InvOp:
			case EndOp:
			keep = true;
			break;

			case StppOp:
			case StvpOp:
			case StpvOp:
			case StvvOp:
			CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
			i = vecad[ arg[0] - 1 ];
			keep = vecad_parrent[i];
			break;

			default:
			keep = (tape[i_var].parrent > 0);
			break;
		}

		// start check if get a match in the hash table
		unsigned short code      = hash_code(
			EndOp               , 
			arg                 ,
			play->num_rec_par() ,
			play->GetPar()
		);
		bool           match     = false;
		size_t         match_var;
		size_t         hash_var;
		const size_t*  hash_arg;
		Base           hash_par;
		Base           par;
		if( keep ) switch( op )
		{
			// Unary operator where operand is arg[0]
			case AbsOp:
			case AcosOp:
			case AsinOp:
			case AtanOp:
			case CosOp:
			case CoshOp:
			case DisOp:
			case ExpOp:
			case LogOp:
			case SinOp:
			case SinhOp:
			case SqrtOp:
			match_var = optimize_unary_match(
				i_var               ,  // inputs
				op                  ,
				arg                 ,
				play->num_rec_par() ,
				play->GetPar()      ,
				hash_table_var      ,
				tape                , 
				code                , // outputs
				new_arg
			);
			if( match_var > 0 )
				tape[i_var].new_var = match_var;
			else
			{	rec->PutArg( new_arg[0] );
				tape[i_var].new_var = rec->PutOp(op);
			}
			break;
			// ---------------------------------------------------
			// Non-commutative binary operators where 
			// left is a variable and right is a parameter
			case DivvpOp:
			case PowvpOp:
			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) >  0 );
			new_arg[0] = tape[arg[0]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_var );
			new_arg[1] = arg[1];
			code      = hash_code(
				op                  , 
				new_arg             ,
				play->num_rec_par() ,
				play->GetPar()
			);
			hash_var   = hash_table_var[code];
			hash_arg   = tape[hash_var].arg;
			//
			par        = play->GetPar( arg[1] );
			if( op == tape[hash_var].op )
			{	match   = (
					new_arg[0] == tape[hash_arg[0]].new_var
				);
				hash_par= play->GetPar( hash_arg[1] );
				match  &= IdenticalEqualPar(par, hash_par); 
			}
			if( match )
				tape[i_var].new_var = tape[hash_var].new_var;
			else
			{
				new_arg[1] = rec->PutPar(par);
				rec->PutArg( new_arg[0], new_arg[1] );
				tape[i_var].new_var = rec->PutOp(op);
			}
			break;
			// ---------------------------------------------------
			// Non-commutative binary operators where 
			// left is a parameter and right is a variable
			case DivpvOp:
			case PowpvOp:
			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) >  0 );
			new_arg[1] = tape[arg[1]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			new_arg[0] = arg[0];
			code       = hash_code(
				op                  , 
				new_arg             ,
				play->num_rec_par() ,
				play->GetPar()
			);
			hash_var   = hash_table_var[code];
			hash_arg   = tape[hash_var].arg;
			//
			par        = play->GetPar( arg[0] );
			if( op == tape[hash_var].op )
			{	match   = (
					new_arg[1] == tape[hash_arg[1]].new_var
				);
				hash_par= play->GetPar( hash_arg[0] );
				match  &= IdenticalEqualPar(par, hash_par); 
			}
			if( match )
				tape[i_var].new_var = tape[hash_var].new_var;
			else
			{
				new_arg[0] = rec->PutPar(par);
				rec->PutArg( new_arg[0], new_arg[1] );
				tape[i_var].new_var = rec->PutOp(op);
			}
			break;
			// ---------------------------------------------------
			// Non-commutative binary operator where 
			// both operators are variables
			case DivvvOp:
			case PowvvOp:
			case SubvvOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) >  0 );
			new_arg[0] = tape[arg[0]].new_var;
			new_arg[1] = tape[arg[1]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_var );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			code       = hash_code(
				op                  , 
				new_arg             ,
				play->num_rec_par() ,
				play->GetPar()
			);
			hash_var   = hash_table_var[code];
			hash_arg   = tape[hash_var].arg;
			//
			if( op == tape[hash_var].op )
			{	match  = (
					new_arg[0] == tape[hash_arg[0]].new_var
				);
				match &= (
					new_arg[1] == tape[hash_arg[1]].new_var
				);
			}
			if( match )
				tape[i_var].new_var = tape[hash_var].new_var;
			else
			{	rec->PutArg( new_arg[0], new_arg[1] );
				tape[i_var].new_var = rec->PutOp(op);
			}
			break;
			// ---------------------------------------------------
			// Commutative binary operators where 
			// left is a variable and right is a parameter
			case AddvpOp:
			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) >  0 );
			new_arg[0] = tape[arg[0]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_var );
			new_arg[1] = arg[1];
			code       = hash_code(
				op                  , 
				new_arg             ,
				play->num_rec_par() ,
				play->GetPar()
			);
			hash_var   = hash_table_var[code];
			hash_arg   = tape[hash_var].arg;
			//
			par        = play->GetPar( arg[1] );
			if( op == tape[hash_var].op )
			{	match   = (
					new_arg[0] == tape[hash_arg[0]].new_var
				);
				hash_par= play->GetPar( hash_arg[1] );
				match  &= IdenticalEqualPar(par, hash_par); 
			}
			if(! match )
			{	OpCode tmp_op = AddpvOp;
				if(op == MulvpOp) 
					tmp_op = MulpvOp;
				size_t tmp_arg[2];
				tmp_arg[0] = new_arg[1];
				tmp_arg[1] = new_arg[0];
				unsigned short tmp_code = hash_code(
					tmp_op              , 
					tmp_arg             ,
					play->num_rec_par() ,
					play->GetPar()
				);
				hash_var = hash_table_var[tmp_code];
				if( tmp_op == tape[hash_var].op )
				{
				hash_arg = tape[hash_var].arg;
				match    = (
					new_arg[0] == tape[hash_arg[1]].new_var
				);
				hash_par = play->GetPar( hash_arg[0] );
				match   &= IdenticalEqualPar(par, hash_par);
				}
			}
			if( match )
				tape[i_var].new_var = tape[hash_var].new_var;
			else
			{
				new_arg[1] = rec->PutPar(par);
				rec->PutArg( new_arg[0], new_arg[1] );
				tape[i_var].new_var = rec->PutOp(op);
			}
			break;
			// ---------------------------------------------------
			// Commutative binary operators where 
			// left is a parameter and right is a variable
			case AddpvOp:
			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) >  0 );
			new_arg[1] = tape[arg[1]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			new_arg[0] = arg[0];
			code       = hash_code(
				op                  , 
				new_arg             ,
				play->num_rec_par() ,
				play->GetPar()
			);
			hash_var   = hash_table_var[code];
			hash_arg   = tape[hash_var].arg;
			//
			par        = play->GetPar( arg[0] );
			if( op == tape[hash_var].op )
			{	match   = (
					new_arg[1] == tape[hash_arg[1]].new_var
				);
				hash_par= play->GetPar( hash_arg[0] );
				match  &= IdenticalEqualPar(par, hash_par); 
			}
			if(! match )
			{	OpCode tmp_op = AddvpOp;
				if(op == MulpvOp) 
					tmp_op = MulvpOp;
				size_t tmp_arg[2];
				tmp_arg[0] = new_arg[1];
				tmp_arg[1] = new_arg[0];
				unsigned short tmp_code = hash_code( 
					tmp_op              , 
					tmp_arg             ,
					play->num_rec_par() ,
					play->GetPar()
				);
				hash_var = hash_table_var[tmp_code];
				if( tmp_op == tape[hash_var].op )
				{
				hash_arg = tape[hash_var].arg;
				match    = (
					new_arg[1] == tape[hash_arg[0]].new_var
				);
				hash_par = play->GetPar( hash_arg[1] );
				match   &= IdenticalEqualPar(par, hash_par);
				}
			}
			if( match )
				tape[i_var].new_var = tape[hash_var].new_var;
			else
			{
				new_arg[0] = rec->PutPar(par);
				rec->PutArg( new_arg[0], new_arg[1] );
				tape[i_var].new_var = rec->PutOp(op);
			}
			break;
			// ---------------------------------------------------
			// Commutative binary operator where 
			// both operators are variables
			case AddvvOp:
			case MulvvOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) >  0 );
			new_arg[0] = tape[arg[0]].new_var;
			new_arg[1] = tape[arg[1]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_var );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			code       = hash_code(
				op                  , 
				new_arg             ,
				play->num_rec_par() ,
				play->GetPar()
			);
			hash_var   = hash_table_var[code];
			hash_arg   = tape[hash_var].arg;
			//
			if( op == tape[hash_var].op )
			{	match   = (
					new_arg[0] == tape[hash_arg[0]].new_var
				);
				match  &= (
					new_arg[1] == tape[hash_arg[1]].new_var
				);
			}
			if(! match )
			{	size_t tmp_arg[2];
				tmp_arg[0] = new_arg[1];
				tmp_arg[1] = new_arg[0];
				unsigned short tmp_code = hash_code(
					op                  , 
					tmp_arg             ,
					play->num_rec_par() ,
					play->GetPar()
				);
				hash_var = hash_table_var[tmp_code];
				if( op == tape[hash_var].op )
				{
				hash_arg = tape[hash_var].arg;
				match    = (
					new_arg[0] == tape[hash_arg[1]].new_var
				);
				match   &= (
					new_arg[1] == tape[hash_arg[0]].new_var
				);
				}
			}
			if( match )
				tape[i_var].new_var = tape[hash_var].new_var;
			else
			{
				rec->PutArg( new_arg[0], new_arg[1] );
				tape[i_var].new_var = rec->PutOp(op);
			}
			break;
			// ---------------------------------------------------
			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 6, 1);
			new_arg[0] = arg[0];
			new_arg[1] = arg[1];
			mask = 1;
			for(i = 2; i < 6; i++)
			{	if( arg[1] & mask )
				{	new_arg[i] = tape[arg[i]].new_var;
					CPPAD_ASSERT_UNKNOWN( 
						new_arg[i] < num_var 
					);
				}
				else	new_arg[i] = rec->PutPar( 
						play->GetPar( arg[i] )
				);
				mask = mask << 1;
			}
			rec->PutArg(
				new_arg[0] ,
				new_arg[1] ,
				new_arg[2] ,
				new_arg[3] ,
				new_arg[4] ,
				new_arg[5] 
			);
			tape[i_var].new_var = rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Operations with no arguments and no results
			case EndOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 0);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Operations with no arguments and one result
			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			tape[i_var].new_var = rec->PutOp(op);
			break;
 			// ---------------------------------------------------
			// Operations with one argument that is a parameter
			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0] ) );

			rec->PutArg( new_arg[0] );
			tape[i_var].new_var = rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Load using a parameter index
			case LdpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = arg[1];
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_vecad_ind );
			rec->PutArg( 
				new_arg[0], 
				new_arg[1], 
				0
			);
			tape[i_var].new_var = rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Load using a variable index
			case LdvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = tape[arg[1]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			rec->PutArg( 
				new_arg[0], 
				new_arg[1], 
				0
			);
			tape[i_var].new_var = rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Store a parameter using a parameter index
			case StppOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			new_arg[2] = rec->PutPar( play->GetPar(arg[2]) );
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_vecad_ind );
			rec->PutArg(
				new_arg[0], 
				new_arg[1], 
				new_arg[2]
			);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Store a parameter using a variable index
			case StvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = tape[arg[1]].new_var;
			new_arg[2] = rec->PutPar( play->GetPar(arg[2]) );
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			rec->PutArg(
				new_arg[0], 
				new_arg[1], 
				new_arg[2]
			);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Store a variable using a parameter index
			case StpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			new_arg[2] = tape[arg[2]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			CPPAD_ASSERT_UNKNOWN( new_arg[2] < num_var );
			rec->PutArg(
				new_arg[0], 
				new_arg[1], 
				new_arg[2]
			);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Store a variable using a variable index
			case StvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = tape[arg[1]].new_var;
			new_arg[2] = tape[arg[2]].new_var;
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			CPPAD_ASSERT_UNKNOWN( new_arg[2] < num_var );
			rec->PutArg(
				new_arg[0], 
				new_arg[1], 
				new_arg[2]
			);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(false);

		}
		if( keep & (! match) & (NumRes(op) > 0) )
		{	// put most recent match for this code in hash table
			hash_table_var[code] = i_var;
		}

	}
	// modify the dependent variable vector to new indices
	for(i = 0; i < dep_taddr.size(); i++ )
	{	CPPAD_ASSERT_UNKNOWN( tape[ dep_taddr[i] ].new_var < num_var );
		dep_taddr[i] = tape[ dep_taddr[i] ].new_var;
	}
}

/*!
Optimize a player object operation sequence

The operation sequence for this object is replaced by one with fewer operations
but the same funcition and derivative values.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

*/
template <class Base>
void ADFun<Base>::optimize(void)
{	// place to store the optimized version of the recording
	recorder<Base> rec;

	// number of independent variables
	size_t n = ind_taddr_.size();

# ifndef NDEBUG
	size_t i, j, m = dep_taddr_.size();
	CppAD::vector<Base> x(n), y(m), check(m);
	bool check_zero_order = taylor_per_var_ > 0;
	if( check_zero_order )
	{	// zero order coefficients for independent vars
		for(j = 0; j < n; j++)
		{	CPPAD_ASSERT_UNKNOWN( play_.GetOp(j+1) == InvOp );
			CPPAD_ASSERT_UNKNOWN( ind_taddr_[j]    == j+1   );
			x[j] = taylor_[ ind_taddr_[j] * taylor_col_dim_ + 0];
		}
		// zero order coefficients for dependent vars
		for(i = 0; i < m; i++)
		{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < total_num_var_ );
			y[i] = taylor_[ dep_taddr_[i] * taylor_col_dim_ + 0];
		}
	}
# endif

	// create the optimized recording
	CppAD::optimize<Base>(n, dep_taddr_, &play_, &rec);

	// now replace the recording
	play_ = rec;

	// number of variables in the recording
	total_num_var_ = rec.num_rec_var();

	// free memory allocated for sparse Jacobian calculation
	// (the results are no longer valid)
	for_jac_sparse_pack_.resize(0, 0);
	for_jac_sparse_set_.resize(0,0);

	// free old Taylor coefficient memory
	if( taylor_ != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(taylor_);
	taylor_         = CPPAD_NULL;
	taylor_per_var_ = 0;
	taylor_col_dim_ = 0;

# ifndef NDEBUG
	if( check_zero_order )
	{
		// zero order forward calculation using new operation sequence
		check = Forward(0, x);

		// check results
		for(i = 0; i < m; i++) CPPAD_ASSERT_KNOWN( 
			check[i] == y[i] ,
			"Error during check of f.optimize()."
		);

		// Erase memory that this calculation was done so NDEBUG gives 
		// same final state for this object (from users perspective)
		taylor_per_var_ = 0;
	}
# endif
}

CPPAD_END_NAMESPACE
# endif
