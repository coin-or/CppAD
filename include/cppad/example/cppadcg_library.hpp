# ifndef CPPAD_EXAMPLE_CPPADCG_LIBRARY_HPP
# define CPPAD_EXAMPLE_CPPADCG_LIBRARY_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <cppad/cg/cppadcg.hpp>

class cppadcg_library {
private:
    // dynamic_lib_
    std::unique_ptr< CppAD::cg::DynamicLib<double> > dynamic_lib_;
    //
    // model_ (contains a reference to dynamic_lib_)
    std::unique_ptr< CppAD::cg::GenericModel<double> > model_;
public:
    // fun_name( cg_fun, file_name)
    cppadcg_library(
        CppAD::ADFun< CppAD::cg::CG<double> >& cg_fun    ,
        const std::string&       file_name
    );
    //
    // fun_name( file_name )
    cppadcg_library(const std::string& file_name);
    //
    // fun_name()
    cppadcg_library(void);
    //
    // swap(other_fun)
    void swap(cppadcg_library& other_fun);
    //
    // y = fun_name(x)
    CppAD::vector<double>  operator()(const CppAD::vector<double> & x);
};

# endif
