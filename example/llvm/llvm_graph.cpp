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
$begin llvm_graph.cpp$$

$section LLVM To and From C++ AD Graph Examples$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <limits>
//
# include "algo.hpp"
# include "algo2adfun.hpp"
# include "llvm_ir.hpp"

bool llvm_graph(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // np, nx
    size_t np = 1;
    size_t nx = 2;
    //
    // f
    CppAD::ADFun<double> f;
    ok  &= algo2adfun(np, nx, f);
    //
    // x, p
    vector<double> x(nx), p(np);
    for(size_t i = 0; i < np; ++i)
        p[i] = double(i + 4);
    for(size_t i = 0; i < nx; ++i)
        x[i] = double(i + np + 4);
    vector<double> check = algo(p, x);
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
        ok = false;
    }
    ir_obj.print();
    //
    return ok;
}
// END C++
