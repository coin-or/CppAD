# ifndef CPPAD_CORE_AD_TYPE_HPP
# define CPPAD_CORE_AD_TYPE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/local/is_pod.hpp>

# ifdef NDEBUG
# define CPPAD_ASSERT_AD_TYPE(ad_type)
# else
# define CPPAD_ASSERT_AD_TYPE(ad_type)          \
    switch(ad_type)                             \
    {   case constant_enum:                     \
        case dynamic_enum:                      \
        case variable_enum:                     \
        break;                                  \
                                                \
        default:                                \
        CPPAD_ASSERT_UNKNOWN(false);            \
    }
# endif


namespace CppAD {
    // BEGIN TYPEDEF
    typedef enum {
        identical_zero__enum,     // identically zero
        constant_enum,            // constant parameter
        dynamic_enum,             // dynamic parameter
        variable_enum,            // variable
        number_ad_type_enum       // number of valid values for type_ad_enum
    } ad_type_enum;
    // END TYPEDEF

    // BEGIN IS_POD
    namespace local {
        template <> inline bool
        is_pod<ad_type_enum>(void) { return true; }
    }
    // END IS_POD
}


/*
$begin ad_type_enum$$
$spell
    enum
    typedef
    CppAD
    namespace
$$

$section Type of AD an Object$$

$head typedef$$
This typedef is in the $code CppAD$$ namespace:
$srcthisfile%0%// BEGIN TYPEDEF%// END TYPEDEF%0%$$

$head is_pod$$
The following informs $cref is_pod$$ that this is plain old data.
$srcthisfile%0%// BEGIN IS_POD%// END IS_POD%0%$$

$head Atomic Function$$
Only some of the values are valid for the user atomic function API; see
$cref/atomic_three/atomic_three/ad_type/$$ and
$cref/atomic_four/atomic_four_for_type/ad_type/$$.

$head ASSERT_AD_TYPE$$
The syntax $codei%CPPAD_ASSERT_AD_TYPE(%ad_type%)%$$
is used to check that an AD type is one of the following:
$code constant_enum$$, $code dynamic_enum$$, $code variable_enum$$.
The other values are only used for special purposes.

$end
*/



# endif
