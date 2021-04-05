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
$begin llvm_link_atomic.cpp$$
$spell
    cpp
    ir
$$

$section Example Linking an Atomic AD Function$$

$head atomic.cpp$$
The prototype for this function is the same as for
$code compiled_ir_t$$ functions; see
$cref/atomic functions/compiled_ir_t/Atomic Functions/$$:
$srcfile%example/llvm/atomic.cpp%0%// BEGIN C++%// END C++%0%$$


$head This File$$
$srcthisfile%0%// BEGIN C++%// END C++%2%$$
$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/core/llvm/ir.hpp>
# include <cppad/core/llvm/link.hpp>


namespace { // BEGIN_EMPTY_NAMESPACE
using CppAD::vector;
// BEGIN_ATOMIC_RECIPROCAL
class atomic_reciprocal : public CppAD::atomic_three<double> {
public:
    atomic_reciprocal(const std::string& name) :
    CppAD::atomic_three<double>(name)
    { }
private:
    virtual bool for_type(
        const vector<double>&               parameter_x ,
        const vector<CppAD::ad_type_enum>&  type_x      ,
        vector<CppAD::ad_type_enum>&        type_y      )
    {   assert( parameter_x.size() == type_x.size() );
        bool ok = type_x.size() == 1; // n
        ok     &= type_y.size() == 1; // m
        if( ! ok )
            return false;
        type_y[0] = type_x[0];
        return true;
    }
    virtual bool forward(
        const vector<double>&               parameter_x  ,
        const vector<CppAD::ad_type_enum>&  type_x       ,
        size_t                              need_y       ,
        size_t                              order_low    ,
        size_t                              order_up     ,
        const vector<double>&               taylor_x     ,
        vector<double>&                     taylor_y     )
    {
# ifndef NDEBUG
        size_t n = taylor_x.size() / (order_up + 1);
        size_t m = taylor_y.size() / (order_up + 1);
# endif
        assert( n == 1 );
        assert( m == 1 );
        assert( order_low <= order_up );

        // return flag
        bool ok = order_up == 0;
        if( ! ok )
            return ok;

        // Order zero forward mode.
        // This case must always be implemented
        // y^0 = g( x^0 ) = 1 / x^0
        taylor_y[0] = 1. / taylor_x[0];
        //
        return ok;
    }
}; // END_ATOMIC_RECIPROCAL
} // END_EMPTY_NAMESPACE


bool link_atomic(void)
{   bool ok = true;
    using CppAD::llvm_ir;
    using CppAD::AD;
    //
    atomic_reciprocal afun("reciprocal");
    //
    size_t nx = 1, ny = 1;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
    ax[0] = 1.0;
    CppAD::Independent(ax);
    afun(ax, ay);
    CppAD::ADFun<double> f(ax, ay);
    f.function_name_set("link_atomic");
    //
    // create a cpp_graph from this function
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    std::string function_name = graph_obj.function_name_get();
    //
    // llvm_ir object
    llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // create object file
    std::string adfun_obj_file = "llvm_" + function_name + ".o";
    msg = ir_obj.to_object_file(adfun_obj_file);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // link_obj
    CppAD::llvm_link link_obj(msg);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load object file
    msg = link_obj.object_file(adfun_obj_file);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // The function atomic_reciprocal is in the the following library
    std::string atomic_lib = "libatomic.so";
    //
    // load library
    msg = link_obj.dynamic_lib(atomic_lib);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    std::vector<double> input(nx);
    //
    // vector to hold return value
    CppAD::vector<double> output(ny);
    output[0] = std::numeric_limits<double>::quiet_NaN();
    //
    // call
    int32_t error_no;
    int32_t len_input   = static_cast<int32_t>(nx);
    int32_t len_output  = static_cast<int32_t>(ny);
    //
    // reciprocal(2.0)
    input[0] = 2.0;
    error_no = fun_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no == 0;
    ok &= output[0] == 0.5;
    //
    return ok;
}
// END C++
