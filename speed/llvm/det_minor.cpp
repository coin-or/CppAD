/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin llvm_det_minor.cpp$$
$spell
$$

$section llvm Speed: Gradient of Determinant by Minor Expansion$$

$head Specifications$$
See $cref link_det_minor$$.

$head PASS_JACOBIAN_TO_CODE_GEN$$
If this is one, the Jacobian of the determinant is the function passed
to CppADCodeGen.  In this case,  the $code code_gen_fun$$
$cref/function/code_gen_fun/Syntax/function/$$ is used to calculate
the Jacobian of the determinant.
Otherwise, this flag is zero and the determinant function is passed
to CppADCodeGen. In this case, the $code code_gen_fun$$
$cref/jacobian/code_gen_fun/Syntax/jacobian/$$ is used to calculate
the Jacobian of the determinant.
$srccode%cpp% */
# define PASS_JACOBIAN_TO_CODE_GEN 1
/* %$$

$head Implementation$$
$srccode%cpp% */
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

# include <cppad/cppad.hpp>
# include <cppad/core/llvm/ir.hpp>
# include <cppad/core/llvm/link.hpp>
# include <cppad/core/llvm/function_ptr.hpp>
# include <cppad/core/llvm/compiled_ir.hpp>

# include <map>
extern std::map<std::string, bool> global_option;

namespace {
    // a_vector
    typedef CppAD::AD<double>       a_double;
    typedef CppAD::vector<a_double> a_vector;
    //
    // function_name
    std::string function_name = "speed_det_minor";
    //
    // setup
    std::string setup( // error_msg
        // inputs
        size_t size                ,
        // outputs
        CppAD::llvm_link& link_obj )
    {   std::string msg;
        //
        // object for computing determinant
        CppAD::det_by_minor<a_double>   a_det(size);
        //
        // number of independent variables
        size_t nx = size * size;
        //
        // choose a matrix
        CppAD::vector<double> matrix(nx);
        CppAD::uniform_01(nx, matrix);
        //
        // copy to independent variables
        a_vector   aA(nx);
        for(size_t j = 0; j < nx; ++j)
            aA[j] = matrix[j];
        //
        // declare independent variables for function computation
        bool record_compare   = false;
        size_t abort_op_index = 0;
        CppAD::Independent(aA, abort_op_index, record_compare);
        //
        // AD computation of the determinant
        a_vector adetA(1);
        adetA[0] = a_det(aA);
        //
        // create function objects for f : A -> detA
        CppAD::ADFun<double> f;
        f.Dependent(aA, adetA);
        //
        // create AD verison of f
        CppAD::ADFun<a_double, double> af;
        af = f.base2ad();
        //
        // declare independent variables for gradient computation
        CppAD::Independent(aA, abort_op_index, record_compare);
        //
        // vectors of reverse mode weights
        CppAD::vector<a_double> aw(1);
        aw[0] = 1.0;
        //
        // AD computation of the gradient
        a_vector agradient(nx);
        af.Forward(0, aA);
        agradient = af.Reverse(1, aw);
        //
        // create function objects for g : A -> det'( detA  )
        CppAD::ADFun<double> g;
        g.Dependent(aA, agradient);
        g.function_name_set(function_name);
        //
        // ir_obj
        CppAD::cpp_graph graph_obj;
        g.to_graph(graph_obj);
        CppAD::llvm_ir ir_obj;
        msg = ir_obj.from_graph(graph_obj);
        if( msg != "" )
            return msg;
        //
        // optimize ?
        if( global_option["optimize"] )
            ir_obj.optimize();
        //
        // convert to object file
        std::string file_name = "llvm_" + function_name + ".o";
        msg = ir_obj.to_object_file(file_name);
        if( msg != "" )
            return msg;
        //
        // link_obj
        msg = link_obj.initialize();
        if( msg != "" )
            return msg;
        msg = link_obj.object_file(file_name);
        //
        // msg == ""
        return msg;
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
    // object containing function mapping matrix to gradient of determinant
    static CppAD::llvm_link     static_link_obj;
    static CppAD::compiled_ir_t static_fun_ptr;
    //
    // size corresponding to link_obj
    static size_t static_size = 0;
    //
    // number of independent variables
    size_t nx = size * size;
    //
    // onetape
    bool onetape = global_option["onetape"];
    //
    // msg
    std::string msg;
    //
    // len_message, message
    int32_t len_message = 1;
    CppAD::vector<char> message(len_message);
    // ----------------------------------------------------------------------
    if( job == "setup" )
    {   if( onetape )
        {
# ifndef NDEBUG
            msg = setup(size, static_link_obj);
            CPPAD_ASSERT_KNOWN( msg == "", msg.c_str() );
            msg = static_link_obj.function_ptr(function_name, static_fun_ptr);
            CPPAD_ASSERT_KNOWN( msg == "", msg.c_str() );
# else
            setup(size, static_link_obj);
            static_link_obj.function_ptr(function_name, static_fun_ptr);
# endif
            static_size = size;
        }
        else
        {   static_size = 0;
        }
        return true;
    }
    if( job ==  "teardown" )
    {   static_link_obj.initialize();
        return true;
    }
    // -----------------------------------------------------------------------
    CPPAD_ASSERT_UNKNOWN( job == "run" );
    if( onetape ) while(repeat--)
    {   // use if before assert to avoid warning that static_size is not used
        if( size != static_size )
        {   CPPAD_ASSERT_UNKNOWN( size == static_size );
        }

        // get next matrix
        CppAD::uniform_01(nx, matrix);

        // evaluate the gradient
        int32_t len_input   = static_cast<int32_t>(nx);
        int32_t len_output  = static_cast<int32_t>(nx);
# ifndef NDEBUG
        int32_t error_no =
# endif
        static_fun_ptr(
            len_input,   matrix.data()   ,
            len_output,  gradient.data() ,
            len_message, message.data()
        );
        CPPAD_ASSERT_UNKNOWN( error_no == 0 );
    }
    else while(repeat--)
    {
# ifndef NDEBUG
        msg = setup(size, static_link_obj);
        CPPAD_ASSERT_KNOWN( msg == "", msg.c_str() );
        msg = static_link_obj.function_ptr(function_name, static_fun_ptr);
        CPPAD_ASSERT_KNOWN( msg == "", msg.c_str() );
# else
        setup(size, static_link_obj);
        static_link_obj.function_ptr(function_name, static_fun_ptr);
# endif
        static_size = size;

        // get next matrix
        CppAD::uniform_01(nx, matrix);

        // evaluate the gradient
        int32_t len_input   = static_cast<int32_t>(nx);
        int32_t len_output  = static_cast<int32_t>(nx);
# ifndef NDEBUG
        int32_t error_no =
# endif
        static_fun_ptr(
            len_input,   matrix.data()   ,
            len_output,  gradient.data() ,
            len_message, message.data()
        );
        CPPAD_ASSERT_UNKNOWN( error_no == 0 );
    }
    return true;
}
/* %$$
$end
*/
