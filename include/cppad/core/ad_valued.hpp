# ifndef CPPAD_CORE_AD_VALUED_HPP
# define CPPAD_CORE_AD_VALUED_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ADValued}

AD Valued Operations and Functions
##################################

{xrst_comment atomic.omh includes atomic_two.hpp}

Contents
********
{xrst_toc_table
   include/cppad/core/arithmetic.hpp
   include/cppad/core/standard_math.hpp
   include/cppad/core/cond_exp.hpp
   include/cppad/core/discrete/user.xrst
   include/cppad/core/numeric_limits.hpp
   include/cppad/core/atomic/atomic.xrst
}

{xrst_end ADValued}
*/

// include MathOther.h after CondExp.h because some MathOther.h routines use
// CondExp.h and CondExp.h is not sufficently declared in Declare.h

# include <cppad/core/arithmetic.hpp>
# include <cppad/core/standard_math.hpp>
# include <cppad/core/azmul.hpp>
# include <cppad/core/cond_exp.hpp>
# include <cppad/core/discrete/discrete.hpp>
# include <cppad/core/atomic/four/atomic.hpp>
# include <cppad/core/atomic/three/atomic.hpp>
# include <cppad/core/atomic/four/atomic.hpp>
# include <cppad/core/chkpoint_two/chkpoint_two.hpp>
# include <cppad/core/atomic/two/atomic.hpp>
# include <cppad/core/atomic/one/atomic.hpp>
# include <cppad/core/chkpoint_one/chkpoint_one.hpp>

# endif
