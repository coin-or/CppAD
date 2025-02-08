# ifndef CPPAD_CORE_PRINT_FOR_HPP
# define CPPAD_CORE_PRINT_FOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin PrintFor}
{xrst_spell
   notpos
}

Printing AD Values During Forward Mode
######################################

Syntax
******
| *f* . ``Forward`` (0, *x* )
| *f* . ``Forward`` (0, *x* , *s* )
| ``PrintFor`` ( *before* , *value* )
| ``PrintFor`` ( *notpos* , *before* , *value* , *after* )

See Also
********
:ref:`ad_output-name`

Purpose
*******
The :ref:`zero order forward<forward_zero-name>` mode command

   *f* . ``Forward`` (0, *x* )

sets the
:ref:`glossary@Tape@Independent Variable` vector
equal to *x* .
It then computes a value for all of the dependent variables in the
:ref:`operation sequence<glossary@Operation@Sequence>` corresponding
to *f* .
Putting a ``PrintFor`` in the operation sequence,
prints *value* , corresponding to *x* ,
to be printed during zero order forward operations.

f.Forward(0, x)
***************
The objects *f* , *x* , and the purpose
for this operation, are documented in :ref:`Forward-name` .

notpos
******
If present, the argument *notpos* has one of the following prototypes

| |tab| ``const AD`` < *Base* >& *notpos*
| |tab| ``const VecAD`` < *Base* >:: ``reference&`` *notpos*

In this case
the text and *value* will be printed if and only if
*notpos* is not positive (greater than zero) and a finite number.

before
******
The argument *before* has prototype

   ``const char`` * *before*

This text is written to ``std::cout`` before *value* .

value
*****
The argument *value* has one of the following prototypes

| |tab| ``const AD`` < *Base* >& *value*
| |tab| ``const VecAD`` < *Base* >:: ``reference&`` *value*

The *value* , that corresponds to *x* ,
is written to ``std::cout`` during the execution of

   *f* . ``Forward`` (0, *x* )

Note that *value* may be a
:ref:`glossary@Variable` or
:ref:`glossary@Parameter` .
If a parameter is
:ref:`glossary@Parameter@Dynamic` its value
will depend on the previous call to :ref:`new_dynamic-name` .

after
*****
The argument *after* has prototype

   ``const char`` * *after*

This text is written to ``std::cout`` after *value* .

s
*
You can redirect this output to any standard output stream using the syntax

   *f* . ``Forward`` (0, *x* , *s* )

see :ref:`forward_zero@s` in the zero order forward mode documentation.

Discussion
**********
This is helpful for understanding why tape evaluations have trouble.
For example, if one of the operations in *f* is
``log`` ( *value* ) and *value*  < 0 ,
the corresponding result will :ref:`nan-name` .

Example
*******
{xrst_toc_hidden
   example/print_for/print_for.cpp
   example/general/print_for.cpp
}
The program
:ref:`print_for_cout.cpp-name`
is an example and test that prints to standard output.
The output of this program
states the conditions for passing and failing the test.
The function
:ref:`print_for_string.cpp-name`
is an example and test that prints to an standard string stream.
This function automatically check for correct output.

{xrst_end PrintFor}
------------------------------------------------------------------------------
*/

# include <cstring>

namespace CppAD {
   template <class Base>
   void PrintFor(
      const AD<Base>& notpos        ,
      const char*     before        ,
      const AD<Base>& value         ,
      const char*     after         )
   {  CPPAD_ASSERT_NARG_NRES(local::PriOp, 5, 0);

      // check for case where we are not recording operations
      local::ADTape<Base>* tape = AD<Base>::tape_ptr();
      if( tape == nullptr )
         return;

      CPPAD_ASSERT_KNOWN(
         std::strlen(before) <= 1000 ,
         "PrintFor: length of before is greater than 1000 characters"
      );
      CPPAD_ASSERT_KNOWN(
         std::strlen(after) <= 1000 ,
         "PrintFor: length of after is greater than 1000 characters"
      );
      addr_t arg0, arg1, arg2, arg3, arg4;

      // arg[0] = base 2 representation of [Var(notpos), Var(value)]
      arg0 = 0;

      // arg[1] = address for notpos
      if( Constant(notpos) )
         arg1  = tape->Rec_.put_con_par(notpos.value_);
      else if( Dynamic(notpos) )
         arg1  = notpos.taddr_;
      else
      {  arg0 += 1;
         arg1  = notpos.taddr_;
      }

      // arg[2] = address of before
      arg2 = tape->Rec_.PutTxt(before);

      // arg[3] = address for value
      if( Constant(value) )
         arg3  = tape->Rec_.put_con_par(value.value_);
      else if( Dynamic(value) )
         arg3  = value.taddr_;
      else
      {  arg0 += 2;
         arg3  = value.taddr_;
      }

      // arg[4] = address of after
      arg4 = tape->Rec_.PutTxt(after);

      // put the operator in the tape
      tape->Rec_.PutArg(arg0, arg1, arg2, arg3, arg4);
      tape->Rec_.PutOp(local::PriOp);
   }
   // Fold all other cases into the case above
   template <class Base>
   void PrintFor(const char* before, const AD<Base>& value)
   {  PrintFor(AD<Base>(0), before, value, "" ); }
   //
   template <class Base>
   void PrintFor(const char* before, const VecAD_reference<Base>& value)
   {  PrintFor(AD<Base>(0), before, value.ADBase(), "" ); }
   //
   template <class Base>
   void PrintFor(
      const VecAD_reference<Base>& notpos ,
      const char                  *before ,
      const VecAD_reference<Base>& value  ,
      const char                  *after  )
   {  PrintFor(notpos.ADBase(), before, value.ADBase(), after); }
   //
   template <class Base>
   void PrintFor(
      const VecAD_reference<Base>& notpos ,
      const char                  *before ,
      const AD<Base>&              value  ,
      const char                  *after  )
   {  PrintFor(notpos.ADBase(), before, value, after); }
   //
   template <class Base>
   void PrintFor(
      const AD<Base>&              notpos ,
      const char                  *before ,
      const VecAD_reference<Base>& value  ,
      const char                  *after  )
   {  PrintFor(notpos, before, value.ADBase(), after); }
}

# endif
