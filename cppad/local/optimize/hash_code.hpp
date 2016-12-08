// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_HASH_CODE_HPP
# define CPPAD_LOCAL_OPTIMIZE_HASH_CODE_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file local/optimize/hash_code.hpp
CppAD hashing utility.
*/


// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {
/*!
Specialized hash code for a CppAD operator and its arguments
(used during optimization).

\param op
is the operator that we are computing a hash code for.

\param num_arg
number of elements of arg to include in the hash code.

\param arg
is a vector of length num_arg
containing the corresponding argument indices for this operator.

\return
is a hash code that is between zero and CPPAD_HASH_TABLE_SIZE - 1.
*/

inline unsigned short optimize_hash_code(
	OpCode        op      ,
	size_t        num_arg ,
	const addr_t* arg     )
{	typedef unsigned short ushort;

	CPPAD_ASSERT_UNKNOWN(
		std::numeric_limits<ushort>::max() >= CPPAD_HASH_TABLE_SIZE
	);
	CPPAD_ASSERT_UNKNOWN( size_t(op) < size_t(NumberOp) );
	CPPAD_ASSERT_UNKNOWN( sizeof(addr_t) % sizeof(ushort)  == 0 );
	//
	ushort op_fac = static_cast<ushort> (
		CPPAD_HASH_TABLE_SIZE / static_cast<ushort>(NumberOp)
	);
	CPPAD_ASSERT_UNKNOWN( op_fac > 0 );

	// number of shorts per addr_t value
	size_t short_addr_t   = sizeof(addr_t) / sizeof(ushort);

	// initialize with value that separates operators as much as possible
	ushort code = static_cast<ushort>( static_cast<ushort>(op) * op_fac );

	for(size_t i = 0; i < num_arg; ++i)
	{	const ushort* v = reinterpret_cast<const ushort*>(arg + i);
		for(size_t j = 0; j < short_addr_t; ++j)
			code += v[j];
	}
	return code % CPPAD_HASH_TABLE_SIZE;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
