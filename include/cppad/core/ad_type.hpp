# ifndef CPPAD_CORE_AD_TYPE_HPP
# define CPPAD_CORE_AD_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/is_pod.hpp>

# ifdef NDEBUG
# define CPPAD_ASSERT_AD_TYPE(ad_obj)
# else
# define CPPAD_ASSERT_AD_TYPE(ad_obj)               \
   switch(ad_obj.ad_type_)                          \
   {  case constant_enum:                           \
      CPPAD_ASSERT_UNKNOWN( ad_obj.tape_id_ == 0 ); \
      break;                                        \
                                                    \
      case dynamic_enum:                            \
      case variable_enum:                           \
      break;                                        \
                                                    \
      default:                                      \
      CPPAD_ASSERT_UNKNOWN(false);                  \
   }                                                \
   CPPAD_ASSERT_UNKNOWN(                            \
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
{xrst_begin ad_type_enum dev}
{xrst_spell
   typedef
}

Type of AD an Object
####################

typedef
*******
This typedef is in the ``CppAD`` namespace:
{xrst_literal
   // BEGIN TYPEDEF
   // END TYPEDEF
}

is_pod
******
The following informs :ref:`is_pod-name` that this is plain old data.
{xrst_literal
   // BEGIN IS_POD
   // END IS_POD
}

Atomic Function
***************
Only some of the values are valid for the user atomic function API; see
:ref:`atomic_three<atomic_three_define@ad_type>` and
:ref:`atomic_four<atomic_four_for_type@ad_type>` .

ASSERT_AD_TYPE
**************
If *ad_obj* is an ``AD`` < *Base* > object, the syntax

   ``CPPAD_ASSERT_AD_TYPE`` ( *ad_obj* )

check that *ad_obj* satisfies the following conditions:

#. *ad_obj* . ``ad_type_`` is one of the following:
   ``constant_enum`` , ``dynamic_enum`` , ``variable_enum`` .
#. *ad_obj* . ``ad_type_`` is ``constant_enum`` , then
   *ad_obj* . ``tape_id_`` == 0 .

{xrst_end ad_type_enum}
*/



# endif
