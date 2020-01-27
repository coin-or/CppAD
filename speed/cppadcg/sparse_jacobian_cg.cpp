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
$begin cppadcg_sparse_jacobian_cg.cpp$$
$spell
    const
    namespace
    work work
    jac
    CppAD
    cppad
    cppadcg
    cg
    hpp
    bool
    typedef
    endif
    std
    cout
    endl
    Jacobian
    nnz
    subgraph
    vec
$$

$section Cppadcg Speed: Source Generation: Sparse Jacobian$$

$head Syntax$$
$codei%sparse_jacobian_cg(%size_vec%)%$$

$head Purpose$$
This program generates C++ source code that computes the Jacobian of the
function $cref sparse_jac_fun$$.

$head n_size$$
The positive integer $icode n_size$$
is the size of the vector $icode size_vec$$.
This is the number of sizes that the source code is generated for.

$head size_vec$$
For $icode%i% = 1, %...%, %n_size%$$,
$icode%size_vec%[%i%]%$$ is a positive integer specifying the
dimension of the domain space for the function.

$head sparse_jacobian.c$$
The source code is written to the file
$code sparse_jacobian.c$$ in the current working directory.
The corresponding function call has the following syntax:
$codei%
     %flag% = sparse_jacobian_c(
            %subgraph%, %optimize%, %size%, %nnz%, %x%, %y%
    )
%$$
see $cref cppadcg_sparse_jacobian.c$$.

$head Assumptions$$
The row vector $icode row$$ and column vector $icode col$$
that define the sparsity pattern can be determined by calling
$codei%
    choose_row_col_sparse_jacobian(%n%, %m%, %row%, %col%)
%$$
where $icode n$$ is the size and $icode m$$ is $codei%2*%n%$$.


$head Implementation$$
$srccode%cpp% */
# include <cppad/cg/cppadcg.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>
//
extern void choose_row_col_sparse_jacobian(
    size_t n, size_t m, CppAD::vector<size_t>& row, CppAD::vector<size_t>& col
);

namespace {
    using CppAD::vector;
    typedef vector<size_t>          s_vector;
    typedef vector<bool>            b_vector;
    typedef CppAD::cg::CG<double>   c_double;
    typedef CppAD::AD<c_double>     ac_double;
    typedef vector<c_double>        c_vector;
    typedef vector<ac_double>       ac_vector;

    void calc_sparsity(
        CppAD::sparse_rc<s_vector>& sparsity ,
        CppAD::ADFun<c_double>&     c_f      )
    {   bool transpose     = false;
        bool internal_bool = false;
        bool dependency    = false;
        size_t n = c_f.Domain();
        //
        CppAD::sparse_rc<s_vector> identity;
        identity.resize(n, n, n);
        for(size_t k = 0; k < n; k++)
            identity.set(k, k, k);
        //
        c_f.for_jac_sparsity(
            identity, transpose, dependency, internal_bool, sparsity
        );
    }
}

void sparse_jacobian_cg(const CppAD::vector<size_t>& size_vec )
{
    //
    // optimization options: no conditional skips or compare operators
    std::string optimize_options =
        "no_conditional_skip no_compare_op no_print_for_op";
    //
    // Open file sparse_jacobian.c were souce code will be written
    std::fstream fs;
    fs.open("sparse_jacobian.c", std::fstream::out);
    //
    // -----------------------------------------------------------------------
    // loop over sizes, optimize, subgraph
    // -----------------------------------------------------------------------
    size_t n_size = size_vec.size();
    for(size_t i = 0; i < n_size; ++i)
    for(size_t opt = 0; opt < 2; ++opt)
    for(size_t sub = 0; sub < 2; ++sub)
    {
        size_t n     = size_vec[i]; // number of independent variables
        size_t m     = 2 * n;       // number of dependent variables
        //
        // determine row and colunn vectors in sparsity pattern
        s_vector row, col;
        choose_row_col_sparse_jacobian(n, m, row, col);
        //
        ac_vector ac_x(n);          // AD domain space vector
        ac_vector ac_y(m);          // AD range space vector y = f(x)
        CppAD::ADFun<c_double> cf;  // AD function object
        //
        // declare sparsity pattern
        CppAD::sparse_rc<s_vector>  sparsity;
        //
        // declare subset where Jacobian is evaluated
        // equal to entire sparsity pattern and using c_double
        CppAD::sparse_rc<s_vector> subset_pattern;
        size_t nr  = m;
        size_t nc  = n;
        size_t nnz = row.size();
        subset_pattern.resize(nr, nc, nnz);
        for(size_t k = 0; k < nnz; k++)
            subset_pattern.set(k, row[k], col[k]);
        CppAD::sparse_rcv<s_vector, c_vector> c_subset( subset_pattern );
        const c_vector& c_subset_val( c_subset.val() );
        //
        // coloring method
        std::string coloring = "cppad";
        //
        // maximum number of colors at once
        size_t group_max = 1;
        //
        // do not even record comparison operators
        size_t abort_op_index = 0;
        bool record_compare   = false;
        //
        // values of independent variables do not matter
        for(size_t j = 0; j < n; j++)
            ac_x[j] = ac_double( double(j) / double(n) );
        //
        // declare independent variables
        CppAD::Independent(ac_x, abort_op_index, record_compare);
        //
        // AD computation of f(x) (order zero derivative is function value)
        size_t order = 0;
        CppAD::sparse_jac_fun<ac_double>(m, n, ac_x, row, col, order, ac_y);
        //
        // create function object f : x -> y
        CppAD::ADFun<c_double> c_f;
        c_f.Dependent(ac_x, ac_y);
        //
        if( opt == 1 )
            c_f.optimize(optimize_options);
        //
        // source code generator used for sparse_jacobian_c(x) = d/dx f(x)
        CppAD::cg::CodeHandler<double> code_handler;
        //
        // declare the independent variables in sparse_jacobian_c
        c_vector c_x(n);
        //
        // evaluate sparse sparse jacobian as a function of c_x
        if( sub == 1 )
        {   // user reverse mode becasue forward not yet implemented
            c_f.subgraph_jac_rev(c_x, c_subset);
        }
        else
        {
            // calculate the Jacobian sparsity pattern for this function
            calc_sparsity(sparsity, c_f);
            //
            // structure that holds some of the work done by sparse_jac_for
            CppAD::sparse_jac_work work;
            //
            // calculate the Jacobian at this x
            c_f.sparse_jac_for(
                group_max, c_x, c_subset, sparsity, coloring, work
            );
        }
        //
        // set the dependent variables in sparse_jacobiain(x)
        c_vector c_y(nnz);
        for(size_t k = 0; k < nnz; k++)
            c_y[k] = c_subset_val[k];
        //
        // Mapping from variables in this program to variables in source_code
        // independent variable = x
        // dependent variable   = y
        // temporary variable   = v
        CppAD::cg::LanguageC<double> langC("double");
        CppAD::cg::LangCDefaultVariableNameGenerator<double> nameGen;
        //
        // generate the source code
        std::ostringstream source_code;
        code_handler.generateCode(source_code, langC, c_y, nameGen);
        //
        // number of temporary variables
        size_t nv = code_handler.getTemporaryVariableCount();
        //
        // wrap the string generated by code_handler into a function
        // sparse_jacobian_<size>[_opt][_sub](x, y)
        std::string name = "sparse_jacobian_" + CppAD::to_string(size_vec[i]);
        if( opt == 1 )
            name += "_opt";
        if( sub == 1 )
            name += "_sub";
        std::string source_str;
        source_str +=
            "# include <assert.h>\n"
            "// " + name + "\n"
            "static void " + name + "(int nnz, const double* x, double* y)\n"
            "{\n"
            "\tassert( nnz == " + CppAD::to_string(nnz) + " );\n"
        ;
        if( nv > 0 )
            source_str += "\tdouble v[" + CppAD::to_string(nv) + "];\n";
        source_str += "// Begin code generated by CppADCodeGen\n";
        source_str += source_code.str();
        source_str +=
            "// End code generated by CppADCodeGen\n"
            "}\n"
        ;
        fs << source_str;
    }
    //
    // sparse_jacobian_c(subgraph, optimize, size, nnz, x, y)
    fs <<
    "\nint sparse_jacobian_c(\n"
    "\tint subgraph    ,\n"
    "\tint optimize    ,\n"
    "\tint size        ,\n"
    "\tint nnz         ,\n"
    "\tconst double* x ,\n"
    "\tdouble* y       )\n"
    "{\tswitch( size )\n"
    "\t{\n"
    ;
    for(size_t i = 0; i < n_size; ++i)
    {   std::string size_i = CppAD::to_string(size_vec[i]);
        fs << "\t\tcase " + size_i + ":\n";
        fs << "\t\tif( optimize )\n";
        fs << "\t\t{\n";
        fs << "\t\t\tif( subgraph)\n";
        fs << "\t\t\t\tsparse_jacobian_" + size_i + "_opt_sub(nnz, x, y);\n";
        fs << "\t\t\telse\n";
        fs << "\t\t\t\tsparse_jacobian_" + size_i + "_opt(nnz, x, y);\n";
        fs << "\t\t}\n";
        fs << "\t\telse\n";
        fs << "\t\t{\n";
        fs << "\t\tif( subgraph)\n";
        fs << "\t\t\tsparse_jacobian_" + size_i + "_sub(nnz, x, y);\n";
        fs << "\t\telse\n";
        fs << "\t\t\tsparse_jacobian_" + size_i + "(nnz, x, y);\n";
        fs << "\t\t}\n";
        fs << "\t\tbreak;\n\n";
    }
    fs <<
    "\t\tdefault:\n"
    "\t\treturn 1;\n"
    "\t}\n"
    "\treturn 0;\n"
    "}\n"
    ;
    fs.close();
    //
    return;
}
/* %$$
$end
*/
