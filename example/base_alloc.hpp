// $Id$
# ifndef CPPAD_BASE_ALLOC_HPP
# define CPPAD_BASE_ALLOC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin base_alloc.hpp$$
$spell
	azmul
	expm1
	atanh
	acosh
	asinh
	Rel
	Lt Le Eq Ge Gt
	Cond
	enum
	geq
	cos
	sqrt
	cppad.hpp
	alloc
	op
	const
	ptrdbl
	bool
	CppAD
	sizeof
	inline
	atan
	ostream
	namespace
	exp
	tanh
	acos
	asin
	std
	fabs
	erf
	endif
$$
$section Example AD<Base> Where Base Constructor Allocates Memory$$

$head Purpose$$
Demonstrate use of $codei%AD<%Base%>%$$
where memory is allocated for each element of the type $icode Base$$.
In addition, this is a complete example where all the
$cref/required Base/base_require/$$ type
operations are defined (as apposed to other examples where
some of the operations for the Base type are already defined).

$head Include File$$
This file uses some of the definitions in $cref base_require$$
and $cref thread_alloc$$.
$codep */
# include <cppad/base_require.hpp>
# include <cppad/utility/thread_alloc.hpp>
/* $$

$head Computed Assignment Macro$$
This macro is used for the
$code base_alloc$$ computed assignment operators; to be specific,
used with $icode op $$ equal to
$code +=$$,
$code -=$$,
$code *=$$,
$code /=$$.
$codep */
# define BASE_ALLOC_ASSIGN_OPERATOR(op) \
	void operator op (const base_alloc& x) \
	{	*ptrdbl_ op *x.ptrdbl_; }
/* $$

$head Binary Operator Macro$$
This macro is used for the
$code base_alloc$$ binary operators (as member functions); to be specific,
used with $icode op $$ equal to
$code +$$,
$code -$$,
$code *$$,
$code /$$.
$codep */
# define BASE_ALLOC_BINARY_OPERATOR(op) const \
	base_alloc operator op (const base_alloc& x) const \
	{	base_alloc result; \
		double   dbl = *ptrdbl_; \
		double x_dbl = *x.ptrdbl_; \
		*result.ptrdbl_ = dbl op x_dbl; \
		return result; \
	}
/* $$

$head Boolean Operator Macro$$
This macro can be used for the
$code base_alloc$$ binary operators that have a
$code bool$$ result; to be specific,
used with $icode op $$ equal to
$code ==$$,
$code !=$$,
$code <$$,
$code <=$$,
$code >=$$, and
$code >$$,
$codep */
# define BASE_ALLOC_BOOL_OPERATOR(op) const \
	bool operator op (const base_alloc& x) const \
	{	double   dbl = *ptrdbl_; \
		double x_dbl = *x.ptrdbl_; \
		return dbl op x_dbl; \
	}
/* $$

$head Class Definition$$
The following example class
defines the necessary $cref base_member$$ functions.
It is made more complicated by storing a pointer to a $code double$$
instead of the $code double$$ value itself.
$codep */

class base_alloc {
public:
	double* ptrdbl_;

	base_alloc(void)
	{	size_t cap;
		void* v  = CppAD::thread_alloc::get_memory(sizeof(double), cap);
		ptrdbl_  = static_cast<double*>(v);
	}
	base_alloc(double dbl)
	{	size_t cap;
		void *v  = CppAD::thread_alloc::get_memory(sizeof(double), cap);
		ptrdbl_  = static_cast<double*>(v);
		*ptrdbl_ = dbl;
	}
	base_alloc(const base_alloc& x)
	{	size_t cap;
		void *v  = CppAD::thread_alloc::get_memory(sizeof(double), cap);
		ptrdbl_  = static_cast<double*>(v);
		*ptrdbl_ = *x.ptrdbl_;
	}
	~base_alloc(void)
	{	void* v  = static_cast<void*>(ptrdbl_);
		CppAD::thread_alloc::return_memory(v);
	}
	base_alloc operator-(void) const
	{	base_alloc result;
		*result.ptrdbl_ = - *ptrdbl_;
		return result;
	}
	base_alloc operator+(void) const
	{	return *this; }
	void operator=(const base_alloc& x)
	{	*ptrdbl_ = *x.ptrdbl_; }
	BASE_ALLOC_ASSIGN_OPERATOR(+=)
	BASE_ALLOC_ASSIGN_OPERATOR(-=)
	BASE_ALLOC_ASSIGN_OPERATOR(*=)
	BASE_ALLOC_ASSIGN_OPERATOR(/=)
	BASE_ALLOC_BINARY_OPERATOR(+)
	BASE_ALLOC_BINARY_OPERATOR(-)
	BASE_ALLOC_BINARY_OPERATOR(*)
	BASE_ALLOC_BINARY_OPERATOR(/)
	BASE_ALLOC_BOOL_OPERATOR(==)
	BASE_ALLOC_BOOL_OPERATOR(!=)
	// The <= operator is not necessary for the base type requirements
	// (needed so we can use NearEqual with base_alloc arguments).
	BASE_ALLOC_BOOL_OPERATOR(<=)
};
/* $$

$head CondExpOp$$
The type $code base_alloc$$ does not use $cref CondExp$$ operations.
Hence its $code CondExpOp$$ function is defined by
$codep */
namespace CppAD {
	inline base_alloc CondExpOp(
		enum CompareOp     cop          ,
		const base_alloc&       left         ,
		const base_alloc&       right        ,
		const base_alloc&       exp_if_true  ,
		const base_alloc&       exp_if_false )
	{	// not used
		assert(false);

		// to void compiler error
		return base_alloc();
	}
}
/* $$

$head CondExpRel$$
The $cref/CPPAD_COND_EXP_REL/base_cond_exp/CondExpRel/$$ macro invocation
$codep */
namespace CppAD {
	CPPAD_COND_EXP_REL(base_alloc)
}
/* $$
uses $code CondExpOp$$ above to
define $codei%CondExp%Rel%$$ for $code base_alloc$$ arguments
and $icode%Rel%$$ equal to
$code Lt$$, $code Le$$, $code Eq$$, $code Ge$$, and $code Gt$$.

$head EqualOpSeq$$
The type $code base_alloc$$ is simple (in this respect) and so we define
$codep */
namespace CppAD {
	inline bool EqualOpSeq(const base_alloc& x, const base_alloc& y)
	{	return *x.ptrdbl_ == *y.ptrdbl_; }
}
/* $$

$head Identical$$
The type $code base_alloc$$ is simple (in this respect) and so we define
$codep */
namespace CppAD {
	inline bool IdenticalPar(const base_alloc& x)
	{	return true; }
	inline bool IdenticalZero(const base_alloc& x)
	{	return (*x.ptrdbl_ == 0.0); }
	inline bool IdenticalOne(const base_alloc& x)
	{	return (*x.ptrdbl_ == 1.0); }
	inline bool IdenticalEqualPar(const base_alloc& x, const base_alloc& y)
	{	return (*x.ptrdbl_ == *y.ptrdbl_); }
}
/* $$

$head Output Operator$$
$codep */
namespace CppAD {
	std::ostream& operator << (std::ostream &os, const base_alloc& x)
	{	os << *x.ptrdbl_;
		return os;
	}
}
/* $$

$head Integer$$
$codep */
namespace CppAD {
	inline int Integer(const base_alloc& x)
	{	return static_cast<int>(*x.ptrdbl_); }
}
/* $$

$head azmul$$
$codep */
namespace CppAD {
	CPPAD_AZMUL( base_alloc )
}
/* $$

$head Ordered$$
The $code base_alloc$$ type supports ordered comparisons
$codep */
namespace CppAD {
	inline bool GreaterThanZero(const base_alloc& x)
	{	return *x.ptrdbl_ > 0.0; }
	inline bool GreaterThanOrZero(const base_alloc& x)
	{	return *x.ptrdbl_ >= 0.0; }
	inline bool LessThanZero(const base_alloc& x)
	{	return *x.ptrdbl_ < 0.0; }
	inline bool LessThanOrZero(const base_alloc& x)
	{	return *x.ptrdbl_ <= 0.f; }
	inline bool abs_geq(const base_alloc& x, const base_alloc& y)
	{	return std::fabs(*x.ptrdbl_) >= std::fabs(*y.ptrdbl_); }
}
/* $$

$head Unary Standard Math$$
The macro
$cref/CPPAD_STANDARD_MATH_UNARY/base_std_math/CPPAD_STANDARD_MATH_UNARY/$$
would not work with the type $code base_alloc$$ so we define
a special macro for this type:
$codep */
# define BASE_ALLOC_STD_MATH(fun) \
	inline base_alloc fun (const base_alloc& x) \
	{ return   std::fun(*x.ptrdbl_); }
/* $$
The following invocations of the macro above define the
$cref/unary standard math/base_std_math/Unary Standard Math/$$ functions
(except for $code abs$$):
$codep */
namespace CppAD {
	BASE_ALLOC_STD_MATH(acos)
	BASE_ALLOC_STD_MATH(asin)
	BASE_ALLOC_STD_MATH(atan)
	BASE_ALLOC_STD_MATH(cos)
	BASE_ALLOC_STD_MATH(cosh)
	BASE_ALLOC_STD_MATH(exp)
	BASE_ALLOC_STD_MATH(log)
	BASE_ALLOC_STD_MATH(log10)
	BASE_ALLOC_STD_MATH(sin)
	BASE_ALLOC_STD_MATH(sinh)
	BASE_ALLOC_STD_MATH(sqrt)
	BASE_ALLOC_STD_MATH(tan)
	BASE_ALLOC_STD_MATH(tanh)
}
/* $$
The absolute value function is special because its $code std$$ name is
$code fabs$$
$codep */
namespace CppAD {
	inline base_alloc abs(const base_alloc& x)
	{	return std::fabs(*x.ptrdbl_); }
}
/* $$

$head erf, asinh, acosh, atanh, expm1, log1p$$
The following defines the
$cref/erf, asinh, acosh, atanh, expm1, log1p
	/base_std_math
	/erf, asinh, acosh, atanh, expm1, log1p
/$$ functions
required by $code AD<base_alloc>$$:
$codep */
# if CPPAD_USE_CPLUSPLUS_2011
	BASE_ALLOC_STD_MATH(erf)
	BASE_ALLOC_STD_MATH(asinh)
	BASE_ALLOC_STD_MATH(acosh)
	BASE_ALLOC_STD_MATH(atanh)
	BASE_ALLOC_STD_MATH(expm1)
	BASE_ALLOC_STD_MATH(log1p)
# endif
/* $$

$head sign$$
The following defines the $code CppAD::sign$$ function that
is required to use $code AD<base_alloc>$$:
$codep */
namespace CppAD {
	inline base_alloc sign(const base_alloc& x)
	{	if( *x.ptrdbl_ > 0.0 )
			return 1.0;
		if( *x.ptrdbl_ == 0.0 )
			return 0.0;
		return -1.0;
	}
}
/* $$

$head pow $$
The following defines a $code CppAD::pow$$ function that
is required to use $code AD<base_alloc>$$:
$codep */
namespace CppAD {
	inline base_alloc pow(const base_alloc& x, const base_alloc& y)
	{ return std::pow(*x.ptrdbl_, *y.ptrdbl_); }
}
/* $$

$head numeric_limits$$
The following defines the CppAD $cref numeric_limits$$
for the type $code base_alloc$$:
$codep */
namespace CppAD {
	CPPAD_NUMERIC_LIMITS(double, base_alloc)
}
/* $$

$end
*/
# endif
