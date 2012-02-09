/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/base_require.hpp>
# include <cppad/thread_alloc.hpp>

# define MY_BASE_ASSIGN_OPERATOR(op) \
	void operator op (const my_base& x) \
	{	*ptrdbl_ op *x.ptrdbl_; }

# define MY_BASE_MY_BASE_OPERATOR(op) const \
	my_base operator op (const my_base& x) const \
	{	my_base result; \
		double   dbl = *ptrdbl_; \
		double x_dbl = *x.ptrdbl_; \
		*result.ptrdbl_ = dbl op x_dbl; \
		return result; \
	}

# define MY_BASE_BOOL_OPERATOR(op) const \
	bool operator op (const my_base& x) const \
	{	double   dbl = *ptrdbl_; \
		double x_dbl = *x.ptrdbl_; \
		return dbl op x_dbl; \
	}

namespace {
	using CppAD::thread_alloc;

	class my_base {
	public:
		double* ptrdbl_;

		// missing from requirements
		my_base(void)
		{	size_t cap;
			void* v  = thread_alloc::get_memory(sizeof(double), cap); 
			ptrdbl_  = static_cast<double*>(v);
		}
		my_base(double dbl)
		{	size_t cap;
			void *v  = thread_alloc::get_memory(sizeof(double), cap); 
			ptrdbl_  = static_cast<double*>(v);
			*ptrdbl_ = dbl;
		}
		~my_base(void)
		{	void* v  = static_cast<void*>(ptrdbl_);
			thread_alloc::return_memory(v);
		}
		void operator=(const my_base& x)
		{	*ptrdbl_ = *x.ptrdbl_; }
		my_base operator-(void) const
		{	my_base result;
			*result.ptrdbl_ = - *ptrdbl_;
			return result;
		}
		MY_BASE_ASSIGN_OPERATOR(+=)
		MY_BASE_ASSIGN_OPERATOR(-=)
		MY_BASE_ASSIGN_OPERATOR(*=)
		MY_BASE_ASSIGN_OPERATOR(/=)
		MY_BASE_MY_BASE_OPERATOR(+)
		MY_BASE_MY_BASE_OPERATOR(-)
		MY_BASE_MY_BASE_OPERATOR(*)
		MY_BASE_MY_BASE_OPERATOR(/)
		MY_BASE_BOOL_OPERATOR(!=)
		MY_BASE_BOOL_OPERATOR(==)
	};
}

// missing
namespace CppAD {
	std::ostream& operator << (std::ostream &os, const my_base& x)
	{	os << *x.ptrdbl_;
		return os;
	}
	inline my_base   atan(const my_base& x) { return   std::atan(*x.ptrdbl_); }
	inline my_base CondExpOp( 
		enum CompareOp     cop          ,
		const my_base&       left         ,
		const my_base&       right        , 
		const my_base&       exp_if_true  , 
		const my_base&       exp_if_false )
	{	// not used
		assert(false);
	}
	// use CondExpOp above to define all conditional expression cases
	CPPAD_COND_EXP_REL(my_base)
	//
	inline bool EqualOpSeq(const my_base& x, const my_base& y)
	{	return *x.ptrdbl_ == *y.ptrdbl_; }
	inline bool IdenticalPar(const my_base& x)
	{	return true; }
	inline bool IdenticalZero(const my_base& x)
	{	return (*x.ptrdbl_ == 0.0); }
	inline bool IdenticalOne(const my_base& x)
	{	return (*x.ptrdbl_ == 1.0); }
	inline bool IdenticalEqualPar(const my_base& x, const my_base& y)
	{	return (*x.ptrdbl_ == *y.ptrdbl_); }
	inline int Integer(const my_base& x)
	{	return static_cast<int>(*x.ptrdbl_); }
	template <>
	inline my_base epsilon<my_base>(void)
	{	return std::numeric_limits<double>::epsilon(); }
	inline bool GreaterThanZero(const my_base& x)
	{	return *x.ptrdbl_ > 0.0; }
	inline bool GreaterThanOrZero(const my_base& x)
	{	return *x.ptrdbl_ >= 0.0; }
	inline bool LessThanZero(const my_base& x)
	{	return *x.ptrdbl_ < 0.0; }
	inline bool LessThanOrZero(const my_base& x)
	{	return *x.ptrdbl_ <= 0.f; }
	inline bool abs_geq(const my_base& x, const my_base& y)
	{	return std::fabs(*x.ptrdbl_) >= std::fabs(*y.ptrdbl_); }

	inline my_base   acos(const my_base& x) { return   std::acos(*x.ptrdbl_); }
	inline my_base   asin(const my_base& x) { return   std::asin(*x.ptrdbl_); }
	inline my_base    cos(const my_base& x) { return    std::cos(*x.ptrdbl_); }
	inline my_base   cosh(const my_base& x) { return   std::cosh(*x.ptrdbl_); }
	inline my_base    exp(const my_base& x) { return    std::exp(*x.ptrdbl_); }
	inline my_base    log(const my_base& x) { return    std::log(*x.ptrdbl_); }
	inline my_base  log10(const my_base& x) { return  std::log10(*x.ptrdbl_); }
	inline my_base    sin(const my_base& x) { return    std::sin(*x.ptrdbl_); }
	inline my_base   sinh(const my_base& x) { return   std::sinh(*x.ptrdbl_); }
	inline my_base   sqrt(const my_base& x) { return   std::sqrt(*x.ptrdbl_); }
	inline my_base    tan(const my_base& x) { return    std::tan(*x.ptrdbl_); }
	inline my_base   tanh(const my_base& x) { return   std::tanh(*x.ptrdbl_); }

	inline my_base abs(const my_base& x)
	{	return std::fabs(*x.ptrdbl_); }
	inline my_base sign(const my_base& x)
	{	if( *x.ptrdbl_ > 0.0 )
			return 1.0;
		if( *x.ptrdbl_ == 0.0 )
			return 0.0;
		return -1.0;
	}
	inline my_base pow(const my_base& x, const my_base& y)
	{ return std::pow(*x.ptrdbl_, *y.ptrdbl_); }
}

# include <cppad/cppad.hpp>

bool base_alloc(void)
{	bool ok = true;
	typedef CppAD::AD<my_base> my_ad;

	// set static memory correspoding to isnan
	CppAD::isnan( my_base(0.) );
	CppAD::isnan( my_ad(0.) );
	bool set_static = true;
	CppAD::memory_leak(set_static);
	

	// y = x^2
	CppAD::vector<my_ad>   a_x(1), a_y(1);
	a_x[0] = my_ad(1.);
	CppAD::Independent(a_x);
	a_y[0] = a_x[0];
	CppAD::ADFun<my_base> f(a_x, a_y);

	return ok;
}
