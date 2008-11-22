# ifndef CPPAD_DEFINE_INCLUDED
# define CPPAD_DEFINE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Define$$ $comment CppAD Developer Documentation$$
$spell
	bool
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

$head CPPAD_INLINE$$
Microsoft Visual C++ version 9.0 generates a warning if a template
function is declared as a friend
(this was not a problem for version 7.0). 
The warning identifier is
$code warning C4396:$$ and it contains the text
$code 
the inline specifier cannot be used when a friend declaration refers 
to a specialization of a function template
$$.
This happens even if the function is not a specialization.
The macro $code CPPAD_INLINE$$ is defined as empty for Microsoft compilers.
 

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

$head CPPAD_FOLD_ASSIGNMENT_OPERATOR$$
The syntax
$syntax%
	CPPAD_FOLD_ASSIGNMENT_OPERATOR(%Op%)
%$$
assumes that the operator
$syntax%
	%left% %Op% %right%
%$$
is defined for the case where $italic left$$ and $italic right$$ 
have type $syntax%AD<%Base%>%$$.
It uses this case to define the cases where
$italic left$$ has type $syntax%AD<%Base%>%$$ and
$italic right$$ has type
$syntax%VecAD_reference<%Base%>%$$,
$syntax%AD<%Base%>%$$,
$italic Base$$, or
$code double$$.
The argument $italic right$$ is $code const$$ and call by reference.
This macro converts the operands to $syntax%AD<%Base%>%$$ and then
uses the definition of the same operation for that case. 

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
$code double$$.
All of the arguments are $code const$$ and call by reference.
This macro converts the operands to $syntax%AD<%Base%>%$$ and then
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
$code double$$.
All of the arguments are $code const$$ and call by reference.
This macro converts the operands to $syntax%AD<%Base%>%$$ and then
uses the definition of the same operation for that case. 


$end
*/

# ifdef _MSC_VER
# define CPPAD_INLINE
# else
# define CPPAD_INLINE inline
# endif

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


# define CPPAD_FOLD_ASSIGNMENT_OPERATOR(Op)                             \
                                                                        \
template <class Base>                                                   \
inline AD<Base>& operator Op                                            \
(AD<Base> &left, double right)                                          \
{	return left Op AD<Base>(right); }                               \
                                                                        \
template <class Base>                                                   \
inline AD<Base>& operator Op                                            \
(AD<Base> &left, const Base &right)                                     \
{	return left Op AD<Base>(right); }                               \
                                                                        \
inline AD<double>& operator Op                                          \
(AD<double> &left, const double &right)                                 \
{	return left Op AD<double>(right); }                             \
                                                                        \
template <class Base>                                                   \
inline AD<Base>& operator Op                                            \
(AD<Base> &left, const VecAD_reference<Base> &right)                    \
{	return left Op right.ADBase(); }

// =====================================================================

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
/* Operations double */                                                \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const double &left, const AD<Base> &right)                    \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const double &left, const VecAD_reference<Base> &right)       \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const AD<Base> &left, const double &right)                    \
{	return left Op AD<Base>(right); }                              \
                                                                       \
template <class Base>                                                  \
inline AD<Base> operator Op                                            \
	(const VecAD_reference<Base> &left, const double &right)       \
{	return left.ADBase() Op AD<Base>(right); }                     \
/* ----------------------------------------------------------------*/  \
/* Special case to avoid ambuigity when Base is double */              \
                                                                       \
inline AD<double> operator Op                                          \
	(const double &left, const AD<double> &right)                  \
{	return AD<double>(left) Op right; }                            \
                                                                       \
inline AD<double> operator Op                                          \
	(const double &left, const VecAD_reference<double> &right)     \
{	return AD<double>(left) Op right.ADBase(); }                   \
                                                                       \
inline AD<double> operator Op                                          \
	(const AD<double> &left, const double &right)                  \
{	return left Op AD<double>(right); }                            \
                                                                       \
inline AD<double> operator Op                                          \
	(const VecAD_reference<double> &left, const double &right)     \
{	return left.ADBase() Op AD<double>(right); }

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
/* Operations double */                                                \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const double &left, const AD<Base> &right)                    \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const double &left, const VecAD_reference<Base> &right)       \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const AD<Base> &left, const double &right)                    \
{	return left Op AD<Base>(right); }                              \
                                                                       \
template <class Base>                                                  \
inline bool operator Op                                                \
	(const VecAD_reference<Base> &left, const double &right)       \
{	return left.ADBase() Op AD<Base>(right); }                     \
/* ----------------------------------------------------------------*/  \
/* Special case to avoid ambuigity when Base is double */              \
                                                                       \
inline bool operator Op                                                \
	(const double &left, const AD<double> &right)                  \
{	return AD<double>(left) Op right; }                            \
                                                                       \
inline bool operator Op                                                \
	(const double &left, const VecAD_reference<double> &right)     \
{	return AD<double>(left) Op right.ADBase(); }                   \
                                                                       \
inline bool operator Op                                                \
	(const AD<double> &left, const double &right)                  \
{	return left Op AD<double>(right); }                            \
                                                                       \
inline bool operator Op                                                \
	(const VecAD_reference<double> &left, const double &right)     \
{	return left.ADBase() Op AD<double>(right); }

# endif
