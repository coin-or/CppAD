/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:

-------------------------------------------------------------------------- */
/*
$begin compiled_fun$$
$spell
    CppAD
    cppad
    hpp
    cppadcg
    cg
    eval_jac
    jacobian
    enum
$$

$section Compile and Link Source For an AD Function$$

$head Syntax$$
$codei%# include <cppad/example/compiled_fun.hpp>
%$$

$subhead Constructor$$
$codei%compiled_fun %fun_name%()
%$$
$codei%compiled_fun %fun_name%(%file_name%)
%$$
$codei%compiled_fun %fun_name%(%file_name%, %cg_fun%)
%$$
$codei%compiled_fun %fun_name%(%file_name%, %cg_fun%, %eval_jac%)
%$$

$subhead Operations$$
$icode%fun_name%.swap(%other_fun%)
%$$
$icode%y% = %fun_name%(%x%)
%$$
$icode%J% = %fun_name%.jacobian(%x%)
%$$


$head Prototype$$

$subhead Constructors$$
$srcthisfile%
    0%// BEGIN_CTOR_VOID%// END_CTOR_VOID%1
%$$
$srcthisfile%
    0%// BEGIN_CTOR_FILE_NAME%// END_CTOR_FILE_NAME%1
%$$
$srcthisfile%
    0%// BEGIN_CTOR_CG_FUN%// END_CTOR_CG_FUN%1
%$$

$subhead Operations$$
$srcthisfile%
    0%// BEGIN_SWAP_OTHER_FUN%// END_SWAP_OTHER_FUN%1
%$$
$srcthisfile%
    0%// BEGIN_FUN_NAME_X%// END_FUN_NAME_X%1
%$$
$pre

$$

$head CppAD::cg::CG<double>$$
This is the CppAD $icode Base$$ type for the function
$icode cg_fun$$.
It is defined by
$href%https://github.com/joaoleal/CppADCodeGen%CppADCodeGen%$$.
and used to convert the $icode cg_fun$$ function object to source code,
compile the source code, and then link the corresponding function evaluation
$codei%
    %y% = cg_fun.Forward(0, %x%)
%$$.

$head Speed$$
The conversion to source and linking is expect to take a significant
amount of time and the evaluation of the function is expected to be
much faster; see the following speed tests:
$table
$rref cppadcg_det_minor.cpp$$
$rref cppadcg_sparse_jacobian.cpp$$
$tend


$head fun_name$$
This is the name of the $code compiled_fun$$ object.

$head other_fun$$
This is the name of another $code compiled_fun$$ object.

$head file_name$$
This is the absolute or relative path for the
file that contains the dynamic library.
It does not include the files extension at the end that is used
for dynamic libraries on this system.
If $icode cg_fun$$ is not present in the constructor,
it must have been present in a previous constructor with the same
$icode file_name$$.

$head cg_fun$$
This is a CppAD function object that corresponds to a function
$latex f : \B{R}^n \rightarrow \B{R}^m$$.
If this arguments is present in the constructor,
a new dynamic library is created.

$head eval_jac$$
If this argument is present in the constructor,
it determines which type of Jacobian $latex f'(x)$$ will be enabled.
The possible choices for $icode eval_jac$$ are:
$table
$icode eval_jac$$                 $pre  $$ $cnext Available Jacobian
$rnext
$code compiled_fun::none_enum$$   $pre  $$ $cnext none
$rnext
$code compiled_fun::dense_enum$$  $pre  $$ $cnext $icode%fun_name%.jacobian%$$
$tend
The default value for $icode eval_jac$$ is none.

$head swap$$
This exchanges the library in $icode fun_name$$ with the library in
$icode other_fun$$.

$head x$$
is a vector of size $icode n$$ specifying the argument value
at which the function will be evaluated.

$head y$$
This return value has size $icode m$$ and is the value of $latex f(x)$$.

$head J$$
This return value has size $icode%m% * %n%$$ and is the value of
the Jacobian $latex f'(x)$$ where
$latex \[
    J[ i \cdot n + j ] =  ( \partial f_i / \partial x_j )  (x)
\] $$

$children%
    example/compiled_fun/function.cpp%
    example/compiled_fun/jacobian.cpp
%$$
$head Examples$$
$table
$rref compiled_fun_function.cpp$$
$rref compiled_fun_jacobian.cpp$$
$tend

$end
*/

# include <cppad/example/compiled_fun.hpp>

// ---------------------------------------------------------------------------
// compiled_fun fun_name(file_name, cg_name, eval_jac)
//
// BEGIN_CTOR_CG_FUN
compiled_fun::compiled_fun(
    const std::string&                     file_name  ,
    CppAD::ADFun< CppAD::cg::CG<double> >& cg_fun     ,
    evaluation_enum                        eval_jac   )
// END_CTOR_CG_FUN
{   // Generate source code
    CppAD::cg::ModelCSourceGen<double> cgen(cg_fun, "model");
    switch(eval_jac)
    {   case none_enum:
        break;

        case dense_enum:
        cgen.setCreateJacobian(true);
        break;

        case sparse_enum:
        CPPAD_ASSERT_KNOWN( false,
            "compiled_fun::sparse_jacobian not yet implemented"
        );
        break;
    }
    CppAD::cg::ModelLibraryCSourceGen<double> libcgen(cgen);

    // Compile source, create the library file, and load the library
    CppAD::cg::DynamicModelLibraryProcessor<double> proc(libcgen, file_name);
    CppAD::cg::ClangCompiler<double> compiler;
    bool loadLib = true;
    dynamic_lib_ = proc.createDynamicLibrary(compiler, loadLib);
    //
    // create the model object
    model_        = dynamic_lib_->model("model");
}
// ---------------------------------------------------------------------------
// compiled_fun fun_name(file_name)
//
// BEGIN_CTOR_FILE_NAME
compiled_fun::compiled_fun(const std::string&  file_name )
// END_CTOR_FILE_NAME
{   // file name plus extension used for dynamic libraries on this system
    std::string file_name_ext = file_name +
        CppAD::cg::system::SystemInfo<>::DYNAMIC_LIB_EXTENSION;

    // load the library
    CppAD::cg::DynamicLib<double>* ptr =
        new CppAD::cg::LinuxDynamicLib<double>(file_name_ext);
    dynamic_lib_  = std::unique_ptr< CppAD::cg::DynamicLib<double> >(ptr);
    //
    // create the model object
    model_        = dynamic_lib_->model("model");
}
// ---------------------------------------------------------------------------
// compiled_fun fun_name
//
// BEGIN_CTOR_VOID
compiled_fun::compiled_fun(void)
// END_CTOR_VOID
{ }
// --------------------------------------------------------------------------
// fun_name.swap(other_fun)
//
// BEGIN_SWAP_OTHER_FUN
void compiled_fun::swap(compiled_fun& other_fun)
// END_SWAP_OTHER_FUN
{   std::swap(dynamic_lib_, other_fun.dynamic_lib_);
    std::swap(model_, other_fun.model_ );
}
// --------------------------------------------------------------------------
// y = fun_name(x)
//
// BEGIN_FUN_NAME_X
CppAD::vector<double>
compiled_fun::operator()(const CppAD::vector<double>& x)
// END_FUN_NAME_X
{   return model_->ForwardZero(x);
}
// --------------------------------------------------------------------------
// J = fun_name.jacobian(x)
//
// BEGIN_JACOBIAN
CppAD::vector<double>
compiled_fun::jacobian(const CppAD::vector<double>& x)
// END_JACOBIAN
{   CPPAD_ASSERT_KNOWN( model_->isJacobianAvailable() ,
        "compiled_fun: dense jacobian not enables during constructor"
    );
    return model_-> Jacobian(x);
}
