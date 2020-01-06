# ifndef CPPAD_LOCAL_RECORD_PUT_DYN_VECAD_HPP
# define CPPAD_LOCAL_RECORD_PUT_DYN_VECAD_HPP
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
-------------------------------------------------------------------------------
$begin put_dyn_vecad_ind$$
$spell
    Vec
    dyn
    vecad
    ind
    dyn
$$

$section Add One Index to End of Combined Dynamic VecAD Vector$$

$head Syntax$$
$icode%offset% = %rec%.put_dyn_vecad_ind(%vec_ind%)%$$

$head Prototype$$
$srcfile%include/cppad/local/record/put_dyn_vecad.hpp%
    0%// BEGIN_PUT_DYN_VECAD_IND%// END_PUT_DYN_VECAD_IND%1
%$$

$head Purpose$$
For each dynamic parameter VecAD vector, this routine is used to store the
length of the vector followed by the parameter index corresponding to initial
value in the vector; i.e., the values just before it changed from a
constant parameter to a dynamic parameter.

$head vec_ind$$
is the index to be placed at the end of the combined vector of VecAD indices.

$head offset$$
is the index in the combined dynamic parameter VecAD vector
where the value $icode vec_ind$$ is stored.
This index starts at zero after the recorder default constructor and
increments by one for each call to put_dyn_vecad_ind.

$end
*/
// BEGIN_PUT_DYN_VECAD_IND
template <class Base>
addr_t recorder<Base>::put_dyn_vecad_ind(addr_t vec_ind)
// END_PUT_DYN_VECAD_IND
{   size_t offset = all_dyn_vecad_ind_.size();
    all_dyn_vecad_ind_.push_back( vec_ind );
    CPPAD_ASSERT_KNOWN(
        size_t( addr_t( offset ) ) == offset,
        "cppad_tape_addr_type cannot support needed index range"
    );
    return static_cast<addr_t>( offset );
}
/*
------------------------------------------------------------------------------
$begin recorder_put_dyn_vecad$$
$spell
    Vec
    var
    vecad
    dyn
$$
$section Tape Initialization for a Variable VecAD Object$$

$head Syntax$$
$icode%offset% = %rec%.put_dyn_vecad(%length%, %data%)%$$

$head Prototype$$
$srcfile%include/cppad/local/record/put_dyn_vecad.hpp%
    0%// BEGIN_PUT_DYN_VECAD_VEC%// END_PUT_DYN_VECAD_VEC%1
%$$

$head Usage$$
This routine should be called once for each dynamic VecAD object just
before it changes from a constant parameter to a dynamic parameter.

$head length$$
is the size of the VecAD object.

$head data$$
vector of initial values for the VecAD object
(values just before it becomes a dynamic parameter).

$head offset$$
index of the start of this VecAD vector in the combined dynamic VecAD vector.
The value corresponding to $icode offset$$ is the length of this VecAD vector.
There are $icode length$$ more indices following the length.
The values for these indices are the
constant parameter indices in the tape for the initial value of the
corresponding VecAD element.

$end
*/
// BEGIN_PUT_DYN_VECAD_VEC
template <class Base>
addr_t recorder<Base>::put_dyn_vecad(
    size_t                        length   ,
    const pod_vector_maybe<Base>& data     )
// END_PUT_DYN_VECAD_VEC
{   CPPAD_ASSERT_UNKNOWN( length > 0 );
    CPPAD_ASSERT_KNOWN(
        size_t( std::numeric_limits<addr_t>::max() ) >= length,
        "A VecAD vector length is too large fur cppad_tape_addr_type"
    );

    // store the length in VecInd
    addr_t start = put_dyn_vecad_ind( addr_t(length) );

    // store indices of the values in VecInd
    for(size_t i = 0; i < length; i++)
    {
        addr_t value_index = put_con_par( data[i] );
        put_dyn_vecad_ind( value_index );
    }

    // return the taddr of the length (where the vector starts)
    return start;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
