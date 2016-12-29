// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/* cppad_swig.i */
%{
# include <swig/a_double.hpp>
# include <swig/a_fun.hpp>
# include <swig/function.hpp>
%}

# ifdef SWIGPYTHON
%module py_cppad
# endif
# ifdef SWIGOCTAVE
%module m_cppad
# endif
# ifdef SWIGPERL
%module pl_cppad
# endif

%ignore  ptr;
%include "std_vector.i"
%include <swig/a_double.hpp>
%include <swig/a_fun.hpp>
%include <swig/function.hpp>

%template(vector_double) std::vector<double>;
%template(vector_ad)     std::vector<a_double>;

