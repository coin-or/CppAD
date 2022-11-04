# ifndef CPPAD_CORE_AD_CTOR_HPP
# define CPPAD_CORE_AD_CTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------

$begin ad_ctor$$
$spell
   cppad
   ctor
   initializes
   Vec
   const
$$


$section AD Constructors $$

$head Syntax$$
$codei%AD<%Base%> %ay%()
%$$
$codei%AD<%Base%> %ay%(%x%)
%$$

$head Purpose$$
creates a new $codei%AD<%Base%>%$$ object $icode ay$$
and initializes it as a
equal to $icode x$$.

$head x$$

$subhead implicit$$
There is an implicit constructor where $icode x$$ has prototype
$codei%
   const VecAD<%Base%>& %x%
%$$
There also is an implicit constructor where $icode x$$ has prototype
$codei%
   const %Base%& %x%
%$$
In this case, $icode ay$$ is a
$cref/constant parameter/glossary/Parameter/Constant/$$

$subhead explicit$$
There is an explicit constructor where $icode x$$ has prototype
$codei%
   const %Type%& %x%
%$$
for any type that has an explicit constructor of the form
$icode%Base%(%x%)%$$.
In this case, $icode ay$$ is a
$cref/constant parameter/glossary/Parameter/Constant/$$

$head ay$$
The target $icode ay$$ has prototype
$codei%
   AD<%Base%> %ay%
%$$

$head Example$$
$children%
   example/general/ad_ctor.cpp
%$$
The files $cref ad_ctor.cpp$$ contain examples and tests of these operations.
It test returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
\file ad_ctor.hpp
AD<Base> constructors and and copy operations.
*/

/*!
\page AD_default_ctor
Use default copy constructor
because they may be optimized better than the code below:
\code
template <class Base>
AD<Base>::AD(const AD &x)
{
   value_    = x.value_;
   tape_id_  = x.tape_id_;
   taddr_    = x.taddr_;
   ad_type_  = x.ad_type_;

   return;
}
\endcode
*/

/*!
Default Constructor.

\tparam Base
Base type for this AD object.
*/
template <class Base>
AD<Base>::AD(void)
: value_()
, tape_id_(0)
, taddr_(0)
, ad_type_(constant_enum)
{ }

// --------------------------------------------------------------------------
# ifdef CPPAD_FOR_TMB
/*!
Constructor from double.

\param d
is value corresponding to this AD object.
The tape identifier will be an invalid tape identifier,
so this object is initially a parameter.

\par CPPAD_FOR_TMB
This constructor is defined when CPPAD_FOR_TMB is defined.
*/
template <class Base>
AD<Base>::AD(const double &d)
: value_( Base(d) )
, tape_id_(0)
, taddr_(0)
, ad_type_(constant_enum)
{  // check that this is a parameter
   CPPAD_ASSERT_UNKNOWN( Parameter(*this) );
}
// --------------------------------------------------------------------------
# else
// --------------------------------------------------------------------------
/*!
Constructor from Base type.

\tparam Base
Base type for this AD object.

\param b
is the Base type value corresponding to this AD object.
The tape identifier will be an invalid tape identifier,
so this object is initially a parameter.

\par CPPAD_FOR_TMB
This constructor is defined when CPPAD_FOR_TMB is not defined.
*/
template <class Base>
AD<Base>::AD(const Base &b)
: value_(b)
, tape_id_(0)
, taddr_(0)
, ad_type_(constant_enum)
{  // check that this is a parameter
   CPPAD_ASSERT_UNKNOWN( Parameter(*this) );
}
# endif
// --------------------------------------------------------------------------

/*!
Constructor from an ADVec<Base> element drops the vector information.

\tparam Base
Base type for this AD object.
*/
template <class Base>
AD<Base>::AD(const VecAD_reference<Base> &x)
{  *this = x.ADBase(); }

/*!
Constructor from any other type, converts to Base type, and uses constructor
from Base type.

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
AD<Base>::AD(const T &t)
: value_( Base(t) )
, tape_id_(0)
, taddr_(0)
, ad_type_(constant_enum)
{ }

} // END_CPPAD_NAMESPACE
# endif
