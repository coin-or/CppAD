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
$begin llvm_ir_to_graph$$
$spell
    llvm_ir
    obj
    vec
    op
    Ptr
    Mul
    Div
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

$subhead Arithmetic$$
$code FAdd$$,
$code FSub$$,
$code FMul$$,
$code FDiv$$,

$subhead Memory Access$$
$code Load$$,
$code GetElementPtr$$,
$code Store$$.

$subhead Other$$
$code Ret$$,

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
# include <cppad/core/llvm_ir.hpp>
//
# include <llvm/IR/InstIterator.h>
# include <llvm/IR/Constants.h>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_TO_GRAPH
std::string llvm_ir::to_graph(CppAD::cpp_graph&  graph_obj) const
// END_TO_GRAPH
{   //
    // initialize return value with name of this routine
    std::string msg = "llvm_ir::to_graph: ";
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
    // types used by interface to DenseMap
    typedef std::pair<const llvm::Value*, size_t> pair_size;
# ifndef NDEBUG
    //
    // len_input
    const llvm::Argument* len_input  = function_ir->arg_begin() + 0;
    //
    // len_output
    const llvm::Argument* len_output = function_ir->arg_begin() + 2;
# endif
    //
    // input_ptr
    const llvm::Argument* input_ptr  = function_ir->arg_begin() + 1;
    //
    // output_ptr
    const llvm::Argument* output_ptr = function_ir->arg_begin() + 3;
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
    // The operands and corresponding type for each instruction
    // Used by both passes
    CppAD::vector<llvm::Value*>       operand;
    CppAD::vector<llvm::Type::TypeID> type_id;
    //
    // First Pass
    // determine the floating point constants in the graph
    CPPAD_ASSERT_UNKNOWN( graph_obj.constant_vec_size() == 0 );
    for(llvm::const_inst_iterator itr = begin_inst; itr != end_inst; ++itr)
    {   unsigned n_operand = itr->getNumOperands();
        operand.resize(n_operand);
        for(unsigned i = 0; i < n_operand; ++i)
            operand[i] = itr->getOperand(i);

        for(unsigned i = 0; i < n_operand; ++i)
        {   if( llvm::isa<llvm::ConstantFP>(operand[i]) )
            {   size_t node = llvm_value2graph_node.lookup(operand[i]);
                if( node == 0 )
                {   // First occurance of this constant
                    // dbl
                    const llvm::ConstantFP* constant_fp =
                        llvm::dyn_cast<llvm::ConstantFP>(operand[i]);
                    const llvm::APFloat* apfloat = &constant_fp->getValue();
                    double  dbl = apfloat->convertToDouble();
                    //
                    // node
                    size_t n_constant = graph_obj.constant_vec_size();
                    node = 1 + n_dynamic_ind_ + n_variable_ind_ + n_constant;
                    //
                    // add this constant do data structure
                    llvm_value2graph_node.insert(pair_size(operand[i], node));
                    graph_obj.constant_vec_push_back(dbl);
                }
            }
        }
    }
    // n_constant
    size_t n_constant = graph_obj.constant_vec_size();
    //
    // node index 1 corresponds to first element of input vector
    llvm_ptr2graph_node.insert( pair_size(input_ptr, 1) );
    //
    // Dependent variable index 0 corresponds to first lement of output vector.
    // Add one to these indices so zero can be used for value not found.
    llvm_ptr2dep_var_ind.insert( pair_size(output_ptr, 1) );
    //
    // mapping from dependent variable index to graph node index
    CppAD::vector<size_t> dependent(n_variable_dep_);
# ifndef NDEBUG
    for(size_t i = 0; i < n_variable_dep_; ++i)
        CPPAD_ASSERT_UNKNOWN( dependent[i] == 0 );
# endif
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
        for(unsigned i = 0; i < n_operand; ++i)
        {   operand[i] = itr->getOperand(i);
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
        // count or instructions
# ifndef NDEBUG
        size_t or_count   = 0;
        size_t zext_count = 0;
# endif
        //
        // op_code values are defined in llvm/IR/Instructions.def
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
            llvm_value2graph_node.insert( pair_size(result , node) );
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::Br:
            // branch used to abort and return error_no
            CPPAD_ASSERT_UNKNOWN( n_operand = 3 );
            CPPAD_ASSERT_UNKNOWN( type_id[0] == llvm::Type::IntegerTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1] == llvm::Type::LabelTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[2] == llvm::Type::LabelTyID );
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::FAdd:
            case llvm::Instruction::FSub:
            case llvm::Instruction::FMul:
            case llvm::Instruction::FDiv:
            // This instruction creates a new node in the graph that corresonds
            // to the sum of two other nodes.
            CPPAD_ASSERT_UNKNOWN( n_operand == 2 );
            CPPAD_ASSERT_UNKNOWN( result_type_id == llvm::Type::DoubleTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[0]     == llvm::Type::DoubleTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1]     == llvm::Type::DoubleTyID );
            //
            // mapping from this result to the correspondign new node in graph
            llvm_value2graph_node.insert( pair_size(result , ++result_node) );
            //
            // put this operator in the graph
            switch( op_code )
            {   case llvm::Instruction::FAdd:
                graph_obj.operator_vec_push_back( CppAD::graph::add_graph_op );
                break;

                case llvm::Instruction::FSub:
                graph_obj.operator_vec_push_back( CppAD::graph::sub_graph_op );
                break;

                case llvm::Instruction::FMul:
                graph_obj.operator_vec_push_back( CppAD::graph::mul_graph_op );
                break;

                case llvm::Instruction::FDiv:
                graph_obj.operator_vec_push_back( CppAD::graph::div_graph_op );
                break;

                default:
                break;
            }
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
                llvm_ptr2dep_var_ind.insert( pair_size(result, index) );
            }
            else
            {    CPPAD_ASSERT_UNKNOWN( operand[0] == input_ptr );
                // The only use of input_ptr is load values
                //
                // first element of input_ptr corresponds to node index 1
                node = *uint64 + 1;
                llvm_ptr2graph_node.insert( pair_size(result, node) );
            }
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::ICmp:
            // This operand is only used to check len_input and len_output
            CPPAD_ASSERT_UNKNOWN( n_operand == 2 );
            CPPAD_ASSERT_UNKNOWN( result_type_id == llvm::Type::IntegerTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[0]     == llvm::Type::IntegerTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1]     == llvm::Type::IntegerTyID );
            CPPAD_ASSERT_UNKNOWN(
                operand[0] == len_input  || operand[0] == len_output ||
                operand[1] == len_input  || operand[1] == len_output
            );
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::Or:
            // This operand is only used once to or the two ICmp operations
            CPPAD_ASSERT_UNKNOWN( ++or_count < 2 );
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
            case llvm::Instruction::ZExt:
            // This operand is only used once to convert return value fron
            // one bit integer to 32 bit integer.
            CPPAD_ASSERT_UNKNOWN( ++zext_count < 2 );
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

} // END_CPPAD_NAMESPACE
