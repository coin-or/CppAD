// $Id$
# ifndef CPPAD_HARMONIC_WORK_HPP
# define CPPAD_HARMONIC_WORK_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

extern void harmonic_worker(void);
extern bool harmonic_setup(size_t num_sum, size_t num_threads);
extern bool harmonic_combine(double& sum);

# endif
