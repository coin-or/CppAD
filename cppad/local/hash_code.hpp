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

\par Restrictions
It is assumed (and checked when NDEBUG is not defined) that the 
$codei%sizeof(%Value%)%$$ is even and that
$code sizeof(unsigned short)$$ is equal to two.
*/


template <class Value>
unsigned short hash_code(const Value& value)
{	CPPAD_ASSERT_UNKNOWN( sizeof(unsigned short) == 2 );
	static unsigned short n   = sizeof(value) / 2;
	CPPAD_ASSERT_UNKNOWN( sizeof(value) == 2 * n );

	const unsigned short* v;
	size_t                i;
	unsigned short        code;

	v    = reinterpret_cast<const unsigned short*>(& value);
	i    = n;
	code = v[i];
	while(i--)
		code += v[i];

	return code;
}
		
CPPAD_END_NAMESPACE
# endif
