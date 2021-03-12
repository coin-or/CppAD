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
$begin llvm_from_to_graph.cpp$$

$section LLVM To and From C++ AD Graph Examples$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <limits>
# include <cppad/cppad.hpp>
# include <cppad/core/llvm_ir.hpp>
namespace {
    template <class VectorFloat>
    VectorFloat algo(const VectorFloat& p, const VectorFloat& x)
    // END_PROTOTYPE
    {   size_t nx = x.size();
        size_t np = p.size();
        size_t ny = nx + np;
        //
        // define the actual function
        VectorFloat y(ny);
        for(size_t i = 0; i < np; ++i)
            y[i] = p[i] + double(i);
        for(size_t i = 0; i < nx; ++i)
            y[i + np] = x[i] + double(i + np);
        //
        // some operations that optimizer should remove
        typename VectorFloat::value_type sum = 0.0;
        for(size_t i = 0; i < ny; i++)
            sum += y[i];
        //
        return y;
    }
    void algo2adfun(size_t np, size_t nx, CppAD::ADFun<double>& adfun)
    {   using CppAD::AD;
        //
        // ap, ax
        CPPAD_TESTVECTOR( AD<double> ) ap(np), ax(nx);
        for(size_t i = 0; i < np; ++i)
            ap[i] = AD<double>( i + 1 );
        for(size_t i = 0; i < nx; ++i)
            ax[i] = AD<double>( i + np );
        //
        // ax independent variables,  ap dynamic parameters
        CppAD::Independent(ax, ap);
        //
        // ay
        CPPAD_TESTVECTOR( AD<double> ) ay = algo(ap, ax);
        //
        // f : x -> y
        adfun.Dependent(ax, ay);
        //
        // function_name
        adfun.function_name_set("from_to_graph");
        return;
    }
}

bool from_to_graph(void)
{   bool ok = true;
    //
    // np, nx
    size_t np = 1;
    size_t nx = 2;
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
    CPPAD_TESTVECTOR(double) p(np), x(nx);
    for(size_t i = 0; i < np; ++i)
        p[i] = std::rand() / rand_max;
    for(size_t i = 0; i < nx; ++i)
        x[i] = std::rand() / rand_max;
    //
    // y = g(p; x)
    g.new_dynamic(p);
    CPPAD_TESTVECTOR(double) y = g.Forward(0, x);
    //
    // check
    double         eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    CPPAD_TESTVECTOR(double) check = algo(p, x);
    size_t         ny    = g.Range();
    ok &= check.size() == ny;
    ok &= f.Range()    == ny;
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(y[i], check[i], eps99, eps99);
    //
    return ok;
}
// END C++
