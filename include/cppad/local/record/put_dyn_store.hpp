# ifndef CPPAD_LOCAL_RECORD_PUT_DYN_STORE_HPP
# define CPPAD_LOCAL_RECORD_PUT_DYN_STORE_HPP
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
$begin recorder_put_dyn_store$$
$spell
    dyn
    Vec
$$

$section Put a Dynamic Parameter Load Operator in Recording$$

$head Syntax$$
$icode%rec%.put_dyn_store(%offset%, %vector_index%, %right%)%$$

$head Prototype$$
$srcfile%include/cppad/local/record/put_dyn_store.hpp%
    0%// BEGIN_PUT_DYN_LOAD%// END_PROTOTYPE%1
%$$

$head offset$$
is the offset of this VecAD vector in combined dynamic VecAD array.
This corresponds to the first element of this vector and not its size
(which comes just before the first element).

$head vector_index$$
is the parameter index corresponding to the index in the
corresponding $cref VecAD$$ for this store operation.

$head right$$
is the parameter index corresponding to the right hand size in the
corresponding $cref VecAD$$ for this store operation.

$end
*/
// BEGIN_PUT_DYN_LOAD
template <class Base>
void recorder<Base>::put_dyn_store(
    addr_t        offset       ,
    addr_t        vector_index ,
    addr_t        right        )
// END_PROTOTYPE
{   CPPAD_ASSERT_UNKNOWN( num_arg_dyn(store_dyn) == 3 );
    double nan = std::numeric_limits<double>::quiet_NaN();

    // put dummy dynamic parameter in recording do that
    // number of dynamic parametrer is same as number dynamic operators.
    all_par_vec_.push_back( Base(nan) );
    dyn_par_is_.push_back( true );

    // operator corresponding to this dynamic parameter
    dyn_par_op_.push_back( opcode_t(store_dyn) );

    // arg[0]
    dyn_par_arg_.push_back( offset );

    // arg[1]
    dyn_par_arg_.push_back( vector_index );

    // arg[2]
    dyn_par_arg_.push_back( right );
}
} } // END_CPPAD_LOCAL_NAMESPACE
# endif
