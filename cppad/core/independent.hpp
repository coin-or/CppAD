# ifndef CPPAD_CORE_INDEPENDENT_HPP
# define CPPAD_CORE_INDEPENDENT_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
---------------------------------------------------------------------------

$begin Independent$$
$spell
	op
	alloc
	num
	Cpp
	bool
	const
	var
	typename
$$

$section Declare Independent Variables and Start Recording$$

$head Syntax$$
$codei%Independent(%x%)
%$$
$codei%Independent(%x%, %abort_op_index%)
%$$
$codei%Independent(%x%, %abort_op_index%, %record_compare%)
%$$
$codei%Independent(%x%, %abort_op_index%, %record_compare%, %dynamic%)
%$$

$head Start Recording$$
The syntax above starts recording
$cref/AD of Base/glossary/AD of Base/$$ operations
with $icode x$$ as the independent variable vector.
Once the
$cref/operation sequence/glossary/Operation/Sequence/$$ is completed,
it must be transferred to a function object or aborted; see below.

$head Stop Recording$$
The recording is stopped,
and the operation sequence is transferred to the AD function object $icode f$$,
using either the $cref/function constructor/FunConstruct/$$
$codei%
	ADFun<%Base%> %f%(%x%, %y%)
%$$
or the $cref/dependent variable specifier/Dependent/$$
$codei%
	%f%.Dependent(%x%, %y%)
%$$
The only other way to stop a recording is using
$cref abort_recording$$.
Between when the recording is started and when it stopped,
we refer to the elements of $icode x$$,
and the values that depend on the elements of $icode x$$,
as $codei%AD<%Base%>%$$ variables.

$head x$$
The vector $icode x$$ has prototype
$codei%
	%VectorAD% &%x%
%$$
(see $icode VectorAD$$ below).
The size of the vector $icode x$$, must be greater than zero,
and is the number of independent variables for this
AD operation sequence.

$head abort_op_index$$
If this argument is present,
it specifies the operator index at which the execution is be aborted
by calling the CppAD $cref/error handler/ErrorHandler/$$.
When this error handler leads to an assert, the user
can inspect the call stack to see the source code corresponding to
this operator index; see
$cref/purpose/compare_change/op_index/Purpose/$$.
No abort will occur if $icode abort_op_index$$ is zero,
if $cref/NDEBUG/Faq/Speed/NDEBUG/$$ is defined.
If this argument is not present, the default value zero is used
for $icode abort_index$$.

$head record_compare$$
This argument has prototype
$codei%
	bool %record_compare%
%$$
If it is present,
it specifies if AD $cref compare$$  operations are recorded.
It takes extra time and memory to record these operations.
On the other hand, they can be useful for detecting when and why
a functions recording would change; see $icode abort_op_index$$ above and
$cref compare_change$$.
If this argument is not present, the default value $code true$$ is used
for $icode record_compare$$.
If this argument is false, $icode abort_op_index$$ must be zero.

$head dynamic$$
If this argument is present, it has prototype
$codei%
	const %VectorAD%& %dynamic%
%$$
(see $icode Vector$$ below).
It specifies the
$cref/dynamic parameter/glossary/Dynamic Parameter/$$ vector.
The value of these parameters,
in the $cref ADFun$$ object $icode f$$,
that can be changed using $cref new_dynamic$$.

$head VectorAD$$
The type $icode VectorAD$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$codei%AD<%Base%>%$$.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$head Parallel Mode$$
Each thread can have one, and only one, active recording.
A call to $code Independent$$ starts the recording for the current thread.
The recording must be stopped by a corresponding call to
$codei%
	ADFun<%Base%> %f%( %x%, %y%)
%$$
or
$codei%
	%f%.Dependent( %x%, %y%)
%$$
or $cref abort_recording$$
preformed by the same thread; i.e.,
$cref/thread_alloc::thread_num/ta_thread_num/$$ must be the same.

$head Example$$
$children%
	example/general/independent.cpp
%$$
The file
$cref independent.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/
# include <cppad/local/independent.hpp>
/*!
\file core/independent.hpp
Declare the independent variables
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Declaration of independent variables.

\tparam VectorAD
This is simple vector type with elements of type AD<Base>.

\param x
Vector of the independent variablers.

\param abort_op_index
operator index at which execution will be aborted (during  the recording
of operations). The value zero corresponds to not aborting (will not match).

\param record_compare
should comparison operators be recorded.

\param dynamic
is the dynamic parameter vector.
*/
template <typename VectorAD>
inline void Independent(
	VectorAD&  x              ,
	size_t     abort_op_index ,
	bool       record_compare ,
	VectorAD&  dynamic        )
{	CPPAD_ASSERT_KNOWN(
		abort_op_index == 0 || record_compare,
		"Independent: abort_op_index is non-zero and record_compare is false."
	);
	typedef typename VectorAD::value_type ADBase;
	typedef typename ADBase::value_type   Base;
	CPPAD_ASSERT_KNOWN(
		ADBase::tape_ptr() == CPPAD_NULL,
		"Independent: cannot create a new tape because\n"
		"a previous tape is still active (for this thread).\n"
		"AD<Base>::abort_recording() would abort this previous recording."
	);
	local::ADTape<Base>* tape = ADBase::tape_manage(tape_manage_new);
	tape->Independent(x, abort_op_index, record_compare, dynamic);
}
// ---------------------------------------------------------------------------
/*!
Declare independent variables using default for dynamic.

\tparam VectorAD
This is simple vector type with elements of type AD<Base>.

\param x
Vector of the independent variablers.

\param abort_op_index
operator index at which execution will be aborted (during  the recording
of operations). The value zero corresponds to not aborting (will not match).

\param record_compare
should comparison operators be recorded.
*/
template <typename VectorAD>
inline void Independent(VectorAD &x, size_t abort_op_index, bool record_compare)
{	VectorAD dynamic(0); // empty vector
	Independent(x, abort_op_index, record_compare, dynamic);
}
// ---------------------------------------------------------------------------
/*!
Declare independent variables using default for record_compare and dynamic.

\tparam VectorAD
This is simple vector type with elements of type AD<Base>.

\param x
Vector of the independent variablers.

\param abort_op_index
operator index at which execution will be aborted (during  the recording
of operations). The value zero corresponds to not aborting (will not match).
*/
template <typename VectorAD>
inline void Independent(VectorAD &x, size_t abort_op_index)
{	bool     record_compare = true;
	VectorAD dynamic(0); // empty vector
	Independent(x, abort_op_index, record_compare, dynamic);
}
// ---------------------------------------------------------------------------
/*!
Declare independent variables using default for
record_compare, abort_op_index, and dynamic.

\tparam VectorAD
This is simple vector type with elements of type AD<Base>.

\param x
Vector of the independent variablers.
*/
template <typename VectorAD>
inline void Independent(VectorAD &x)
{	size_t   abort_op_index = 0;
	bool     record_compare = true;
	VectorAD dynamic(0); // empty vector
	Independent(x, abort_op_index, record_compare, dynamic);
}

} // END_CPPAD_NAMESPACE
# endif
