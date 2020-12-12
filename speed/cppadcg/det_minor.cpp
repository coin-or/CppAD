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

namespace {
    //
    // typedefs
    typedef CppAD::cg::CG<double>     c_double;
    typedef CppAD::AD<c_double>      ac_double;
    typedef CppAD::vector<double>     d_vector;
    typedef CppAD::vector<ac_double> ac_vector;
    //
    // setup
    void setup(compiled_fun& g, size_t size )
    {   // optimization options
        std::string optimize_options =
            "no_conditional_skip no_compare_op no_print_for_op";
        //
        // object for computing determinant
        CppAD::det_by_minor<ac_double>   ac_det(size);
        //
        // number of independent variables
        size_t nx = size * size;
        //
        // choose a matrix
        CppAD::vector<double> matrix(nx);
        CppAD::uniform_01(nx, matrix);
        //
        // copy to independent variables
        ac_vector   ac_A(nx);
        for(size_t j = 0; j < nx; ++j)
            ac_A[j] = matrix[j];
        //
        // declare independent variables for function computation
        bool record_compare   = false;
        size_t abort_op_index = 0;
        CppAD::Independent(ac_A, abort_op_index, record_compare);
        //
        // AD computation of the determinant
        ac_vector ac_detA(1);
        ac_detA[0] = ac_det(ac_A);
        //
        // create function objects for f : A -> detA
        CppAD::ADFun<c_double>            c_f;
        CppAD::ADFun<ac_double, c_double> ac_f;
        c_f.Dependent(ac_A, ac_detA);
        if( global_option["optimize"] )
            c_f.optimize(optimize_options);
        ac_f = c_f.base2ad();
        //
        // declare independent variables for gradient computation
        CppAD::Independent(ac_A, abort_op_index, record_compare);
        //
        // vectors of reverse mode weights
        CppAD::vector<ac_double> ac_w(1);
        ac_w[0] = ac_double(1.0);
        //
        // AD computation of the gradient
        ac_vector ac_gradient(nx);
        ac_f.Forward(0, ac_A);
        ac_gradient = ac_f.Reverse(1, ac_w);
        //
        // create function objects for g : A -> det'( detA  )
        CppAD::ADFun<c_double> c_g;
        c_g.Dependent(ac_A, ac_gradient);
        if( global_option["optimize"] )
            c_g.optimize(optimize_options);
        compiled_fun g_tmp(c_g, "det_minor");
        //
        // static_g
        g.swap(g_tmp);
    }
}

bool link_det_minor(
    const std::string&         job      ,
    size_t                     size     ,
    size_t                     repeat   ,
    CppAD::vector<double>     &matrix   ,
    CppAD::vector<double>     &gradient )
{   CPPAD_ASSERT_UNKNOWN( matrix.size() == size * size );
    CPPAD_ASSERT_UNKNOWN( gradient.size() == size * size );
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
    // function object mapping matrix to gradiend of determinant
    static compiled_fun static_g;
    //
    // size correspmnding to static_g
    static size_t static_size = 0;
    //
    // number of independent variables
    size_t nx = size * size;
    //
    // onetape
    bool onetape = global_option["onetape"];
    // ----------------------------------------------------------------------
    if( job == "setup" )
    {   if( onetape )
        {   setup(static_g, size);
            static_size = size;
        }
        else
        {   static_size = 0;
        }
        return true;
    }
    if( job ==  "teardown" )
    {   compiled_fun g;
        static_g.swap(g);
        return true;
    }
    // -----------------------------------------------------------------------
    CPPAD_ASSERT_UNKNOWN( job == "run" );
    if( onetape ) while(repeat--)
    {   // use if before assert to vaoid warning that static_size is not used
        if( size != static_size )
        {   CPPAD_ASSERT_UNKNOWN( size == static_size );
        }

        // get next matrix
        CppAD::uniform_01(nx, matrix);

        // evaluate the gradient
        gradient = static_g(matrix);
    }
    else while(repeat--)
    {   setup(static_g, size);
        static_size = size;

        // get next matrix
        CppAD::uniform_01(nx, matrix);

        // evaluate the gradient
        gradient = static_g(matrix);
    }
    return true;
}
/* %$$
$end
*/
