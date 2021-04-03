/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
// BEGIN C++
# include <cstdint>

extern "C" int32_t atomic_reciprocal( // return value is called error_no
    int32_t len_input, double* input, int32_t len_output, double* output
)
{   if( len_input != 1 )
        return 2;
    if( len_output != 1 )
        return 3;
    //
    output[0] = 1.0 / input[0];
    //
    return 0;
}
// END C++
