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
# include <cppad/core/llvm/ir.hpp>
//
# include <llvm/IR/Instructions.h>
# include <llvm/IR/InstIterator.h>
# include <llvm/IR/Constants.h>

//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_TO_GRAPH
std::string llvm_ir::to_graph(CppAD::cpp_graph&  graph_obj) const
// END_TO_GRAPH
{   using graph::graph_op_enum;
    //
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
    // The maps above assume the default constructor for size_t yeilds zero
    CPPAD_ASSERT_UNKNOWN( size_t() == 0 );
    //
    // maps used to detect azmul operators
    llvm::DenseMap<const llvm::Value*, const llvm::Value*>  llvm_left2cmp;
    llvm::DenseMap<const llvm::Value*, const llvm::Value*>  llvm_cmp2select;
    //
    // map from a name to a graph operator
    llvm::StringMap<size_t> name2graph_op;
    //
    // types used by interface to DenseMap
    typedef std::pair<const llvm::Value*, size_t>             pair_size;
    typedef std::pair<const llvm::Value*, const llvm::Value*> pair_value;
    typedef std::pair<llvm::StringRef,    size_t>             string_pair;
    //
    // name2graph_op
    size_t n_graph_op = size_t( graph::n_graph_op );
    for(size_t i_op = 0; i_op < n_graph_op; ++i_op)
    {   graph_op_enum op_enum = graph_op_enum( i_op );
        const char* name = local::graph::op_enum2name[op_enum];
        switch( op_enum )
        {   // unary functions
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
            // add one to the operaotor value so we can use zero for not found
            name2graph_op.insert( string_pair(name, i_op + 1) );
            break;

            case graph::abs_graph_op:
            name2graph_op.insert( string_pair("cppad_link_fabs", i_op + 1) );
            break;

            default:
            break;
        }
    }
    //
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
    // The zero floating point constant
# ifndef NDEBUG
    llvm::Value* fp_zero = llvm::ConstantFP::get(
        *context_ir_, llvm::APFloat(0.0)
    );
# endif
    //
    // First Pass
    // determine the floating point constants in the graph
    CPPAD_ASSERT_UNKNOWN( graph_obj.constant_vec_size() == 0 );
    for(llvm::const_inst_iterator itr = begin_inst; itr != end_inst; ++itr)
    {   unsigned n_operand = itr->getNumOperands();
        operand.resize(n_operand);
        for(unsigned i = 0; i < n_operand; ++i)
            operand[i] = itr->getOperand(i);
        //
        // constant_vec in graph.obj
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
        //
        // azmul map chain
        const unsigned op_code    = itr->getOpcode();
        const llvm::Value* result = llvm::dyn_cast<llvm::Value>( &(*itr) );
        switch( op_code )
        {
            case llvm::Instruction::FCmp:
            // The assert below assume operator came from an azmul operation.
            CPPAD_ASSERT_UNKNOWN( n_operand == 2 );
            CPPAD_ASSERT_UNKNOWN( operand[1] == fp_zero );
# ifndef NDEBUG
            {   const llvm::CmpInst* cmp_inst =
                    llvm::dyn_cast<llvm::CmpInst>(&*itr);
                CPPAD_ASSERT_UNKNOWN(
                    cmp_inst->getPredicate() == llvm::CmpInst::FCMP_OEQ
                );
            }
# endif
            // start of chain from multiply left operand to select result
            llvm_left2cmp.insert( pair_value( operand[0], result ) );
            break;

            case llvm::Instruction::Select:
            CPPAD_ASSERT_UNKNOWN( n_operand == 3 );
            // end of chain from multiply left operand to select result
            llvm_cmp2select.insert( pair_value( operand[0], result ) );
            break;
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
        size_t                   i_op;
        std::string              str;
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
            CPPAD_ASSERT_UNKNOWN( n_operand == 3 );
            CPPAD_ASSERT_UNKNOWN( type_id[0] == llvm::Type::IntegerTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1] == llvm::Type::LabelTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[2] == llvm::Type::LabelTyID );
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::Call:
            // We assume that this is a cmath call
            CPPAD_ASSERT_UNKNOWN( n_operand == 2 );
            CPPAD_ASSERT_UNKNOWN( type_id[0] == llvm::Type::DoubleTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[1] == llvm::Type::PointerTyID );
            str  = operand[1]->getName().str();
            i_op = name2graph_op.lookup( str.c_str() );
            if( i_op == 0 )
            {   msg += "Cannot call the function " + str;
                return msg;
            }
            // subtract one that was added so zero means not defined
            graph_obj.operator_vec_push_back( graph_op_enum(i_op - 1) );
            //
            // mapping from this result to the correspondign new node in graph
            llvm_value2graph_node.insert( pair_size(result , ++result_node) );
            //
            // put this operator in the graph
            node = llvm_value2graph_node.lookup(operand[0]);
            CPPAD_ASSERT_UNKNOWN( node != 0 );
            graph_obj.operator_arg_push_back( node );
            break;
            //
            // --------------------------------------------------------------
            case llvm::Instruction::FCmp:
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
                graph_obj.operator_vec_push_back( graph::add_graph_op );
                break;

                case llvm::Instruction::FSub:
                graph_obj.operator_vec_push_back( graph::sub_graph_op );
                break;

                case llvm::Instruction::FMul:
                {   const llvm::Value* left   = operand[0];
                    const llvm::Value* cmp    = llvm_left2cmp.lookup(left);
                    const llvm::Value* select = llvm_cmp2select.lookup(cmp);
                    graph_op_enum op;
                    if( select == nullptr )
                    {   // this is a normal multiply
                        op = graph::mul_graph_op;
                    }
                    else
                    {   // this is an absolute zero multiply
                        op = graph::azmul_graph_op;
                    }
                    graph_obj.operator_vec_push_back(op);
                }
                break;

                case llvm::Instruction::FDiv:
                graph_obj.operator_vec_push_back( graph::div_graph_op );
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
            case llvm::Instruction::FNeg:
            CPPAD_ASSERT_UNKNOWN( n_operand == 1 );
            CPPAD_ASSERT_UNKNOWN( result_type_id == llvm::Type::DoubleTyID );
            CPPAD_ASSERT_UNKNOWN( type_id[0]     == llvm::Type::DoubleTyID );
            //
            // mapping from this result to the correspondign new node in graph
            llvm_value2graph_node.insert( pair_size(result , ++result_node) );
            //
            // put this operator in the graph
            graph_obj.operator_vec_push_back( graph::neg_graph_op );
            node = llvm_value2graph_node.lookup(operand[0]);
            CPPAD_ASSERT_UNKNOWN( node != 0 );
            graph_obj.operator_arg_push_back( node );
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
            case llvm::Instruction::Select:
            CPPAD_ASSERT_UNKNOWN( n_operand == 3 );
            CPPAD_ASSERT_UNKNOWN( operand[1] == fp_zero );
            {   const llvm::Value* mul = operand[2];
                node  = llvm_value2graph_node.lookup(mul);
                llvm_value2graph_node.insert( pair_size(result, node ) );
            }
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
