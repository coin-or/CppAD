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
$begin graph2llvm_ir$$
$spell
    llvm_ir
    obj
    vec
    op
$$

$section Converting a C++ AD Graph to LLVM Intermediate Representation$$

$head Syntax$$
$icode%ok% = graph2llvm_ir(%graph_obj%, %context_ir%, %module_ir%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head graph_obj$$
This a the $cref cpp_ad_graph$$ corresponding the function
that is converted to llvm intermediate representation.

$head context_ir$$
module input value of this argument does not matter and will be lost.
Upon return, $icode context_ir$$ is the llvm context for the
intermediate representation.

$head module_ir$$
The input value of this argument does not matter and will be lost.
Upon return, $icode module_ir$$ is the llvm module for the
intermediate representation.

$head Restrictions$$
The following limitations are placed on $icode graph_obj$$
(and expected to be removed in the future).
$list number$$
$cref/discrete_name_vec/cpp_ad_graph/discrete_name_vec/$$ must be empty.
$lnext
$cref/atomic_name_vec/cpp_ad_graph/atomic_name_vec/$$ must be empty.
$lnext
$cref/print_text_vec/cpp_ad_graph/print_text_vec/$$ must be empty.
$lnext
Only the following operators my appear in
$cref/operator_vec/cpp_ad_graph/operator_vec/$$:
$code add_graph_op$$.
$lend

$head ok$$
The return value $icode ok$$ is true (false) if none (any) of the restrictions
on $icode graph_obj$$ are violated.
In this case, the return values in $icode context_ir$$, $icode module_ir$$
are not specified.

$end
*/
# include "graph2llvm_ir.hpp"
//
# include <llvm/IR/Type.h>
# include <llvm/IR/DerivedTypes.h>
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/Verifier.h>
//
// BEGIN_PROTOTYPE
bool graph2llvm_ir(
    const CppAD::cpp_graph&             graph_obj  ,
    std::unique_ptr<llvm::LLVMContext>& context_ir ,
    std::unique_ptr<llvm::Module>&      module_ir  )
// END_PROTOTYPE
{   bool ok = true;
    using std::string;
    using CppAD::graph::graph_op_enum;
    //
    size_t np  = graph_obj.n_dynamic_ind_get();
    size_t nx  = graph_obj.n_variable_ind_get();
    size_t nc  = graph_obj.constant_vec_size();
    size_t nop = graph_obj.operator_vec_size();
    size_t ny  = graph_obj.dependent_vec_size();
    //
    //
    // This is a simple case
    ok &= graph_obj.discrete_name_vec_size() == 0;
    ok &= graph_obj.atomic_name_vec_size() == 0;
    ok &= graph_obj.print_text_vec_size() == 0;
    //
    // function_name
    string function_name = graph_obj.function_name_get();
    //
    // context_ir
    context_ir = std::make_unique<llvm::LLVMContext>();
    //
    // module_ir
    module_ir = std::make_unique<llvm::Module>("test", *context_ir);
    //
    // llvm_double
    llvm::Type* llvm_double = llvm::Type::getDoubleTy(*context_ir);
    //
    // llvm_double_ptr
    llvm::PointerType* llvm_double_ptr =
        llvm::PointerType::getUnqual(llvm_double);
    //
    // void_t
    llvm::Type* void_t = llvm::Type::getVoidTy(*context_ir);
    //
    // function_t
    // void (*function_t) (double *, double*)
    std::vector<llvm::Type*> param_types = { llvm_double_ptr, llvm_double_ptr };
    bool                     is_var_arg  = "false";
    llvm::Type*              result_type = void_t;
    llvm::FunctionType*      function_t  = llvm::FunctionType::get(
            result_type, param_types, is_var_arg
    );
    //
    // function_ir
    // Create the IR function entry and insert this entry into the module
    auto            addr_space     = llvm::Function::ExternalLinkage;
    llvm::Function *function_ir    = llvm::Function::Create(
        function_t, addr_space, function_name, module_ir.get()
    );
    //
    // Make sure there are two arguments
    CPPAD_ASSERT_UNKNOWN(
        function_ir->arg_begin() + 2  == function_ir->arg_end()
    );
    //
    // input_ptr
    llvm::Argument *input_ptr  = function_ir->arg_begin() + 0;
    input_ptr->setName("input_ptr");
    //
    // output_ptr
    llvm::Argument *output_ptr  = function_ir->arg_begin() + 1;
    output_ptr->setName("output_ptr");
    //
    // Add a basic block at entry point to the function.
    llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(
        *context_ir, "EntryBlock", function_ir
    );
    //
    // Create a basic block builder with default parameters.  The builder will
    // automatically append instructions to the basic block `basic_block'.
    llvm::IRBuilder<> builder(basic_block);
    //
    // graph_ir
    // Initialize with nothing at index zero
    std::vector<llvm::Value*> graph_ir;
    graph_ir.push_back(nullptr);
    //
    // index_vector
    std::vector<llvm::Value*> index_vector(1);
    //
    // graph_ir
    // independent parameters
    for(size_t i = 0; i < np; ++i)
    {   index_vector[0] = llvm::ConstantInt::get(
            *context_ir, llvm::APInt(32, i, false)
        );
        string name        = "p_" + std::to_string(i);
        llvm::Value* ptr   = builder.CreateGEP(
            llvm_double, input_ptr, index_vector
        );
        llvm::Value* value = builder.CreateLoad(llvm_double, ptr, name);
        graph_ir.push_back(value);
    }
    //
    // graph_ir
    // independent variables
    for(size_t i = 0; i < nx; ++i)
    {   index_vector[0] = llvm::ConstantInt::get(
            *context_ir, llvm::APInt(32, np + i, false)
        );
        string name        = "x_" + std::to_string(i);
        llvm::Value* ptr   = builder.CreateGEP(
            llvm_double, input_ptr, index_vector
        );
        llvm::Value* value = builder.CreateLoad(llvm_double, ptr, name);
        graph_ir.push_back(value);
    }
    //
    // graph_ir
    // constants
    for(size_t i = 0; i < nc; ++i)
    {   double c_i = graph_obj.constant_vec_get(i);
        llvm::Value* value = llvm::ConstantFP::get(
            *context_ir, llvm::APFloat(c_i)
        );
        graph_ir.push_back(value);
    }
    // graph_ir
    // operators in the graph
    CppAD::cpp_graph::const_iterator itr;
    for(size_t iop = 0; iop < nop; ++iop)
    {   if( iop == 0 )
            itr = graph_obj.begin();
        else
            ++itr;
        CppAD::cpp_graph::const_iterator::value_type itr_value = *itr;
        //
        const CppAD::vector<size_t>& str_index( *itr_value.str_index_ptr );
        const CppAD::vector<size_t>&       arg( *itr_value.arg_node_ptr );
        graph_op_enum  op_enum  = itr_value.op_enum;
        size_t         n_result = itr_value.n_result;
        size_t         n_arg    = arg.size();
        size_t         n_str    = str_index.size();
        //
        llvm::Value* value;
        switch( op_enum )
        {
            case CppAD::graph::add_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            CPPAD_ASSERT_UNKNOWN( n_result == 1);
            CPPAD_ASSERT_UNKNOWN( n_str == 0 );
            value = builder.CreateFAdd( graph_ir[arg[0]], graph_ir[arg[1]] );
            graph_ir.push_back(value);
            break;

            default:
            ok = false;
        }
    }
    // set dependent variable values
    for(size_t i = 0; i < ny; ++i)
    {
        bool is_signed = false;
        index_vector[0] = llvm::ConstantInt::get(
            *context_ir, llvm::APInt(32, i, is_signed)
        );
        string name        = "y_" + std::to_string(i);
        llvm::Value* ptr   = builder.CreateGEP(
            llvm_double, output_ptr, index_vector
        );
        size_t node_index  = graph_obj.dependent_vec_get(i);
        bool is_volatile   = false;
        builder.CreateStore(graph_ir[node_index], ptr, is_volatile);
        graph_ir[node_index]->setName(name);
    }
    // void return value for this function
    builder.CreateRetVoid();
    //
    // check retreiving this function from this module
    CPPAD_ASSERT_UNKNOWN(
        function_ir == module_ir->getFunction(function_name)
    );
    // Validate the generated code, checking for consistency
    bool found_error = llvm::verifyFunction(*function_ir);
    ok &= ! found_error;
    //
    return ok;
}
