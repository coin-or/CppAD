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
size functions in $cref/SeqProperty/$$.
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
by calling the function $cref/f.size_var()/SeqProperty/size_var/$$
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
in the operation sequence; see $cref/size_var/SeqProperty/size_var/$$.
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

	// set the number of operators
	const size_t numop_m1 = play->num_rec_op() - 1;

	// number of  VecAD indices 
	size_t num_vecad_ind   = play->num_rec_vecad_ind();

	// number of VecAD vectors
	size_t num_vecad_vec   = play->num_rec_vecad_vec();

	// -------------------------------------------------------------
	// Determine which variables are connected to the dependent variables. 

	// initialize all except the dependent variables as not connected. 
	CppAD::vector<bool> connected(num_var);
	for(i = 0; i < num_var; i++)
		connected[i] = false;
	for(j = 0; j < m; j++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr[j] < num_var );
		connected[ dep_taddr[j] ] = true;
	}

	// vecad_connected contains a flag for each VecAD object.
	// vecad maps a VecAD index (which corresponds to the beginning of the
	// VecAD object) to the vecad_connected index for the VecAD object.
	CppAD::vector<bool>   vecad_connected(num_vecad_vec);
	CppAD::vector<size_t> vecad_offset(num_vecad_vec);
	CppAD::vector<size_t> vecad(num_vecad_ind);
	j = 0;
	for(i = 0; i < num_vecad_vec; i++)
	{	vecad_connected[i] = false;
		// offset for this VecAD
		vecad_offset[i] = j + 1;
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
	play->start_reverse();
	size_t i_op = 2;
	size_t mask;
# if CPPAD_OPTIMIZE_TRACE
	std::cout << std::endl;
# endif
	while(i_op > 1 )
	{	// next op
		play->next_reverse(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );

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
			connected[ arg[0] ] |= connected[ i_var ];
			break;

			// Unary operator where operand is arg[1]
			case AddpvOp:
			case DivpvOp:
			case MulpvOp:
			case PowpvOp:
			case SubpvOp:
			case PrivOp:
			connected[ arg[1] ] |= connected[ i_var ];
			break;

			// Binary operator where operands are arg[0], arg[1]
			case AddvvOp:
			case DivvvOp:
			case MulvvOp:
			case PowvvOp:
			case SubvvOp:
			connected[ arg[0] ] |= connected[ i_var ];
			connected[ arg[1] ] |= connected[ i_var ];
			break;

			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
			mask = 1;
			for(i = 2; i < 6; i++)
			{	if( arg[1] & mask )
				{	CPPAD_ASSERT_UNKNOWN( arg[i] < i_var );
					connected[ arg[i] ] |= 
						connected[ i_var ];
				}
				mask = mask << 1;
			}

			// Operations where there is noting to do
			case ComOp:
			case InvOp:
			case NonOp:
			case ParOp:
			case PripOp:
			case StppOp:
			break; 

			// Load using a parameter index
			case LdpOp:
			i = vecad[ arg[0] - 1 ];
			vecad_connected[i] |= connected[ i_var ];
			break;

			// Load using a variable index
			case LdvOp:
			i = vecad[ arg[0] - 1 ];
			vecad_connected[i]  |= connected[ i_var ];
			connected[ arg[1] ] |= connected[ i_var ];
			break;

			// Store a variable using a parameter index
			case StpvOp:
			i = vecad[ arg[0] - 1 ];
			connected[ arg[2] ] |= vecad_connected[i];
			break;

			// Store a variable using a variable index
			case StvvOp:
			i = vecad[ arg[0] - 1 ];
			connected[ arg[1] ] |= vecad_connected[i];
			connected[ arg[2] ] |= vecad_connected[i];
			break;

			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
# if CPPAD_OPTIMIZE_TRACE
	for(i = 0; i < num_vecad_vec; i++) 
	{	std::cout << "VecAD: off=" << std::setw(5) << vecad_offset[i];
		std::cout << "connected = " << vecad_connected[i];
		std::cout << std::endl; 
	}
# endif
	// -------------------------------------------------------------

	// Erase all information in the recording
	rec->Erase();


	// initialize mapping from old variable index to new variable index
	CppAD::vector<size_t> new_var(num_var);
	for(i = 0; i < num_var; i++)
		new_var[i] = num_var; // initialize as an invalid value

	// Put the necessary VecAD objects in the tape
	CppAD::vector<size_t> new_vecad_ind(num_vecad_ind);
	for(i = 0; i < num_vecad_ind; i++)
		new_vecad_ind[i] = num_vecad_ind; // an invalid value 

	j = 0;     // index into the old set of indices
	for(i = 0; i < num_vecad_vec; i++)
	{	// length of this VecAD
		size_t length = play->GetVecInd(j);
		if( connected[i] )
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

	// put the beginning NonOp in the recording
	CPPAD_ASSERT_UNKNOWN( op == NonOp );
	CPPAD_ASSERT_NARG_NRES(NonOp, 0, 1);
	new_var[ i_var ] = rec->PutOp(NonOp);

	// temporary buffer for new argument values
	size_t new_arg[6];

	while(i_op < numop_m1 )
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
			keep = true;
			break;

			case StppOp:
			case StvpOp:
			case StpvOp:
			case StvvOp:
			CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
			i = vecad[ arg[0] - 1 ];
			keep = vecad_connected[i];
			break;

			default:
			keep = connected[i_var];
			break;
		}

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
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0  );
			new_arg[0] = new_var[ arg[0] ];
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_var );

			rec->PutArg( new_arg[0] );
			new_var[ i_var ] = rec->PutOp(op);
			break;

			// Binary operators where left operand is a variable
			// and right operand is a parameter
			case AddvpOp:
			case DivvpOp:
			case MulvpOp:
			case PowvpOp:
			case SubvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			new_arg[0] = new_var[ arg[0] ];
			new_arg[1] = rec->PutPar( play->GetPar( arg[1] ) );
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_var );

			rec->PutArg( new_arg[0], new_arg[1] );
			new_var[ i_var ] = rec->PutOp(op);
			break;

			// Binary operators where left operand is a parameter
			// and right operand is a variable
			case AddpvOp:
			case DivpvOp:
			case MulpvOp:
			case PowpvOp:
			case SubpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			new_arg[0] = rec->PutPar( play->GetPar( arg[0] ) );
			new_arg[1] = new_var[ arg[1] ];
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );

			rec->PutArg( new_arg[0], new_arg[1] );
			new_var[ i_var ] = rec->PutOp(op);
			break;

			// Binary operator where both operators are variables
			case AddvvOp:
			case DivvvOp:
			case MulvvOp:
			case PowvvOp:
			case SubvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			new_arg[0] = new_var[ arg[0] ];
			new_arg[1] = new_var[ arg[1] ];
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_var );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );

			rec->PutArg( new_arg[0], new_arg[1] );
			new_var[ i_var ] = rec->PutOp(op);
			break;

			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 6, 1);
			new_arg[0] = arg[0];
			new_arg[1] = arg[1];
			mask = 1;
			for(i = 2; i < 6; i++)
			{	if( arg[1] & mask )
				{	new_arg[i] = new_var[ arg[i] ];
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
			new_var[ i_var ] = rec->PutOp(op);
			break;

			// Operations with no arguments and one result
			case InvOp:
			case NonOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			new_var[ i_var ] = rec->PutOp(op);
			break;
 
			// Operations with one argument that is a parameter
			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0] ) );

			rec->PutArg( new_arg[0] );
			new_var[ i_var ] = rec->PutOp(op);
			break;

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
			new_var[ i_var ] = rec->PutOp(op);
			break;

			// Load using a variable index
			case LdvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = new_var[ arg[1] ];
			CPPAD_ASSERT_UNKNOWN( new_arg[0] < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( new_arg[1] < num_var );
			rec->PutArg( 
				new_arg[0], 
				new_arg[1], 
				0
			);
			new_var[ i_var ] = rec->PutOp(op);
			break;

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

			// Store a parameter using a variable index
			case StvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = new_var[ arg[1] ];
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

			// Store a variable using a parameter index
			case StpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			new_arg[2] = new_var[ arg[2] ];
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

			// Store a variable using a variable index
			case StvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = new_var[ arg[1] ];
			new_arg[2] = new_var[ arg[2] ];
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


			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(0);

		}

	}
	// modify the dependent variable vector to new indices
	for(i = 0; i < dep_taddr.size(); i++ )
	{	CPPAD_ASSERT_UNKNOWN( new_var[ dep_taddr[i] ] < num_var );
		dep_taddr[i] = new_var[ dep_taddr[i] ];
	}
}
CPPAD_END_NAMESPACE

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
void CppAD::ADFun<Base>::optimize(void)
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

# endif
