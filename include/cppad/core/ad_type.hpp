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
# define CPPAD_ASSERT_AD_TYPE(ad_obj)
# else
# define CPPAD_ASSERT_AD_TYPE(ad_obj)                 \
    switch(ad_obj.ad_type_)                           \
    {   case constant_enum:                           \
        CPPAD_ASSERT_UNKNOWN( ad_obj.tape_id_ == 0 ); \
        break;                                        \
                                                      \
        case dynamic_enum:                            \
        case variable_enum:                           \
        break;                                        \
                                                      \
        default:                                      \
        CPPAD_ASSERT_UNKNOWN(false);                  \
    }                                                 \
    CPPAD_ASSERT_UNKNOWN(                             \
        ad_obj.tape_id_ == 0 ||                       \
        ad_obj.ad_type_ == dynamic_enum ||            \
        ad_obj.ad_type_ == variable_enum              \
    );
# endif


namespace CppAD {
    // BEGIN TYPEDEF
    typedef enum {
        identical_zero_enum,      // identically zero
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
    obj
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
$cref/atomic_three/atomic_three_define/ad_type/$$ and
$cref/atomic_four/atomic_four_for_type/ad_type/$$.

$head ASSERT_AD_TYPE$$
If $icode ad_obj$$ is an $codei%AD<%Base%>%$$ object, the syntax
$codei%
    CPPAD_ASSERT_AD_TYPE(%ad_obj%)
%$$
check that $icode ad_obj$$ satisfies the following conditions:

$list number$$
$icode%ad_obj%.ad_type_%$$ is one of the following:
$code constant_enum$$, $code dynamic_enum$$, $code variable_enum$$.
$lnext
$icode%ad_obj%.ad_type_%$$ is $code constant_enum$$, then
$icode%ad_obj%.tape_id_ == 0%$$.
$lend

$end
*/



# endif
