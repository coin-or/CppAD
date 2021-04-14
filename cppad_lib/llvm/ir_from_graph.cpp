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
---------------------------------------------------------------------------
$begin llvm_ir_from_graph$$
$spell
    llvm_ir
    obj
    vec
    op
    mul
    div
$$

$section Convert a C++ AD Graph to LLVM Intermediate Representation$$

$head Syntax$$
$icode%msg% = %ir_obj%.from_graph(%graph_obj%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_FROM_GRAPH%// END_FROM_GRAPH%1%$$

$head graph_obj$$
This a the $cref cpp_ad_graph$$ corresponding the function
that is converted to llvm intermediate representation.

$subhead Restrictions$$
The following limitations are placed on $icode graph_obj$$
(expected to be removed in the future).
$list number$$
$cref/function_name/cpp_ad_graph/function_name/$$ must not be empty.
$lend

$head ir_obj$$
Its input value of this $cref/llvm_ir/llvm_ir_ctor/$$ object does not matter.
Upon return, it contains an LLVM Intermediate Representation (IR)
corresponding to the function in $icode graph_obj$$.

$head msg$$
If the return value $icode msg$$ is the empty string,
no error was detected.
Otherwise, $icode msg$$ describes the error and the return value
of $icode ir_obj$$ is unspecified.

$children%
    example/llvm/from_to_graph.cpp
%$$
$head Example$$
The file $cref llvm_from_to_graph.cpp$$ contains an example and test
of this operation.

$end
*/
# include <cppad/core/llvm/ir.hpp>
//
# include <llvm/IR/Verifier.h>
# include <llvm/IR/Constants.h>
# include <llvm/IR/IRBuilder.h>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_FROM_GRAPH
std::string llvm_ir::from_graph(const CppAD::cpp_graph&  graph_obj)
// END_FROM_GRAPH
{   //
    // initialize return valuse as an identifier of this routines
    std::string msg = "llvm_ir::from_graph: ";
    using std::string;
    using graph::graph_op_enum;
    using CppAD::local::graph::op_enum2name;
    //
    // scalar values
    n_dynamic_ind_     = graph_obj.n_dynamic_ind_get();
    n_variable_ind_    = graph_obj.n_variable_ind_get();
    n_variable_dep_    = graph_obj.dependent_vec_size();
    size_t n_constant  = graph_obj.constant_vec_size();
    size_t n_operator  = graph_obj.operator_vec_size();
    //
    // function_name_
    function_name_ = graph_obj.function_name_get();
    if( function_name_ == "" )
    {   msg += "graph_obj.function_name_get() is empty";
        return msg;
    }
    //
    // context_ir_
    context_ir_ = std::make_unique<llvm::LLVMContext>();
    //
    // module_ir_
    module_ir_ = std::make_unique<llvm::Module>("llvm_ir", *context_ir_);
    //
    // double_t
    llvm::Type* double_t = llvm::Type::getDoubleTy(*context_ir_);
    //
    // double_ptr_t
    llvm::PointerType* double_ptr_t = llvm::PointerType::getUnqual(double_t);
    //
    // int_32_t
    llvm::Type* int_32_t = llvm::Type::getInt32Ty(*context_ir_);
    //
    // int_8_t
    llvm::Type* int_8_t = llvm::Type::getInt8Ty(*context_ir_);
    //
    // int_8_ptr
    llvm::PointerType* int_8_ptr_t = llvm::PointerType::getUnqual(int_8_t);
    //
    // arguments to FunctionType
    std::vector<llvm::Type*> param_types;
    bool                     is_var_arg;
    llvm::Type*              result_type;
    //
    // int (*ad_fun_t) (int, double *, int, double*, int, char*)
    // used for this function and atomic functions
    is_var_arg  = false;
    result_type = int_32_t;
    param_types = {
        int_32_t, double_ptr_t, int_32_t, double_ptr_t, int_32_t, int_8_ptr_t
    };
    llvm::FunctionType* ad_fun_t  = llvm::FunctionType::get(
            result_type, param_types, is_var_arg
    );
    //
    // double (*unary_fun_t)(double)
    // double (*binary_fun_t)(double, double)
    param_types  = { double_t };
    is_var_arg   = false;
    result_type  = double_t;
    llvm::FunctionType* unary_fun_t = llvm::FunctionType::get(
        result_type, param_types, is_var_arg
    );
    param_types  = { double_t, double_t };
    llvm::FunctionType* binary_fun_t = llvm::FunctionType::get(
        result_type, param_types, is_var_arg
    );
    //
    // unary_args, binary_args
    // used to call funcitons
    std::vector<llvm::Value*> unary_args(1);
    std::vector<llvm::Value*> binary_args(2);
    //
    // int (*print_fun_t) (int, int, char*, double. char*, double, char*)
    // used for print operators
    is_var_arg   = false;
    result_type  = int_32_t;
    param_types = {int_32_t, int_32_t,
        int_8_ptr_t, double_t, int_8_ptr_t, double_t, int_8_ptr_t
    };
    llvm::FunctionType* print_fun_t = llvm::FunctionType::get(
        result_type, param_types, is_var_arg
    );
    //
    //
    // empty_attributes
    llvm::AttributeList empty_attributes;
    //
    // function_ir
    // Create the IR function entry and insert this entry into the module
    auto            addr_space     = llvm::Function::ExternalLinkage;
    llvm::Function *function_ir    = llvm::Function::Create(
        ad_fun_t, addr_space, function_name_, module_ir_.get()
    );
    //
    // Make sure there are six arguments
    CPPAD_ASSERT_UNKNOWN(
        function_ir->arg_begin() + 6  == function_ir->arg_end()
    );
    //
    // len_input
    llvm::Argument* len_input  = function_ir->arg_begin() + 0;
    len_input->setName("len_input");
    //
    // input_ptr
    llvm::Argument* input_ptr  = function_ir->arg_begin() + 1;
    input_ptr->setName("input_ptr");
    //
    // len_output
    llvm::Argument* len_output  = function_ir->arg_begin() + 2;
    len_output->setName("len_output");
    //
    // output_ptr
    llvm::Argument* output_ptr  = function_ir->arg_begin() + 3;
    output_ptr->setName("output_ptr");
    //
    // len_msg
    llvm::Argument* len_msg  = function_ir->arg_begin() + 4;
    len_msg->setName("len_msg");
    //
    // msg_ptr
    llvm::Argument* msg_ptr  = function_ir->arg_begin() + 5;
    msg_ptr->setName("msg_ptr");
    //
    // Add a basic block at entry point to the function.
    llvm::BasicBlock* basic_block = llvm::BasicBlock::Create(
        *context_ir_, "EntryBlock", function_ir
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
    // one_index, two_index
    std::vector<llvm::Value*> one_index(1);
    std::vector<llvm::Value*> two_index(2);
    // ----------------------------------------------------------------------
    // some constants
    // ----------------------------------------------------------------------
    // bool_zero
    llvm::Value* bool_zero = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(1, 0, false)
    );
    // char_zero
    // (the C++ specifications do not say if char is signed or unsigned)
    llvm::Value* char_zero = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(8, 0, false)
    );
    // int_zero, int_one, int_two, int_three
    llvm::Value* int_zero = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, 0, true)
    );
    llvm::Value* int_two = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, 2, true)
    );
    llvm::Value* int_three = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, 3, true)
    );
    llvm::Value* int_four = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, 4, true)
    );
    //
    // The zero floating point constant
    llvm::Value* fp_zero = llvm::ConstantFP::get(
        *context_ir_, llvm::APFloat(0.0)
    );
    // ----------------------------------------------------------------------
    // check for error in len_input, len_output, or len_msg
    // ----------------------------------------------------------------------
    // error_no
    size_t n_input = n_dynamic_ind_ + n_variable_ind_;
    llvm::Value* expected_len_input = llvm::ConstantInt::get(
        *context_ir_, llvm::APInt(32, n_input, true)
    );
    llvm::Value* compare_len_input = builder.CreateICmpNE(
        len_input, expected_len_input
    );
    llvm::Value* error_no = builder.CreateSelect(
        compare_len_input, int_two, int_zero, "error_no"
    );
    //
    // error_no
    llvm::Value* expected_len_output = llvm::ConstantInt::get (
        *context_ir_, llvm::APInt(32, n_variable_dep_, true)
    );
    llvm::Value* compare_len_output = builder.CreateICmpNE(
        len_output, expected_len_output
    );
    error_no = builder.CreateSelect(
        compare_len_output, int_three,  error_no, "error_no"
    );
    //
    // error_no
    llvm::Value* compare_len_msg = builder.CreateICmpSLE(
        len_msg, int_zero
    );
    error_no = builder.CreateSelect(
        compare_len_msg, int_four,  error_no, "error_no"
    );
    //
    // length_error
    llvm::Value* length_error = builder.CreateICmpNE(
        error_no, int_zero, "length_error"
    );
    //
    // error_bb, merge_bb
    llvm::BasicBlock* error_bb =
        llvm::BasicBlock::Create(*context_ir_, "error_bb");
    llvm::BasicBlock* merge_bb =
        llvm::BasicBlock::Create(*context_ir_, "merge_bb");
    //
    // if length_error, return error_no
    builder.CreateCondBr(length_error, error_bb, merge_bb);
    function_ir->getBasicBlockList().push_back(error_bb);
    builder.SetInsertPoint(error_bb);
    builder.CreateRet(error_no);
    function_ir->getBasicBlockList().push_back(merge_bb);
    builder.SetInsertPoint(merge_bb);
    // ------------------------------------------------------------------------
    // print_text_value
    // ------------------------------------------------------------------------
    size_t n_print_text = graph_obj.print_text_vec_size();
    CppAD::vector<llvm::Value*> print_text_vec_value(n_print_text);
    for(size_t i = 0; i < n_print_text; ++i)
    {   // Attempting to pass the result of CreateGlobalStringPtr to a dynamic
        // library routine generates linker assertion, so pass a local copy.
        // In addition, the optimizer changes copying the string to
        // element, by element initialization of the local copy.
        //
        // str
        std::string str = graph_obj.print_text_vec_get(i);
        //
        // lenp1
        llvm::Value* lenp1  = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, str.size() + 1, false)
        );
        //
        // local_ptr
        llvm::Value* local_ptr = builder.CreateAlloca(int_8_t, lenp1);
        //
        // *local_ptr = str
        for(size_t j = 0; j <= str.size(); ++j)
        {   // value
            llvm::Value* value = char_zero;  // terminating null character
            if( j < str.size() )
            {   unsigned char ch = static_cast<unsigned char>( str[j] );
                value = llvm::ConstantInt::get(
                    *context_ir_, llvm::APInt(8, ch, false)
                );
            }
            // ptr
            one_index[0] = llvm::ConstantInt::get(
                *context_ir_, llvm::APInt(32, j, false)
            );
            llvm::Value* ptr   = builder.CreateGEP(
                int_8_t, local_ptr,  one_index
            );
            //
            // local[j] = str[j]
            bool is_volatile = false;
            builder.CreateStore(value, ptr, is_volatile);
        }
        //
        print_text_vec_value[i] = local_ptr;
    }
    //
    // initilaize compare_change
    llvm::Value* compare_change = bool_zero;
    //
    // initialize msg as empty
    llvm::Value* print_n_in = int_zero;
    //
    // initialize op_enum2callee as empty
    std::vector<llvm::FunctionCallee> op_enum2callee( graph::n_graph_op );
    //
    // ------------------------------------------------------------------------
    // graph_ir
    // independent parameters
    for(size_t i = 0; i < n_dynamic_ind_; ++i)
    {   one_index[0] = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, i, false)
        );
        string name        = "p_" + std::to_string(i);
        llvm::Value* ptr   = builder.CreateGEP(
            double_t, input_ptr, one_index
        );
        llvm::Value* value = builder.CreateLoad(double_t, ptr, name);
        graph_ir.push_back(value);
    }
    //
    // graph_ir
    // independent variables
    for(size_t i = 0; i < n_variable_ind_; ++i)
    {   one_index[0] = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, n_dynamic_ind_ + i, false)
        );
        string name        = "x_" + std::to_string(i);
        llvm::Value* ptr   = builder.CreateGEP(
            double_t, input_ptr, one_index
        );
        llvm::Value* value = builder.CreateLoad(double_t, ptr, name);
        graph_ir.push_back(value);
    }
    //
    // graph_ir
    // constants
    for(size_t i = 0; i < n_constant; ++i)
    {   double c_i = graph_obj.constant_vec_get(i);
        llvm::Value* value = llvm::ConstantFP::get(
            *context_ir_, llvm::APFloat(c_i)
        );
        graph_ir.push_back(value);
    }
    // graph_ir
    // operators in the graph
    CppAD::cpp_graph::const_iterator itr;
    for(size_t iop = 0; iop < n_operator; ++iop)
    {   if( iop == 0 )
            itr = graph_obj.begin();
        else
            ++itr;
        //
        // arg, op_enum, str_index
        CppAD::cpp_graph::const_iterator::value_type itr_value = *itr;
        const CppAD::vector<size_t>& arg( *itr_value.arg_node_ptr );
        graph_op_enum                op_enum  = itr_value.op_enum;
        const CppAD::vector<size_t>& str_index( *itr_value.str_index_ptr );
        size_t         n_result = itr_value.n_result;
        size_t         n_arg    = arg.size();
# ifndef NDEBUG
        size_t         n_str    = str_index.size();
        //
        switch( op_enum )
        {   // Unary operators
            case graph::abs_graph_op:
            case graph::neg_graph_op:
            case graph::sign_graph_op:
            //
            case graph::acos_graph_op:
            case graph::acosh_graph_op:
            case graph::asin_graph_op:
            case graph::asinh_graph_op:
            case graph::atan_graph_op:
            case graph::atanh_graph_op:
            case graph::cos_graph_op:
            case graph::cosh_graph_op:
            case graph::erf_graph_op:
            case graph::erfc_graph_op:
            case graph::exp_graph_op:
            case graph::expm1_graph_op:
            case graph::log1p_graph_op:
            case graph::log_graph_op:
            case graph::sin_graph_op:
            case graph::sinh_graph_op:
            case graph::sqrt_graph_op:
            case graph::tan_graph_op:
            case graph::tanh_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
            CPPAD_ASSERT_UNKNOWN( n_result == 1);
            CPPAD_ASSERT_UNKNOWN( n_str == 0 );
            break;

            // Binary operators
            case graph::azmul_graph_op:
            case graph::add_graph_op:
            case graph::div_graph_op:
            case graph::mul_graph_op:
            case graph::pow_graph_op:
            case graph::sub_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            CPPAD_ASSERT_UNKNOWN( n_result == 1);
            CPPAD_ASSERT_UNKNOWN( n_str == 0 );
            break;

            // Conditional Expressions
            case graph::cexp_eq_graph_op:
            case graph::cexp_le_graph_op:
            case graph::cexp_lt_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_arg == 4 );
            CPPAD_ASSERT_UNKNOWN( n_result == 1);
            CPPAD_ASSERT_UNKNOWN( n_str == 0 );
            break;

            // Comparison Operators
            case graph::comp_eq_graph_op:
            case graph::comp_le_graph_op:
            case graph::comp_lt_graph_op:
            case graph::comp_ne_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            CPPAD_ASSERT_UNKNOWN( n_result == 0);
            CPPAD_ASSERT_UNKNOWN( n_str == 0 );
            break;

            // Summation Operator
            case graph::sum_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_result == 1 );
            CPPAD_ASSERT_UNKNOWN( n_str == 0 );
            CPPAD_ASSERT_UNKNOWN( n_arg >= 2 );
            break;

            // discrete functions
            case graph::discrete_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_result == 1 );
            CPPAD_ASSERT_UNKNOWN( n_str == 1 );
            CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
            break;

            // atomic functions
            case graph::atom_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_str == 1 );
            break;

            // Print Operator
            case graph::print_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_result == 0 );
            CPPAD_ASSERT_UNKNOWN( n_str == 2 );
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            break;


            default:
            msg += "graph_obj has following unsupported operator ";
            msg += op_enum2name[op_enum];
            return msg;
        }
# endif
        // temporaries used in switch cases
        llvm::Value*             value;
        llvm::Value*             compare;
        llvm::CmpInst::Predicate pred;
        const char*              name;
        switch( op_enum )
        {   // -------------------------------------------------------------
            // simple operators that translate to one llvm instruction
            // -------------------------------------------------------------
            //
            // unary functions
            case graph::abs_graph_op:
            case graph::acos_graph_op:
            case graph::acosh_graph_op:
            case graph::asin_graph_op:
            case graph::asinh_graph_op:
            case graph::atan_graph_op:
            case graph::atanh_graph_op:
            case graph::cos_graph_op:
            case graph::cosh_graph_op:
            case graph::erf_graph_op:
            case graph::erfc_graph_op:
            case graph::exp_graph_op:
            case graph::expm1_graph_op:
            case graph::log1p_graph_op:
            case graph::log_graph_op:
            case graph::sign_graph_op:
            case graph::sin_graph_op:
            case graph::sinh_graph_op:
            case graph::sqrt_graph_op:
            case graph::tan_graph_op:
            case graph::tanh_graph_op:
            if( op_enum2callee[op_enum].getCallee() == nullptr )
            {   // We only define functions that are used to simply IR print
                switch( op_enum )
                {   case graph::abs_graph_op:
                    name = "cppad_link_fabs";
                    break;

                    case graph::sign_graph_op:
                    name = "cppad_link_sign";
                    break;

                    default:
                    name = op_enum2name[op_enum];
                    break;
                }
                op_enum2callee[op_enum] = module_ir_->getOrInsertFunction(
                    name, unary_fun_t, empty_attributes
                );
            }
            unary_args[0] = graph_ir[ arg[0] ];
            value = builder.CreateCall(
                op_enum2callee[op_enum], unary_args, op_enum2name[op_enum]
            );
            graph_ir.push_back(value);
            break;
            //
            // discrete functions
            case graph::discrete_graph_op:
            unary_args[0] = graph_ir[ arg[0] ];
            {   const std::string fun_name = "discrete_" +
                    graph_obj.discrete_name_vec_get(str_index[0]);
                llvm::FunctionCallee callee = module_ir_->getOrInsertFunction(
                    fun_name.c_str(), unary_fun_t, empty_attributes
                );
                unary_args[0] = graph_ir[ arg[0] ];
                value = builder.CreateCall(
                    callee, unary_args, fun_name.c_str()
                );
                graph_ir.push_back(value);
            }
            break;
            //
            // binary functions
            case graph::pow_graph_op:
            binary_args[0] = graph_ir[ arg[0] ];
            binary_args[1] = graph_ir[ arg[1] ];
            if( op_enum2callee[op_enum].getCallee() == nullptr )
            {   name = op_enum2name[op_enum];
                op_enum2callee[op_enum] = module_ir_->getOrInsertFunction(
                    name, binary_fun_t, empty_attributes
                );
            }
            value = builder.CreateCall(
                op_enum2callee[op_enum], binary_args, op_enum2name[op_enum]
            );
            graph_ir.push_back(value);
            break;
            //
            // binary operators
            case graph::add_graph_op:
            value = builder.CreateFAdd(graph_ir[arg[0]], graph_ir[arg[1]]);
            graph_ir.push_back(value);
            break;
            //
            case graph::div_graph_op:
            value = builder.CreateFDiv(graph_ir[arg[0]], graph_ir[arg[1]]);
            graph_ir.push_back(value);
            break;
            //
            case graph::mul_graph_op:
            value = builder.CreateFMul(graph_ir[arg[0]], graph_ir[arg[1]]);
            graph_ir.push_back(value);
            break;
            //
            case graph::neg_graph_op:
            value = builder.CreateFNeg(graph_ir[arg[0]]);
            graph_ir.push_back(value);
            break;
            //
            case graph::sub_graph_op:
            value = builder.CreateFSub(graph_ir[arg[0]], graph_ir[arg[1]]);
            graph_ir.push_back(value);
            break;
            // --------------------------------------------------------------
            // Print Operator
            // --------------------------------------------------------------
            case graph::print_graph_op:
            if( op_enum2callee[op_enum].getCallee() == nullptr )
            {   name = "cppad_link_print";
                op_enum2callee[op_enum] = module_ir_->getOrInsertFunction(
                    name, print_fun_t, empty_attributes
                );
            }
            {   std::vector<llvm::Value*> print_args(7);
                //
                // n_in
                print_args[0] = print_n_in;
                //
                // len_msg
                print_args[1] = len_msg;
                //
                // msg
                print_args[2] = msg_ptr;
                //
                // notpos
                print_args[3] = graph_ir[ arg[0] ];
                //
                // before
                print_args[4] = print_text_vec_value[ str_index[0] ];
                //
                // value
                print_args[5] = graph_ir[ arg[1] ];
                //
                // after
                print_args[6] = print_text_vec_value[ str_index[1] ];
                //
                // call cppad_link_print
                value = builder.CreateCall(
                    op_enum2callee[op_enum], print_args, "no_out"
                );
                //
                // update message length
                print_n_in = value;
            }
            break;
            // --------------------------------------------------------------
            // Contitional Expressions
            // --------------------------------------------------------------
            case graph::cexp_eq_graph_op:
            case graph::cexp_le_graph_op:
            case graph::cexp_lt_graph_op:
            //
            if( op_enum == graph::cexp_eq_graph_op )
                pred = llvm::FCmpInst::FCMP_OEQ;
            else if( op_enum == graph::cexp_le_graph_op )
                pred = llvm::FCmpInst::FCMP_OLE;
            else
                pred = llvm::FCmpInst::FCMP_OLT;
            //
            compare = builder.CreateFCmp(
                pred, graph_ir[arg[0]], graph_ir[arg[1]]
            );
            // Note that only conditional expressions use select with
            // double operands
            value = builder.CreateSelect(
                compare, graph_ir[arg[2]], graph_ir[arg[3]]
            );
            graph_ir.push_back(value);
            break;
            // --------------------------------------------------------------
            // azmul(x, y)
            // --------------------------------------------------------------
            case graph::azmul_graph_op:
            // value = x * y
            value   = builder.CreateFMul(graph_ir[arg[0]], graph_ir[arg[1]]);
            // compare = x == 0.0
            pred    = llvm::FCmpInst::FCMP_OEQ;
            compare = builder.CreateFCmp( pred, graph_ir[arg[0]], fp_zero );
            // value = azmul(x, y);
            value = builder.CreateSelect(compare, fp_zero, value);
            graph_ir.push_back(value);
            break;
            // -------------------------------------------------------------
            // Summation Operator
            // -------------------------------------------------------------
            case graph::sum_graph_op:
            value = graph_ir[arg[0]];
            for(size_t i = 1; i < arg.size(); ++i)
                value = builder.CreateFAdd(value, graph_ir[arg[i]]);
            graph_ir.push_back(value);
            break;
            // --------------------------------------------------------------
            // Comparison operators
            // --------------------------------------------------------------
            case graph::comp_eq_graph_op:
            case graph::comp_le_graph_op:
            case graph::comp_lt_graph_op:
            case graph::comp_ne_graph_op:
# ifdef NDEBUG
            // Change the comparisions so that true corresponds to
            // comparison changed.
            switch( op_enum )
            {   case graph::comp_eq_graph_op:
                pred    = llvm::FCmpInst::FCMP_ONE;
                break;
                case graph::comp_le_graph_op:
                pred    = llvm::FCmpInst::FCMP_OLT;
                break;
                case graph::comp_lt_graph_op:
                pred    = llvm::FCmpInst::FCMP_OLE;
                break;
                case graph::comp_ne_graph_op:
                pred    = llvm::FCmpInst::FCMP_OEQ;
                break;

                default:
                CPPAD_ASSERT_UNKNOWN(false);
                // set pred to avoid warning
                pred    = llvm::FCmpInst::FCMP_ONE;
                break;
            }
            compare = builder.CreateFCmp(
                pred, graph_ir[arg[1]], graph_ir[arg[0]]
            );
# else
            // name is used by llvm_ir.to_graph to check that optimizer did
            // not change the sense of comparison operators.
            switch( op_enum )
            {   case graph::comp_eq_graph_op:
                pred    = llvm::FCmpInst::FCMP_ONE;
                name    = "eq";
                break;
                case graph::comp_le_graph_op:
                pred    = llvm::FCmpInst::FCMP_OLT;
                name    = "le";
                break;
                case graph::comp_lt_graph_op:
                pred    = llvm::FCmpInst::FCMP_OLE;
                name    = "lt";
                break;
                case graph::comp_ne_graph_op:
                pred    = llvm::FCmpInst::FCMP_OEQ;
                name    = "ne";
                break;

                default:
                // set pred to avoid warning
                pred    = llvm::FCmpInst::FCMP_ONE;
                CPPAD_ASSERT_UNKNOWN(false);
                break;
            }
            compare = builder.CreateFCmp(
                pred, graph_ir[arg[1]], graph_ir[arg[0]], name
            );
# endif
            // compare is one of operands in Or instructions
            // (this is assumed by llvm_ir.to_graph).
            compare_change = builder.CreateOr(
                compare, compare_change, "compare_change"
            );
            break;
            // --------------------------------------------------------------
            // atomic functions
            // --------------------------------------------------------------
            case graph::atom_graph_op:
            {   // fun_name
                const std::string fun_name = "atomic_" +
                    graph_obj.atomic_name_vec_get(str_index[0]);
                //
                // callee
                llvm::FunctionCallee callee = module_ir_->getOrInsertFunction(
                    fun_name.c_str(), ad_fun_t, empty_attributes
                );
                //
                // atom_args
                std::vector<llvm::Value*> atom_args(6);
                //
                // atom_args[0] = len_input
                value = llvm::ConstantInt::get(
                    *context_ir_, llvm::APInt(32, n_arg, true)
                );
                atom_args[0] = value;
                //
                // atom_args[2] = len_output
                value = llvm::ConstantInt::get(
                    *context_ir_, llvm::APInt(32, n_result, true)
                );
                atom_args[2] = value;
                //
                // 2DO: include message returned by atomic fucntion
                // atom_args[4] = len_msg
                value = llvm::ConstantInt::get(
                    *context_ir_, llvm::APInt(32, 1, true)
                );
                atom_args[4] = value;
                //
                // atom_args[1] = input
                llvm::Value* atom_in = builder.CreateAlloca(
                    double_t,       // Type*  Ty
                    atom_args[0],   // Value* ArraySize
                    "atom_in"       // const Twine& Name
                );
                atom_args[1] = atom_in;
                //
                // copy arguments to atom_in
                for(size_t i = 0; i < n_arg; ++i)
                {   // index for this element
                    one_index[0] = llvm::ConstantInt::get(
                        *context_ir_, llvm::APInt(32, i, false)
                    );
                    // pointer to this element
                    string element_name = std::to_string(i) + "_in";
                    llvm::Value* ptr = builder.CreateGEP(
                        double_t, atom_in, one_index, element_name
                    );
                    // store value for this element
                    bool is_volatile = false;
                    builder.CreateStore(graph_ir[ arg[i] ], ptr, is_volatile);
                }
                //
                // atom_args[3] = output
                llvm::Value* atom_out = builder.CreateAlloca(
                    double_t,       // Type*  Ty
                    atom_args[2],   // Value* ArraySize
                    "atom_out"      // const Twine& Name
                );
                atom_args[3] = atom_out;
                //
                // 2DO: include message returned by atomic function
                // atom_args[5] = msg
                value = builder.CreateAlloca(
                    int_8_t,        // Type*  Ty
                    atom_args[4],   // Value* ArraySize
                    "atom_msg"      // const Twine& Name
                );
                atom_args[5] = value;
                //
                // call the atomic function
                value = builder.CreateCall(
                    callee, atom_args, fun_name.c_str()
                );
                //
                // copy results to graph_ir
                for(size_t i = 0; i < n_result; ++i)
                {   // index for this element
                    one_index[0] = llvm::ConstantInt::get(
                        *context_ir_, llvm::APInt(32, i, false)
                    );
                    // pointer to this element
                    string element_name = std::to_string(i) + "_out";
                    llvm::Value* ptr = builder.CreateGEP(
                        double_t, atom_out, one_index, element_name
                    );
                    // get this element
                    value = builder.CreateLoad(double_t, ptr);
                    graph_ir.push_back(value);
                }
            }
            break;
            // --------------------------------------------------------------
            // This operator is not yet supported
            // --------------------------------------------------------------
            default:
            msg += "graph_obj has following unsupported operator ";
            msg += op_enum2name[op_enum];
            return msg;
        }
    }
    // set dependent variable values
    for(size_t i = 0; i < n_variable_dep_; ++i)
    {
        bool is_signed = false;
        one_index[0] = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, i, is_signed)
        );
        string name        = "y_" + std::to_string(i);
        llvm::Value* ptr   = builder.CreateGEP(
            double_t, output_ptr, one_index
        );
        size_t node_index  = graph_obj.dependent_vec_get(i);
        bool is_volatile   = false;
        builder.CreateStore(graph_ir[node_index], ptr, is_volatile);
        graph_ir[node_index]->setName(name);
    }
    //
    // terminate the msg_ptr vector with a '\0' character
    one_index[0] = print_n_in;
    llvm::Value* msg_end = builder.CreateGEP(
        int_8_t, msg_ptr, one_index
    );
    bool is_volatile = false;
    builder.CreateStore(char_zero, msg_end, is_volatile);
    //
    // return zero for no error
    error_no = builder.CreateZExtOrTrunc(
        compare_change, int_32_t, "error_no"
    );
    builder.CreateRet(error_no);
    //
    // check retreiving this function from this module
    CPPAD_ASSERT_UNKNOWN(
        function_ir == module_ir_->getFunction(function_name_)
    );
    // Validate the generated code, checking for consistency
    bool found_error = llvm::verifyFunction(*function_ir);
    if( found_error )
    {   msg += "error during verification of llvm_ir function";
        return msg;
    }
    //
    // No error
    msg = "";
    return msg;
}

} // END_CPPAD_NAMESPACE
