/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <limits>
# include <cppad/cppad.hpp>
# include <cppad/core/llvm_ir.hpp>
# include <cppad/core/llvm_link.hpp>
namespace { // BEGIN_EMPTY_NAMESPACE
// -----------------------------------------------------------------------------
// algo
template <class VectorFloat>
VectorFloat algo(const VectorFloat& p, const VectorFloat& x)
// END_PROTOTYPE
{   size_t nx = x.size();
    size_t np = p.size();
    size_t ny = nx + np;
    typedef typename VectorFloat::value_type value_type;
    //
    // number of operator: add, sub, mul, div
    size_t n_operator = 4;
    //
    // operation counter mod n_operator
    size_t operation_count = 0;
    //
    // define the actual function
    VectorFloat y(ny);
    for(size_t i = 0; i < ny; ++i)
    {   value_type arg;
        if( i < np )
            arg = p[i];
        else
            arg = x[i-np];
        operation_count = (operation_count + 1) % n_operator;
        switch( operation_count )
        {   case 1:
            y[i] = arg + double(i);
            break;

            case 2:
            y[i] = double(i) - arg;

            case 3:
            y[i] = arg * double(i);

            case 4:
            y[i] = double(i) / arg;
        }
    }
    //
    // some operations that optimizer should get removed by optimizer
    typename VectorFloat::value_type sum = 0.0;
    for(size_t i = 0; i < ny; i++)
        sum += y[i];
    //
    return y;
}
// -----------------------------------------------------------------------------
// algo2adfun
void algo2adfun(size_t np, size_t nx, CppAD::ADFun<double>& adfun)
{   using CppAD::AD;
    using CppAD::vector;
    //
    // ap, ax
    vector< AD<double> > ap(np), ax(nx);
    for(size_t i = 0; i < np; ++i)
        ap[i] = AD<double>( i + 1 );
    for(size_t i = 0; i < nx; ++i)
        ax[i] = AD<double>( i + np );
    //
    // ax independent variables,  ap dynamic parameters
    CppAD::Independent(ax, ap);
    //
    // ay
    vector< AD<double> > ay = algo(ap, ax);
    //
    // f : x -> y
    adfun.Dependent(ax, ay);
    //
    // function_name
    adfun.function_name_set("llvm_algo");
    return;
}
// -----------------------------------------------------------------------------
// tst_llvm_ir
bool tst_llvm_ir(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // np, nx
    size_t np = 1;
    size_t nx = 3;
    //
    // f
    CppAD::ADFun<double> f;
    algo2adfun(np, nx, f);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimize
    ir_obj.optimize();
    //
    // graph_obj
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // g
    CppAD::ADFun<double> g;
    g.from_graph(graph_obj);
    // ---------------------------------------------------------------------
    // Check zero order forward mode for g
    //
    // rand_max
    double rand_max = double(RAND_MAX);
    //
    // x, p
    vector<double> p(np), x(nx);
    for(size_t i = 0; i < np; ++i)
        p[i] = std::rand() / rand_max;
    for(size_t i = 0; i < nx; ++i)
        x[i] = std::rand() / rand_max;
    //
    // y = g(p; x)
    g.new_dynamic(p);
    vector<double> y = g.Forward(0, x);
    //
    // check
    double         eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    vector<double> check = algo(p, x);
    size_t         ny    = g.Range();
    ok &= check.size() == ny;
    ok &= f.Range()    == ny;
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(y[i], check[i], eps99, eps99);
    //
    return ok;
}
// tst_llvm_link
bool tst_llvm_link(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::llvm_ir;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    size_t np = 1;
    size_t nx = 3;
    CppAD::ADFun<double> f;
    algo2adfun(np, nx, f);
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
    // bin/test_one.sh.in assumes file_name matchees "llvm_*.o"
    std::string file_name = function_name + ".o";
    ir_obj.to_object_file(file_name);
    //
    // link_obj
    CppAD::llvm_link link_obj;
    //
    // load file
    link_obj.load(file_name);
    //
    // function_ptr
    CppAD::llvm_compiled_t function_ptr;
    msg = link_obj.compiled(function_name, function_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    CppAD::vector<double> input(np + nx);
    for(size_t i = 0; i < np + nx; ++i)
        input[i] = double(i) + 4.0;
    //
    // vector to hold return value
    size_t ny = f.Range();
    CppAD::vector<double> output(ny);
    for(size_t i = 0; i < ny; ++i)
        output[i] = 0.0;
    //
    // incorrect call to function
    int32_t len_input   = 0;
    int32_t len_output  = 0;
    int32_t error_no    = function_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no != 0;
    for(size_t i = 0; i < ny; ++i)
        ok &= output[i] == 0.0;
    //
    // correct call function
    len_input   = static_cast<int32_t>(np + nx);
    len_output  = static_cast<int32_t>(ny);
    error_no    = function_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no == 0;
    //
    // check output
    CppAD::vector<double> p(np), x(nx), check(ny);
    for(size_t i = 0; i < np; ++i)
        p[i] = input[i];
    for(size_t i = 0; i < nx; ++i)
        x[i] = input[np + i];
    check = algo(p, x);
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(output[i], check[i], eps99, eps99);
    //
    return ok;
}

} // END_EMPTY_NAMESPACE

bool llvm_tst(void)
{   bool ok = true;
    ok     &= tst_llvm_ir();
    ok     &= tst_llvm_link();
    return ok;
}
