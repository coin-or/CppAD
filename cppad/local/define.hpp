# ifndef CPPAD_DEFINE_INCLUDED
# define CPPAD_DEFINE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Define$$ $comment CppAD Developer Documentation$$
$spell
	cppad.hpp
	Microsoft
	VecAD_reference
	Cpp
	inline
	namespace
	std
	const
	Op
	OpenMp
$$
$aindex head$$

$mindex macro definition$$
$section Macros Used by CppAD Implementation$$ 

$head CPPAD_NULL$$
The preprocessor symbol $code CPPAD_NULL$$ is used for a null pointer. 
If it is not yet defined,
it is defined when $code cppad/local/define.hpp/$$ is included.

$head CPPAD_MAX_NUM_THREADS$$
The preprocessor symbol $code CPPAD_MAX_NUM_THREADS$$ 
is the maximum number of OpenMp threads that can
be used with CppAD.
If it is not yet defined,
it is defined when $code cppad/local/define.hpp/$$ is included.

$head CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR$$
The syntax
$syntax%
	CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(%Op%)
%$$
assumes that the operator
$syntax%
	%left% %Op% %right%
%$$
is defined for the case where $italic left$$ and $italic right$$ 
and the result of the operation all 
have type $syntax%AD<%Base%>%$$.
It uses this case to define the cases either $italic left$$
or $italic right$$ has type
$syntax%VecAD_reference<%Base%>%$$ or
$syntax%AD<%Base%>%$$
and the type of the other operand is one of the following:
$syntax%VecAD_reference<%Base%>%$$,
$syntax%AD<%Base%>%$$,
$italic Base$$,
$code int$$.
All of the arguments are $code const$$ and call by reference,
except for the $code int$$ case which is call by value.
In all cases, it converts the operands to $syntax%AD<%Base%>%$$ and then
uses the definition of the same operation for that case. 

$head CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR$$
The syntax
$syntax%
	CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(%Op%)
%$$
assumes that the operator
$syntax%
	%left% %Op% %right%
%$$
is defined for the case where $italic left$$ and $italic right$$ 
have type $syntax%AD<%Base%>%$$
and the result has type $code bool$$.
It uses this case to define the cases either $italic left$$
or $italic right$$ has type
$syntax%VecAD_reference<%Base%>%$$ or
$syntax%AD<%Base%>%$$
and the type of the other operand is one of the following:
$syntax%VecAD_reference<%Base%>%$$,
$syntax%AD<%Base%>%$$,
$italic Base$$,
$code int$$.
All of the arguments are $code const$$ and call by reference,
except for the $code int$$ case which is call by value.
In all cases, it converts the operands to $syntax%AD<%Base%>%$$ and then
uses the definition of the same operation for that case. 


$end
*/

# ifndef CPPAD_NULL
# define CPPAD_NULL     0
# endif

# ifndef CPPAD_MAX_NUM_THREADS
# ifdef _OPENMP
# define CPPAD_MAX_NUM_THREADS 32
# else
# define CPPAD_MAX_NUM_THREADS 1
# endif
# endif


# define CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(Op)                      \
/* ----------------------------------------------------------------*/  \
/* Operations with VecAD_reference<Base> and AD<Base> only*/           \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
(const AD<Base> &left, const VecAD_reference<Base> &right)             \
{	return left Op right.ADBase(); }                               \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
(const VecAD_reference<Base> &left, const VecAD_reference<Base> &right)\
{	return left.ADBase() Op right.ADBase(); }                      \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const VecAD_reference<Base> &left, const AD<Base> &right)     \
{	return left.ADBase() Op right; }                               \
/* ----------------------------------------------------------------*/  \
/* Operations Base */                                                  \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const Base &left, const AD<Base> &right)                      \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const Base &left, const VecAD_reference<Base> &right)         \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const AD<Base> &left, const Base &right)                      \
{	return left Op AD<Base>(right); }                              \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const VecAD_reference<Base> &left, const Base &right)         \
{	return left.ADBase() Op AD<Base>(right); }                     \
                                                                       \
/* ----------------------------------------------------------------*/  \
/* Operations int */                                                   \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(int left, const AD<Base> &right)                              \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(int left, const VecAD_reference<Base> &right)                 \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const AD<Base> &left, int right)                              \
{	return left Op AD<Base>(right); }                              \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const VecAD_reference<Base> &left, int right)                 \
{	return left.ADBase() Op AD<Base>(right); }
// =======================================================================


# define CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(Op)                    \
/* ----------------------------------------------------------------*/  \
/* Operations with VecAD_reference<Base> and AD<Base> only*/           \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
(const AD<Base> &left, const VecAD_reference<Base> &right)             \
{	return left Op right.ADBase(); }                               \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
(const VecAD_reference<Base> &left, const VecAD_reference<Base> &right)\
{	return left.ADBase() Op right.ADBase(); }                      \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const VecAD_reference<Base> &left, const AD<Base> &right)     \
{	return left.ADBase() Op right; }                               \
/* ----------------------------------------------------------------*/  \
/* Operations Base */                                                  \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const Base &left, const AD<Base> &right)                      \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const Base &left, const VecAD_reference<Base> &right)         \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const AD<Base> &left, const Base &right)                      \
{	return left Op AD<Base>(right); }                              \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const VecAD_reference<Base> &left, const Base &right)         \
{	return left.ADBase() Op AD<Base>(right); }                     \
                                                                       \
/* ----------------------------------------------------------------*/  \
/* Operations int */                                                   \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(int left, const AD<Base> &right)                              \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(int left, const VecAD_reference<Base> &right)                 \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const AD<Base> &left, int right)                              \
{	return left Op AD<Base>(right); }                              \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const VecAD_reference<Base> &left, int right)                 \
{	return left.ADBase() Op AD<Base>(right); }

# endif
