# ifndef CppADCompareIncluded
# define CppADCompareIncluded

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

template <class Base>

// -------------- RecordCompare(cop, result, left, right) --------------------
void ADTape<Base>::RecordCompare(
	enum CompareOp  cop   ,
	bool           result ,
	const AD<Base> &left  ,
	const AD<Base> &right )
{	size_t ind0, ind1, ind2, ind3;

	// ind[1] = base 2 representation of [result, Var(left), Var(right])
	ind1 = 0;

	// ind[2] = left address
	if( Parameter(left) )
		ind2 = Rec.PutPar(left.value);
	else
	{	ind1 += 2;
		ind2 =  left.taddr;
	}

	// ind[3] = right address
	if( Parameter(right) )
		ind3 = Rec.PutPar(right.value);
	else
	{	ind1 += 4;
		ind3 =  right.taddr;
	}

	// If both left and right are parameters, do not need to record
	if( ind1 == 0 )
		return;

	// ind[1] & 1 = result
	if( result )
		ind1+= 1;

	// ind[0] = cop 
	ind0 = size_t (cop);

	CppADUnknownError( ind1 > 1 );
	CppADUnknownError( NumInd(ComOp) == 4 );

	// put the operator in the tape
	Rec.PutOp(ComOp);
	Rec.PutInd(ind0, ind1, ind2, ind3);
}

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

	if( AD<Base>::Tape()->State() == Empty )
		return result;

	AD<Base>::Tape()->RecordCompare(CompareLt, result, *this, right);

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

	if( AD<Base>::Tape()->State() == Empty )
		return result;

	AD<Base>::Tape()->RecordCompare(CompareLe, result, *this, right);

	return result;
}
# endif

CppADFoldBinaryOperator(bool, <=)


// -------------------------------- > -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator > (const AD<Base> &right) const
{	bool result =  (value > right.value); 
	return result;
}

# else
template <class Base>
inline bool AD<Base>::operator > (const AD<Base> &right) const
{	bool result =  (value > right.value); 

	if( AD<Base>::Tape()->State() == Empty )
		return result;

	AD<Base>::Tape()->RecordCompare(CompareGt, result, *this, right);

	return result;
}
# endif

CppADFoldBinaryOperator(bool, >)

// -------------------------------- >= -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator >= (const AD<Base> &right) const
{ 	bool result =  (value >= right.value); 
	return result;
}

# else
template <class Base>
inline bool AD<Base>::operator >= (const AD<Base> &right) const
{ 	bool result =  (value >= right.value); 

	if( AD<Base>::Tape()->State() == Empty )
		return result;

	AD<Base>::Tape()->RecordCompare(CompareGe, result, *this, right);

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

	if( AD<Base>::Tape()->State() == Empty )
		return result;

	AD<Base>::Tape()->RecordCompare(CompareEq, result, *this, right);

	return result;
}
# endif

CppADFoldBinaryOperator(bool, ==)

// -------------------------------- != -------------------------
# ifdef NDEBUG

template <class Base>
inline bool AD<Base>::operator != (const AD<Base> &right) const
{	bool result =  (value != right.value);
	return result;
}

# else
template <class Base>
inline bool AD<Base>::operator != (const AD<Base> &right) const
{	bool result =  (value != right.value);

	if( AD<Base>::Tape()->State() == Empty )
		return result;

	AD<Base>::Tape()->RecordCompare(CompareNe, result, *this, right);

	return result;
}
# endif

CppADFoldBinaryOperator(bool, !=)

} // END CppAD namespace

# endif
