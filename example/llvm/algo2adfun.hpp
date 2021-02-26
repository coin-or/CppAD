# ifndef CPPAD_EXAMPLE_LLVM_ALGO2ADFUN_HPP
# define CPPAD_EXAMPLE_LLVM_ALGO2ADFUN_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>

extern bool algo2adfun(
    size_t                np    ,
    size_t                nx    ,
    CppAD::ADFun<double>& adfun
);

# endif
