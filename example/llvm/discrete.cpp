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
extern "C" double discrete_heaviside(double x)
{   if( x < 0.0 )
        return 0.0;
    if( x == 0.0 )
        return 0.5;
    return 1.0;
}
// END C++
