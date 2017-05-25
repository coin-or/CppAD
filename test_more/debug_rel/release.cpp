/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>

// Returns a pointer to a double that has value 5.0
// and must be freed using thread_alloc::return_memory.
double *release_thread_alloc(void)
{	size_t min_bytes = sizeof(double);
	size_t cap_bytes;
	void*   v_ptr = CppAD::thread_alloc::get_memory(min_bytes, cap_bytes);
	double* d_ptr = reinterpret_cast<double*>(v_ptr);
	*d_ptr        = 5.0;
	return d_ptr;
}

// just use ADFun<double> constructor
void release_adfun_ctor(void)
{
	CppAD::vector< CppAD::AD<double> > ax(1), ay(1);
	ax[0] = 0.;
	CppAD::Independent(ax);
	ay[0] = fabs(ax[0]);
	CppAD::ADFun<double> f(ax, ay);
	return;
}
