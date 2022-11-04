# ifndef CPPAD_CORE_TO_CSRC_HPP
# define CPPAD_CORE_TO_CSRC_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>
# include <cppad/core/graph/cpp_graph.hpp>
# include <cppad/local/graph/csrc_writer.hpp>
/*
------------------------------------------------------------------------------
$begin to_csrc$$
$spell
   Json
   cpp
   csrc
   cppad
   nu
   bool
   Vec
   typedef
   const
   namespace
   cdecl
   declspec
   dllimport
   jit
   underbar
$$

$section C Source Code Corresponding to an ADFun Object$$

$head Syntax$$
$codei%
   %fun%.to_csrc(%os%, %c_type%)
%$$

$head Prototype$$
$srcthisfile%
   0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head fun$$
is the $cref/ADFun/adfun/$$ object.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$head os$$
The C source code representation of the function $icode fun$$
is written to $icode os$$.

$head c_type$$
The possible values for this argument are:
$code float$$, $code double$$, or $code long_double$$.

$head JIT Functions$$
$icode%flag% = cppad_jit_%function_name%(
   %nu%, %u%, %ny%, %y%, %compare_change%
)%$$
$codei%
typedef int (*jit_%c_type%)(
   size_t, const %type%*, size_t, %type%*, size_t*
)
%$$
Here $icode type$$ is the same as $icode c_type$$ except that the
underbar in $code long_double$$ is replaced by a space.
The function type $codei%jit_%c_type%$$ is defined in the CppAD namespace.
In the case of the Visual C++ compiler ($code _MSC_VER$$ is defined),
$code __cdecl$$ and $code __declspec(dllimport)$$ are added to
the function type definition.

$head Atomic Callbacks$$
$icode%flag% = cppad_atomic_%function_name%(
   %call_id%, %nu%, %u%, %ny%, %y%, %compare_change%
)%$$
$codei%
typedef int (*jit_%c_type%)(
   size_t, size_t, const %type%*, size_t, %type%*, size_t*
)
%$$
The function type $codei%atomic_%c_type%$$ is defined in the CppAD namespace.
The corresponding function evaluates zero order forward mode for the
atomic function with the specified $icode function_name$$.

$head call_id$$
This argument is only used during atomic four function callbacks,
in which case it is the corresponding
$cref/call_id/atomic_four_call/call_id/$$.

$head nu$$
is the number of independent dynamic parameters
plus number of independent variables for the function $icode fun$$.

$head u$$
is a C vector of size $icode nu$$ containing the independent dynamic parameters
and independent variables
The independent dynamic parameter come first as in the same order as
$cref/dynamic/Independent/dynamic/$$ in the call to $code Independent$$
for this function.
The independent variables are in the same order as
$cref/x/Independent/x/$$ in the call to $code Independent$$ for this function.

$head ny$$
is the number of dependent values for this function
(a dependent value can be a variable, dynamic parameter, or constant parameter).

$head y$$
is a C vector of size $icode ny$$.
This input values of its elements do not matter.
Upon return, it contains the function value correspond to $icode u$$.

$head compare_change$$
This argument is both an input and an output.
The number of comparison operators that change their bool result value
is added to $icode compare_change$$. This way, $icode compare_change$$
can be used to accumulate the number of changes between multiplier calls.

$head flag$$
If this is zero, no error was detected.
If it is one (two), $icode nu$$ ($icode ny$$) does not have its expected value.

$head Restrictions$$
The $code to_csrc$$ routine is not implemented for
$cref/VecAD/vecad/$$ operations.

$children%
   example/jit/jit.omh
%$$
$head Example$$
The section $cref example_jit$$ contains examples and tests
that use $code to_csrc$$.


$end
*/
# include <cppad/local/graph/csrc_writer.hpp>

# ifdef _MSC_VER
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
