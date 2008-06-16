/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

namespace { // ---------------------------------------------------------

template <class Vector>
Vector eval_g(const Vector&  x)
{	Vector g(2);

	g[0] = x[0] * x[1] * x[2] * x[3];
	g[1] = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3];

	return g;
}

template <class Vector>
Vector eval_jac_g(const Vector&  x)
{	Vector jac_g(8);

	// g[0] = x[0] * x[1] * x[2] * x[3];
	jac_g[0] = x[1] * x[2] * x[3];
	jac_g[1] = x[0] * x[2] * x[3];
	jac_g[2] = x[0] * x[1] * x[3];
	jac_g[3] = x[0] * x[1] * x[2];

	// g[1] = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3];
	jac_g[4+0] = 2. * x[0];
	jac_g[4+1] = 2. * x[1];
	jac_g[4+2] = 2. * x[2];
	jac_g[4+3] = 2. * x[3];

	return jac_g;
}


} // End empty namespace 

bool jacobian(void)
{	bool ok = true;
	using CppAD::vector;
	size_t j, k;

	size_t n = 4;
	size_t m = 2;
	vector< CppAD::AD<double> > ad_x(n);
	vector< CppAD::AD<double> > ad_g(m);

	vector<double> x(n);
	x[0] = 1.; x[2] = 5.0; x[2] = 5.0; x[3] = 1.0;
	for(j = 0; j < n; j++)
		ad_x[j] = x[j];
	//
	CppAD::Independent(ad_x);
	ad_g = eval_g(ad_x);
	CppAD::ADFun<double> fun_g(ad_x, ad_g);

	vector<double> check(m * n);
	check = eval_jac_g(x);

	vector<double> jac_g = fun_g.Jacobian(x);

	for(k = 0; k < m *n; k++)
		ok &= CppAD::NearEqual(jac_g[k], check[k], 1e-10, 1e-10);

	jac_g = fun_g.SparseJacobian(x);

	for(k = 0; k < m *n; k++)
		ok &= CppAD::NearEqual(jac_g[k], check[k], 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
