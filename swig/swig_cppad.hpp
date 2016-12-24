// $Id$
# ifndef CPPAD_SWIG_SWIG_CPPAD_HPP
# define CPPAD_SWIG_SWIG_CPPAD_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <vector>
# include <cppad/configure.hpp>

// declarations without definitions
namespace CppAD {
	template <class Base> class AD;
	template <class Base> class ADFun;
}
class a_double;
class a_fun;

/// structure with exact same member data as AD<double>
struct a_double_data {
	/// Base value corresponding to this object
	double                 value;
	/// tape identifier correspoding to taddr
	CPPAD_TAPE_ID_TYPE     tape_id;
	/// tape address for this object
	CPPAD_TAPE_ADDR_TYPE   taddr;
};

// --------------------------------------------------------------------------
// functions
std::vector<a_double> independent(const std::vector<double>& x);
void abort_recording(void);

// --------------------------------------------------------------------------
// a_double
// --------------------------------------------------------------------------
/// swig class that acts the same as CppAD::AD<double>
class a_double
{	friend std::vector<a_double> independent(const std::vector<double>& x);
	friend class a_fun;
	//
	// private members are not in swig interface
	private:
	/// data for this object
	a_double_data        data_;
	/// pointer to this as an AD<double> object
	CppAD::AD<double>* ptr(void);
	/// const version of pointer to this as an AD<double> object
	const CppAD::AD<double>* ptr(void) const;
	/// ctor from CppAD::AD<double>
	a_double(const CppAD::AD<double>* ad_ptr);
	// -----------------------------------------------------------------------
	// public members are in swig interface
	public:
	/// default ctor
	a_double(void);
	/// destructor
	~a_double(void);
	/// ctor from double
	a_double(const double& value);
	/// ctor from a_double
	a_double(const a_double& ad);
	/// conversion to double
	double value(void) const;
	// binary operators with a_double result
	a_double operator+(const a_double& ad) const;
	a_double operator-(const a_double& ad) const;
	a_double operator*(const a_double& ad) const;
	a_double operator/(const a_double& ad) const;
};
// --------------------------------------------------------------------------
// a_fun
// --------------------------------------------------------------------------
/// swig class that acts the same as CppAD::ADFun<double>
class a_fun
{	// private members are not in swig interface
	private:
	/// ADFun<double> representation
	CppAD::ADFun<double>* ptr_;
	// -----------------------------------------------------------------------
	// public members are in swig interface
	public:
	/// default ctor
	a_fun(void);
	/// destructor
	~a_fun(void);
	/// constrtuctor
	a_fun( const std::vector<a_double>& ax, const std::vector<a_double>& ay );
};
// --------------------------------------------------------------------------





# endif
