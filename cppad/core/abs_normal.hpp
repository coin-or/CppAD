# ifndef CPPAD_CORE_ABS_NORMAL_HPP
# define CPPAD_CORE_ABS_NORMAL_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin abs_normal$$
$spell
	Andreas Griewank
	Jens-Uwe Bernt
	Manuel Radons
	Tom Streubel
	const
$$
$latex
\newcommand{\B}[1]{ {\bf #1} }
$$


$section Create An Abs-normal Representation of a Function$$

$head Under Construction$$
This is an in-progress design, and does not yet have an implementation.

$head Syntax$$
$icode%g%.abs_normal(%f%)%$$

$head Reference$$
Andreas Griewank, Jens-Uwe Bernt, Manuel Radons, Tom Streubel,
$italic Solving piecewise linear systems in abs-normal form$$,
Linear Algebra and its Applications,
vol. 471 (2015), pages 500-530.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%>& %f%
%$$
It represents a function $latex f : \B{R}^n \rightarrow \B{R}^m$$.
We assume that the only non-smooth terms in the representation are
absolute value functions and use $latex s \in \B{Z}_+$$
to represent the number of these terms.
It is effectively $code const$$, except that some internal state
that is not relevant to the user; see
$cref/const ADFun/wish_list/const ADFun/$$.

$head g$$
The object $icode g$$ has prototype
$codei%
	ADFun<%Base%> %g%
%$$
The initial function representation in $icode g$$ is lost.
Upon return it represents the smooth function
$latex g : \B{R}^{n + s} \rightarrow  \B{R}^{m + s}$$ is defined by
$latex \[
g( x , u )
=
\left[ \begin{array}{c} y(x, u) \\ z(x, u) \end{array} \right]
\] $$
were $latex y(x, u)$$ and $latex z(x, u)$$ are defined below.

$subhead a(x)$$
Let $latex \zeta_0 ( x )$$
denote the argument for the first absolute value term in $latex f(x)$$,
$latex \zeta_1 ( x , |\zeta_0 (x)| )$$ for the second term, and so on.
For $latex i = 0 , \ldots , {s-1}$$ define
$latex \[
a_i (x)
=
| \zeta_i ( x , a_0 (x) , \ldots , a_{i-1} (x ) ) |
\] $$
This defines $latex a : \B{R}^n \rightarrow \B{R}^s$$.

$subhead z(x, u)$$
Define the smooth function
$latex z : \B{R}^{n + s} \rightarrow  \B{R}^s$$ by
$latex \[
z_i ( x , u ) = \zeta_i ( x , u_0 , \ldots , u_{i-1} )
\] $$

$subhead y(x, u)$$
There is a smooth function
$latex y : \B{R}^{n + s} \rightarrow  \B{R}^m$$
such that $latex y( x , u ) = f(x)$$ whenever $latex u = a(x)$$.

$head Affine Approximation$$
We define the affine approximations
$latex \[
\begin{array}{rcl}
y[ \hat{x} ]( x , u )
& = &
y ( \hat{x}, a( \hat{x} ) )
	+ \partial_x y ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \partial_u y ( \hat{x}, a( \hat{x} ) ) ( u - a( \hat{x} ) )
\\
z[ \hat{x} ]( x , u )
& = &
z ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \partial_u z ( \hat{x}, a( \hat{x} ) ) ( u - a( \hat{x} ) )
\end{array}
\] $$
It follows that
$latex \[
\begin{array}{rcl}
y( x , u )
& = &
y[ \hat{x} ]( x , u ) + o ( x - \hat{x}, u - a( \hat{x} ) )
\\
z( x , u )
& = &
z[ \hat{x} ]( x , u ) + o ( x - \hat{x}, u - a( \hat{x} ) )
\end{array}
\] $$

$head Abs-normal Approximation$$

$subhead Approximating a(x)$$
The function $latex a(x)$$ is not smooth, but it is equal to
$latex | z(x, u) |$$ when $latex u = a(x)$$.
Furthermore
$latex \[
z[ \hat{x} ]( x , u )
=
z ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \partial_u z ( \hat{x}, a( \hat{x} ) ) ( u - a( \hat{x} ) )
\] $$
Now the partial of $latex z_i$$ with respect to $latex u_j$$ is zero
for $latex j \geq i$$. It follows that
$latex \[
z_i [ \hat{x} ]( x , u )
=
z_i ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z_i ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \sum_{j < i} \partial_{u(j)}
		z_i ( \hat{x}, a( \hat{x} ) ) ( u_j - a_j ( \hat{x} ) )
\] $$
Considering the case $latex i = 0$$ we define
$latex \[
a_0 [ \hat{x} ]( x )
=
| z_0 [ \hat{x} ]( x , u ) |
=
\left|
	z_0 ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z_0 ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
\right|
\] $$
It follows that
$latex \[
	a_0 (x) = a_0 [ \hat{x} ]( x ) + o ( x - \hat{x} )
\] $$
In general, we define $latex a_i [ \hat{x} ]$$ using
$latex a_j [ \hat{x} ]$$ for $latex j < i$$ as follows:
$latex \[
a_i [ \hat{x} ]( x )
=
\left |
	z_i ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z_i ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \sum_{j < i} \partial_{u(j)}
		z_i ( \hat{x}, a( \hat{x} ) )
			( a_j [ \hat{x} ] ( x )  - a_j ( \hat{x} ) )
\right|
\] $$
It follows that
$latex \[
	a (x) = a[ \hat{x} ]( x ) + o ( x - \hat{x} )
\] $$
Note that in the case where $latex z(x, u)$$ and $latex y(x, u)$$ are
affine,
$latex \[
	a[ \hat{x} ]( x ) = a( x )
\] $$


$subhead Approximating f(x)$$
$latex \[
f(x)
=
y ( x , a(x ) )
=
y [ \hat{x} ] ( x , a[ \hat{x} ] ( x ) )
+ o( \Delta x )
\] $$

$head Correspondence to Literature$$
Using the notation
$latex Z = \partial_x z(\hat{x}, \hat{u})$$,
$latex L = \partial_u z(\hat{x}, \hat{u})$$,
$latex J = \partial_x y(\hat{x}, \hat{u})$$,
$latex Y = \partial_u y(\hat{x}, \hat{u})$$,
the approximation for $latex z$$ and $latex y$$ are
$latex \[
\begin{array}{rcl}
z[ \hat{x} ]( x , u )
& = &
z ( \hat{x}, a( \hat{x} ) ) + Z ( x - \hat{x} ) + L ( u - a( \hat{x} ) )
\\
y[ \hat{x} ]( x , u )
& = &
y ( \hat{x}, a( \hat{x} ) ) + J ( x - \hat{x} ) + Y ( u - a( \hat{x} ) )
\end{array}
\] $$
Moving the terms with $latex \hat{x}$$ together, we have
$latex \[
\begin{array}{rcl}
z[ \hat{x} ]( x , u )
& = &
z ( \hat{x}, a( \hat{x} ) ) - Z \hat{x} - L a( \hat{x} )  + Z x + L u
\\
y[ \hat{x} ]( x , u )
& = &
y ( \hat{x}, a( \hat{x} ) ) - J \hat{x} - Y a( \hat{x} )  + J x + Y u
\end{array}
\] $$
Using the notation
$latex c = z ( \hat{x}, \hat{u} ) - Z \hat{x} - L \hat{u}$$,
$latex b = y ( \hat{x}, \hat{u} ) - J \hat{x} - Y \hat{u}$$,
we have
$latex \[
\begin{array}{rcl}
z[ \hat{x} ]( x , u ) & = & c + Z x + L u
\\
y[ \hat{x} ]( x , u ) & = & b + J x + Y u
\end{array}
\] $$
Considering the affine case, where the approximations are exact,
and choosing $latex u = a(x) = |z(x, u)|$$, we obtain
$latex \[
\begin{array}{rcl}
z( x , a(x ) ) & = & c + Z x + L |z( x , a(x ) )|
\\
y( x , a(x ) ) & = & b + J x + Y |z( x , a(x ) )|
\end{array}
\] $$
This is Equation (2) of the reference.

$end
-------------------------------------------------------------------------------
*/
/*!
file abs_normal.hpp
Create an abs-normal representation of a function
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
Create an abs-normal representation of an ADFun object.

\tparam Base
base type for this abs-normal form and for the function beging represented;
i.e., f.

\param f
is the function that this object will represent in abs-normal form.
This is effectively const except that the play back state f.play_
is used.
*/
template <class Base>
void ADFun<Base>::abs_normal(ADFun<Base>& f)
{
	// -----------------------------------------------------------------------
	// Forward sweep to determine number of absolute value operations in f
	// -----------------------------------------------------------------------
	// The argument and result index in f for each absolute value operator
	CppAD::vector<size_t> f_abs_arg, f_abs_res;
	//
	OpCode        op;                 // this operator
	const addr_t* arg = CPPAD_NULL;   // arguments for this operator
	size_t        i_op;               // index of this operator
	size_t        i_var;              // variable index for this operator
	f.play_->forward_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
	//
	bool    more_operators = true;
	while( more_operators )
	{
		// next op
		f.play_->forward_next(op, arg, i_op, i_var);
		switch( op )
		{	// absolute value operator
			case AbsOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			f_abs_arg.push_back( arg[0] );
			f_abs_res.push_back( i_var );
			break;

			case CSumOp:
			// CSumOp has a variable number of arguments
			f.play_->forward_csum(op, arg, i_op, i_var);
			break;

			case CSkipOp:
			// CSkip has a variable number of arguments
			f.play_->forward_cskip(op, arg, i_op, i_var);
			break;

			case EndOp:
			more_operators = false;
			break;

			default:
			break;
		}
	}
	// ------------------------------------------------------------------------
	// Forward sweep to create new recording
	// ------------------------------------------------------------------------
	// recorder for new operation sequence
	recorder<Base> rec;
	//
	// number of variables in both operation sequences
	// (the AbsOp operators are replace by InvOp operators)
	const size_t num_var = f.play_.num_var_rec();
	//
	// mapping from old variable index to new variable index
	CppAD::vector<size_t> f2g_var(num_var);
	for(i_var = 0; i_var < num_var; i_var++)
		f2g_var[i_var] = 0; // used for auxillary and phantom variables
	//
	// record the independent variables in f
	f.play_->forward_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
	more_operators   = true;
	while( more_operators )
	{	switch( op )
		{
			case BeginOp:
			// phantom variable
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			CPPAD_ASSERT_UNKNOWN( arg[0] == 0 );
			CPPAD_ASSERT_UNKNOWN( new_var == 0 );
			rec.PutArg(0);
			f2g_var[i_var] = rec.PutOp(op);
			break;

			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			f2g_var[i_var] = rec.PutOp(op);
			break;

			default:
			more_operators = false;
			break;
		}
		if( more_operators )
			f.play_->forward_next(op, arg, i_op, i_var);
	}
	CPPAD_ASSERT_UNKNOWN( f.Domain() == new_var );
	//
	// record the independent variables corresponding AbsOp results
	for(size_t index_abs = 0; index_abs < f_abs_var.size(); index_abs++)
		f2g_var[ f_abs_res[index_abs] ] = rec.PutOp(InvOp);
	//
	// used to hold new argument vector
	addr_t new_arg[6];
	//
	// Parameters in recording of f
	Base* f_parameter = f.play_.GetPar();
	//
	// now loop through the rest of the
	more_operators = true;
	index_abs      = 0;
	while( more_operators )
	{	switch( op )
		{
			case AbsOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			CPPAD_ASSERT_UNKNOWN( f_abs_arg[index_abs] ==  arg[0] );
			CPPAD_ASSERT_UNKNOWN( f_abs_res[index_abs] ==  i_var );
			CPPAD_ASSERT_UNKNOWN( f2g_var[i_var] > 0 );
			++index_abs;
			break;

			case EndOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 0);
			rec.PutOp(op);
			more_operators = false;
			break;


			// --------------------------------------------------------------
			// Unary operator where operand is arg[0]
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			case AbsOp:
			case AcosOp:
			case AcoshOp:
			case AsinOp:
			case AsinhOp:
			case AtanOp:
			case AtanhOp:
			case CosOp:
			case CoshOp:
			case ExpOp:
			case Expm1Op:
			case LogOp:
			case Log1pOp:
			case SignOp:
			case SinOp:
			case SinhOp:
			case SqrtOp:
			case TanOp:
			case TanhOp:
			new_arg[0] = f2g_var[ arg[0] ];
			rec.PutArg( new_arg[0] );
			f2g_var[i_var] = rec.PutOp( op );
			break;

			case ErfOp:
			// Error function is a special case
			// second argument is always the parameter 0
			// third argument is always the parameter 2 / sqrt(pi)
			CPPAD_ASSERT_NARG_NRES(0, 3, 5);
			rec.PutArg( rec.PutPar( Base(0) ) );
			rec.PutArg( rec.PutPar(
				Base( 1.0 / std::sqrt( std::atan(1.0) ) )
			) );
			f2g_var[i_var] = rec.PutOp(op);
			break;
			// --------------------------------------------------------------
			// Binary operators, left variable, right parameter, one result
			case SubvpOp;
			case DivvpOp;
			case PowvpOp;
			case ZmulvpOp;
			new_arg[0] = f2g_var[ arg[0] ];
			new_arg[1] = rec.PutPar( f_parameter[ arg[1] ] );
			rec.PutArg( new_arg[0], new_arg[1] );
			f2g_var[i_var] = rec.PutOp(op);
			break;
			// --------------------------------------------------------------
			// Binary operators, left parameter, right variable, one result
			case AddpvOp;
			case SubpvOp;
			case DivpvOp;
			case PowpvOp;
			case ZmulpvOp;
			new_arg[0] = rec.PutPar( f_parameter[ arg[0] ] );
			new_arg[1] = f2g_var[ arg[1] ];
			rec.PutArg( new_arg[0], new_arg[1] );
			f2g_var[i_var] = rec.PutOp(op);
			break;
			// --------------------------------------------------------------
			// Binary operators, both operands are variables, one result
			case AddvvOp;
			case SubvvOp;
			case MulvvOp;
			case DivvvOp;
			case ZmulvvOp;
			new_arg[0] = f2g_var[ arg[0] ];
			new_arg[1] = f2g_var[ arg[1] ];
			rec.PutArg( new_arg[0], new_arg[1] );
			f2g_var[i_var] = rec.PutOp(op);
			break;
			// ---------------------------------------------------
			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 6, 1);
			new_arg[0] = arg[0];
			new_arg[1] = arg[1];
			mask = 1;
			for(size_t i = 2; i < 6; i++)
			{	if( arg[1] & mask )
					new_arg[i] = f2g_var[ arg[i] ];
				else
					new_arg[i] = rec.PutPar( f_parameter[ arg[i] ] );
				mask = mask << 1;
			}
			rec.PutArg(
				new_arg[0] ,
				new_arg[1] ,
				new_arg[2] ,
				new_arg[3] ,
				new_arg[4] ,
				new_arg[5]
			);
			f2g_var[i_var] = rec->PutOp(op);
			break;


		}
	}


}

} // END_CPPAD_NAMESPACE

# endif
