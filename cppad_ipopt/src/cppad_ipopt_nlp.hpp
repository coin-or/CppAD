# ifndef CPPAD_CPPAD_IPOPT_SRC_CPPAD_IPOPT_NLP_HPP
# define CPPAD_CPPAD_IPOPT_SRC_CPPAD_IPOPT_NLP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cppad_ipopt_nlp app}
{xrst_spell
   fg
   infeasibility
   iterates
   lagrange
   libs
   lipopt
   maxiter
   naninf
   rll
   unrecoverable
}
Nonlinear Programming Using the CppAD Interface to Ipopt
########################################################

Deprecated 2012-11-28
*********************
This interface to Ipopt is deprecated, use :ref:`ipopt_solve-name` instead.

Syntax
******

| # ``include`` ``"cppad_ipopt_nlp.hpp"``
| ``cppad_ipopt_solution`` *solution* ;
| ``cppad_ipopt_nlp`` *cppad_nlp* (
| |tab| *n* , *m* , *x_i* , *x_l* , *x_u* , *g_l* , *g_u* , & *fg_info* , & *solution*
| )
| ``export LD_LIBRARY_PATH`` = ``$LD_LIBRARY_PATH:`` *ipopt_library_paths*

Purpose
*******
The class ``cppad_ipopt_nlp`` is used to solve nonlinear programming
problems of the form

.. math::

   \begin{array}{rll}
   {\rm minimize}      & f(x)
   \\
   {\rm subject \; to} & g^l \leq g(x) \leq g^u
   \\
                  & x^l  \leq x   \leq x^u
   \end{array}

This is done using
`Ipopt <http://www.coin-or.org/projects/Ipopt.xml>`_
optimizer and
`CppAD <http://www.coin-or.org/CppAD/>`_
Algorithmic Differentiation package.

cppad_ipopt namespace
*********************
All of the declarations for these routines
are in the ``cppad_ipopt`` namespace
(not the ``CppAD`` namespace).
For example; :ref:`cppad_ipopt_nlp@SizeVector` below
actually denotes the type ``cppad_ipopt::SizeVector`` .

ipopt_library_paths
*******************
If you are linking to a shared version of the Ipopt library,
you may have to add a path to the ``LD_LIBRARY_PATH`` .
You can determine the directory you need to add using the command

   ``pkg-config ipopt --libs``

The output will have the following form

   ``-L`` *dir* ``-lipopt``

You may need to add the directory %dir% to ``LD_LIBRARY_PATH%`` ; e.g.,

   ``export LD_LIBRARY_PATH`` =" *dir* : ``$LD_LIBRARY_PATH`` "

fg(x)
*****
The function :math:`fg : \B{R}^n \rightarrow \B{R}^{m+1}` is defined by

.. math::
   :nowrap:

   \begin{eqnarray}
      fg_0 (x)     & = & f(x)         \\
      fg_1 (x)     & = & g_0 (x)      \\
                    & \vdots &         \\
      fg_m (x)     & = & g_{m-1} (x)
      \end{eqnarray}

Index Vector
============
We define an *index vector* as a vector of non-negative integers
for which none of the values are equal; i.e.,
it is both a vector and a set.
If :math:`I` is an index vector :math:`|I|` is used to denote the
number of elements in :math:`I` and :math:`\| I \|` is used
to denote the value of the maximum element in :math:`I`.

Projection
==========
Given an index vector :math:`J` and a positive integer :math:`n`
where :math:`n > \| J \|`, we use :math:`J \otimes n` for
the mapping :math:`( J \otimes n ) : \B{R}^n \rightarrow \B{R}^{|J|}` defined by

.. math::

   [ J \otimes n ] (x)_j = x_{J(j)}

for :math:`j = 0 , \ldots |J| - 1`.

Injection
=========
Given an index vector :math:`I` and a positive integer :math:`m`
where :math:`m > \| I \|`, we use :math:`m \otimes I` for
the mapping :math:`( m \otimes I ): \B{R}^{|I|} \rightarrow \B{R}^m` defined by

.. math::

   [ m \otimes I ] (y)_i = \left\{ \begin{array}{ll}
   y_k & {\rm if} \; i = I(k) \; {\rm for \; some} \;
      k \in \{ 0 , \cdots, |I|-1 \}
   \\
   0   & {\rm otherwise}
   \end{array} \right.

Representation
==============
In many applications, each of the component functions of :math:`fg(x)`
only depend on a few of the components of :math:`x`.
In this case, expressing :math:`fg(x)` in terms of simpler functions
with fewer arguments can greatly reduce the amount of work required
to compute its derivatives.

We use the functions
:math:`r_k : \B{R}^{q(k)} \rightarrow \B{R}^{p(k)}`
for :math:`k = 0 , \ldots , K` to express our
representation of :math:`fg(x)` in terms of simpler functions
as follows

.. math::

   fg(x) = \sum_{k=0}^{K-1} \; \sum_{\ell=0}^{L(k) - 1}
   [ (m+1) \otimes I_{k,\ell} ] \; \circ
        \; r_k \; \circ \; [ J_{k,\ell} \otimes n ] \; (x)

where :math:`\circ` represents function composition,
for :math:`k = 0 , \ldots , K - 1`, and :math:`\ell = 0 , \ldots , L(k)`,
:math:`I_{k,\ell}` and  :math:`J_{k,\ell}` are index vectors with
:math:`| J_{k,\ell} | = q(k)`,
:math:`\| J_{k,\ell} \| < n`,
:math:`| I_{k,\ell} | = p(k)`, and
:math:`\| I_{k,\ell} \| \leq m`.

Simple Representation
*********************
In the simple representation,
:math:`r_0 (x) = fg(x)`,
:math:`K = 1`,
:math:`q(0) = n`,
:math:`p(0) = m+1`,
:math:`L(0) = 1`,
:math:`I_{0,0} = (0 , \ldots , m)`,
and :math:`J_{0,0} = (0 , \ldots , n-1)`.

SizeVector
**********
The type ``SizeVector`` is defined by the
``cppad_ipopt_nlp.hpp`` include file to be a
:ref:`SimpleVector-name` class with elements of type
``size_t`` .

NumberVector
************
The type ``NumberVector`` is defined by the
``cppad_ipopt_nlp.hpp`` include file to be a
:ref:`SimpleVector-name` class with elements of type
``Ipopt::Number`` .

ADNumber
********
The type ``ADNumber`` is defined by the
``cppad_ipopt_nlp.hpp`` include file to be a
an AD type that can be used to compute derivatives.

ADVector
********
The type ``ADVector`` is defined by the
``cppad_ipopt_nlp.hpp`` include file to be a
:ref:`SimpleVector-name` class with elements of type
``ADNumber`` .

n
*
The argument *n* has prototype

   ``size_t`` *n*

It specifies the dimension of the argument space;
i.e., :math:`x \in \B{R}^n`.

m
*
The argument *m* has prototype

   ``size_t`` *m*

It specifies the dimension of the range space for :math:`g`;
i.e., :math:`g : \B{R}^n \rightarrow \B{R}^m`.

x_i
***
The argument *x_i* has prototype

   ``const NumberVector&`` *x_i*

and its size is equal to :math:`n`.
It specifies the initial point where Ipopt starts the optimization process.

x_l
***
The argument *x_l* has prototype

   ``const NumberVector&`` *x_l*

and its size is equal to :math:`n`.
It specifies the lower limits for the argument in the optimization problem;
i.e., :math:`x^l`.

x_u
***
The argument *x_u* has prototype

   ``const NumberVector&`` *x_u*

and its size is equal to :math:`n`.
It specifies the upper limits for the argument in the optimization problem;
i.e., :math:`x^u`.

g_l
***
The argument *g_l* has prototype

   ``const NumberVector&`` *g_l*

and its size is equal to :math:`m`.
It specifies the lower limits for the constraints in the optimization problem;
i.e., :math:`g^l`.

g_u
***
The argument *g_u* has prototype

   ``const NumberVector&`` *g_u*

and its size is equal to :math:`n`.
It specifies the upper limits for the constraints in the optimization problem;
i.e., :math:`g^u`.

fg_info
*******
The argument *fg_info* has prototype

   *FG_info fg_info*

where the class *FG_info* is derived from the
base class ``cppad_ipopt_fg_info`` .
Certain virtual member functions of *fg_info* are used to
compute the value of :math:`fg(x)`.
The specifications for these member functions are given below:

fg_info.number_functions
========================
This member function has prototype

   ``virtual size_t cppad_ipopt_fg_info::number_functions`` ( ``void`` )

If *K* has type ``size_t`` , the syntax

   *K* = *fg_info* . ``number_functions`` ()

sets *K* to the number of functions used in the
representation of :math:`fg(x)`; i.e., :math:`K` in
the :ref:`cppad_ipopt_nlp@fg(x)@Representation` above.

The ``cppad_ipopt_fg_info`` implementation of this function
corresponds to the simple representation mentioned above; i.e.
*K*  = 1 .

fg_info.eval_r
==============
This member function has the prototype

   ``virtual ADVector cppad_ipopt_fg_info::eval_r`` ( ``size_t`` *k* , ``const ADVector&`` *u* ) = 0;

Thus it is a pure virtual function and must be defined in the
derived class *FG_info* .

This function computes the value of :math:`r_k (u)`
used in the :ref:`cppad_ipopt_nlp@fg(x)@Representation`
for :math:`fg(x)`.
If *k* in :math:`\{0 , \ldots , K-1 \}` has type ``size_t`` ,
*u* is an ``ADVector`` of size *q* ( *k* )
and *r* is an ``ADVector`` of size *p* ( *k* )
the syntax

   *r* = *fg_info* . ``eval_r`` ( *k* , *u* )

set *r* to the vector :math:`r_k (u)`.

fg_info.retape
==============
This member function has the prototype

   ``virtual bool cppad_ipopt_fg_info::retape`` ( ``size_t`` *k* )

If *k* in :math:`\{0 , \ldots , K-1 \}` has type ``size_t`` ,
and *retape* has type ``bool`` ,
the syntax

   *retape* = *fg_info* . ``retape`` ( *k* )

sets *retape* to true or false.
If *retape* is true,
``cppad_ipopt_nlp`` will retape the operation sequence
corresponding to :math:`r_k (u)` for
every value of *u* .
An ``cppad_ipopt_nlp`` object
should use much less memory and run faster if *retape* is false.
You can test both the true and false cases to make sure
the operation sequence does not depend on *u* .

The ``cppad_ipopt_fg_info`` implementation of this function
sets *retape* to true
(while slower it is also safer to always retape).

fg_info.domain_size
===================
This member function has prototype

   ``virtual size_t cppad_ipopt_fg_info::domain_size`` ( ``size_t`` *k* )

If *k* in :math:`\{0 , \ldots , K-1 \}` has type ``size_t`` ,
and *q* has type ``size_t`` , the syntax

   *q* = *fg_info* . ``domain_size`` ( *k* )

sets *q* to the dimension of the domain space for :math:`r_k (u)`;
i.e., :math:`q(k)` in
the :ref:`cppad_ipopt_nlp@fg(x)@Representation` above.

The ``cppad_ipopt_h_base`` implementation of this function
corresponds to the simple representation mentioned above; i.e.,
:math:`q = n`.

fg_info.range_size
==================
This member function has prototype

   ``virtual size_t cppad_ipopt_fg_info::range_size`` ( ``size_t`` *k* )

If *k* in :math:`\{0 , \ldots , K-1 \}` has type ``size_t`` ,
and *p* has type ``size_t`` , the syntax

   *p* = *fg_info* . ``range_size`` ( *k* )

sets *p* to the dimension of the range space for :math:`r_k (u)`;
i.e., :math:`p(k)` in
the :ref:`cppad_ipopt_nlp@fg(x)@Representation` above.

The ``cppad_ipopt_h_base`` implementation of this function
corresponds to the simple representation mentioned above; i.e.,
:math:`p = m+1`.

fg_info.number_terms
====================
This member function has prototype

   ``virtual size_t cppad_ipopt_fg_info::number_terms`` ( ``size_t`` *k* )

If *k* in :math:`\{0 , \ldots , K-1 \}` has type ``size_t`` ,
and *L* has type ``size_t`` , the syntax

   *L* = *fg_info* . ``number_terms`` ( *k* )

sets *L* to the number of terms in representation
for this value of *k* ;
i.e., :math:`L(k)` in
the :ref:`cppad_ipopt_nlp@fg(x)@Representation` above.

The ``cppad_ipopt_h_base`` implementation of this function
corresponds to the simple representation mentioned above; i.e.,
:math:`L = 1`.

fg_info.index
=============
This member function has prototype

| |tab| ``virtual void cppad_ipopt_fg_info::index`` (
| |tab| |tab| ``size_t`` *k* , ``size_t`` *ell* , ``SizeVector&`` *I* , ``SizeVector&`` *J*
| |tab| )

The argument

   *k*

has type ``size_t``
and is a value between zero and :math:`K-1` inclusive.
The argument

   *ell*

has type ``size_t``
and is a value between zero and :math:`L(k)-1` inclusive.
The argument

   *I*

is a :ref:`SimpleVector-name` with elements
of type ``size_t`` and size greater than or equal to :math:`p(k)`.
The input value of the elements of *I* does not matter.
The output value of
the first :math:`p(k)` elements of *I*
must be the corresponding elements of :math:`I_{k,ell}`
in the :ref:`cppad_ipopt_nlp@fg(x)@Representation` above.
The argument

   *J*

is a :ref:`SimpleVector-name` with elements
of type ``size_t`` and size greater than or equal to :math:`q(k)`.
The input value of the elements of *J* does not matter.
The output value of
the first :math:`q(k)` elements of *J*
must be the corresponding elements of :math:`J_{k,ell}`
in the :ref:`cppad_ipopt_nlp@fg(x)@Representation` above.

The ``cppad_ipopt_h_base`` implementation of this function
corresponds to the simple representation mentioned above; i.e.,
for :math:`i = 0 , \ldots , m`,
*I* [ *i* ] = *i* ,
and  for :math:`j = 0 , \ldots , n-1`,
*J* [ *j* ] = *j* .

solution
********
After the optimization process is completed, *solution* contains
the following information:

status
======
The *status* field of *solution* has prototype

   ``cppad_ipopt_solution::solution_status`` *solution* . ``status``

It is the final Ipopt status for the optimizer.
Here is a list of the possible values for the status:

.. list-table::
   :widths: auto

   * - *status*
     - Meaning
   * - not_defined
     - The optimizer did not return a final status to this ``cppad_ipopt_nlp``
       object.
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

   ``NumberVector`` *solution* . ``x``

and its size is equal to :math:`n`.
It is the final :math:`x` value for the optimizer.

z_l
===
The ``z_l`` field of *solution* has prototype

   ``NumberVector`` *solution* . ``z_l``

and its size is equal to :math:`n`.
It is the final Lagrange multipliers for the
lower bounds on :math:`x`.

z_u
===
The ``z_u`` field of *solution* has prototype

   ``NumberVector`` *solution* . ``z_u``

and its size is equal to :math:`n`.
It is the final Lagrange multipliers for the
upper bounds on :math:`x`.

g
=
The ``g`` field of *solution* has prototype

   ``NumberVector`` *solution* . ``g``

and its size is equal to :math:`m`.
It is the final value for the constraint function :math:`g(x)`.

lambda
======
The ``lambda`` field of *solution* has prototype

   ``NumberVector`` *solution* . ``lambda``

and its size is equal to :math:`m`.
It is the final value for the
Lagrange multipliers corresponding to the constraint function.

obj_value
=========
The ``obj_value`` field of *solution* has prototype

   ``Number`` *solution* . ``obj_value``

It is the final value of the objective function :math:`f(x)`.

{xrst_end cppad_ipopt_nlp}
-----------------------------------------------------------------------------
*/
# include <cppad/cppad.hpp>
# include <coin-or/IpIpoptApplication.hpp>
# include <coin-or/IpTNLP.hpp>

/*!
\file cppad_ipopt_nlp.hpp
\brief CppAD interface to Ipopt

\ingroup cppad_ipopt_nlp_cpp
*/

// ---------------------------------------------------------------------------
namespace cppad_ipopt {
// ---------------------------------------------------------------------------

/// A scalar value used to record operation sequence.
typedef CppAD::AD<Ipopt::Number>       ADNumber;
/// A simple vector of values used to record operation sequence
typedef CppAD::vector<ADNumber>        ADVector;
/// A simple vector of size_t values.
typedef CppAD::vector<size_t>          SizeVector;
/// A simple vector of values used by Ipopt
typedef CppAD::vector<Ipopt::Number>   NumberVector;

/*!
Abstract base class user derives from to define the functions in the problem.
*/
class cppad_ipopt_fg_info
{
   /// allow cppad_ipopt_nlp class complete access to this class
   friend class cppad_ipopt_nlp;
private:
   /// domain space dimension for the functions f(x), g(x)
   size_t n_;
   /// range space dimension for the function g(x)
   size_t m_;
   /// the cppad_ipopt_nlp constructor uses this method to set n_
   void set_n(size_t n)
   {  n_ = n; }
   /// the cppad_ipopt_nlp constructor uses this method to set m_
   void set_m(size_t m)
   {  m_ = m; }

public:
   /// destructor virtual so user derived class destructor gets called
   virtual ~cppad_ipopt_fg_info(void)
   { }
   /// number_functions; i.e. K (simple representation uses 1)
   virtual size_t number_functions(void)
   {  return 1; }
   /// function that evaluates the users representation for f(x) and
   /// and g(x) is pure virtual so user must define it in derived class
   virtual ADVector eval_r(size_t k, const ADVector& u) = 0;
   /// should the function r_k (u) be retaped when ever the arguemnt
   /// u changes (default is true which is safe but slow)
   virtual bool retape(size_t k)
   {  return true; }
   /// domain_size q[k] for r_k (u) (simple representation uses n)
   virtual size_t domain_size(size_t k)
   {  return n_; }
   /// range_size p[k] for r_k (u) (simple representation uses m+1)
   virtual size_t range_size(size_t k)
   {  return m_ + 1; }
   /// number_terms that use r_k (u) (simple represenation uses 1)
   virtual size_t number_terms(size_t k)
   {  return 1; }
   /// return the index vectors I_{k,ell} and J_{k,ell}
   /// (simple representation uses I[i] = i and J[j] = j)
   virtual void index(size_t k, size_t ell, SizeVector& I, SizeVector& J)
   {  assert( I.size() >= m_ + 1 );
      assert( J.size() >= n_ );
      for(size_t i = 0; i <= m_; i++)
         I[i] = i;
      for(size_t j = 0; j < n_; j++)
         J[j] = j;
   }
};

/*!
Class that contains information about the problem solution

\section Nonlinear_Programming_Problem Nonlinear Programming Problem
We are give smooth functions
\f$ f : {\bf R}^n \rightarrow {\bf R} \f$
and
\f$ g : {\bf R}^n \rightarrow {\bf R}^m \f$
and wish to solve the problem
\f[
\begin{array}{rcl}
{\rm minimize} & f(x) & {\rm w.r.t.} \; x \in {\bf R}^n
\\
{\rm subject \; to} & g^l \leq g(x) \leq g^u
\\
& x^l \leq x \leq x^u
\end{array}
\f]


\section Users_Representation Users Representation
The functions
\f$ f : {\bf R}^n \rightarrow {\bf R} \f$ and
\f$ g : {\bf R}^n \rightarrow {\bf R}^m \f$ are defined by
\f[
\left( \begin{array}{c} f(x) \\ g(x) \end{array} \right)
=
\sum_{k=0}^{K-1} \; \sum_{\ell=0}^{L(k) - 1}
[ (m+1) \otimes I_{k,\ell} ] \; \circ
     \; r_k \; \circ \; [ J_{k,\ell} \otimes n ] \; (x)
\f]
where for \f$ k = 0 , \ldots , K-1\f$,
\f$ r_k : {\bf R}^{q(k)} \rightarrow {\bf R}^{p(k)} \f$.

\section Deprecated_Evaluation_Methods Evaluation Methods
The set of evaluation methods for this class is
\verbatim
   { eval_f, eval_grad_f, eval_g, eval_jac_g, eval_h }
\endverbatim
Note that the bool return flag for the evaluations methods
does not appear in the Ipopt documentation.
Looking at the code, it seems to be a flag telling Ipopt to abort
when the flag is false.

*/
class cppad_ipopt_solution
{
public:
   /// possible values for he solution status
   enum solution_status {
      not_defined,
      success,
      maxiter_exceeded,
      stop_at_tiny_step,
      stop_at_acceptable_point,
      local_infeasibility,
      user_requested_stop,
      feasible_point_found,
      diverging_iterates,
      restoration_failure,
      error_in_step_computation,
      invalid_number_detected,
      too_few_degrees_of_freedom,
      internal_error,
      unknown
   }  status;
   /// the approximation solution
   NumberVector      x;
   /// Lagrange multipliers corresponding to lower bounds on x
   NumberVector      z_l;
   /// Lagrange multipliers corresponding to upper bounds on x
   NumberVector      z_u;
   /// value of g(x)
   NumberVector      g;
   /// Lagrange multipliers correspondiing constraints on g(x)
   NumberVector      lambda;
   /// value of f(x)
   Ipopt::Number     obj_value;
   /// constructor initializes solution status as not yet defined
   cppad_ipopt_solution(void)
   {  status = not_defined; }
};

/*!
Class connects Ipopt to CppAD for derivative and sparsity pattern calculations.
*/
class cppad_ipopt_nlp : public Ipopt::TNLP
{
private:
   /// A Scalar value used by Ipopt
   typedef Ipopt::Number                         Number;
   /// An index value used by Ipopt
   typedef Ipopt::Index                          Index;
   /// Indexing style used in Ipopt sparsity structure
   typedef Ipopt::TNLP::IndexStyleEnum           IndexStyleEnum;
   /// A simple vector of boolean values
   typedef CppAD::vectorBool                     BoolVector;
   /// A simple vector of AD function objects
   typedef CppAD::vector< CppAD::ADFun<Number> > ADFunVector;
   /// A simple vector of simple vectors of boolean values
   typedef CppAD::vector<BoolVector>             BoolVectorVector;
   /// A mapping that is dense in i, sparse in j, and maps (i, j)
   /// to the corresponding sparsity index in Ipopt.
   typedef CppAD::vector< std::map<size_t,size_t> > IndexMap;

   // ------------------------------------------------------------------
   // Values directly passed in to constuctor
   // ------------------------------------------------------------------
   /// dimension of the domain space for f(x) and g(x)
   /// (passed to ctor)
   const size_t                    n_;
   /// dimension of the range space for g(x)
   /// (passed to ctor)
   const size_t                    m_;
   /// dimension of the range space for g(x)
   /// (passed to ctor)
   const NumberVector              x_i_;
   /// lower limit for x
   /// (size n_), (passed to ctor)
   const NumberVector              x_l_;
   /// upper limit for x
   /// (size n_) (passed to ctor)
   const NumberVector              x_u_;
   /// lower limit for g(x)
   /// (size m_) (passed to ctor)
   const NumberVector              g_l_;
   /// upper limit for g(x)
   /// (size m_) (passed to ctor)
   const NumberVector              g_u_;
   /// pointer to base class version of derived class object used to get
   /// information about the user's representation for f(x) and g(x)
   /// (passed to ctor)
   cppad_ipopt_fg_info* const      fg_info_;
   /// pointer to object where final results are stored
   /// (passed to ctor)
   cppad_ipopt_solution* const     solution_;
   /// plus infinity as a value of type Number
   const Number                    infinity_;

   // ------------------------------------------------------------------
   // Effectively const values determined during constructor using calls
   // to fg_info:
   // ------------------------------------------------------------------
   /// The value of \f$ K \f$ in the representation.
   /// (effectively const)
   size_t                 K_;
   /// Does operation sequence for \f$ r_k (u) \f$ depend on \f$ u \f$.
   /// (size K_) (effectively const)
   BoolVector             retape_;
   /// <tt>q_[k]</tt> is the domain space dimension for \f$ r_k (u) \f$
   /// (size K_) (effectively const)
   SizeVector             q_;
   /// <tt>p_[k]</tt> is the range space dimension for \f$ r_k (u) \f$
   /// (size K_) (effectively const)
   SizeVector             p_;
   /// <tt>L_[k]</tt> is number of times \f$ r_k (u) \f$ appears in
   /// the representation summation
   /// (size K_) (effectively const)
   SizeVector             L_;
   // -------------------------------------------------------------------
   // Other effectively const values determined by the constructor:
   // -------------------------------------------------------------------
   /*!
   CppAD sparsity patterns for \f$ \{ r_k^{(1)} (u) \} \f$ (set by ctor).

   For <tt>k = 0 , ... , K_-1, pattern_jac_r_[k]</tt>
   is a CppAD sparsity pattern for the Jacobian of \f$ r_k (u) \f$
   and as such it has size <tt>p_[k]*q_[k]</tt>.
   (effectively const)
   */
   BoolVectorVector                 pattern_jac_r_;

   /*!
   CppAD sparsity patterns for \f$ \{ r_k^{(2)} (u) \} \f$ (set by ctor).

   For <tt>k = 0 , ... , K_-1, pattern_jac_r_[k]</tt>
   is a CppAD sparsity pattern for the Hessian of
   \f[
      R(u) = \sum_{i=0}^{p[k]-1}  r_k (u)_i
   \f]
   and as such it has size <tt>q_[k]*q_[k]</tt>.
   (effectively const)
   */
   BoolVectorVector                 pattern_hes_r_;

   /// number non-zero is Ipopt sparsity structor for Jacobian of g(x)
   /// (effectively const)
   size_t                           nnz_jac_g_;
   /// row indices in Ipopt sparsity structor for Jacobian of g(x)
   /// (effectively const)
   SizeVector                       iRow_jac_g_;
   /// column indices in Ipopt sparsity structor for Jacobian of g(x)
   /// (effectively const)
   SizeVector                       jCol_jac_g_;

   /// number non-zero is Ipopt sparsity structor for Hessian of Lagragian
   /// (effectively const)
   size_t                           nnz_h_lag_;
   /// row indices in Ipopt sparsity structor for Hessian of Lagragian
   /// (effectively const)
   SizeVector                       iRow_h_lag_;
   /// column indices in Ipopt sparsity structor for Hessian of Lagragian
   /// (effectively const)
   SizeVector                       jCol_h_lag_;

   /*!
   Mapping from (i, j) in Jacobian of g(x) to Ipopt sparsity structure

   For <tt>i = 0 , ... , m_-1, index_jac_g_[i]</tt>
   is a standard map from column index values j to the corresponding
   index in the Ipopt sparsity structure for the Jacobian of g(x).
   */
   IndexMap                         index_jac_g_;

   /*!
   Mapping from (i, j) in Hessian of fg(x) to Ipopt sparsity structure

   For <tt>i = 0 , ... , n_-1, index_hes_fg_[i]</tt>
   is a standard map from column index values j to the corresponding
   index in the Ipopt sparsity structure for the Hessian of the Lagragian.
   */
   IndexMap                         index_hes_fg_;
   // -----------------------------------------------------------------
   // Values that are changed by routine other than the constructor:
   // -----------------------------------------------------------------

   /// For <tt>k = 0 , ... , K_-1, r_fun_[k]</tt>
   /// is a the CppAD function object corresponding to \f$ r_k (u) \f$.
   ADFunVector                      r_fun_;
   /*!
   Is r_fun[k] OK for current x.

   For <tt>k = 0 , ... , K_-1, tape_ok_[k]</tt>
   is true if current operations sequence in <tt>r_fun_[k]</tt>
   OK for this value of \f$ x \f$.
   Note that \f$ u = [ J_{k,\ell} \otimes n ] (x) \f$ may depend on the
   value of \f$ \ell \f$.
   */
   BoolVector             tape_ok_;

   /// work space of size equal maximum of <tt>q[k]</tt> w.r.t k.
   SizeVector             J_;
   /// work space of size equal maximum of <tt>p[k]</tt> w.r.t k.
   SizeVector             I_;
   // ------------------------------------------------------------
   // Private Methods
   // ------------------------------------------------------------
   /// block the default constructor from use
   cppad_ipopt_nlp(const cppad_ipopt_nlp&);
   /// blocks the assignment operator from use
   cppad_ipopt_nlp& operator=(const cppad_ipopt_nlp&);
public:
   // ----------------------------------------------------------------
   // See cppad_ipopt_nlp.cpp for doxygen documentation of these methods
   // ----------------------------------------------------------------

   /// only constructor for cppad_ipopot_nlp
   cppad_ipopt_nlp(
      size_t n                         ,
      size_t m                         ,
      const NumberVector    &x_i       ,
      const NumberVector    &x_l       ,
      const NumberVector    &x_u       ,
      const NumberVector    &g_l       ,
      const NumberVector    &g_u       ,
      cppad_ipopt_fg_info*   fg_info   ,
      cppad_ipopt_solution*  solution
   );

   // use virtual so that derived class destructor gets called.
   virtual ~cppad_ipopt_nlp();

   // return info about the nlp
   virtual bool get_nlp_info(
      Index&          n           ,
      Index&          m           ,
      Index&          nnz_jac_g   ,
      Index&          nnz_h_lag   ,
      IndexStyleEnum& index_style
   );

   // return bounds for my problem
   virtual bool get_bounds_info(
      Index           n   ,
      Number*         x_l ,
      Number*         x_u ,
      Index           m   ,
      Number*         g_l ,
      Number*         g_u
   );

   // return the starting point for the algorithm
   virtual bool get_starting_point(
      Index          n            ,
      bool           init_x       ,
      Number*        x            ,
      bool           init_z       ,
      Number*        z_L          ,
      Number*        z_U          ,
      Index          m            ,
      bool           init_lambda  ,
      Number*        lambda
   );

   // return the objective value
   virtual bool eval_f(
      Index          n           ,
      const Number*  x           ,
      bool           new_x       ,
      Number&        obj_value
   );

   // Method to return the gradient of the objective
   virtual bool eval_grad_f(
      Index          n           ,
      const Number*  x           ,
      bool           new_x       ,
      Number*        grad_f
   );

   // return the constraint residuals
   virtual bool eval_g(
      Index          n           ,
      const Number*  x           ,
      bool           new_x       ,
      Index          m           ,
      Number*        g
   );

   // Method to return:
   // 1) The structure of the jacobian (if "values" is NULL)
   // 2) The values of the jacobian (if "values" is not NULL)
   virtual bool eval_jac_g(
      Index          n           ,
      const Number*  x           ,
      bool           new_x       ,
      Index          m           ,
      Index          nele_jac    ,
      Index*         iRow        ,
      Index*         jCol        ,
      Number*        values
   );

   // Method to return:
   //  1) structure of hessian of the lagrangian (if "values" is NULL)
   //  2) values of hessian of the lagrangian (if "values" is not NULL)
   virtual bool eval_h(
      Index          n           ,
      const Number*  x           ,
      bool           new_x       ,
      Number         obj_factor  ,
      Index          m           ,
      const Number*  lambda      ,
      bool           new_lambda  ,
      Index          nele_hess   ,
      Index*         iRow        ,
      Index*         jCol        ,
      Number*        values
   );

   // called when the algorithm is completed so the TNLP can
   // store/write the solution
   virtual void finalize_solution(
      Ipopt::SolverReturn       status      ,
      Index                      n          ,
      const Number*              x          ,
      const Number*              z_L        ,
      const Number*              z_U        ,
      Index                      m          ,
      const Number*              g          ,
      const Number*              lambda     ,
      Number                     obj_value  ,
      const Ipopt::IpoptData*           ip_data    ,
      Ipopt::IpoptCalculatedQuantities* ip_cq
   );

   virtual bool intermediate_callback(
      Ipopt::AlgorithmMode              mode,
      Index                             iter,
      Number                            obj_value,
      Number                            inf_pr,
      Number                            inf_du,
      Number                            mu,
      Number                            d_norm,
      Number                            regularization_size,
      Number                            alpha_du,
      Number                            alpha_pr,
      Index                             ls_trials,
      const Ipopt::IpoptData*           ip_data,
      Ipopt::IpoptCalculatedQuantities* ip_cq
   );

};


// ---------------------------------------------------------------------------
} // end namespace cppad_ipopt
// ---------------------------------------------------------------------------

# endif
