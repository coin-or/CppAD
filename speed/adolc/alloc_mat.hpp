/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# ifndef CPPAD_ADOLC_ALLOC_MAT_INCLUDED
# define CPPAD_ADOLC_ALLOC_MAT_INCLUDED

double** adolc_alloc_mat(size_t m, size_t n);
void adolc_free_mat(double** mat);

# endif 
