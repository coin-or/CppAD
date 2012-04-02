/* $Id$ */
# ifndef CPPAD_AD_COPY_INCLUDED
# define CPPAD_AD_COPY_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------

$begin ad_copy$$
$spell
	Vec
	const
$$

$index AD, copy$$
$index AD, assignment$$

$index constructor, AD$$
$index assignment, AD$$

$index convert, to AD$$
$index Base, convert to AD$$
$index double, convert to AD$$
$index VecAD, convert to AD$$

$section AD Copy Constructor and Assignment Operator$$

$head Syntax$$

$subhead Constructor$$
$syntax%AD<%Base%> %y%(%x%)
%$$
$syntax%AD<%Base%> %y% = %x%$$

$subhead Assignment$$
$syntax%%y% = %x%$$

$head Purpose$$
The constructor creates a new $syntax%AD<%Base%>%$$ object $italic y$$ 
and the assignment operator uses an existing $italic y$$.
In either case, 
$italic y$$ has the same value as $italic x$$,
and the same dependence on the 
$cref/independent variables/glossary/Tape/Independent Variable/$$
($italic y$$ is a 
$xref/glossary/Variable/variable/$$ if and only if $italic x$$ is a variable).

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %Type% &%x%
%$$ 
where $italic Type$$ is
$syntax%VecAD<%Base%>::reference%$$,
$syntax%AD<%Base%>%$$,
$italic Base$$, or
$code double$$.

$head y$$
The target $italic y$$ has prototype
$syntax%
	AD<%Base%> &%y%
%$$

$head Example$$
$children%
	example/copy_ad.cpp%
	example/copy_base.cpp%
	example/eq.cpp
	
%$$
The following files contain examples and tests of these operations.
Each test returns true if it succeeds and false otherwise.
$table
$rref CopyAD.cpp$$
$rref CopyBase.cpp$$
$rref Eq.cpp$$
$tend

$end
------------------------------------------------------------------------------
*/

CPPAD_BEGIN_NAMESPACE

/*!
\file ad_copy.hpp
AD<Base> constructors and and copy operations.

\defgroup ad_copy_hpp ad_copy.hpp
*/
/* \{ */

/*!
\page AD_Base_Default
Use default copy constructor and assignment operator
because they may be optimized better than the code below:
\code
template <class Base>
inline AD<Base>::AD(const AD &x) 
{
	value_   = x.value_;
	tape_id_ = x.tape_id_;
	taddr_    = x.taddr_;

	return;
}
template <class Base>
inline AD<Base>& AD<Base>::operator=(const AD<Base> &right)
{	value_    = right.value_;
	tape_id_  = right.tape_id_;
	taddr_    = right.taddr_;

	return *this;
}
\endcode
*/

/*!
Constructor from Base type.

\tparam Base
Base type for this AD object.

\param b
is the Base type value corresponding to this AD object.
The tape identifier will be an invalid tape identifier,
so this object is initially a parameter.
*/
template <class Base>
inline AD<Base>::AD(const Base &b) 
: value_(b)
, tape_id_(0)
, taddr_(0)
{	// check that this is a parameter
	CPPAD_ASSERT_UNKNOWN( Parameter(*this) );
}	

/*!
Assignment to Base type value.

\tparam Base
Base type for this AD object.

\param b
is the Base type value being assignment to this AD object.
The tape identifier will be an invalid tape identifier,
so this object is initially a parameter.
*/
template <class Base>
inline AD<Base>& AD<Base>::operator=(const Base &b)
{	value_   = b;
	tape_id_ = 0;

	// check that this is a parameter
	CPPAD_ASSERT_UNKNOWN( Parameter(*this) );

	return *this;
}

/*!
Constructor from an ADVec<Base> element drops the vector information.

\tparam Base
Base type for this AD object.
*/
template <class Base>
inline AD<Base>::AD(const VecAD_reference<Base> &x)
{	*this = x.ADBase(); }

/*!
Assignment to an ADVec<Base> element drops the vector information.

\tparam Base
Base type for this AD object.
*/
template <class Base>
inline AD<Base>& AD<Base>::operator=(const VecAD_reference<Base> &x)
{	return *this = x.ADBase(); }

/*!
Constructor from any other type acts link constructor from Base type. 

\tparam Base
Base type for this AD object.

\tparam T
is the the type that is being converted to AD<Base>.
There must be a constructor for Base from Type.

\param t
is the object that is being converted from T to AD<Base>.
*/
template <class Base>
template <class T>
inline AD<Base>::AD(const T &t) 
: value_(Base(t))
, tape_id_(0)
, taddr_(0)
{ }


/*!
Assignment from any other type acts link assignment from Base type. 

\tparam Base
Base type for this AD object.

\tparam T
is the the type that is being assigned to AD<Base>.
There must be an assignment for Base from Type.

\param t
is the object that is being assigned to an AD<Base> object.
*/
template <class Base>
template <class T>
inline AD<Base>& AD<Base>::operator=(const T &t)
{	return *this = Base(t); }


/* \} */
CPPAD_END_NAMESPACE
# endif
