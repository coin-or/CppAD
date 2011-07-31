/* $Id$ */
# ifndef CPPAD_PRINT_OP_INCLUDED
# define CPPAD_PRINT_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


CPPAD_BEGIN_NAMESPACE
/*!
Print operation for parameters; i.e., op = PriOp.

The C++ source code corresponding to this operation is
\verbatim
	f.Forward(0, x)
	PrintFor(text, y)
	PrintFor(text, y, z)
\endverbatim
The PrintFor call puts the print operation on the tape
and the print occurs during the zero order forward mode computation.

\par std::cout
the results are printed on the C++ standard output stream.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param i_z
is the index of the next variable on the tape
(only used for error checking).

\param arg
\a arg[0] & 1
\n
\n
If this is zero, \a y is a parameter. Otherwise it is a variable.
\n
\a arg[0] & 2
\n
If this is zero, \a z is a parameter. Otherwise it is a variable.
\n
\a arg[1]
\n
index of the text to be printed before \a y
(provided that \a z is less than or equal zero).
\n
\n
\a arg[2]
\n
If \a y is a parameter, <code>parameter[arg[2]]</code> is its value.
Othwise <code>taylor[ arg[2] * nc_taylor + 0 ]</code> is the zero
order Taylor coefficient for \a y.
\n
\n
\a arg[3]
\n
If \a z is a parameter, <code>parameter[arg[3]]</code> is its value.
Othwise <code>taylor[ arg[3] * nc_taylor + 0 ]</code> is the zero
order Taylor coefficient for \a z.


\param num_text
is the total number of text characters on the tape
(only used for error checking).

\param text
\b Input: <code>text[arg[1]]</code> is the first character of the text
that will be printed. All the characters from there to (but not including)
the first '\\0' are printed.

\param num_par
is the total number of values in the \a parameter vector

\param parameter
Contains the value of parameters.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
Contains the value of variables.

\par Checked Assertions:
\li NumArg(PriOp)  == 4
\li NumRes(PriOp)  == 0
\li text          !=  CPPAD_NULL
\li arg[1]         <  num_text
\li if y is a variable, arg[2] < i_z, otherwise arg[2] < num_par
\li if z is a variable, arg[3] < i_z, otherwise arg[3] < num_par
*/
template <class Base>
inline void forward_pri_0(
	size_t        i_z         ,
	const addr_t* arg         ,
	size_t        num_text    ,
	const char*   text        ,
	size_t        num_par     ,
	const Base*   parameter   ,
	size_t        nc_taylor   ,
	const Base*   taylor      )
{	Base y, z;
	const char* t;

	CPPAD_ASSERT_NARG_NRES(PriOp, 4, 0);
	// text
	CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_text );
	t = text + arg[1];
	// y
	if( arg[0] & 1 )
	{	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) <= i_z );
		y = taylor[ arg[2] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );
		y = parameter[ arg[2] ];
	}
	// z
	if( arg[0] & 2 )
	{	CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) <= i_z );
		z = taylor[ arg[3] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
		z = parameter[ arg[3] ];
	}
	if( LessThanOrZero( z ) )
		std::cout << t << y;
}

CPPAD_END_NAMESPACE
# endif
