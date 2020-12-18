# ifndef CPPAD_EXAMPLE_COMPILED_FUN_HPP
# define CPPAD_EXAMPLE_COMPILED_FUN_HPP
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

class compiled_fun {
public:
    // type of evaluation for Jacobians and Hessians
    enum evaluation_enum { none_enum, dense_enum, sparse_enum };
private:
    // dynamic_lib_
    std::unique_ptr< CppAD::cg::DynamicLib<double> > dynamic_lib_;
    //
    // model_ (contains a reference to dynamic_lib_)
    std::unique_ptr< CppAD::cg::GenericModel<double> > model_;
    //
public:
    // -----------------------------------------------------------------------
    // constructors
    // -----------------------------------------------------------------------
    // fun_name()
    compiled_fun(void);
    //
    // fun_name( file_name )
    compiled_fun(const std::string& file_name);
    //
    // fun_name(file_name, cg_fun, eval_jac)
    compiled_fun(
        const std::string&                     file_name             ,
        CppAD::ADFun< CppAD::cg::CG<double> >& cg_fun                ,
        evaluation_enum                        eval_jac = none_enum
    );
    // -----------------------------------------------------------------------
    // operations
    // -----------------------------------------------------------------------
    // swap(other_fun)
    void swap(compiled_fun& other_fun);
    //
    // y = fun_name(x)
    CppAD::vector<double>  operator()(const CppAD::vector<double> & x);
    //
    // J = fun_name.jacobian(x)
    CppAD::vector<double>  jacobian(const CppAD::vector<double> & x);
};

# endif
