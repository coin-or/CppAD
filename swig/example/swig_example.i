/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/* swig_example.i */
%module swig_example
%{
# include "swig_example.hpp"
%}

%include "cpointer.i"
%pointer_class(int, int_class);

%include "carrays.i"
%array_functions(int, int_array_ptr);
%array_class(int, int_array_class);

%include "std_vector.i"

%include "swig_example.hpp"

%template(double_class)  template_class<double>;
%template(vector_double) std::vector<double>;
