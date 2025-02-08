# ifndef CPPAD_CORE_BASE_HASH_HPP
# define CPPAD_CORE_BASE_HASH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin base_hash}
{xrst_spell
   adouble
   valgrind
}

Base Type Requirements for Hash Coding Values
#############################################

Syntax
******
| *code* = ``hash_code`` ( *x* )

Purpose
*******
CppAD uses a table of *Base* type values when recording
``AD`` < *Base* > operations.
A hashing function is used to reduce number of values stored in this table;
for example, it is not necessary to store the value 3.0 every
time it is used as a :ref:`con_dyn_var@Parameter` .

Default
*******
The default hashing function works with the set of bits that correspond
to a *Base* value.
In most cases this works well, but in some cases
it does not. For example, in the
:ref:`base_adolc.hpp-name` case, an ``adouble`` value can have
fields that are not initialized and ``valgrind`` reported an error
when these are used to form the hash code.

x
*
This argument has prototype

   *const* ``Base`` & ``x``

It is the value we are forming a hash code for.

code
****
The return value *code* has prototype

   ``unsigned short`` *code*

It is the hash code corresponding to *x* . This intention is the
commonly used values will have different hash codes.
The hash code must satisfy

   *code* < ``CPPAD_HASH_TABLE_SIZE``

so that it is a valid index into the hash code table.

inline
******
If you define this function, it should declare it to be ``inline`` ,
so that you do not get multiple definitions from different compilation units.

Example
*******
See the ``base_alloc`` :ref:`base_alloc.hpp@hash_code`
and the ``adouble`` :ref:`base_adolc.hpp@hash_code` .

{xrst_end base_hash}
*/

/*!
\def CPPAD_HASH_TABLE_SIZE
the codes retruned by hash_code are between zero and CPPAD_HASH_TABLE_SIZE
minus one.
*/
# define CPPAD_HASH_TABLE_SIZE 10000

# endif
