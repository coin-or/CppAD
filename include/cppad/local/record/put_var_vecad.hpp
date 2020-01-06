# ifndef CPPAD_LOCAL_RECORD_ADD_VAR_VECAD_HPP
# define CPPAD_LOCAL_RECORD_ADD_VAR_VECAD_HPP
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
$begin put_var_vecad_ind$$

$section Add One Index to End of Combined Variable VecAD Vector$$

$head Syntax$$
$icode%offset% = %rec%.put_var_vecad_ind(%vec_ind%)%$$

$head Purpose$$
For each variable VecAD vector, this routine is used to store the length
of the vector followed by the parameter index corresponding to initial
value in the vector; i.e., the values just before it changed from a parameter
to a variable.

$head vec_ind$$
is the index to be palced at the end of the vector of VecAD indices.

$head offset$$
is the index in the combined variable VecAD vector
where the value $icode vec_ind$$ is stored.
This index starts at zero after the recorder default constructor and
increments by one for each call to put_var_vecad_ind.
*/
template <class Base>
addr_t recorder<Base>::put_var_vecad_ind(addr_t vec_ind)
{   size_t offset = all_var_vecad_ind_.size();
    all_var_vecad_ind_.push_back( vec_ind );
    CPPAD_ASSERT_KNOWN(
        size_t( addr_t( offset ) ) == offset,
        "cppad_tape_addr_type cannot support needed index range"
    );
    return static_cast<addr_t>( offset );
}
/*
------------------------------------------------------------------------------
$begin recorder_put_var_vecad$$
$spell
    Vec
    var
    vecad
$$
$section Tape Initialization for a Variable VecAD Object$$

$head Syntax$$
$icode%offset% = %rec%.put_var_vecad(%length%, %data%)%$$

$head Prototype$$
$srcfile%include/cppad/local/record/put_var_vecad.hpp%
    0%// BEGIN_ADD_VAR_VECAD%// END_ADD_VAR_VECAD%1
%$$

$head Usage$$
This routine should be called once for each variable VecAD object just
before it changes from a parameter to a variable.

$head length$$
is the size of the VecAD object.

$head data$$
vector of initial values for the VecAD object
(values before it becomes a variable).

$head offset$$
index of the start of this vector in the list of indices
for all the variable VecAD vectors.
The value corresponding to $icode offset$$ is the length of the vector.
There are $icode length$$ more indices following the length.
The values for these indices are the
parameter indices in the tape for the initial value of the corresponding
VecAD element.

$end
*/
// BEGIN_ADD_VAR_VECAD
template <class Base>
addr_t recorder<Base>::put_var_vecad(
    size_t                        length   ,
    const pod_vector_maybe<Base>& data     )
// END_ADD_VAR_VECAD
{   CPPAD_ASSERT_UNKNOWN( length > 0 );
    CPPAD_ASSERT_KNOWN(
        size_t( std::numeric_limits<addr_t>::max() ) >= length,
        "A VecAD vector length is too large fur cppad_tape_addr_type"
    );

    // store the length in VecInd
    addr_t start = put_var_vecad_ind( addr_t(length) );

    // store indices of the values in VecInd
    for(size_t i = 0; i < length; i++)
    {
        addr_t value_index = put_con_par( data[i] );
        put_var_vecad_ind( value_index );
    }

    // return the taddr of the length (where the vector starts)
    return start;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
