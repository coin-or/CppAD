# ifndef CPPAD_LOCAL_PLAY_ADDR_ENUM_HPP
# define CPPAD_LOCAL_PLAY_ADDR_ENUM_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file addr_enum.hpp
*/
/// enum corresponding to type used for addressing iterators for a player
enum addr_enum {
	unsigned_short_enum  ,
	unsigned_int_enum    ,
	size_t_enum
};

} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
