/* $Id$ */
# ifndef CPPAD_HASH_CODE_INCLUDED
# define CPPAD_HASH_CODE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*!
file hash_code.cpp
A General Purpose hashing utility.
*/

/*!
\def CPPAD_HASH_TABLE_SIZE
the codes retruned by hash_code are between zero and CPPAD_HASH_TABLE_SIZE 
minus one. 
*/
# define CPPAD_HASH_TABLE_SIZE 65536

CPPAD_BEGIN_NAMESPACE
/*!
Returns a hash code for an arbitrary value.

\tparam Value
is the type of the argument being hash coded.
It should be a plain old data class; i.e.,
the values included in the equality operator in the object and
not pointed to by the object.

\param value
the value that we are generating a hash code for.

\return
is a hash code that is between zero and CPPAD_HASH_TABLE_SIZE - 1.

\par Checked Assertions
\li \c std::numeric_limits<unsigned short>::max() == CPPAD_HASH_TABLE_SIZE - 1
\li \c sizeof(value) is even 
\li \c sizeof(unsigned short)  == 2
*/

template <class Value>
unsigned short hash_code(const Value& value)
{	CPPAD_ASSERT_UNKNOWN( 
		std::numeric_limits<unsigned short>::max()
		==
		(CPPAD_HASH_TABLE_SIZE-1)
	);
	CPPAD_ASSERT_UNKNOWN( sizeof(unsigned short) == 2 );
	CPPAD_ASSERT_UNKNOWN( sizeof(value) % 2  == 0 );
	static unsigned short n   = sizeof(value) / 2;

	const unsigned short* v;
	size_t                i;
	unsigned short        code;

	v    = reinterpret_cast<const unsigned short*>(& value);
	i    = n - 1;
	code = v[i];
	while(i--)
		code += v[i];

	return code;
}

/*!
Returns a hash code for an arbitrary CppAD operator.

\param op
is the operator that we are computing a hash code for.

\param arg
is a vector of length \c NumArg(op) containing the argument
indices for this operator.

\param npar
is the number of parameters corresponding to this operation sequence.

\param par
is a vector of length \a npar containing the parameters
for this operation sequence; i.e.,
given a parameter index of \c i, the corresponding parameter value is
\a par[i].


\return
is a hash code that is between zero and CPPAD_HASH_TABLE_SIZE - 1.

\par Checked Assertions
\li \c std::numeric_limits<unsigned short>::max() == CPPAD_HASH_TABLE_SIZE - 1
\li \c size_t(op) <= size_t(SubvvOp) < CPPAD_HASH_TABLE_SIZE
\li \c sizeof(size_t) is even 
\li \c sizeof(Base) is even 
\li \c sizeof(unsigned short)  == 2
\li if the j-th argument for this operation is a parameter, arg[j] < npar.
*/

template <class Base>
unsigned short hash_code(
	OpCode        op      , 
	const size_t* arg     , 
	size_t npar           , 
	const Base* par       )
{	CPPAD_ASSERT_UNKNOWN( 
		std::numeric_limits<unsigned short>::max()
		==
		(CPPAD_HASH_TABLE_SIZE-1)
	);
	CPPAD_ASSERT_UNKNOWN( size_t (op) <= size_t(SubvvOp) );
	CPPAD_ASSERT_UNKNOWN( sizeof(unsigned short) == 2 );
	CPPAD_ASSERT_UNKNOWN( sizeof(size_t) % 2  == 0 );
	CPPAD_ASSERT_UNKNOWN( sizeof(Base) % 2  == 0 );
	static unsigned short op_fac = static_cast<unsigned short> (
	CPPAD_HASH_TABLE_SIZE / ( 1 + static_cast<unsigned short>(SubvvOp) ) 
	);
	CPPAD_ASSERT_UNKNOWN( op_fac > 0 );

	// number of shorts per size_t value
	size_t short_size_t   = sizeof(size_t) / 2;

	// number of shorts per Base value
	size_t short_base     = sizeof(Base) /  2;

	// initialize with value that separates operators as much as possible
	unsigned short code = static_cast<unsigned short>(
		static_cast<unsigned short>(op) * op_fac
	);

	// now code in the operands
	size_t i;
	const unsigned short* v;

	// first argument
	switch(op)
	{	// Hash code binary operator parameters by value instead of
		// by index for two reasons. One, it gives better separation.
		// Two, different indices can be same parameter value.
		case AddpvOp:
		case DivpvOp:
		case MulpvOp:
		case PowpvOp:
		case SubpvOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) >= 1 );
		v = reinterpret_cast<const unsigned short*>(par + arg[0]);
		i = short_base;
		while(i--)
			code += v[i];
		break;

		default:
		if( NumArg(op) >= 1 )
		{	v = reinterpret_cast<const unsigned short*>(arg + 0);
			i = short_size_t;
			while(i--)
				code += v[i];
		}
		break;
	}

	// second argument
	switch(op)
	{
		case DivvpOp:
		case PowvpOp:
		case SubvpOp:
		CPPAD_ASSERT_UNKNOWN( NumArg(op) >= 2 );
		v = reinterpret_cast<const unsigned short*>(par + arg[1]);
		i = short_base;
		while(i--)
			code += v[i];
		break;

		default:
		if( NumArg(op) >= 2 )
		{	v = reinterpret_cast<const unsigned short*>(arg + 1);
			i = short_size_t;
			while(i--)
				code += v[i];
		}
		break;
	}

	if( NumArg(op) >= 3 )
	{	v = reinterpret_cast<const unsigned short*>(arg + 2);
		i = short_base * (NumArg(op) - 2);
		while(i)
			code += v[i];
	}

	return code;
}

CPPAD_END_NAMESPACE
# endif
