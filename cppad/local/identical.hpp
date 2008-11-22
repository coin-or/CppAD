# ifndef CPPAD_IDENTICAL_INCLUDED
# define CPPAD_IDENTICAL_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------
$begin IdenticalPar$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index identical, parameter$$
$index parameter, identical$$

$section Check if a Value is Identically a Parameter$$

$head Syntax$$
$syntax%inline bool IdenticalPar(const %Type% &%x%)%$$

$head Description$$
If $italic x$$ is not of type $syntax%AD<%Base%>%$$,
this function returns true. 
Otherwise, it returns true if and only if 
the sequence of values
$syntax%
	%x%, %x%.value_, %x%.value_.value , ...
%$$
are all parameters.

$end 
------------------------------------------------------------------------------
$begin IdenticalZero$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index identical, zero$$
$index zero, identical$$

$section Check if a Value is Identically Zero$$

$head Syntax$$
$syntax%inline bool IdenticalZero(const %Type% &%x%)%$$

$head Description$$
If $italic x$$ is not of type $syntax%AD<%Base%>%$$,
this function returns true if and only if
$italic x$$ is equal to zero.
Otherwise, it returns true if and only if it is equal to zero
and the sequence of values
$syntax%
	%x%, %x%.value_, %x%.value_.value , ...
%$$
are all parameters.

$end 
------------------------------------------------------------------------------


$begin IdenticalOne$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index identical, one$$
$index one, identical$$

$section Check if a Value is Identically One$$

$head Syntax$$
$syntax%inline bool IdenticalOne(const %Type% &%x%)%$$

$head Description$$
If $italic x$$ is not of type $syntax%AD<%Base%>%$$,
this function returns true if and only if
$italic x$$ is equal to zero.
Otherwise, it returns true if and only if it is equal to zero
and the sequence of values
$syntax%
	%x%, %x%.value_, %x%.value_.value , ...
%$$
are all parameters.

$end 
------------------------------------------------------------------------------
$begin IdenticalEqualPar$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index identical, equal$$
$index equal, identical$$

$section Check if a Value is Identically Equal Another$$

$head Syntax$$
$syntax%inline bool IdenticalEqualPar(const %Type% &%x%, const %Type%)%$$

$head Description$$
If $italic Type$$ is $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is identically equal $italic y$$ if and only if
both $italic x$$ and $italic y$$ are parameters
and $syntax%%x%.value_%$$ is identically equal $syntax%%y%.value_%$$.
If $italic Type$$ is not $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is identically equal $italic y$$ if and only if
$syntax%
	%x% == %y%
%$$

$end 
------------------------------------------------------------------------------
*/


namespace CppAD {
	// Parameter -------------------------------------------
	inline bool IdenticalPar(const float &x)
	{	return true; }
	inline bool IdenticalPar(const double &x)
	{	return true; }
	template <class Base>
	CPPAD_INLINE bool IdenticalPar(const AD<Base> &x)
	{	return Parameter(x) && IdenticalPar(x.value_); }

	// Zero -----------------------------------------------
	inline bool IdenticalZero(const float &x)
	{	return (x == 0.); }
	inline bool IdenticalZero(const double &x)
	{	return (x == 0.); }
	template <class Base>
	CPPAD_INLINE bool IdenticalZero(const AD<Base> &x)
	{	return Parameter(x) && IdenticalZero(x.value_); }

	// One -----------------------------------------------
	inline bool IdenticalOne(const float &x)
	{	return (x == 1.); }
	inline bool IdenticalOne(const double &x)
	{	return (x == 1.); }
	template <class Base>
	CPPAD_INLINE bool IdenticalOne(const AD<Base> &x)
	{	return Parameter(x) && IdenticalOne(x.value_); }

	// Equal -----------------------------------------------
	inline bool IdenticalEqualPar(const float &x, const float &y)
	{	return (x == y); }
	inline bool IdenticalEqualPar(const double &x, const double &y)
	{	return (x == y); }
	template <class Base>
	CPPAD_INLINE bool IdenticalEqualPar
	(const AD<Base> &x, const AD<Base> &y)
	{	bool parameter;
		parameter = ( Parameter(x) & Parameter(y) );
		return parameter  && IdenticalEqualPar(x.value_, y.value_); 
	}
}

# endif
