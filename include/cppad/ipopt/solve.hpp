# ifndef CPPAD_IPOPT_SOLVE_HPP
# define CPPAD_IPOPT_SOLVE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin ipopt_solve}
{xrst_spell
   bvector
   doesn
   fg
   gl
   gu
   lagrange
   maxiter
   naninf
   ng
   nx
   rll
   xl
   zl
   zu
}

Use Ipopt to Solve a Nonlinear Programming Problem
##################################################

Syntax
******
| # ``include <cppad/ipopt/solve.hpp>``
| ``ipopt::solve`` (
| |tab| *options* , *xi* , *xl* , *xu* , *gl* , *gu* , *fg_eval* , *solution*
| )

Purpose
*******
The function ``ipopt::solve`` solves nonlinear programming
problems of the form

.. math::

   \begin{array}{rll}
   {\rm minimize}      & f (x)
   \\
   {\rm subject \; to} & gl \leq g(x) \leq gu
   \\
                  & xl  \leq x   \leq xu
   \end{array}

This is done using
`Ipopt <https://coin-or.github.io/Ipopt>`_
optimizer and CppAD for the derivative and sparsity calculations.

Include File
************
If :ref:`cmake@include_ipopt` is on the cmake command line,
the file ``cppad/ipopt/solve.hpp`` is included by ``cppad/cppad.hpp`` .
Otherwise,
``cppad/ipopt/solve.hpp`` can be included directly
(If ``cppad/cppad.hpp`` has not yet been included,
``cppad/ipopt/solve.hpp`` will automatically include it.)

Bvector
*******
The type *Bvector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``bool`` .

Dvector
*******
The type *DVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``double`` .

options
*******
The argument *options* has prototype

   ``const std::string`` *options*

It contains a list of options.
Each option, including the last option,
is terminated by the ``'\n'`` character.
Each line consists of two or three tokens separated by one or more spaces.

Retape
======
You can set the retape flag with the following syntax:

   ``Retape`` *value*

If the value is ``true`` , ``ipopt::solve`` with retape the
:ref:`operation sequence<glossary@Operation@Sequence>` for each
new value of *x* .
If the value is ``false`` , ``ipopt::solve``
will tape the operation sequence at the value
of *xi* and use that sequence for the entire optimization process.
The default value is ``false`` .

Sparse
======
You can set the sparse Jacobian and Hessian flag with the following syntax:

   ``Sparse`` *value* *direction*

If the value is ``true`` , ``ipopt::solve`` will use a sparse
matrix representation for the computation of Jacobians and Hessians.
Otherwise, it will use a full matrix representation for
these calculations.
The default for *value* is ``false`` .
If sparse is true, retape must be false.

It is unclear if :ref:`sparse_jacobian-name` would be faster user
forward or reverse mode so you are able to choose the direction.
If

   *value* == ``true &&`` *direction* == ``forward``

the Jacobians will be calculated using ``SparseJacobianForward`` .
If

   *value* == ``true &&`` *direction* == ``reverse``

the Jacobians will be calculated using ``SparseJacobianReverse`` .

String
======
You can set any Ipopt string option using a line with the following syntax:

   ``String`` *name* *value*

Here *name* is any valid Ipopt string option
and *value* is its setting.

Numeric
=======
You can set any Ipopt numeric option using a line with the following syntax:

   ``Numeric`` *name* *value*

Here *name* is any valid Ipopt numeric option
and *value* is its setting.

Integer
=======
You can set any Ipopt integer option using a line with the following syntax:

   ``Integer`` *name* *value*

Here *name* is any valid Ipopt integer option
and *value* is its setting.

xi
**
The argument *xi* has prototype

   ``const`` *Vector* & *xi*

and its size is equal to *nx* .
It specifies the initial point where Ipopt starts the optimization process.

xl
**
The argument *xl* has prototype

   ``const`` *Vector* & *xl*

and its size is equal to *nx* .
It specifies the lower limits for the argument in the optimization problem.

xu
**
The argument *xu* has prototype

   ``const`` *Vector* & *xu*

and its size is equal to *nx* .
It specifies the upper limits for the argument in the optimization problem.

gl
**
The argument *gl* has prototype

   ``const`` *Vector* & *gl*

and its size is equal to *ng* .
It specifies the lower limits for the constraints in the optimization problem.

gu
**
The argument *gu* has prototype

   ``const`` *Vector* & *gu*

and its size is equal to *ng* .
It specifies the upper limits for the constraints in the optimization problem.

fg_eval
*******
The argument *fg_eval* has prototype

   *FG_eval* *fg_eval*

where the class *FG_eval* is unspecified except for the fact that
it supports the syntax

| |tab| *FG_eval* :: ``ADvector``
| |tab| *fg_eval* ( *fg* , *x* )

The type *ADvector*
and the arguments to *fg* , *x* have the following meaning:

ADvector
========
The type *FG_eval* :: ``ADvector`` must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``AD<double>`` .

x
=
The *fg_eval* argument *x* has prototype

   ``const`` *ADvector* & *x*

where *nx* = *x* . ``size`` () .

fg
==
The *fg_eval* argument *fg* has prototype

   *ADvector* & *fg*

where 1 + *ng* = *fg* . ``size`` () .
The input value of the elements of *fg* does not matter.
Upon return from *fg_eval* ,

   ``fg`` [0] =

:math:`f (x)`

and   for :math:`i = 0, \ldots , ng-1`,

   ``fg`` [1 + ``i`` ] =

:math:`g_i (x)`

solution
********
The argument *solution* has prototype

   ``ipopt::solve_result<`` *Dvector* >& *solution*

After the optimization process is completed, *solution* contains
the following information:

status
======
The *status* field of *solution* has prototype

   ``ipopt::solve_result<`` *Dvector* >:: ``status_type`` *solution* . ``status``

It is the final Ipopt status for the optimizer.
Here is a list of the possible values for the status:

.. list-table::
   :widths: auto

   * - *status*
     - Meaning
   * - not_defined
     - The optimizer did not return a final status for this problem.
   * - unknown
     - The status returned by the optimizer is not defined in the Ipopt
       documentation for ``finalize_solution`` .
   * - success
     - Algorithm terminated successfully at a point satisfying the convergence
       tolerances (see Ipopt options).
   * - maxiter_exceeded
     - The maximum number of iterations was exceeded (see Ipopt options).
   * - stop_at_tiny_step
     - Algorithm terminated because progress was very slow.
   * - stop_at_acceptable_point
     - Algorithm stopped at a point that was converged,
       not to the 'desired' tolerances, but to 'acceptable' tolerances
       (see Ipopt options).
   * - local_infeasibility
     - Algorithm converged to a non-feasible point
       (problem may have no solution).
   * - user_requested_stop
     - This return value should not happen.
   * - diverging_iterates
     - It the iterates are diverging.
   * - restoration_failure
     - Restoration phase failed, algorithm doesn't know how to proceed.
   * - error_in_step_computation
     - An unrecoverable error occurred while Ipopt tried to
       compute the search direction.
   * - invalid_number_detected
     - Algorithm received an invalid number (such as ``nan`` or ``inf`` )
       from the users function *fg_info* . ``eval`` or from the CppAD evaluations
       of its derivatives
       (see the Ipopt option ``check_derivatives_for_naninf`` ).
   * - internal_error
     - An unknown Ipopt internal error occurred.
       Contact the Ipopt authors through the mailing list.

x
=
The ``x`` field of *solution* has prototype

   *Vector* *solution* . ``x``

and its size is equal to *nx* .
It is the final :math:`x` value for the optimizer.

zl
==
The ``zl`` field of *solution* has prototype

   *Vector* *solution* . ``zl``

and its size is equal to *nx* .
It is the final Lagrange multipliers for the
lower bounds on :math:`x`.

zu
==
The ``zu`` field of *solution* has prototype

   *Vector* *solution* . ``zu``

and its size is equal to *nx* .
It is the final Lagrange multipliers for the
upper bounds on :math:`x`.

g
=
The ``g`` field of *solution* has prototype

   *Vector* *solution* . ``g``

and its size is equal to *ng* .
It is the final value for the constraint function :math:`g(x)`.

lambda
======
The ``lambda`` field of *solution* has prototype

   *Vector* > *solution* . ``lambda``

and its size is equal to *ng* .
It is the final value for the
Lagrange multipliers corresponding to the constraint function.

obj_value
=========
The ``obj_value`` field of *solution* has prototype

   ``double`` *solution* . ``obj_value``

It is the final value of the objective function :math:`f(x)`.
{xrst_toc_hidden
   example/ipopt_solve/get_started.cpp
   example/ipopt_solve/retape.cpp
   example/ipopt_solve/ode_inverse.cpp
}
Example
*******
All the examples return true if it succeeds and false otherwise.

get_started
===========
The file
:ref:`example/ipopt_solve/get_started.cpp<ipopt_solve_get_started.cpp-name>`
is an example and test of ``ipopt::solve``
taken from the Ipopt manual.

retape
======
The file
:ref:`example/ipopt_solve/retape.cpp<ipopt_solve_retape.cpp-name>`
demonstrates when it is necessary to specify
:ref:`ipopt_solve@options@Retape` as true.

ode_inverse
===========
The file
:ref:`example/ipopt_solve/ode_inverse.cpp<ipopt_solve_ode_inverse.cpp-name>`
demonstrates using Ipopt to solve for parameters in an ODE model.

{xrst_end ipopt_solve}
-------------------------------------------------------------------------------
*/
# include <cppad/cppad.hpp>
# include <cppad/ipopt/solve_callback.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
namespace ipopt {
/*!
\file solve.hpp
\brief Implement the ipopt::solve Nonlinear Programming Solver
*/

/*!
Use Ipopt to Solve a Nonlinear Programming Problem

\tparam Bvector
simple vector class with elements of type bool.

\tparam Dvector
simple vector class with elements of type double.

\tparam FG_eval
function object used to evaluate f(x) and g(x); see fg_eval below.
It must also support
\code
   FG_eval::ADvector
\endcode
to dentify the type used for the arguments to fg_eval.

\param options
list of options, one for each line.
Ipopt options (are optional) and have one of the following forms
\code
   String   name  value
   Numeric  name  value
   Integer  name  value
\endcode
The following other possible options are listed below:
\code
   Retape   value
\endcode


\param xi
initial argument value to start optimization procedure at.

\param xl
lower limit for argument during optimization

\param xu
upper limit for argument during optimization

\param gl
lower limit for g(x) during optimization.

\param gu
upper limit for g(x) during optimization.

\param fg_eval
function that evaluates the objective and constraints using the syntax
\code
   fg_eval(fg, x)
\endcode

\param solution
structure that holds the solution of the optimization.
*/
template <class Dvector, class FG_eval>
void solve(
   const std::string&                   options   ,
   const Dvector&                       xi        ,
   const Dvector&                       xl        ,
   const Dvector&                       xu        ,
   const Dvector&                       gl        ,
   const Dvector&                       gu        ,
   FG_eval&                             fg_eval   ,
   ipopt::solve_result<Dvector>&        solution  )
{  bool ok = true;

   typedef typename FG_eval::ADvector ADvector;

   CPPAD_ASSERT_KNOWN(
      xi.size() == xl.size() && xi.size() == xu.size() ,
      "ipopt::solve: size of xi, xl, and xu are not all equal."
   );
   CPPAD_ASSERT_KNOWN(
      gl.size() == gu.size() ,
      "ipopt::solve: size of gl and gu are not equal."
   );
   size_t nx = xi.size();
   size_t ng = gl.size();

   // Create an IpoptApplication
   using Ipopt::IpoptApplication;
   Ipopt::SmartPtr<IpoptApplication> app = new IpoptApplication();

   // process the options argument
   size_t begin_1, end_1, begin_2, end_2, begin_3, end_3;
   begin_1     = 0;
   bool retape          = false;
   bool sparse_forward  = false;
   bool sparse_reverse  = false;
   while( begin_1 < options.size() )
   {  // split this line into tokens
      while( options[begin_1] == ' ')
         begin_1++;
      end_1   = options.find_first_of(" \n", begin_1);
      begin_2 = end_1;
      while( options[begin_2] == ' ')
         begin_2++;
      end_2   = options.find_first_of(" \n", begin_2);
      begin_3 = end_2;
      while( options[begin_3] == ' ')
         begin_3++;
      end_3   = options.find_first_of(" \n", begin_3);

      // check for errors
      CPPAD_ASSERT_KNOWN(
         (end_1 != std::string::npos)  &
         (end_2 != std::string::npos)  &
         (end_3 != std::string::npos)  ,
         "ipopt::solve: missing '\\n' at end of an option line"
      );
      CPPAD_ASSERT_KNOWN(
         (end_1 > begin_1) && (end_2 > begin_2) ,
         "ipopt::solve: an option line does not have two tokens"
      );

      // get first two tokens
      std::string tok_1 = options.substr(begin_1, end_1 - begin_1);
      std::string tok_2 = options.substr(begin_2, end_2 - begin_2);

      // get third token
      std::string tok_3;
      bool three_tok = false;
      three_tok |= tok_1 == "Sparse";
      three_tok |= tok_1 == "String";
      three_tok |= tok_1 == "Numeric";
      three_tok |= tok_1 == "Integer";
      if( three_tok )
      {  CPPAD_ASSERT_KNOWN(
            (end_3 > begin_3) ,
            "ipopt::solve: a Sparse, String, Numeric, or Integer\n"
            "option line does not have three tokens."
         );
         tok_3 = options.substr(begin_3, end_3 - begin_3);
      }

      // switch on option type
      if( tok_1 == "Retape" )
      {  CPPAD_ASSERT_KNOWN(
            (tok_2 == "true") || (tok_2 == "false") ,
            "ipopt::solve: Retape value is not true or false"
         );
         retape = (tok_2 == "true");
      }
      else if( tok_1 == "Sparse" )
      {  CPPAD_ASSERT_KNOWN(
            (tok_2 == "true") || (tok_2 == "false") ,
            "ipopt::solve: Sparse value is not true or false"
         );
         CPPAD_ASSERT_KNOWN(
            (tok_3 == "forward") || (tok_3 == "reverse") ,
            "ipopt::solve: Sparse direction is not forward or reverse"
         );
         if( tok_2 == "false" )
         {  sparse_forward = false;
            sparse_reverse = false;
         }
         else
         {  sparse_forward = tok_3 == "forward";
            sparse_reverse = tok_3 == "reverse";
         }
      }
      else if ( tok_1 == "String" )
         app->Options()->SetStringValue(tok_2.c_str(), tok_3.c_str());
      else if ( tok_1 == "Numeric" )
      {  Ipopt::Number value = std::atof( tok_3.c_str() );
         app->Options()->SetNumericValue(tok_2.c_str(), value);
      }
      else if ( tok_1 == "Integer" )
      {  Ipopt::Index value = std::atoi( tok_3.c_str() );
         app->Options()->SetIntegerValue(tok_2.c_str(), value);
      }
      else
         CPPAD_ASSERT_KNOWN(
         false,
         "ipopt::solve: First token is not one of\n"
         "Retape, Sparse, String, Numeric, Integer"
      );

      begin_1 = end_3;
      while( options[begin_1] == ' ')
         begin_1++;
      if( options[begin_1] != '\n' ) CPPAD_ASSERT_KNOWN(
         false,
         "ipopt::solve: either more than three tokens "
         "or no '\\n' at end of a line"
      );
      begin_1++;
   }
   CPPAD_ASSERT_KNOWN(
      ! ( retape & (sparse_forward | sparse_reverse) ) ,
      "ipopt::solve: retape and sparse both true is not supported."
   );

   // Initialize the IpoptApplication and process the options
   Ipopt::ApplicationReturnStatus status = app->Initialize();
   ok    &= status == Ipopt::Solve_Succeeded;
   if( ! ok )
   {  solution.status = solve_result<Dvector>::unknown;
      return;
   }

   // Create an interface from Ipopt to this specific problem.
   // Note the assumption here that ADvector is same as cppd_ipopt::ADvector
   size_t nf = 1;
   Ipopt::SmartPtr<Ipopt::TNLP> cppad_nlp =
   new CppAD::ipopt::solve_callback<Dvector, ADvector, FG_eval>(
      nf,
      nx,
      ng,
      xi,
      xl,
      xu,
      gl,
      gu,
      fg_eval,
      retape,
      sparse_forward,
      sparse_reverse,
      solution
   );

   // Run the IpoptApplication
   app->OptimizeTNLP(cppad_nlp);

   return;
}

} // end ipopt namespace
} // END_CPPAD_NAMESPACE
# endif
