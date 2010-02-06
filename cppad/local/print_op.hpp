/* $Id$ */
# ifndef CPPAD_PRINT_OP_INCLUDED
# define CPPAD_PRINT_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


CPPAD_BEGIN_NAMESPACE
/*!
\file print_op.hpp
Zero order forward mode print operation.
*/


/*!
Print operation for parameters; i.e., op = PripOp.

The C++ source code corresponding to this operation is
\verbatim
	PrintFor(text, y)
	f.Forward(0, x)
\endverbatim
where y is a parameter.
The PrintFor call puts the print operation on the tape
and the print occurs during the zero order forward mode computation.

\par std::cout
the results are printed on the C++ standard output stream.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param arg
\a arg[0]
\n
index of the text that this operation will print.
\n
\n
\a arg[1]
\n
index of the parameter that this operation will print.

\param num_text
is the total number of text characters on the tape
(only used for error checking).

\param text
\b Input: \a text[ \a arg[0] ] is the first character of the text
that will be printed. All the characters from there to (but not including)
the first '\\0' are printed.

\param num_par
is the total number of parameters on the tape
(only used for error checking).

\param parameter
\b Input: \a parameter[ \a arg[1] ] is the parameter value
that will be printed after the text.

\par Checked Assertions:
\li text != CPPAD_NULL
\li parameter != CPPAD_NULL
\li NumArg(PripOp) == 2
\li NumRes(PripOp) == 0
\li arg[0] < num_text
\li arg[1] < num_par
*/
template <class Base>
inline void forward_prip_0(
	const size_t* arg         ,
	size_t        num_text    ,
	const char*   text        ,
	size_t        num_par     ,
	const Base*   parameter   )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( text != CPPAD_NULL )
	CPPAD_ASSERT_UNKNOWN( parameter != CPPAD_NULL )
	CPPAD_ASSERT_UNKNOWN( NumArg(PripOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( NumRes(PripOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( arg[0] < num_text );
	CPPAD_ASSERT_UNKNOWN( arg[1] < num_par );

	std::cout << text + arg[0];
	std::cout << parameter[ arg[1] ];
}

/*!
Print operation for variables; i.e., op = PrivOp.

The C++ source code corresponding to this operation is
\verbatim
	PrintFor(text, y)
	f.Forward(0, x)
\endverbatim
where y is a variable.
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
\a arg[0]
\n
index of the text that this operation will print.
\n
\n
\a arg[1]
\n
index of the variable that this operation will print.

\param num_text
is the total number of text characters on the tape
(only used for error checking).

\param text
\b Input: \a text[ \a arg[0] ] is the first character of the text
that will be printed. All the characters from there to (but not including)
the first '\\0' are printed.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a arg[1] * \a nc_taylor + 0 ] 
is zero order taylor coefficient that will be printed.

\par Checked Assertions:
\li text != CPPAD_NULL
\li NumArg(PrivOp) == 2
\li NumRes(PrivOp) == 0
\li arg[0] < num_test
\li arg[1] <= i_z
*/
template <class Base>
inline void forward_priv_0(
	size_t        i_z         ,
	const size_t* arg         ,
	size_t        num_text    ,
	const char*   text        ,
	size_t        nc_taylor   ,
	const Base*   taylor      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( text != CPPAD_NULL )
	CPPAD_ASSERT_UNKNOWN( NumArg(PripOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( NumRes(PripOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( arg[0] < num_text );
	CPPAD_ASSERT_UNKNOWN( arg[1] <= i_z );

	std::cout << text + arg[0];
	std::cout << taylor[ arg[1] * nc_taylor + 0 ];
}

CPPAD_END_NAMESPACE
# endif
