/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include "llvm_ir.hpp"
//
# include <llvm/IR/DerivedTypes.h>
# include <llvm/IR/InstIterator.h>
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/LegacyPassManager.h>
# include <llvm/IR/Type.h>
# include <llvm/IR/Verifier.h>
//
# include <llvm/Support/FileSystem.h>
# include <llvm/Support/Host.h>
# include <llvm/Support/raw_os_ostream.h>
# include <llvm/Support/TargetRegistry.h>
# include <llvm/Support/TargetSelect.h>
//
# include <llvm/Target/TargetOptions.h>
# include <llvm/Target/TargetMachine.h>
//
//
# include <llvm/Transforms/InstCombine/InstCombine.h>
# include <llvm/Transforms/Scalar.h>
# include <llvm/Transforms/Scalar/GVN.h>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*
------------------------------------------------------------------------------
$begin llvm_ir_ctor$$
$spell
    llvm_ir
    obj
$$

$section LLVM Intermediate Representation Object Constructor$$

$head Syntax$$
$codei%llvm_ir %ir_obj%;%$$

$head Purpose$$
The creates the empty $code llvm_ir$$ object $icode ir_obj$$.

$end
*/
llvm_ir::llvm_ir(void)
:
function_name_(""),
n_dynamic_ind_(0),
n_variable_ind_(0),
n_variable_dep_(0),
context_ir_(nullptr),
module_ir_(nullptr)
{ }
/*
-----------------------------------------------------------------------------
$beign llvm_ir_print$$

$section Print LLVM Internmediate Representation$$

$head Syntax$$
$icode%ir_obj%.print()%$$

$head ir_obj$$
This is a $code const$$  $cref/llvm_ir/llvm_ir_ctor/$$ object.

$end
*/
void llvm_ir::print(void) const
{
    if( function_name_ == "" )
    {   CPPAD_ASSERT_UNKNOWN( n_dynamic_ind_ == 0 );
        CPPAD_ASSERT_UNKNOWN( n_variable_ind_ == 0 );
        CPPAD_ASSERT_UNKNOWN( n_variable_dep_ == 0 );
        CPPAD_ASSERT_UNKNOWN( context_ir_ == nullptr );
        CPPAD_ASSERT_UNKNOWN( module_ir_   == nullptr );
        //
        std::cout << "llvm_ir::print: empty function\n";
        return;
    }
    // os
    llvm::raw_os_ostream os( std::cout );
    // function_ir
    llvm::Function* function_ir = module_ir_->getFunction(function_name_);
    CPPAD_ASSERT_UNKNOWN( function_ir != nullptr );
    // output
    os << *function_ir;
    os.flush();
    return;
}
/*
---------------------------------------------------------------------------
$begin llvm_ir_from_graph$$
$spell
    llvm_ir
    obj
    vec
    op
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
(and expected to be removed in the future).
$list number$$
$cref/function_name/cpp_ad_graph/function_name/$$ must not be empty.
$lnext
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
// BEGIN_FROM_GRAPH
std::string llvm_ir::from_graph(const CppAD::cpp_graph&  graph_obj)
// END_FROM_GRAPH
{   //
    // initialize return valuse as an identifier of this routines
    std::string msg = "llvm_ir::from_graph: ";
    using std::string;
    using CppAD::graph::graph_op_enum;
    //
    // Assumptions
    if( graph_obj.discrete_name_vec_size() != 0)
    {   msg += "graph_obj.discrete_name_vec_size() != 0";
        return msg;
    }
    if( graph_obj.atomic_name_vec_size() != 0)
    {   msg += "graph_obj.atomic_name_vec_size() != 0";
        return msg;
    }
    if( graph_obj.print_text_vec_size() != 0)
    {   msg += "graph_obj.print_text_vec_size() != 0";
        return msg;
    }
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
    module_ir_ = std::make_unique<llvm::Module>("test", *context_ir_);
    //
    // llvm_double
    llvm::Type* llvm_double = llvm::Type::getDoubleTy(*context_ir_);
    //
    // llvm_double_ptr
    llvm::PointerType* llvm_double_ptr =
        llvm::PointerType::getUnqual(llvm_double);
    //
    // int_32_t
    llvm::Type* int_32_t = llvm::Type::getInt32Ty(*context_ir_);
    //
    // function_t
    // void (*function_t) (double *, double*)
    std::vector<llvm::Type*> param_types = {
        int_32_t, llvm_double_ptr, int_32_t, llvm_double_ptr
    };
    bool                     is_var_arg  = false;
    llvm::Type*              result_type = int_32_t;
    llvm::FunctionType*      function_t  = llvm::FunctionType::get(
            result_type, param_types, is_var_arg
    );
    //
    // function_ir
    // Create the IR function entry and insert this entry into the module
    auto            addr_space     = llvm::Function::ExternalLinkage;
    llvm::Function *function_ir    = llvm::Function::Create(
        function_t, addr_space, function_name_, module_ir_.get()
    );
    //
    // Make sure there are four arguments
    CPPAD_ASSERT_UNKNOWN(
        function_ir->arg_begin() + 4  == function_ir->arg_end()
    );
    //
    // input_ptr
    llvm::Argument *input_ptr  = function_ir->arg_begin() + 1;
    input_ptr->setName("input_ptr");
    //
    // output_ptr
    llvm::Argument *output_ptr  = function_ir->arg_begin() + 3;
    output_ptr->setName("output_ptr");
    //
    // Add a basic block at entry point to the function.
    llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(
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
    // index_vector
    std::vector<llvm::Value*> index_vector(1);
    //
    // graph_ir
    // independent parameters
    for(size_t i = 0; i < n_dynamic_ind_; ++i)
    {   index_vector[0] = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, i, false)
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
    for(size_t i = 0; i < n_variable_ind_; ++i)
    {   index_vector[0] = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, n_dynamic_ind_ + i, false)
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
        CppAD::cpp_graph::const_iterator::value_type itr_value = *itr;
        //
        const CppAD::vector<size_t>&       arg( *itr_value.arg_node_ptr );
        graph_op_enum  op_enum  = itr_value.op_enum;
# ifndef NDEBUG
        const CppAD::vector<size_t>& str_index( *itr_value.str_index_ptr );
        size_t         n_result = itr_value.n_result;
        size_t         n_arg    = arg.size();
        size_t         n_str    = str_index.size();
# endif
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
            msg += "graph_obj has following unsupported operator ";
            msg += local::graph::op_enum2name[op_enum];
            return msg;
        }
    }
    // set dependent variable values
    for(size_t i = 0; i < n_variable_dep_; ++i)
    {
        bool is_signed = false;
        index_vector[0] = llvm::ConstantInt::get(
            *context_ir_, llvm::APInt(32, i, is_signed)
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
    // return zero for no error
    bool is_signed = true;
    llvm::Value* no_error = llvm::ConstantInt::get(
        *context_ir_, llvm::APInt(32, 0, is_signed)
    );
    builder.CreateRet(no_error);
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
/*
---------------------------------------------------------------------------
$begin llvm_ir_to_graph$$
$spell
    llvm_ir
    obj
    vec
    op
    Ptr
$$

$section Convert an LLVM Intermediate Representation to a C++ AD Graph$$

$head Syntax$$
$icode%msg% = %ir_obj%.to_graph(%graph_obj%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_TO_GRAPH%// END_TO_GRAPH%1%$$

$head graph_obj$$
The input value of $icode graph_obj$$ does not matter.
Upon return, it is a $cref cpp_ad_graph$$ representation of the function.

$head ir_obj$$
This is a $cref/llvm_ir/llvm_ir_ctor/$$ object.
It contains an LLVM Intermediate Representation (IR)
of the function that is convert to a C++ AD graph representation.

$subhead Restrictions$$
Only the following $code llvm::Instruction$$ operator codes are supported
so far (more are  expected in the future):
$code Load$$,
$code FAdd$$,
$code GetElementPtr$$,
$code Ret$$,
$code Store$$.

$head msg$$
If the return value $icode msg$$ is the empty string,
no error was detected.
Otherwise, $icode msg$$ describes the error and the return value
of $icode graph_obj$$ is unspecified.

$comment example/llvm/from_to_graph.cpp was included by llvm_from_graph$$
$head Example$$
The file $cref llvm_from_to_graph.cpp$$ contains an example and test
of this operation.

$end
*/
// BEGIN_TO_GRAPH
std::string llvm_ir::to_graph(CppAD::cpp_graph&  graph_obj) const
// END_TO_GRAPH
{   //
    // initialize return value with name of this routine
    std::string msg = "llvm_ir::to_graph";
    //
    // function_ir
    const llvm::Function* function_ir = module_ir_->getFunction(function_name_);
    CPPAD_ASSERT_UNKNOWN( function_ir  != nullptr );
    //
    // map and llvm::Value* for a value to graph node index
    llvm::DenseMap<const llvm::Value*, size_t>  llvm_value2graph_node;
    //
    // map llvm::Value* for a pointer to graph node index
    llvm::DenseMap<const llvm::Value*, size_t>  llvm_ptr2graph_node;
    //
    // map llvm::Value* double* to a dependent variable index
    llvm::DenseMap<const llvm::Value*, size_t>  llvm_ptr2dep_var_ind;
    //
    // Assumes the default constructor for size_t yields zero
    CPPAD_ASSERT_UNKNOWN( size_t() == 0 );
    //
    // type used by interface to DenseMap
    typedef std::pair<const llvm::Value*, size_t> pair;
    //
    // input_ptr
    const llvm::Argument *input_ptr  = function_ir->arg_begin() + 1;
    //
    // output_ptr
    const llvm::Argument *output_ptr = function_ir->arg_begin() + 3;
    //
    /// begin_inst
    const llvm::const_inst_iterator begin_inst = llvm::inst_begin(function_ir);
    //
    // end_inst
    const llvm::const_inst_iterator end_inst   = llvm::inst_end(function_ir);
    //
    // drop any inforamtion in this graph object
    graph_obj.initialize();
    //
    // set scalars
    graph_obj.function_name_set(function_name_);
    graph_obj.n_dynamic_ind_set(n_dynamic_ind_);
    graph_obj.n_variable_ind_set(n_variable_ind_);
    //
    // First Pass
    // determine the floating point constants in the graph
    CPPAD_ASSERT_UNKNOWN( graph_obj.constant_vec_size() == 0 );
    for(llvm::const_inst_iterator itr = begin_inst; itr != end_inst; ++itr)
    {   unsigned n_operand = itr->getNumOperands();
        for(size_t i = 0; i < n_operand; ++i)
        {   llvm::Value* operand = itr->getOperand( (unsigned int)(i) );
            if( llvm::isa<llvm::ConstantFP>(operand) )
            {   size_t node = llvm_value2graph_node.lookup(operand);
                if( node == 0 )
                {   // First occurance of this constant
                    // dbl
                    const llvm::ConstantFP* constant_fp =
                        llvm::dyn_cast<llvm::ConstantFP>(operand);
                    const llvm::APFloat* apfloat = &constant_fp->getValue();
                    double  dbl = apfloat->convertToDouble();
                    //
                    // node
                    size_t n_constant = graph_obj.constant_vec_size();
                    node = 1 + n_dynamic_ind_ + n_variable_ind_ + n_constant;
                    //
                    // add this constant do data structure
                    llvm_value2graph_node.insert( pair(operand, node) );
                    graph_obj.constant_vec_push_back(dbl);
                }
            }
        }
    }
    // n_constant
    size_t n_constant = graph_obj.constant_vec_size();
    //
    // node index 1 corresponds to first element of input vector
    llvm_ptr2graph_node.insert( pair(input_ptr, 1) );
    //
    // Dependent variable index 0 corresponds to first lement of output vector.
    // Add one to these indices so zero can be used for value not found.
    llvm_ptr2dep_var_ind.insert( pair(output_ptr, 1) );
    //
    // mapping from dependent variable index to graph node index
    CppAD::vector<size_t> dependent(n_variable_dep_);
# ifndef NDEBUG
    for(size_t i = 0; i < n_variable_dep_; ++i)
        CPPAD_ASSERT_UNKNOWN( dependent[i] == 0 );
# endif
    //
    // The operands and corresponding type for each instruction
    CppAD::vector<llvm::Value*>       operand;
    CppAD::vector<llvm::Type::TypeID> type_id;
    //
    // node index correspoding to first result
    size_t result_node = n_dynamic_ind_ + n_variable_ind_ + n_constant;
    //
    // Second Pass
    for(llvm::const_inst_iterator itr = begin_inst; itr != end_inst; ++itr)
    {
        const llvm::Value* result = llvm::dyn_cast<llvm::Value>( &(*itr) );
# ifndef NDEBUG
        llvm::Type::TypeID result_type_id = result->getType()->getTypeID();
# endif
        unsigned op_code                  = itr->getOpcode();
        unsigned n_operand                = itr->getNumOperands();
        operand.resize(n_operand);
        type_id.resize(n_operand);
        for(size_t i = 0; i < n_operand; ++i)
        {   operand[i] = itr->getOperand((unsigned int)(i));
            type_id[i] = operand[i]->getType()->getTypeID();
        }
        //
        // temporaries used in switch cases
        const llvm::ConstantInt* cint;
        const llvm::APInt*       apint;
        const uint64_t*          uint64;
        size_t                   node;
        size_t                   index;
        //
        switch( op_code )
        {   // --------------------------------------------------------------
            case llvm::Instruction::Load:
            // This instruction is only used to load the first element
            // in the input vector.
            CPPAD_ASSERT_UNKNOWN( n_operand == 1 );
            CPPAD_ASSERT_UNKNOWN( type_id[0] == llvm::Type::PointerTyID );
            node = llvm_ptr2graph_node.lookup(operand[0]);
            CPPAD_ASSERT_UNKNOWN( node != 0 );
            // result is the value that operand[0] points to
            llvm_value2graph_node.insert( pair(result , node) );
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::FAdd:
            // This instruction creates a new node in the graph that corresonds
            // to the sum of two other nodes.
            CPPAD_ASSERT_UNKNOWN( n_operand == 2 );
            CPPAD_ASSERT_UNKNOWN( result_type_id == llvm::Type::DoubleTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[0]     == llvm::Type::DoubleTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1]     == llvm::Type::DoubleTyID );
            //
            // mapping from this result to the correspondign new node in graph
            llvm_value2graph_node.insert( pair(result , ++result_node) );
            //
            // put this operator in the graph
            graph_obj.operator_vec_push_back( CppAD::graph::add_graph_op );
            //
            // add node index correspnding to left and right operands
            for(size_t i = 0; i < 2; ++i)
            {   node = llvm_value2graph_node.lookup(operand[i]);
                CPPAD_ASSERT_UNKNOWN( node != 0 );
                graph_obj.operator_arg_push_back( node );
            }
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::Instruction::GetElementPtr:
            CPPAD_ASSERT_UNKNOWN( n_operand == 2 );
            CPPAD_ASSERT_UNKNOWN( result_type_id == llvm::Type::PointerTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[0]     == llvm::Type::PointerTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1]     == llvm::Type::IntegerTyID );
            cint   = llvm::dyn_cast<const llvm::ConstantInt>(operand[1]);
            apint  = &cint->getValue();
            uint64 = apint->getRawData();
            if( operand[0] == output_ptr )
            {   // The only use of output_ptr is to store values.
                // Use the actual dependent variable index plus 1
                // so that we can use 0 to check for not found.
                index = *uint64 + 1;
                llvm_ptr2dep_var_ind.insert( pair(result, index) );
            }
            else
            {    CPPAD_ASSERT_UNKNOWN( operand[0] == input_ptr );
                // The only use of input_ptr is load values
                //
                // first element of input_ptr corresponds to node index 1
                node = *uint64 + 1;
                llvm_ptr2graph_node.insert( pair(result, node) );
            }
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::Ret:
            // returns int32_t error_no
            CPPAD_ASSERT_UNKNOWN( n_operand == 1 );
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::Store:
            CPPAD_ASSERT_UNKNOWN( n_operand == 2 );
            CPPAD_ASSERT_UNKNOWN( type_id[0] == llvm::Type::DoubleTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1] == llvm::Type::PointerTyID );
            node  = llvm_value2graph_node.lookup(operand[0]);
            index = llvm_ptr2dep_var_ind.lookup(operand[1]);
            CPPAD_ASSERT_UNKNOWN( node != 0 );
            CPPAD_ASSERT_UNKNOWN( index != 0 );
            dependent[ index - 1 ] = node;
            break;
            //
            // --------------------------------------------------------------
            default:
            {   std::string op_name = itr->getOpcodeName();
                msg += "Cannot handle the llvm instruction " + op_name;
                return msg;
            }
            break;
        }
    }
    for(size_t i = 0; i < n_variable_dep_; ++i)
    {   if( dependent[i] == 0 )
        {   msg += "No store instruction for dependent variable index ";
            msg += std::to_string(i);
            return msg;
        }
        graph_obj.dependent_vec_push_back( dependent[i] );
    }
    // No error
    msg = "";
    return msg;
}
/*
------------------------------------------------------------------------------
$begin llvm_ir_optimize$$
$spell
    llvm_ir
    obj
$$

$section Optimize an LLVM Intermediate Representation$$

$head Syntax$$
$icode%ir_obj%.optimize()%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_OPTIMIZE%// END_OPTIMIZE%1%$$

$head ir_obj$$
This is a $cref/llvm_ir/llvm_ir_ctor/$$ object.
It contains an LLVM Intermediate Representation (IR)
that is optimized.

$children%
    example/llvm/optimize.cpp
%$$
$head Example$$
The file $cref llvm_optimize.cpp$$ contains an example and test
of this operation.

$end
*/
//
// BEGIN_OPTIMIZE
void llvm_ir::optimize(void)
// END_OPTIMIZE
{
    // function_ir
    llvm::Function* function_ir = module_ir_->getFunction(function_name_);
    CPPAD_ASSERT_UNKNOWN( function_ir != nullptr )
    //
    // optimization_pass_manager
    std::unique_ptr<llvm::legacy::FunctionPassManager> function_pass_manager;
    function_pass_manager =
        std::make_unique<llvm::legacy::FunctionPassManager>( module_ir_.get() );
    //
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    function_pass_manager->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    function_pass_manager->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    function_pass_manager->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    function_pass_manager->add(llvm::createCFGSimplificationPass());
    //
    function_pass_manager->doInitialization();
    //
    // run the optimizer on the function
    function_pass_manager->run(*function_ir);
    //
    return;
}
/*
-------------------------------------------------------------------------------
$begin llvm_ir_to_object_file$$
$spell
    llvm_ir
    obj
$$

$section Convert On LLVM Intermediate Representation to an Object File$$

$head Syntax$$
$icode%msg% = llvm_to_object_file(%file_name%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head file_name$$
This is the name of the file (not the function in the file)
that is created.  This can be a local or global path.

$head Target Machine$$
The llvm target information for the current host is used
for the target machine; i.e, the machine that is currently executing.

$end
*/
//
// BEGIN_PROTOTYPE
std::string llvm_ir::to_object_file(const std::string& file_name) const
// END_PROTOTYPE
{   //
    // initialize error message as starting with name of this routine
    std::string msg = "llvm_ir::to_object_file: ";
    //
    // Initialize llvm Target functions
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    //
    // target
    std::string          target_triple = llvm::sys::getDefaultTargetTriple();
    std::string          error_message;
    const  llvm::Target* target        = llvm::TargetRegistry::lookupTarget(
        target_triple, error_message
    );
    if( ! target )
    {   msg += error_message;
        return msg;
    }
    //
    // target_machine
    const char*                        cpu      = "generic";
    const char*                        features = "";
    llvm::TargetOptions                target_options;
    llvm::Optional<llvm::Reloc::Model> reloc_model;
    llvm::TargetMachine*  target_machine = target->createTargetMachine(
        target_triple, cpu, features, target_options, reloc_model
    );
    //
    // set target for this module
    module_ir_->setTargetTriple(target_triple);
    //
    // set data layout for this module
    module_ir_->setDataLayout( target_machine->createDataLayout() );
    //
    // output_stream
    std::error_code          std_error_code;
    llvm::sys::fs::OpenFlags open_flag = llvm::sys::fs::OF_None;
    llvm::raw_fd_ostream output_stream(
        file_name, std_error_code, open_flag
    );
    if( std_error_code )
    {   msg += std_error_code.message();
        return msg;
    }
    //
    // pass_manager
    llvm::legacy::PassManager pass_manager;
    llvm::CodeGenFileType     file_type = llvm::CGFT_ObjectFile;
    llvm::raw_pwrite_stream*  dwo_out   = nullptr;
    bool not_supported = target_machine->addPassesToEmitFile(
        pass_manager, output_stream, dwo_out, file_type
    );
    if( not_supported )
    {   msg += "Cannot emit object code for " + target_triple;
        return msg;
    }
    //
    // run the passes and create the output file
    pass_manager.run(*module_ir_);
    output_stream.flush();
    //
    // No error
    msg = "";
    return msg;
}

} // END_CPPAD_NAMESPACE
