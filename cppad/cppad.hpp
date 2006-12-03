# ifndef CPPAD_CPPAD_INCLUDED
# define CPPAD_CPPAD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// ---------------------------------------------------------------------------
// CppAD general purpose library routines (can be included separately)

# include <cppad/pow_int.hpp>
# include <cppad/lu_solve.hpp>
# include <cppad/near_equal.hpp>
# include <cppad/runge_45.hpp>
# include <cppad/romberg_one.hpp>
# include <cppad/romberg_mul.hpp>
# include <cppad/rosen_34.hpp>
# include <cppad/ode_err_control.hpp>
# include <cppad/ode_gear.hpp>
# include <cppad/ode_gear_control.hpp>
# include <cppad/poly.hpp>
# include <cppad/speed_test.hpp>
# include <cppad/vector.hpp>

# include <cppad/check_simple_vector.hpp>
# include <cppad/check_numeric_type.hpp>
# include <cppad/track_new_del.hpp>
# include <cppad/local/cppad_error.hpp>
// --------------------------------------------------------------------------
// System routines that can be used by rest of CppAD with out including 

# include <cstddef>
# include <iostream>
# include <complex>
# include <cmath>

// ---------------------------------------------------------------------------
// definitions needed by rest of includes

// definitions that come from the installation
# include <cppad/local/preprocessor.hpp> // avoid possible conflict in config.h
# include <cppad/config.h>

// definitions that are local to the CppAD include files
# include <cppad/local/define.hpp>

// vectors used with CppAD
# include <cppad/local/cppad_vector.hpp>

// Declare classes and fucntions that are used before defined
# include <cppad/local/declare.hpp>

// ---------------------------------------------------------------------------
// files containing only implementation details

# include <cppad/local/ad.hpp>       // the differentaible object class

// ---------------------------------------------------------------------------
// files containing user interface documentation

# include <cppad/local/op.hpp>      // executes taped operations
# include <cppad/local/user_ad.hpp>  // AD class methods available to the user
# include <cppad/local/ad_fun.hpp>   // ADFun objects

// ---------------------------------------------------------------------------
// library routines that require the rest of CppAD
# include <cppad/local/lu_ratio.hpp>
# include <cppad/local/bender_quad.hpp>

// undo definitions in Define.h
# include <cppad/local/undef.hpp>   

// undo preprocessor symbols that do not begin with CppAD or CPPAD_
# include <cppad/local/preprocessor.hpp>

# endif
