/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin cppadcg_det_minor.cpp$$
$spell
    Cppadcg
$$

$section Cppadcg Speed: Gradient of Determinant by Minor Expansion$$

$head Specifications$$
See $cref link_det_minor$$.

$childtable%
    speed/cppadcg/det_minor_grad.c%
    speed/cppadcg/det_minor_cg.cpp
%$$

$head Implementation$$
$srccode%cpp% */
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/vector.hpp>

# include <map>
extern std::map<std::string, bool> global_option;

// routine created by det_minor_cg
extern "C" int det_minor_grad(int size, const double* x, double* y);

bool link_det_minor(
    size_t                     size     ,
    size_t                     repeat   ,
    CppAD::vector<double>     &matrix   ,
    CppAD::vector<double>     &gradient )
{   assert( matrix.size() == size * size );
    assert( gradient.size() == size * size );
    // --------------------------------------------------------------------
    // check global options
    const char* valid[] = { "onetape", "optimize"};
    size_t n_valid = sizeof(valid) / sizeof(valid[0]);
    typedef std::map<std::string, bool>::iterator iterator;
    //
    for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
    {   if( itr->second )
        {   bool ok = false;
            for(size_t i = 0; i < n_valid; i++)
                ok |= itr->first == valid[i];
            if( ! ok )
                return false;
        }
    }
    if( ! global_option["onetape"] )
        return false;
    // -----------------------------------------------------
    // setup
    // choose a matrix
    while(repeat--)
    {   // choose a matrix
        CppAD::uniform_01(size * size, matrix);

        // compute gradient of determinant
        det_minor_grad( int(size), matrix.data(), gradient.data());
    }
    return true;
}
/* %$$
$end
*/
