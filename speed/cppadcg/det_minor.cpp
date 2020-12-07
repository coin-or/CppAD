/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin cppadcg_det_minor.cpp$$
$spell
    Cppadcg
$$

$section Cppadcg Speed: Gradient of Determinant by Minor Expansion$$

$head Specifications$$
See $cref link_det_minor$$.

$head Implementation$$
$srccode%cpp% */
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/vector.hpp>
# include <cppad/example/compiled_fun.hpp>

# include <map>
extern std::map<std::string, bool> global_option;

bool link_det_minor(
    size_t                     size     ,
    size_t                     repeat   ,
    CppAD::vector<double>     &matrix   ,
    CppAD::vector<double>     &gradient )
{   assert( matrix.size() == size * size );
    assert( gradient.size() == size * size );
    // --------------------------------------------------------------------
    // check global options
    const char* valid[] = { "onetape", "optimize"};
    size_t n_valid = sizeof(valid) / sizeof(valid[0]);
    typedef std::map<std::string, bool>::iterator iterator;
    //
    for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
    {   if( itr->second )
        {   bool ok = false;
            for(size_t i = 0; i < n_valid; i++)
                ok |= itr->first == valid[i];
            if( ! ok )
                return false;
        }
    }
    // --------------------------------------------------------------------
    //
    // optimization options
    std::string optimize_options =
        "no_conditional_skip no_compare_op no_print_for_op";
    //
    // typedefs
    typedef CppAD::cg::CG<double>    c_double;
    typedef CppAD::AD<c_double>      ac_double;
    typedef CppAD::vector<ac_double> ac_vector;
    //
    // object for computing determinant
    CppAD::det_by_minor<ac_double>   ac_det(size);
    //
    // number of independent variables
    size_t nx = size * size;
    // determinant domain space vector
    ac_vector   ac_A(nx);
    // determinant range space vector
    ac_vector   ac_detA(1);
    // gradient of determinant
    ac_vector   ac_gradient(nx);
    //
    // vectors of reverse mode weights
    CppAD::vector<ac_double> ac_w(1);
    ac_w[0] = ac_double(1.0);
    //
    // the AD function objects
    CppAD::ADFun<c_double> c_f;
    CppAD::ADFun<c_double> c_g;
    CppAD::ADFun<ac_double, c_double> ac_f;
    //
    // do not even record comparison operators
    size_t abort_op_index = 0;
    bool record_compare   = false;
    //
    // -----------------------------------------------------------------------
    if( ! global_option["one_tape"] ) while(repeat--)
    {   // choose a matrix
        CppAD::uniform_01(nx, matrix);

        // copy to independent variables
        for(size_t j = 0; j < nx; ++j)
            ac_A[j] = matrix[j];

        // declare independent variables for function computation
        CppAD::Independent(ac_A, abort_op_index, record_compare);

        // AD computation of the determinant
        ac_detA[0] = ac_det(ac_A);

        // create function objects for f : A -> detA
        c_f.Dependent(ac_A, ac_detA);
        ac_f = c_f.base2ad();

        // declare independent variables for gradient computation
        CppAD::Independent(ac_A, abort_op_index, record_compare);

        // AD computation of the gradient
        ac_f.Forward(0, ac_A);
        ac_gradient = ac_f.Reverse(1, ac_w);

        // create function objects for g : A -> det'( detA  )
        c_g.Dependent(ac_A, ac_gradient);
        compiled_fun g(c_g, "det_minor");

        // evaluate the gradient
        gradient = g(matrix);
    }
    else
    {   // choose a matrix
        CppAD::uniform_01(nx, matrix);

        // copy to independent variables
        for(size_t j = 0; j < nx; ++j)
            ac_A[j] = matrix[j];

        // declare independent variables for function computation
        CppAD::Independent(ac_A, abort_op_index, record_compare);

        // AD computation of the determinant
        ac_detA[0] = ac_det(ac_A);

        // create function objects for f : A -> detA
        c_f.Dependent(ac_A, ac_detA);
        ac_f = c_f.base2ad();

        // declare independent variables for gradient computation
        CppAD::Independent(ac_A, abort_op_index, record_compare);

        // AD computation of the gradient
        ac_f.Forward(0, ac_A);
        ac_gradient = ac_f.Reverse(1, ac_w);

        // create function objects for g : A -> det'( detA  )
        c_g.Dependent(ac_A, ac_gradient);
        compiled_fun g(c_g, "det_minor");

        while(repeat--)
        {   // get next matrix
            CppAD::uniform_01(nx, matrix);

            // evaluate the gradient
            gradient = g(matrix);
        }
    }
    return true;
}
/* %$$
$end
*/
