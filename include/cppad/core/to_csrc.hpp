# ifndef CPPAD_CORE_TO_CSRC_HPP
# define CPPAD_CORE_TO_CSRC_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>
# include <cppad/core/graph/cpp_graph.hpp>
# include <cppad/local/graph/csrc_writer.hpp>
/*
------------------------------------------------------------------------------
{xrst_begin to_csrc}
{xrst_spell
   cdecl
   declspec
   dllimport
   ny
   typedef
   underbar
}

C Source Code Corresponding to an ADFun Object
##############################################

Syntax
******
| *fun* . ``to_csrc`` ( *os* , *c_type* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

fun
***
is the :ref:`adfun-name` object.

Base
****
is the type corresponding to this :ref:`adfun-name` object;
i.e., its calculations are done using the type *Base* .

RecBase
*******
in the prototype above, *RecBase* is the same type as *Base* .

os
**
The C source code representation of the function *fun*
is written to *os* .

c_type
******
The possible values for this argument are:
``float`` , ``double`` , or ``long_double`` .

JIT Functions
*************

Function Type
=============
The function type ``jit_``\ *c_type* is defined in the CppAD namespace as:

| ``typedef int`` (* ``jit_``\ *c_type* )(
| |tab| ``size_t`` , ``const`` *type* * , ``size_t`` , *type* * , ``size_t`` *
| )

Here *type* is the same as *c_type* except that the
underbar in ``long_double`` is replaced by a space.
In the case of the Visual C++ compiler (``_MSC_VER`` is defined),
``__cdecl`` and ``__declspec(dllimport)`` are added to
the function type definition.

Syntax
======
| *flag* = ``cppad_jit_``\ *function_name* (
| |tab| *nu* , *u* , *ny* , *y* , *compare_change*
| )

A corresponding function call evaluations zero order forward mode
for the function *fun* and

| *function_name* = *fun*\ ``.function_name_get`` ()

see :ref:`function_name-name` .


Atomic Callbacks
****************

Function Type
=============
The function type ``atomic_``\ *c_type* is defined in the CppAD namespace.

| ``typedef int`` (* ``atomic_`` *c_type* )(
| |tab| ``size_t`` , ``size_t`` , ``const`` *type* * , ``size_t`` , *type* * , ``size_t`` *
| )

Syntax
======
| *flag* = ``cppad_atomic_``\ *atomic_name* (
| |tab| *call_id* , *nu* , *u* , *ny* , *y* , *compare_change*
| )

A corresponding function call evaluates zero order forward mode for the
atomic function with the specified *atomic_name* ; see
atomic_four :ref:`atomic_four_ctor@atomic_four@name` .

call_id
*******
This argument is only used during atomic four function callbacks,
in which case it is the corresponding
:ref:`atomic_four_call@call_id` .

nu
**
is the number of independent dynamic parameters
plus number of independent variables for the function *fun* .

u
*
is a C vector of size *nu* containing the independent dynamic parameters
and independent variables.
The independent dynamic parameter come first as in the same order as
:ref:`Independent@dynamic` in the call to ``Independent``
for this function.
The independent variables are in the same order as
:ref:`Independent@x` in the call to ``Independent`` for this function.

ny
**
is the number of dependent values for this function
(a dependent value can be a variable, dynamic parameter, or constant parameter).

y
*
is a C vector of size *ny* .
This input values of its elements do not matter.
Upon return, it contains the function value correspond to *u* .

compare_change
**************
This argument is both an input and an output.
The number of comparison operators that change their bool result value
is added to *compare_change* . This way, *compare_change*
can be used to accumulate the number of changes between multiplier calls.

flag
****
If this is zero, no error was detected.
If it is one (two), *nu* ( *ny* ) does not have its expected value.

Restrictions
************
The ``to_csrc`` routine is not implemented for
:ref:`vecad-name` operations.
{xrst_toc_hidden
   example/jit/jit.xrst
}
Example
*******
The section :ref:`example_jit-name` contains examples and tests
that use ``to_csrc`` .

{xrst_end to_csrc}
*/
# include <cppad/local/graph/csrc_writer.hpp>

# if CPPAD_C_COMPILER_MSVC_FLAGS
# define CPPAD_FUN_TYPE __cdecl
# define CPPAD_IMPORT   __declspec(dllimport)
# else
# define CPPAD_FUN_TYPE
# define CPPAD_IMPORT
# endif


namespace CppAD {
   extern "C" {
      //
      // jit_c_type
      CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *jit_float)(
         size_t, const float*, size_t, float*, size_t*
      );
      CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *jit_double)(
         size_t, const double*, size_t, double*, size_t*
      );
      CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *jit_long_double)(
         size_t, const long double*, size_t, long double*, size_t*
      );
      //
      // atomic_c_type
      CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *atomic_float)(
         size_t, size_t, const float*, size_t, float*, size_t*
      );
      CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *atomic_double)(
         size_t, size_t, const double*, size_t, double*, size_t*
      );
      CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *atomic_long_double)(
         size_t, size_t, const long double*, size_t, long double*, size_t*
      );
   }
}

# undef CPPAD_FUN_TYPE
# undef CPPAD_IMPORT

// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void CppAD::ADFun<Base,RecBase>::to_csrc(
   std::ostream&      os     ,
   const std::string& c_type )
// END_PROTOTYPE
{  //
   // type
# ifndef NDEBUG
   bool ok = false;
   ok |= c_type == "float";
   ok |= c_type == "double";
   ok |= c_type == "long_double";
   CPPAD_ASSERT_KNOWN(ok,
      "f.to_csrc: c_type is not one of the following: "
      "float, double, long_double"
      );
# endif
   // to_graph return values
   cpp_graph graph_obj;
   //
   // graph corresponding to this function
   to_graph(graph_obj);
   //
   // os
   local::graph::csrc_writer(os, graph_obj, c_type);
   //
   return;
}

# endif
