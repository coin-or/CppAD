# ifndef CPPAD_CORE_ATOMIC_ONE_ATOMIC_HPP
# define CPPAD_CORE_ATOMIC_ONE_ATOMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_one app}
{xrst_spell
   px
   py
   tvector
   tx
   ty
   vx
   vy
}

Defining Atomic Functions: First Generation
###########################################

Deprecated 2013-05-27
*********************
Using ``CPPAD_USER_ATOMIC`` has been deprecated.
Use :ref:`atomic_three-name` instead.

Syntax Function
***************

| ``CPPAD_USER_ATOMIC`` ( *afun* , *Tvector* , *Base* ,
| |tab| *forward* , *reverse* , *for_jac_sparse* , *rev_jac_sparse* , *rev_hes_sparse*
| )

Use Function
============

   *afun* ( *id* , *ax* , *ay* )

Callback Routines
=================

| *ok* = *forward* ( *id* , *k* , *n* , *m* , *vx* , *vy* , *tx* , *ty* )
| *ok* = *reverse* ( *id* , *k* , *n* , *m* , *tx* , *ty* , *px* , *py* )
| *ok* = *for_jac_sparse* ( *id* , *n* , *m* , *q* , *r* , *s* )
| *ok* = *rev_jac_sparse* ( *id* , *n* , *m* , *q* , *r* , *s* )
| *ok* = *rev_hes_sparse* ( *id* , *n* , *m* , *q* , *r* , *s* , *t* , *u* , *v* )

Free Static Memory
==================
``user_atomic`` < *Base* >:: ``clear`` ()

Purpose
*******
In some cases, the user knows how to compute the derivative
of a function

.. math::

   y = f(x) \; {\rm where} \; f : \B{R}^n \rightarrow \B{R}^m

more efficiently than by coding it using ``AD`` < *Base* >
:ref:`atomic_base<glossary@Operation@Atomic>` operations
and letting CppAD do the rest.
In this case, ``CPPAD_USER_ATOMIC`` can be used
add the user code for :math:`f(x)`, and its derivatives,
to the set of ``AD`` < *Base* > atomic operations.

Another possible purpose is to reduce the size of the tape.

Partial Implementation
**********************
The routines
:ref:`atomic_one@forward` ,
:ref:`atomic_one@reverse` ,
:ref:`atomic_one@for_jac_sparse` ,
:ref:`atomic_one@rev_jac_sparse` , and
:ref:`atomic_one@rev_hes_sparse` ,
must be defined by the user.
The *forward* the routine,
for the case *k*  = 0 , must be implemented.
Functions with the correct prototype,
that just return ``false`` ,
can be used for the other cases
(unless they are required by your calculations).
For example, you need not implement
*forward* for the case *k*  == 2 until you require
forward mode calculation of second derivatives.

CPPAD_USER_ATOMIC
*****************
The macro

| ``CPPAD_USER_ATOMIC`` ( *afun* , *Tvector* , *Base* ,
| |tab| *forward* , *reverse* , *for_jac_sparse* , *rev_jac_sparse* , *rev_hes_sparse*
| )

defines the ``AD`` < *Base* > routine *afun* .
This macro can be placed within a namespace
(not the ``CppAD`` namespace)
but must be outside of any routine.

Tvector
=======
The macro argument *Tvector* must be a
:ref:`simple vector template class<SimpleVector-name>` .
It determines the type of vectors used as arguments to the routine
*afun* .

Base
====
The macro argument *Base* specifies the
:ref:`base type<base_require-name>`
corresponding to ``AD`` < *Base>* operation sequences.
Calling the routine *afun* will add the operator defined
by this macro to an ``AD`` < *Base>* operation sequence.

ok
**
For all routines documented below,
the return value *ok* has prototype

   ``bool`` *ok*

If it is ``true`` , the corresponding evaluation succeeded,
otherwise it failed.

id
**
For all routines documented below,
the argument *id* has prototype

   ``size_t`` *id*

Its value in all other calls is the same as in the corresponding
call to *afun* .
It can be used to store and retrieve extra information about
a specific call to *afun* .

k
*
For all routines documented below, the argument *k* has prototype

   ``size_t`` *k*

The value *k* is the order of the Taylor coefficient that
we are evaluating (:ref:`atomic_one@forward` )
or taking the derivative of (:ref:`atomic_one@reverse` ).

n
*
For all routines documented below,
the argument *n* has prototype

   ``size_t`` *n*

It is the size of the vector *ax* in the corresponding call to
*afun* ( *id* , *ax* , *ay* ) ; i.e.,
the dimension of the domain space for :math:`y = f(x)`.

m
*
For all routines documented below, the argument *m* has prototype

   ``size_t`` *m*

It is the size of the vector *ay* in the corresponding call to
*afun* ( *id* , *ax* , *ay* ) ; i.e.,
the dimension of the range space for :math:`y = f(x)`.

tx
**
For all routines documented below,
the argument *tx* has prototype

   ``const CppAD::vector<`` *Base* >& *tx*

and *tx* . ``size`` () >= ( *k* + 1) * *n* .
For :math:`j = 0 , \ldots , n-1` and :math:`\ell = 0 , \ldots , k`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      x_j^\ell & = & tx [ j * ( k + 1 ) + \ell ]
      \\
      X_j (t) & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^k t^k
   \end{eqnarray}

If *tx* . ``size`` () > ( *k* + 1) * *n* ,
the other components of *tx* are not specified and should not be used.
Note that superscripts represent an index for :math:`x_j^\ell`
and an exponent for :math:`t^\ell`.
Also note that the Taylor coefficients for :math:`X(t)` correspond
to the derivatives of :math:`X(t)` at :math:`t = 0` in the following way:

.. math::

   x_j^\ell = \frac{1}{ \ell ! } X_j^{(\ell)} (0)

ty
**
In calls to :ref:`atomic_one@forward` ,
the argument *ty* has prototype

   ``CppAD::vector<`` *Base* >& *ty*

while in calls to :ref:`atomic_one@reverse` it has prototype

   ``const CppAD::vector<`` *Base* >& *ty*

For all calls, *tx* . ``size`` () >= ( *k* + 1) * *m* .
For :math:`i = 0 , \ldots , m-1` and :math:`\ell = 0 , \ldots , k`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      y_i^\ell & = & ty [ i * ( k + 1 ) + \ell ]
      \\
      Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^k t^k + o ( t^k )
   \end{eqnarray}

where :math:`o( t^k ) / t^k \rightarrow 0` as :math:`t \rightarrow 0`.
If *ty* . ``size`` () > ( *k* + 1) * *m* ,
the other components of *ty* are not specified and should not be used.
Note that superscripts represent an index for :math:`y_j^\ell`
and an exponent for :math:`t^\ell`.
Also note that the Taylor coefficients for :math:`Y(t)` correspond
to the derivatives of :math:`Y(t)` at :math:`t = 0` in the following way:

.. math::

   y_j^\ell = \frac{1}{ \ell ! } Y_j^{(\ell)} (0)

forward
=======
In the case of *forward* ,
for :math:`i = 0 , \ldots , m-1`, :math:`ty[ i *( k  + 1) + k ]` is an output
and all the other components of *ty* are inputs.

reverse
=======
In the case of *reverse* ,
all the components of *ty* are inputs.

afun
****
The macro argument *afun* ,
is the name of the AD function corresponding to this atomic
operation (as it is used in the source code).
CppAD uses the other functions,
where the arguments are vectors with elements of type *Base* ,
to implement the function

   *afun* ( *id* , *ax* , *ay* )

where the argument are vectors with elements of type ``AD`` < *Base* > .

ax
==
The *afun* argument *ax* has prototype

   ``const`` *Tvector* < ``AD`` < *Base* > >& *ax*

It is the argument vector :math:`x \in \B{R}^n`
at which the ``AD`` < *Base* > version of
:math:`y = f(x)` is to be evaluated.
The dimension of the domain space for :math:`y = f (x)`
is specified by :ref:`atomic_one@n` = *ax* . ``size`` () ,
which must be greater than zero.

ay
==
The *afun* result *ay* has prototype

   *Tvector* < ``AD`` < *Base* > >& *ay*

The input values of its elements
are not specified (must not matter).
Upon return, it is the ``AD`` < *Base* > version of the
result vector :math:`y = f(x)`.
The dimension of the range space for :math:`y = f (x)`
is specified by :ref:`atomic_one@m` = *ay* . ``size`` () ,
which must be greater than zero.

Parallel Mode
=============
The first call to

   *afun* ( *id* , *ax* , *ay* )

must not be in :ref:`parallel<ta_in_parallel-name>` mode.
In addition, the
:ref:`atomic_one clear<atomic_one@clear>`
routine cannot be called while in parallel mode.

forward
*******
The macro argument *forward* is a
user defined function

   *ok* = *forward* ( *id* , *k* , *n* , *m* , *vx* , *vy* , *tx* , *ty* )

that computes results during a :ref:`Forward-name` mode sweep.
For this call, we are given the Taylor coefficients in *tx*
form order zero through *k* ,
and the Taylor coefficients in  *ty* with order less than *k* .
The *forward* routine computes the
*k* order Taylor coefficients for :math:`y` using the definition
:math:`Y(t) = f[ X(t) ]`.
For example, for :math:`i = 0 , \ldots , m-1`,

.. math::
   :nowrap:

   \begin{eqnarray}
   y_i^0 & = & Y(0)
         = f_i ( x^0 )
   \\
   y_i^1 & = & Y^{(1)} ( 0 )
         = f_i^{(1)} ( x^0 ) X^{(1)} ( 0 )
         = f_i^{(1)} ( x^0 ) x^1
   \\
   y_i^2
   & = & \frac{1}{2 !} Y^{(2)} (0)
   \\
   & = & \frac{1}{2} X^{(1)} (0)^\R{T} f_i^{(2)} ( x^0 ) X^{(1)} ( 0 )
     +   \frac{1}{2} f_i^{(1)} ( x^0 ) X^{(2)} ( 0 )
   \\
   & = & \frac{1}{2} (x^1)^\R{T} f_i^{(2)} ( x^0 ) x^1
     +    f_i^{(1)} ( x^0 ) x^2
   \end{eqnarray}

Then, for :math:`i = 0 , \ldots , m-1`, it sets

.. math::

   ty [ i * (k + 1) + k ] = y_i^k

The other components of *ty* must be left unchanged.

Usage
=====
This routine is used,
with *vx* . ``size`` () > 0 and *k*  == 0 ,
by calls to *afun* .
It is used,
with *vx* . ``size`` () = 0 and
*k* equal to the order of the derivative begin computed,
by calls to :ref:`forward<forward_order-name>` .

vx
==
The *forward* argument *vx* has prototype

   ``const CppAD::vector<bool>&`` *vx*

The case *vx* . ``size`` () > 0 occurs
once for each call to *afun* ,
during the call,
and before any of the other callbacks corresponding to that call.
Hence such a call can be used to cache information attached to
the corresponding *id*
(such as the elements of *vx* ).
If *vx* . ``size`` () > 0 then
*k*  == 0 ,
*vx* . ``size`` () >= *n* , and
for :math:`j = 0 , \ldots , n-1`,
*vx* [ *j* ] is true if and only if
*ax* [ *j* ] is a :ref:`glossary@Variable` .

If *vx* . ``size`` () == 0 ,
then *vy* . ``size`` () == 0 and neither of these vectors
should be used.

vy
==
The *forward* argument *vy* has prototype

   ``CppAD::vector<bool>&`` *vy*

If *vy* . ``size`` () == 0 , it should not be used.
Otherwise,
*k*  == 0 and *vy* . ``size`` () >= *m* .
The input values of the elements of *vy*
are not specified (must not matter).
Upon return, for :math:`j = 0 , \ldots , m-1`,
*vy* [ *i* ] is true if and only if
*ay* [ *j* ] is a variable.
(CppAD uses *vy* to reduce the necessary computations.)

reverse
*******
The macro argument *reverse*
is a user defined function

   *ok* = *reverse* ( *id* , *k* , *n* , *m* , *tx* , *ty* , *px* , *py* )

that computes results during a :ref:`Reverse-name` mode sweep.
The input value of the vectors *tx* and *ty*
contain Taylor coefficient, up to order *k* ,
for :math:`X(t)` and :math:`Y(t)` respectively.
We use the :math:`\{ x_j^\ell \}` and :math:`\{ y_i^\ell \}`
to denote these Taylor coefficients where the implicit range indices are
:math:`i = 0 , \ldots , m-1`,
:math:`j = 0 , \ldots , n-1`,
:math:`\ell = 0 , \ldots , k`.
Using the calculations done by :ref:`atomic_one@forward` ,
the Taylor coefficients :math:`\{ y_i^\ell \}` are a function of the Taylor
coefficients for :math:`\{ x_j^\ell \}`; i.e., given :math:`y = f(x)`
we define the function
:math:`F : \B{R}^{n \times (k+1)} \rightarrow \B{R}^{m \times (k+1)}` by

.. math::

   y_i^\ell =  F_i^\ell ( \{ x_j^\ell \} )

We use :math:`G : \B{R}^{m \times (k+1)} \rightarrow \B{R}`
to denote an arbitrary scalar valued function of the Taylor coefficients for
:math:`Y(t)` and write  :math:`z = G( \{ y_i^\ell \} )`.
The ``reverse`` routine
is given the derivative of :math:`z` with respect to
:math:`\{ y_i^\ell \}` and computes its derivative with respect
to :math:`\{ x_j^\ell \}`.

Usage
=====
This routine is used,
with *k*  + 1 equal to the order of the derivative being calculated,
by calls to :ref:`reverse<reverse_any-name>` .

py
==
The *reverse* argument *py* has prototype

   ``const CppAD::vector<`` *Base* >& *py*

and *py* . ``size`` () >= ( *k* + 1) * *m* .
For :math:`i = 0 , \ldots , m-1` and :math:`\ell = 0 , \ldots , k`,

.. math::

   py[ i * (k + 1 ) + \ell ] = \partial G / \partial y_i^\ell

If *py* . ``size`` () > ( *k* + 1) * *m* ,
the other components of *py* are not specified and should not be used.

px
==
We define the function

.. math::

   H ( \{ x_j^\ell \} ) = G[ F( \{ x_j^\ell \} ) ]

The *reverse* argument *px* has prototype

   ``CppAD::vector<`` *Base* >& *px*

and *px* . ``size`` () >= ( *k* + 1) * *n* .
The input values of the elements of *px*
are not specified (must not matter).
Upon return,
for :math:`j = 0 , \ldots , n-1` and :math:`p = 0 , \ldots , k`,

.. math::
   :nowrap:

   \begin{eqnarray}
   px [ j * (k + 1) + p ] & = & \partial H / \partial x_j^p
   \\
   & = &
   ( \partial G / \partial \{ y_i^\ell \} )
      ( \partial \{ y_i^\ell \} / \partial x_j^p )
   \\
   & = &
   \sum_{i=0}^{m-1} \sum_{\ell=0}^k
   ( \partial G / \partial y_i^\ell ) ( \partial y_i^\ell / \partial x_j^p )
   \\
   & = &
   \sum_{i=0}^{m-1} \sum_{\ell=p}^k
   py[ i * (k + 1 ) + \ell ] ( \partial F_i^\ell / \partial x_j^p )
   \end{eqnarray}

Note that we have used the fact that for :math:`\ell < p`,
:math:`\partial F_i^\ell / \partial x_j^p = 0`.
If *px* . ``size`` () > ( *k* + 1) * *n* ,
the other components of *px* are not specified and should not be used.

for_jac_sparse
**************
The macro argument *for_jac_sparse*
is a user defined function

   *ok* = *for_jac_sparse* ( *id* , *n* , *m* , *q* , *r* , *s* )

that is used to compute results during a forward Jacobian sparsity sweep.
For a fixed :math:`n \times q` matrix :math:`R`,
the Jacobian of :math:`f( x + R * u)` with respect to :math:`u \in \B{R}^q` is

.. math::

   S(x) = f^{(1)} (x) * R

Given a :ref:`glossary@Sparsity Pattern` for :math:`R`,
*for_jac_sparse* computes a sparsity pattern for :math:`S(x)`.

Usage
=====
This routine is used by calls to :ref:`ForSparseJac-name` .

q
=
The *for_jac_sparse* argument *q* has prototype

   ``size_t`` *q*

It specifies the number of columns in
:math:`R \in \B{R}^{n \times q}` and the Jacobian
:math:`S(x) \in \B{R}^{m \times q}`.

r
=
The *for_jac_sparse* argument *r* has prototype

   ``const CppAD::vector< std::set<size_t> >&`` *r*

and *r* . ``size`` () >= *n* .
For :math:`j = 0 , \ldots , n-1`,
all the elements of *r* [ *j* ] are between
zero and *q* ``-1`` inclusive.
This specifies a sparsity pattern for the matrix :math:`R`.

s
=
The *for_jac_sparse* return value *s* has prototype

   ``CppAD::vector< std::set<size_t> >&`` *s*

and *s* . ``size`` () >= *m* .
The input values of its sets
are not specified (must not matter). Upon return
for :math:`i = 0 , \ldots , m-1`,
all the elements of *s* [ *i* ] are between
zero and *q* ``-1`` inclusive.
This represents a sparsity pattern for the matrix :math:`S(x)`.

rev_jac_sparse
**************
The macro argument *rev_jac_sparse*
is a user defined function

   *ok* = *rev_jac_sparse* ( *id* , *n* , *m* , *q* , *r* , *s* )

that is used to compute results during a reverse Jacobian sparsity sweep.
For a fixed :math:`q \times m` matrix :math:`S`,
the Jacobian of :math:`S * f( x )` with respect to :math:`x \in \B{R}^n` is

.. math::

   R(x) = S * f^{(1)} (x)

Given a :ref:`glossary@Sparsity Pattern` for :math:`S`,
*rev_jac_sparse* computes a sparsity pattern for :math:`R(x)`.

Usage
=====
This routine is used by calls to :ref:`RevSparseJac-name`
and to :ref:`optimize-name` .

q
=
The *rev_jac_sparse* argument *q* has prototype

   ``size_t`` *q*

It specifies the number of rows in
:math:`S \in \B{R}^{q \times m}` and the Jacobian
:math:`R(x) \in \B{R}^{q \times n}`.

s
=
The *rev_jac_sparse* argument *s* has prototype

   ``const CppAD::vector< std::set<size_t> >&`` *s*

and *s* . ``size`` () >= *m* .
For :math:`i = 0 , \ldots , m-1`,
all the elements of *s* [ *i* ]
are between zero and *q* ``-1`` inclusive.
This specifies a sparsity pattern for the matrix :math:`S^\R{T}`.

r
=
The *rev_jac_sparse* return value *r* has prototype

   ``CppAD::vector< std::set<size_t> >&`` *r*

and *r* . ``size`` () >= *n* .
The input values of its sets
are not specified (must not matter).
Upon return for :math:`j = 0 , \ldots , n-1`,
all the elements of *r* [ *j* ]
are between zero and *q* ``-1`` inclusive.
This represents a sparsity pattern for the matrix :math:`R(x)^\R{T}`.

rev_hes_sparse
**************
The macro argument *rev_hes_sparse*
is a user defined function

   *ok* = *rev_hes_sparse* ( *id* , *n* , *m* , *q* , *r* , *s* , *t* , *u* , *v* )

There is an unspecified scalar valued function
:math:`g : \B{R}^m \rightarrow \B{R}`.
Given a sparsity pattern for :math:`R`
and information about the function :math:`z = g(y)`,
this routine computes the sparsity pattern for

.. math::

   V(x) = (g \circ f)^{(2)}( x ) R

Usage
=====
This routine is used by calls to :ref:`RevSparseHes-name` .

q
=
The *rev_hes_sparse* argument *q* has prototype

   ``size_t`` *q*

It specifies the number of columns in the sparsity patterns.

r
=
The *rev_hes_sparse* argument *r* has prototype

   ``const CppAD::vector< std::set<size_t> >&`` *r*

and *r* . ``size`` () >= *n* .
For :math:`j = 0 , \ldots , n-1`,
all the elements of *r* [ *j* ] are between
zero and *q* ``-1`` inclusive.
This specifies a sparsity pattern for the matrix :math:`R \in \B{R}^{n \times q}`.

s
=
The *rev_hes_sparse* argument *s* has prototype

   ``const CppAD::vector<bool>&`` *s*

and *s* . ``size`` () >= *m* .
This specifies a sparsity pattern for the matrix
:math:`S(x) = g^{(1)} (y) \in \B{R}^{1 \times m}`.

t
=
The *rev_hes_sparse* argument *t* has prototype

   ``CppAD::vector<bool>&`` *t*

and *t* . ``size`` () >= *n* .
The input values of its elements
are not specified (must not matter).
Upon return it represents a sparsity pattern for the matrix
:math:`T(x) \in \B{R}^{1 \times n}` defined by

.. math::

   T(x)  =  (g \circ f)^{(1)} (x) =  S(x) * f^{(1)} (x)

u
=
The *rev_hes_sparse* argument *u* has prototype

   ``const CppAD::vector< std::set<size_t> >&`` *u*

and *u* . ``size`` () >= *m* .
For :math:`i = 0 , \ldots , m-1`,
all the elements of *u* [ *i* ]
are between zero and *q* ``-1`` inclusive.
This specifies a sparsity pattern
for the matrix :math:`U(x) \in \B{R}^{m \times q}` defined by

.. math::
   :nowrap:

   \begin{eqnarray}
   U(x)
   & = &
   \partial_u \{ \partial_y g[ y + f^{(1)} (x) R u ] \}_{u=0}
   \\
   & = &
   \partial_u \{ g^{(1)} [ y + f^{(1)} (x) R u ] \}_{u=0}
   \\
   & = &
   g^{(2)} (y) f^{(1)} (x) R
   \end{eqnarray}

v
=
The *rev_hes_sparse* argument *v* has prototype

   ``CppAD::vector< std::set<size_t> >&`` *v*

and *v* . ``size`` () >= *n* .
The input values of its elements
are not specified (must not matter).
Upon return, for :math:`j = 0, \ldots , n-1`,
all the elements of *v* [ *j* ]
are between zero and *q* ``-1`` inclusive.
This represents a sparsity pattern for the matrix
:math:`V(x) \in \B{R}^{n \times q}` defined by

.. math::
   :nowrap:

   \begin{eqnarray}
   V(x)
   & = &
   \partial_u [ \partial_x (g \circ f) ( x + R u )  ]_{u=0}
   \\
   & = &
   \partial_u [ (g \circ f)^{(1)}( x + R u )  ]_{u=0}
   \\
   & = &
   (g \circ f)^{(2)}( x ) R
   \\
   & = &
   f^{(1)} (x)^\R{T} g^{(2)} ( y ) f^{(1)} (x)  R
   +
   \sum_{i=1}^m [ g^{(1)} (y) ]_i \; f_i^{(2)} (x) R
   \\
   & = &
   f^{(1)} (x)^\R{T} U(x)
   +
   \sum_{i=1}^m S(x)_i \; f_i^{(2)} (x) R
   \end{eqnarray}

clear
*****
User atomic functions hold onto static work space in order to
increase speed by avoiding system memory allocation calls.
The function call

   ``user_atomic`` < *Base* >:: ``clear`` ()

makes to work space :ref:`available<ta_available-name>` to
for other uses by the same thread.
This should be called when you are done using the
atomic functions for a specific value of *Base* .

Restriction
===========
The atomic function ``clear`` routine cannot be called
while in :ref:`parallel<ta_in_parallel-name>` execution mode.

{xrst_end atomic_one}
------------------------------------------------------------------------------
*/
# include <set>
# include <cppad/core/cppad_assert.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic_one.hpp
user defined atomic operations.
*/

/*!
\def CPPAD_USER_ATOMIC(afun, Tvector,
   forward, reverse, for_jac_sparse, rev_jac_sparse, rev_hes_sparse
)
Defines the function <tt>afun(id, ax, ay)</tt>
where id is ax and ay are vectors with <tt>AD<Base></tt> elements.

\par Tvector
the Simple Vector template class for this function.

\par Base
the base type for the atomic operation.

\par afun
name of the CppAD defined function that corresponding to this operation.
Note that afun, preceeded by a pound sign,
is a version of afun with quotes arround it.

\par forward
name of the user defined function that computes corresponding
results during forward mode.

\par reverse
name of the user defined function that computes corresponding
results during reverse mode.

\par for_jac_sparse
name of the user defined routine that computes corresponding
results during forward mode jacobian sparsity sweeps.

\par rev_jac_sparse
name of the user defined routine that computes corresponding
results during reverse mode jacobian sparsity sweeps.

\par rev_hes_sparse
name of the user defined routine that computes corresponding
results during reverse mode Hessian sparsity sweeps.

\par memory allocation
Note that atomic_one is used as a static object, so its objects
do note get deallocated until the program terminates.
*/
# define CPPAD_USER_ATOMIC(                                           \
     afun            ,                                                \
     Tvector         ,                                                \
     Base            ,                                                \
   forward         ,                                                \
     reverse         ,                                                \
     for_jac_sparse  ,                                                \
     rev_jac_sparse  ,                                                \
     rev_hes_sparse                                                   \
)                                                                     \
inline void afun (                                                    \
     size_t                               id ,                        \
     const Tvector< CppAD::AD<Base> >&    ax ,                        \
     Tvector< CppAD::AD<Base> >&          ay                          \
)                                                                     \
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;                            \
   static CppAD::atomic_one<Base> fun(                              \
          #afun          ,                                            \
          forward        ,                                            \
          reverse        ,                                            \
          for_jac_sparse ,                                            \
          rev_jac_sparse ,                                            \
          rev_hes_sparse                                              \
     );                                                               \
     fun(id, ax, ay);                                                 \
}

/// link so that user_atomic<Base>::clear() still works
template <class Base> class user_atomic : public atomic_base<Base> {
};

/*!
Class that actually implements the <tt>afun(id, ax, ay)</tt> calls.

A new atomic_one object is generated each time the user invokes
the CPPAD_USER_ATOMIC macro; see static object in that macro.
*/
template <class Base>
class atomic_one : public atomic_base<Base> {
public:
   /// disable atomic_one<Base>::clear(void)
   static void clear(void)
   {  CPPAD_ASSERT_KNOWN(
         false,
         "Depreacted API uses user_atomic<Base>::clear()"
      );
   }
   /// type for user routine that computes forward mode results
   typedef bool (*F) (
      size_t                  id ,
      size_t                   k ,
      size_t                   n ,
      size_t                   m ,
      const vector<bool>&     vx ,
      vector<bool>&           vy ,
      const vector<Base>&     tx ,
      vector<Base>&           ty
   );
   /// type for user routine that computes reverse mode results
   typedef bool (*R) (
      size_t                  id ,
      size_t                   k ,
      size_t                   n ,
      size_t                   m ,
      const vector<Base>&     tx ,
      const vector<Base>&     ty ,
      vector<Base>&           px ,
      const vector<Base>&     py
   );
   /// type for user routine that computes forward mode Jacobian sparsity
   typedef bool (*FJS) (
      size_t                           id ,
      size_t                            n ,
      size_t                            m ,
      size_t                            q ,
      const vector< std::set<size_t> >& r ,
      vector< std::set<size_t>  >&      s
   );
   /// type for user routine that computes reverse mode Jacobian sparsity
   typedef bool (*RJS) (
      size_t                           id ,
      size_t                            n ,
      size_t                            m ,
      size_t                            q ,
      vector< std::set<size_t> >&       r ,
      const vector< std::set<size_t> >& s
   );
   /// type for user routine that computes reverse mode Hessian sparsity
   typedef bool (*RHS) (
      size_t                           id ,
      size_t                            n ,
      size_t                            m ,
      size_t                            q ,
      const vector< std::set<size_t> >& r ,
      const vector<bool>&               s ,
      vector<bool>&                     t ,
      const vector< std::set<size_t> >& u ,
      vector< std::set<size_t> >&       v
   );
private:
   /// id value corresponding to next virtual callback
   size_t                  id_;
   /// user's implementation of forward mode
   const F                  f_;
   /// user's implementation of reverse mode
   const R                  r_;
   /// user's implementation of forward jacobian sparsity calculations
   const FJS              fjs_;
   /// user's implementation of reverse jacobian sparsity calculations
   const RJS              rjs_;
   /// user's implementation of reverse Hessian sparsity calculations
   const RHS              rhs_;

public:
   /*!
   Constructor called for each invocation of CPPAD_USER_ATOMIC.

   Put this object in the list of all objects for this class and set
   the constant private data f_, r_, fjs_, rjs_, rhs_.

   \param afun
   is the user's name for the AD version of this atomic operation.

   \param f
   user routine that does forward mode calculations for this operation.

   \param r
   user routine that does reverse mode calculations for this operation.

   \param fjs
   user routine that does forward Jacobian sparsity calculations.

   \param rjs
   user routine that does reverse Jacobian sparsity calculations.

   \param rhs
   user routine that does reverse Hessian sparsity calculations.

   \par
   This constructor can not be used in parallel mode because
   atomic_base has this restriction.
   */
   atomic_one(const char* afun, F f, R r, FJS fjs, RJS rjs, RHS rhs) :
   atomic_base<Base>(afun) // name = afun
   , f_(f)
   , r_(r)
   , fjs_(fjs)
   , rjs_(rjs)
   , rhs_(rhs)
   {  this->option( atomic_base<Base>::set_sparsity_enum );
   }
   /*!
   Implement the user call to <tt>afun(id, ax, ay)</tt>.

   \tparam ADVector
   A simple vector class with elements of type <code>AD<Base></code>.

   \param id
   extra information vector that is just passed through by CppAD,
   and possibly used by user's routines.

   \param ax
   is the argument vector for this call,
   <tt>ax.size()</tt> determines the number of arguments.

   \param ay
   is the result vector for this call,
   <tt>ay.size()</tt> determines the number of results.
   */
   template <class ADVector>
   void operator()(size_t id, const ADVector& ax, ADVector& ay)
   {  // call atomic_base function object
      this->atomic_base<Base>::operator()(ax, ay, id);
      return;
   }
   /*!
   Store id for next virtual function callback

   \param id
   id value corresponding to next virtual callback
   */
   virtual void set_old(size_t id)
   {  id_ = id; }
   /*!
   Link from atomic_one to forward mode

   \copydetails atomic_base::forward
   */
   virtual bool forward(
      size_t                    p ,
      size_t                    q ,
      const vector<bool>&      vx ,
              vector<bool>&      vy ,
      const vector<Base>&      tx ,
              vector<Base>&      ty )
   {  CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
      CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
      size_t n = tx.size() / (q+1);
      size_t m = ty.size() / (q+1);
      size_t i, j, k, ell;

      vector<Base> x(n * (q+1));
      vector<Base> y(m * (q+1));
      vector<bool> empty;

      // atomic_one interface can only handel one order at a time
      // so must just throuh hoops to get multiple orders at one time.
      bool ok = true;
      for(k = p; k <= q; k++)
      {  for(j = 0; j < n; j++)
            for(ell = 0; ell <= k; ell++)
               x[ j * (k+1) + ell ] = tx[ j * (q+1) + ell ];
         for(i = 0; i < m; i++)
            for(ell = 0; ell < k; ell++)
               y[ i * (k+1) + ell ] = ty[ i * (q+1) + ell ];
         if( k == 0 )
            ok &= f_(id_, k, n, m, vx, vy, x, y);
         else
            ok &= f_(id_, k, n, m, empty, empty, x, y);
         for(i = 0; i < m; i++)
            ty[ i * (q+1) + k ] = y[ i * (k+1) + k];
      }
      return ok;
   }
   /*!
   Link from atomic_one to reverse mode

   \copydetails atomic_base::reverse
   */
   virtual bool reverse(
      size_t                   q ,
      const vector<Base>&     tx ,
      const vector<Base>&     ty ,
              vector<Base>&     px ,
      const vector<Base>&     py )
   {  CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
      CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
      size_t n = tx.size() / (q+1);
      size_t m = ty.size() / (q+1);
      bool   ok = r_(id_, q, n, m, tx, ty, px, py);
      return ok;
   }
   /*!
   Link from forward Jacobian sparsity sweep to atomic_one

   \copydetails atomic_base::for_sparse_jac
   */
   virtual bool for_sparse_jac(
      size_t                                q ,
      const vector< std::set<size_t> >&     r ,
              vector< std::set<size_t> >&     s ,
      const vector<Base>&                   x )
   {  size_t n = r.size();
      size_t m = s.size();
      bool ok  = fjs_(id_, n, m, q, r, s);
      return ok;
   }

   /*!
   Link from reverse Jacobian sparsity sweep to atomic_one.

   \copydetails atomic_base::rev_sparse_jac
   */
   virtual bool rev_sparse_jac(
      size_t                               q  ,
      const vector< std::set<size_t> >&    rt ,
              vector< std::set<size_t> >&    st ,
      const vector<Base>&                   x )
   {  size_t n = st.size();
      size_t m = rt.size();
      bool ok  = rjs_(id_, n, m, q, st, rt);
      return ok;
   }
   /*!
   Link from reverse Hessian sparsity sweep to atomic_one

   \copydetails atomic_base::rev_sparse_hes
   */
   virtual bool rev_sparse_hes(
      const vector<bool>&                   vx,
      const vector<bool>&                   s ,
              vector<bool>&                   t ,
      size_t                                q ,
      const vector< std::set<size_t> >&     r ,
      const vector< std::set<size_t> >&     u ,
              vector< std::set<size_t> >&     v ,
      const vector<Base>&                   x )
   {  size_t m = u.size();
      size_t n = v.size();
      CPPAD_ASSERT_UNKNOWN( r.size() == n );
      CPPAD_ASSERT_UNKNOWN( s.size() == m );
      CPPAD_ASSERT_UNKNOWN( t.size() == n );
      //
      // old interface used id instead of vx
      bool ok = rhs_(id_, n, m, q, r, s, t, u, v);
      return ok;
   }
};

} // END_CPPAD_NAMESPACE
# endif
