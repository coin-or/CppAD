# ifndef CPPAD_LOCAL_RECORD_PUT_VAR_VECAD_HPP
# define CPPAD_LOCAL_RECORD_PUT_VAR_VECAD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/record/recorder.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
------------------------------------------------------------------------------
{xrst_begin put_var_vecad_ind dev}

Add One Index to End of Combined Variable VecAD Vector
######################################################

Syntax
******
| *offset* = *rec* . ``put_var_vecad_ind`` ( *vec_ind* )

Prototype
*********
{xrst_literal
   // BEGIN_PUT_VAR_VECAD_IND
   // END_PUT_VAR_VECAD_IND
}

Purpose
*******
For each variable VecAD vector, this routine is used to store the length
of the vector followed by the parameter index corresponding to initial
value in the vector; i.e., the values just before it changed from a parameter
to a variable.

vec_ind
*******
is the index to be placed at the end of the combined vector of VecAD indices.

offset
******
is the index in the combined variable VecAD vector
where the value *vec_ind* is stored.
This index starts at zero after the recorder default constructor and
increments by one for each call to put_var_vecad_ind.

{xrst_end put_var_vecad_ind}
*/
// BEGIN_PUT_VAR_VECAD_IND
template <class Base>
addr_t recorder<Base>::put_var_vecad_ind(addr_t vec_ind)
// END_PUT_VAR_VECAD_IND
{  size_t offset = all_var_vecad_ind_.size();
   all_var_vecad_ind_.push_back( vec_ind );
   CPPAD_ASSERT_KNOWN(
      size_t( addr_t( offset ) ) == offset,
      "cppad_tape_addr_type cannot support needed index range"
   );
   return static_cast<addr_t>( offset );
}
/*
------------------------------------------------------------------------------
{xrst_begin recorder_put_var_vecad dev}
{xrst_spell
   taddr
}
Tape Initialization for a Variable VecAD Object
###############################################

Syntax
******
*offset* = *rec* . ``put_var_vecad`` ( *length* , *taddr* )

Prototype
*********
{xrst_literal
   // BEGIN_PUT_VAR_VECAD_VEC
   // END_PUT_VAR_VECAD_VEC
}

Usage
*****
This routine should be called once for each variable VecAD object just
before it changes from a parameter to a variable.

length
******
is the size of the VecAD object.

taddr
*****
vector of parameter indices corresponding to the value of this VecAD vector
just before it becomes a variable.

offset
******
index of the start of this VecAD vector in the combined variable VecAD vector.
The value corresponding to *offset* is the length of this VecAD vector.
There are *length* more indices following the length.
These values are the parameter indices.

{xrst_end recorder_put_var_vecad}
*/
// BEGIN_PUT_VAR_VECAD_VEC
template <class Base>
addr_t recorder<Base>::put_var_vecad(
   size_t                        length   ,
   const pod_vector<addr_t>&     taddr    )
// END_PUT_VAR_VECAD_VEC
{  CPPAD_ASSERT_UNKNOWN( length > 0 );
   CPPAD_ASSERT_UNKNOWN( length == taddr.size() );
   CPPAD_ASSERT_KNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >= length,
      "A VecAD vector length is too large fur cppad_tape_addr_type"
   );

   // store the length in VecInd
   addr_t start = put_var_vecad_ind( addr_t(length) );

   // store indices of the values in VecInd
   for(size_t i = 0; i < length; i++)
      put_var_vecad_ind( taddr[i] );

   // return the taddr of the length (where the vector starts)
   return start;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
