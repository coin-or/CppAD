# ifndef CPPAD_CORE_ATOMIC_TWO_OPTION_HPP
# define CPPAD_CORE_ATOMIC_TWO_OPTION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_option app}
{xrst_spell
   typedef
}

Set Atomic Function Options
###########################

Syntax
******
| *afun* . ``option`` ( *option_value* )

Scope
*****
These settings do not apply to individual *afun* calls,
but rather all subsequent uses of the corresponding atomic operation
in an :ref:`ADFun-name` object.

atomic_sparsity
***************
Note that, if you use :ref:`optimize-name` , these sparsity patterns are used
to determine the :ref:`dependency<dependency.cpp-name>` relationship between
argument and result variables.

pack_sparsity_enum
==================
If *option_value* is ``atomic_base`` < *Base* >:: ``pack_sparsity_enum`` ,
then the type used by *afun* for
:ref:`sparsity patterns<glossary@Sparsity Pattern>` ,
(after the option is set) will be

   ``typedef CppAD::vectorBool`` *atomic_sparsity*

If *r* is a sparsity pattern
for a matrix :math:`R \in \B{R}^{p \times q}`:
*r* . ``size`` () == *p* * *q* .

bool_sparsity_enum
==================
If *option_value* is ``atomic_base`` < *Base* >:: ``bool_sparsity_enum`` ,
then the type used by *afun* for
:ref:`sparsity patterns<glossary@Sparsity Pattern>` ,
(after the option is set) will be

   ``typedef CppAD::vector<bool>`` *atomic_sparsity*

If *r* is a sparsity pattern
for a matrix :math:`R \in \B{R}^{p \times q}`:
*r* . ``size`` () == *p* * *q* .

set_sparsity_enum
=================
If *option_value* is *atomic_base* < ``Base`` >:: *set_sparsity_enum* ,
then the type used by *afun* for
:ref:`sparsity patterns<glossary@Sparsity Pattern>` ,
(after the option is set) will be

   ``typedef CppAD::vector< std::set<size_t> >`` *atomic_sparsity*

If *r* is a sparsity pattern
for a matrix :math:`R \in \B{R}^{p \times q}`:
*r* . ``size`` () == *p* , and for :math:`i = 0 , \ldots , p-1`,
the elements of *r* [ *i* ] are between zero and :math:`q-1` inclusive.

{xrst_end atomic_two_option}
------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_option.hpp
Setting atomic_base options.
*/

/*!
Setting atomic_base options.

\param option_value
new option value.
*/
template <class Base>
void atomic_base<Base>::option(enum option_enum option_value)
{  switch( option_value )
   {  case pack_sparsity_enum:
      case bool_sparsity_enum:
      case set_sparsity_enum:
      sparsity_ = option_value;
      break;

      default:
      CPPAD_ASSERT_KNOWN(
         false,
         "atoic_base::option: option_value is not valid"
      );
   }
   return;
}

} // END_CPPAD_NAMESPACE
# endif
