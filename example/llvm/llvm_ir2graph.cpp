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
$begin llvm_ir2graph$$
$spell
    llvm_ir
    obj
    ind
$$

$section Convert an LLVM Intermediate Representation to a C++ AD Graph$$

$head Syntax$$
$icode%msg% = llvm_ir2graph(%graph_obj%, %module_ir%
    %function_name%, %n_dynamic_ind%, %n_variable_ind%, %n_dependent%
)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head graph_obj$$
The input value of this argument does not matter.
Upon return, it is a the $cref cpp_ad_graph$$ corresponding the function

$head module_ir$$
This is the llvm module that contains the function.

$head function_name$$
This is the name of the function in the module.

$head n_dynamic_ind$$
This is the number of independent dynamic parameters in the function.

$head n_variable_ind$$
This is the number of independent variables in the function.

$head n_dependent$$
This is the number of dependent variables in the function.

$head msg$$
If the return value $icode msg$$ is the empty string,
no error was detected.
Otherwise, $icode msg$$ describes the error and the return value
of $icode graph_obj$$ is unspecified.

$end
*/
# include "llvm_ir2graph.hpp"
// BEGIN_PROTOTYPE
std::string llvm_ir2graph(
    CppAD::cpp_graph&                         graph_obj      ,
    const llvm::Module*                       module_ir      ,
    const std::string&                        function_name  ,
    size_t                                    n_dynamic_ind  ,
    size_t                                    n_variable_ind ,
    size_t                                    n_dependent    )
// END_PROTOTYPE
{   //
    // initialize return value with name of this routine
    std::string msg = "llvm_ir2graph";
    //
    // function_ir
    const llvm::Function* function_ir = module_ir->getFunction(function_name);
    if( ! function_ir )
    {   msg += "Cannot find function " + function_name + " in llvm_ir";
        return msg;
    }
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
    const llvm::Argument *input_ptr  = function_ir->arg_begin() + 0;
    //
    // output_ptr
    const llvm::Argument *output_ptr = function_ir->arg_begin() + 1;
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
    graph_obj.function_name_set(function_name);
    graph_obj.n_dynamic_ind_set(n_dynamic_ind);
    graph_obj.n_variable_ind_set(n_variable_ind);
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
                    node = 1 + n_dynamic_ind + n_variable_ind + n_constant;
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
    CppAD::vector<size_t> dependent(n_dependent);
# ifndef NDEBUG
    for(size_t i = 0; i < n_dependent; ++i)
        CPPAD_ASSERT_UNKNOWN( dependent[i] == 0 );
# endif
    //
    // The operands and corresponding type for each instruction
    CppAD::vector<llvm::Value*>       operand;
    CppAD::vector<llvm::Type::TypeID> type_id;
    //
    // node index correspoding to first result
    size_t result_node = n_dynamic_ind + n_variable_ind + n_constant;
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
            CPPAD_ASSERT_UNKNOWN( n_operand == 0 );
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
            }
            break;
        }
    }
    for(size_t i = 0; i < n_dependent; ++i)
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
