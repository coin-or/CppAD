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

# include <CppAD/pow_int.hpp>
# include <CppAD/lu_solve.hpp>
# include <CppAD/near_equal.hpp>
# include <CppAD/runge_45.hpp>
# include <CppAD/romberg_one.hpp>
# include <CppAD/romberg_mul.hpp>
# include <CppAD/rosen_34.hpp>
# include <CppAD/ode_err_control.hpp>
# include <CppAD/ode_gear.hpp>
# include <CppAD/ode_gear_control.hpp>
# include <CppAD/poly.hpp>
# include <CppAD/speed_test.hpp>
# include <CppAD/vector.hpp>

# include <CppAD/check_simple_vector.hpp>
# include <CppAD/check_numeric_type.hpp>
# include <CppAD/track_new_del.hpp>
# include <CppAD/local/cppad_error.hpp>
// --------------------------------------------------------------------------
// System routines that can be used by rest of CppAD with out including 

# include <cstddef>
# include <iostream>
# include <complex>
# include <cmath>

// ---------------------------------------------------------------------------
// definitions needed by rest of includes

// definitions that come from the installation
# include <CppAD/local/preprocessor.hpp> // avoid possible conflict in config.h
# include <CppAD/config.h>

// definitions that are local to the CppAD include files
# include <CppAD/local/define.hpp>

// vectors used with CppAD
# include <CppAD/local/cppad_vector.hpp>

// Declare classes and fucntions that are used before defined
# include <CppAD/local/declare.hpp>

// ---------------------------------------------------------------------------
// files containing only implementation details

# include <CppAD/local/ad.hpp>       // the differentaible object class

// ---------------------------------------------------------------------------
// files containing user interface documentation

# include <CppAD/local/op.hpp>      // executes taped operations
# include <CppAD/local/user_ad.hpp>  // AD class methods available to the user
# include <CppAD/local/ad_fun.hpp>   // ADFun objects

// ---------------------------------------------------------------------------
// library routines that require the rest of CppAD
# include <CppAD/local/lu_ratio.hpp>
# include <CppAD/local/bender_quad.hpp>

// undo definitions in Define.h
# include <CppAD/local/undef.hpp>   

// undo preprocessor symbols that do not begin with CppAD or CPPAD_
# include <CppAD/local/preprocessor.hpp>

# endif
