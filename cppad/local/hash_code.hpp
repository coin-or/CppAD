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

\return
is a hash code that is between zero and CPPAD_HASH_TABLE_SIZE - 1.

\par Checked Assertions
\li \c std::numeric_limits<unsigned short>::max() == CPPAD_HASH_TABLE_SIZE - 1
\li \c size_t(op) < CPPAD_HASH_TABLE_SIZE
\li \c sizeof(size_t) is even 
\li \c sizeof(unsigned short)  == 2
*/

inline unsigned short hash_code(OpCode op, const size_t* arg)
{	static size_t half_table_size = CPPAD_HASH_TABLE_SIZE / 2;

	CPPAD_ASSERT_UNKNOWN( 
		std::numeric_limits<unsigned short>::max()
		==
		(CPPAD_HASH_TABLE_SIZE-1)
	);
	CPPAD_ASSERT_UNKNOWN( size_t (op) < CPPAD_HASH_TABLE_SIZE );
	CPPAD_ASSERT_UNKNOWN( sizeof(unsigned short) == 2 );
	CPPAD_ASSERT_UNKNOWN( sizeof(size_t) % 2  == 0 );

	// number of shorts per size_t value
	size_t n   = sizeof(size_t) / 2;

	// number of shorts corresponding to the arg vector
	n = n * NumArg(op);

	unsigned short code = static_cast<unsigned short>(op);
	if( code > 0 ) while( code < half_table_size )
		code *= 2;

	const unsigned short* v
		= reinterpret_cast<const unsigned short*>(arg);
	while(n--)
		code += v[n];

	return code;
}

CPPAD_END_NAMESPACE
# endif
