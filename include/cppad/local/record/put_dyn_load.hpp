# ifndef CPPAD_LOCAL_RECORD_PUT_DYN_LOAD_HPP
# define CPPAD_LOCAL_RECORD_PUT_DYN_LOAD_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/local/record/recorder.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
$begin recorder_put_dyn_load$$
$spell
    dyn
    Vec
$$

$section Put a Dynamic Parameter Load Operator in Recording$$

$head Syntax$$
$icode%parameter_index% = %rec%.put_dyn_load(%par%, %offset%, %vector_index%)%$$

$head Prototype$$
$srcfile%include/cppad/local/record/put_dyn_load.hpp%
    0%// BEGIN_PUT_DYN_LOAD%// END_PROTOTYPE%1
%$$

$head par$$
is the parameter value during the recording of this VecAD load operation.

$head offset$$
is the offset of this VecAD vector in combined VecAD array.
This corresponds to the first element of this vector and not its size
(which comes just before the first element).

$head vector_index$$
is the parameter index corresponding to the index in the
corresponding $cref VecAD$$ for this load operation.

$head parameter_index$$
Is the parameter index in the tape corresponding to this load operation.

$end
*/
// BEGIN_PUT_DYN_LOAD
template <class Base>
addr_t recorder<Base>::put_dyn_load(
    const  Base&  par          ,
    addr_t        offset       ,
    addr_t        vector_index )
// END_PROTOTYPE
{   CPPAD_ASSERT_UNKNOWN( num_arg_dyn(load_dyn) == 3 );

    // index of this load operation
    addr_t load_index = addr_t( num_load_op_rec_ );

    // parameter_index
    addr_t parameter_index = addr_t( all_par_vec_.size() );

    // put dynamic parameter in recording
    all_par_vec_.push_back( par );
    dyn_par_is_.push_back( true );

    // operator corresponding to this dynamic parameter
    dyn_par_op_.push_back( opcode_t(load_dyn) );

    // arg[0]
    dyn_par_arg_.push_back( offset );

    // arg[1]
    dyn_par_arg_.push_back( load_index );

    // arg[2]
    dyn_par_arg_.push_back( vector_index );

    // add this VecAD load operation to the total count
    ++num_load_op_rec_;

    // parameter_index
    return parameter_index;
}
} } // END_CPPAD_LOCAL_NAMESPACE
# endif
