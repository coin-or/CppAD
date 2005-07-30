# ifndef CppADCompIncluded
# define CppADCompIncluded

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
-------------------------------------------------------------------------------
$begin Compare$$
$spell
	Op
	bool
	const
	inline
$$

$mindex comparison operator$$
$index <$$
$index <=$$
$index >$$
$index >=$$
$index ==$$
$index !=$$

$section The AD Comparison Operators$$


$table
$bold Syntax$$ 
$cnext 
$syntax%%left% %Op% %right%$$
$tend

$fend 20$$

$head Description$$
Suppose that either $italic left$$ or $italic right$$ is an
$syntax%AD<%Base%>%$$ object, 
and the other operand is an $code int$$, $italic Base$$
or $syntax%AD<%Base%>%$$ object.
In this case
$syntax%
	%left% %Op% %right%
%$$
returns a $code bool$$ value (true or false)
where $italic Op$$ is any of the following operators:
$table
$italic Op$$ $cnext $bold True Case$$ 
$rnext
$code <$$    $cnext $italic left$$ is less than $italic right$$ 
$rnext
$code <=$$   $cnext $italic left$$ is less than or equal $italic right$$ 
$rnext
$code >$$    $cnext $italic left$$ is greater than $italic right$$ 
$rnext
$code >=$$   $cnext $italic left$$ is greater than or equal $italic right$$ 
$rnext
$code ==$$   $cnext $italic left$$ is equal to $italic right$$ 
$rnext
$code !=$$   $cnext $italic left$$ is not equal to $italic right$$ 
$tend

$head Definition$$
An operator $italic Op$$ is a $italic Base$$ comparison operator
if for each pair of $italic Base$$ objects $italic x$$ and $italic y$$,
$syntax%
	%x% %Op% %y%
%$$ is defined and returns a $code bool$$ value.
For example,
$code <$$ is a $code double$$ comparison operator
but $code +$$ is not a $code double$$ comparison operator
(see $xref/BinaryOp//Base type binary operator/$$).

$head Assumptions$$
If one of the $italic Op$$ operators listed above
is used with an $syntax%AD<%Base%>%$$ object,
it is assumed that the same operator is supported by the base type 
$italic Base$$.

$head Example$$
$children%
	Example/Compare.cpp
%$$
The file
$xref/Compare.cpp/$$
contains an example and a test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// -------------------------------- < -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator < (const AD<Base> &right) const
{	bool result =  (value < right.value); 
	return result;
}

# else
template <class Base>
inline bool AD<Base>::operator < (const AD<Base> &right) const
{	bool result =  (value < right.value); 

	bool vleft, vright;
	if( Tape()->State() != Recording )
		return result;

	vleft     =  Variable(*this);
	vright    =  Variable(right);
	size_t id = result * 4 + vleft * 2 + vright;
	switch( id )
	{
		case 0:
		Tape()->RecordOp(LtfppOp, value, right.value);
		break;
		
		case 1:
		Tape()->RecordOp(LtfpvOp, value, right.taddr);
		break;

		case 2:
		Tape()->RecordOp(LtfvpOp, taddr, right.value);
		break;

		case 3:
		Tape()->RecordOp(LtfvvOp, taddr, right.taddr);
		break;

		case 4:
		Tape()->RecordOp(LttppOp, value, right.value);
		break;
		
		case 5:
		Tape()->RecordOp(LttpvOp, value, right.taddr);
		break;

		case 6:
		Tape()->RecordOp(LttvpOp, taddr, right.value);
		break;

		case 7:
		Tape()->RecordOp(LttvvOp, taddr, right.taddr);
		break;
	}
	return result;
}
# endif

CppADFoldBinaryOperator(bool, <)

// -------------------------------- <= -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator <= (const AD<Base> &right) const
{ 	bool result =  (value <= right.value); 
	return result;
}

# else
template <class Base>
inline bool AD<Base>::operator <= (const AD<Base> &right) const
{ 	bool result =  (value <= right.value); 

	bool  vleft, vright;
	if( Tape()->State() != Recording )
		return result;

	vleft     =  Variable(*this);
	vright    =  Variable(right);
	size_t id = result * 4 + vleft * 2 + vright;
	switch( id )
	{
		case 0:
		Tape()->RecordOp(LefppOp, value, right.value);
		break;
		
		case 1:
		Tape()->RecordOp(LefpvOp, value, right.taddr);
		break;

		case 2:
		Tape()->RecordOp(LefvpOp, taddr, right.value);
		break;

		case 3:
		Tape()->RecordOp(LefvvOp, taddr, right.taddr);
		break;

		case 4:
		Tape()->RecordOp(LetppOp, value, right.value);
		break;
		
		case 5:
		Tape()->RecordOp(LetpvOp, value, right.taddr);
		break;

		case 6:
		Tape()->RecordOp(LetvpOp, taddr, right.value);
		break;

		case 7:
		Tape()->RecordOp(LetvvOp, taddr, right.taddr);
		break;
	}
	return result;
}
# endif

CppADFoldBinaryOperator(bool, <=)

// -------------------------------- > -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator > (const AD<Base> &left) const
{	bool result =  (left.value < value);      // reverse order and use < 
	return result;
}

# else 
template <class Base>
inline bool AD<Base>::operator > (const AD<Base> &left) const
{	bool result =  (left.value < value);      // reverse order and use < 

	bool  vleft, vright;
	if( Tape()->State() != Recording )
		return result;

	vleft     =  Variable(left);
	vright    =  Variable(*this);
	size_t id = result * 4 + vleft * 2 + vright;
	switch( id )
	{
		case 0:
		Tape()->RecordOp(LtfppOp, left.value, value);
		break;
		
		case 1:
		Tape()->RecordOp(LtfpvOp, left.value, taddr);
		break;

		case 2:
		Tape()->RecordOp(LtfvpOp, left.taddr, value);
		break;

		case 3:
		Tape()->RecordOp(LtfvvOp, left.taddr, taddr);
		break;

		case 4:
		Tape()->RecordOp(LttppOp, left.value, value);
		break;
		
		case 5:
		Tape()->RecordOp(LttpvOp, left.value, taddr);
		break;

		case 6:
		Tape()->RecordOp(LttvpOp, left.taddr, value);
		break;

		case 7:
		Tape()->RecordOp(LttvvOp, left.taddr, taddr);
		break;
	}
	return result;
}
# endif

CppADFoldBinaryOperator(bool, >)


// -------------------------------- >= -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator >= (const AD<Base> &left) const
{	bool result =  (left.value <= value);      // reverse order and use <= 
	return result;
}

# else
template <class Base>
inline bool AD<Base>::operator >= (const AD<Base> &left) const
{	bool result =  (left.value <= value);      // reverse order and use <= 

	bool vleft, vright;
	if( Tape()->State() != Recording )
		return result;

	vleft     =  Variable(left);
	vright    =  Variable(*this);
	size_t id = result * 4 + vleft * 2 + vright;
	switch( id )
	{
		case 0:
		Tape()->RecordOp(LefppOp, left.value, value);
		break;
		
		case 1:
		Tape()->RecordOp(LefpvOp, left.value, taddr);
		break;

		case 2:
		Tape()->RecordOp(LefvpOp, left.taddr, value);
		break;

		case 3:
		Tape()->RecordOp(LefvvOp, left.taddr, taddr);
		break;

		case 4:
		Tape()->RecordOp(LetppOp, left.value, value);
		break;
		
		case 5:
		Tape()->RecordOp(LetpvOp, left.value, taddr);
		break;

		case 6:
		Tape()->RecordOp(LetvpOp, left.taddr, value);
		break;

		case 7:
		Tape()->RecordOp(LetvvOp, left.taddr, taddr);
		break;
	}
	return result;
}
# endif

CppADFoldBinaryOperator(bool, >=)

// -------------------------------- == -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator == (const AD<Base> &right) const
{	bool result =  (value == right.value); 
	return result;
}

# else 
template <class Base>
inline bool AD<Base>::operator == (const AD<Base> &right) const
{	bool result =  (value == right.value); 

	bool vleft, vright;
	if( Tape()->State() != Recording )
		return result;

	vleft     =  Variable(*this);
	vright    =  Variable(right);
	size_t id = result * 4 + vleft * 2 + vright;
	switch( id )
	{
		case 0:
		Tape()->RecordOp(EqfppOp, value, right.value);
		break;
		
		case 1:
		Tape()->RecordOp(EqfpvOp, value, right.taddr);
		break;

		case 2:
		Tape()->RecordOp(EqfvpOp, taddr, right.value);
		break;

		case 3:
		Tape()->RecordOp(EqfvvOp, taddr, right.taddr);
		break;

		case 4:
		Tape()->RecordOp(EqtppOp, value, right.value);
		break;
		
		case 5:
		Tape()->RecordOp(EqtpvOp, value, right.taddr);
		break;

		case 6:
		Tape()->RecordOp(EqtvpOp, taddr, right.value);
		break;

		case 7:
		Tape()->RecordOp(EqtvvOp, taddr, right.taddr);
		break;
	}
	return result;
}
# endif

CppADFoldBinaryOperator(bool, ==)

// -------------------------------- != -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator != (const AD<Base> &right) const
{	bool result =  (value == right.value);   // convert to == check 
	return ! result;
}

# else
template <class Base>
inline bool AD<Base>::operator != (const AD<Base> &right) const
{	bool result =  (value == right.value);   // convert to == check 

	bool vleft, vright;
	if( Tape()->State() != Recording )
		return ! result;

	vleft     =  Variable(*this);
	vright    =  Variable(right);
	size_t id = result * 4 + vleft * 2 + vright;
	switch( id )
	{
		case 0:
		Tape()->RecordOp(EqfppOp, value, right.value);
		break;
		
		case 1:
		Tape()->RecordOp(EqfpvOp, value, right.taddr);
		break;

		case 2:
		Tape()->RecordOp(EqfvpOp, taddr, right.value);
		break;

		case 3:
		Tape()->RecordOp(EqfvvOp, taddr, right.taddr);
		break;

		case 4:
		Tape()->RecordOp(EqtppOp, value, right.value);
		break;
		
		case 5:
		Tape()->RecordOp(EqtpvOp, value, right.taddr);
		break;

		case 6:
		Tape()->RecordOp(EqtvpOp, taddr, right.value);
		break;

		case 7:
		Tape()->RecordOp(EqtvvOp, taddr, right.taddr);
		break;
	}
	return ! result;
}
# endif

CppADFoldBinaryOperator(bool, !=)

} // END CppAD namespace

# endif
