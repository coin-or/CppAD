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
$begin cppadcg_det_minor_cg.cpp$$
$spell
    Cppadcg
    typedef
    cppad
    CppAD
    det
    hpp
    const
    bool
    std
    cg
$$

$section Cppadcg Speed: Source Generation: Gradient of Determinant by Minor$$

$head Syntax$$
$icode%flag% = ./det_minor_cg %size% [--optimize]%$$

$head Purpose$$
This program generates C++ source code that computes the derivative of the
determinant of a square matrix.

$head size$$
Is a positive integer specifying the
row and column dimension of the matrix.

$head --optimize$$
The command line argument is optional.
If it is present,
the AD function representing the determinant be optimized
before computing its derivative.

$head flag$$
If source code generation completes successfully,
the integer value zero is returned.

$head det_minor_grad.c$$
The source code is written to the file
$code det_minor_grad.c$$ in the current working directory.
The corresponding function call has the following syntax:
$icode%
     %flag% = det_minor_grad(%size%, %x%, %y%)
%$$
see $cref cppadcg_det_minor_grad.c$$.

$head Implementation$$
$srccode%cpp% */
# include <cppad/cg/cppadcg.hpp>
# include <cppad/speed/det_by_minor.hpp>

int main(int argc, char* argv[] )
{   char* usage =
    "flag = ./det_minor_cg size [--optimize]\n"
    "The integer size > 0 is the row and column dimension of the matrix.\n"
    "If --optimize is present, the AD function is optimized before\n"
    "the derivative is computed and source code generated.\n"
    ;
    if( argc < 2 || 3 < argc )
    {   std::cerr << usage;
        std::exit(1);
    }
    int size_int = std::atoi(argv[1]);
    if( size_int <= 0 )
    {   std::err << usage;
        std::exit(1);
    }
    bool optimize = false;
    if( argc == 3 )
    {   if( std::strcmp( argv[2] , "--optimize") != 0 )
        {   std::err << usage;
            std::exit(1);
        }
        optimize = true;
    }

    // --------------------------------------------------------------------
    // optimization options: no conditional skips or compare operators
    std::string optimize_options =
        "no_conditional_skip no_compare_op no_print_for_op";
    // -----------------------------------------------------
    // setup

    // object for computing determinant
    typedef CppAD::cg::CG<double>    c_double;
    typedef CppAD::AD<c_double>      ac_double;
    typedef CppAD::vector<c_double>  c_vector;
    typedef CppAD::vector<ac_double> ac_vector;
    CppAD::det_by_minor<ac_double>   ac_det(size);

    size_t nd = 1;           // number of dependent variables in determinant
    size_t nx = size * size; // number of independent variables
    ac_vector   ac_A(nx);    // determinant domain space vector
    ac_vector   ac_detA(nd); // determinant range space vector


    // the AD function object
    CppAD::ADFun<c_double> c_f;

    // do not even record comparison operators

    // ---------------------------------------------------------------------
    // values of independent variables do not matter
    for(size_t i = 0; i < nx; i++)
        ac_A[i] = ac_double( double(i) / double(nx) );

    // declare independent variables without comparison operators
    size_t abort_op_index = 0;
    bool record_compare   = false;
    Independent(ac_A, abort_op_index, record_compare);

    // AD computation of the determinant
    ac_detA[0] = ac_det(ac_A);

    // create function object f : A -> detA
    c_f.Dependent(ac_A, ac_detA);

    if( optimize )
        c_f.optimize(optimize_options);

    // source code generator used for det_minor_grad(x) = d/dx f(x)
    CppAD::cg::CodeHandler<double> code_handler;

    // declare the independent variables in det_minor_grad
    c_vector c_x(nx);
    code_handler.makeVariables(c_x);

    // declare the dependent variables in det_minor_grad
    size_t ny = nd * nx;
    c_vector c_y(ny);

    // evaluate the determinant as a function of c_x
    c_f.Forward(0, c_x);

    // evaluate the gradient using reverse mode
    CppAD::vector<c_double> c_w(nd);
    c_w[0] = c_double(1.0);
    c_y    = c_f.Reverse(1, c_w);

    // Mapping from variables in this program to variables in source_code
    // independent variable = x
    // dependent variable   = y
    // temporary variable   = v
    CppAD::cg::LanguageC<double> langC("double");
    CppAD::cg::LangCDefaultVariableNameGenerator<double> nameGen;

    // generate the source code
    std::ostringstream source_code;
    code_handler.generateCode(source_code, langC, c_y, nameGen);

    // number of temporary variables
    size_t nv = code_handler.getTemporaryVariableCount();

    // wrap the string generated by code_handler into a function
    // y = det_minor_grad(x)
    std::string source_str =
        "int det_minor_grad(int size, const double* x, double* y) {\n";
    source_str += "\tif( size != " + CppAD:to_string(nx) + ");\n";
    source_str += "\t\treturn 1;"
    source_str += "\tdouble v[" + CppAD::to_string(nv) + "];\n";
    source_str += "// Begin code generated by CppADCodeGen\n";
    source_str += source_code.str();
    source_str +=
        "// End code generated by CppADCodeGen\n"
        "\treturn 0;\n"
        "}\n"
    ;
    // write source code to det_minor_grad.cpp
    std::fstream fs;
    fs.open("det_minor_grad.c", std::fstream::out);
    fs << source_str;
    fs.close();

    // return flag
    return 0;
}
/* %$$
$end
*/
