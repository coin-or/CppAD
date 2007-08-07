/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cstddef>                    // for size_t
# include <cmath>                      // for exp
# include <cppad/ode_err_control.hpp>  // CppAD::OdeErrControl
# include <cppad/near_equal.hpp>       // CppAD::NearEqual
# include <cppad/vector.hpp>           // CppAD::vector
# include <cppad/runge_45.hpp>         // CppAD::Runge45

/* -------------------------------------------------------------------------
Test relative error with zero initial conditions.
(Uses minimum step size to integrate).
*/

namespace {
	// --------------------------------------------------------------
	class Fun_one {
	private:
		 size_t n;   // dimension of the state space
	public:
		// constructor
		Fun_one(size_t n_) : n(n_)
		{ } 

		// given x(0) = 0
		// solution is x_i (t) = t^(i+1)
		void Ode(
			const double                &t, 
			const CppAD::vector<double> &x, 
			CppAD::vector<double>       &f)
		{	size_t i;
			f[0] = 1.;
			for(i = 1; i < n; i++)
				f[i] = (i+1) * x[i-1];
		}
	};

	// --------------------------------------------------------------
	class Method_one {
	private:
		Fun_one F;
	public:
		// constructor
		Method_one(size_t n_) : F(n_)
		{ }
		void step(
			double ta, 
			double tb, 
			CppAD::vector<double> &xa ,
			CppAD::vector<double> &xb ,
			CppAD::vector<double> &eb )
		{	xb = CppAD::Runge45(F, 1, ta, tb, xa, eb);
		}
		size_t order(void)
		{	return 4; }
	};
}

bool OdeErrControl_one(void)
{	bool   ok = true;     // initial return value

	// Runge45 should yield exact results for x_i (t) = t^(i+1), i < 4
	size_t  n = 6;        
	
	// construct method for n component solution
	Method_one method(n);

	// inputs to OdeErrControl

	double ti   = 0.;
	double tf   = .9;
	double smin = 1e-2;
	double smax = 1.;
	double scur = .5;
	double erel = 1e-7;

	CppAD::vector<double> xi(n);
	CppAD::vector<double> eabs(n);
	size_t i;
	for(i = 0; i < n; i++)
	{	xi[i]   = 0.;
		eabs[i] = 0.;
	}

	// outputs from OdeErrControl

	CppAD::vector<double> ef(n);
	CppAD::vector<double> xf(n);
	
	xf = OdeErrControl(method,
		ti, tf, xi, smin, smax, scur, eabs, erel, ef);

	double check = 1.;
	for(i = 0; i < n; i++)
	{	check *= tf;
		ok &= CppAD::NearEqual(check, xf[i], erel, 0.);
	}

	return ok;
}

/*
Old example new just a test
Define 
$latex X : \R \rightarrow \R^2$$ by
$latex \[
\begin{array}{rcl}
X_0 (t) & = &  - \exp ( - w_0 t )  \\
X_1 (t) & = & \frac{w_0}{w_1 - w_0} [ \exp ( - w_0 t ) - \exp( - w_1 t )]
\end{array}
\] $$
It follows that $latex X_0 (0) = 1$$, $latex X_1 (0) = 0$$ and
$latex \[
\begin{array}{rcl}
	X_0^{(1)} (t) & = & - w_0 X_0 (t)  \\
	X_1^{(1)} (t) & = & + w_0 X_0 (t) - w_1 X_1 (t) 
\end{array}
\] $$
*/

namespace {
	// --------------------------------------------------------------
	class Fun_two {
	private:
		 CppAD::vector<double> w;
	public:
		// constructor
		Fun_two(const CppAD::vector<double> &w_) : w(w_)
		{ } 

		// set f = x'(t)
		void Ode(
			const double                &t, 
			const CppAD::vector<double> &x, 
			CppAD::vector<double>       &f)
		{	f[0] = - w[0] * x[0];
			f[1] = + w[0] * x[0] - w[1] * x[1];	
		}
	};

	// --------------------------------------------------------------
	class Method_two {
	private:
		Fun_two F;
	public:
		// constructor
		Method_two(const CppAD::vector<double> &w_) : F(w_)
		{ }
		void step(
			double ta, 
			double tb, 
			CppAD::vector<double> &xa ,
			CppAD::vector<double> &xb ,
			CppAD::vector<double> &eb )
		{	xb = CppAD::Runge45(F, 1, ta, tb, xa, eb);
		}
		size_t order(void)
		{	return 4; }
	};
}

bool OdeErrControl_two(void)
{	bool ok = true;     // initial return value

	CppAD::vector<double> w(2);
	w[0] = 10.;
	w[1] = 1.;
	Method_two method(w);

	CppAD::vector<double> xi(2);
	xi[0] = 1.;
	xi[1] = 0.;

	CppAD::vector<double> eabs(2);
	eabs[0] = 1e-4;
	eabs[1] = 1e-4;

	// inputs
	double ti   = 0.;
	double tf   = 1.;
	double smin = 1e-4;
	double smax = 1.;
	double scur = .5;
	double erel = 0.;

	// outputs
	CppAD::vector<double> ef(2);
	CppAD::vector<double> xf(2);
	CppAD::vector<double> maxabs(2);
	size_t nstep;

	
	xf = OdeErrControl(method,
		ti, tf, xi, smin, smax, scur, eabs, erel, ef, maxabs, nstep);

	double x0 = exp(-w[0]*tf);
	ok &= CppAD::NearEqual(x0, xf[0], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[0], 1e-4, 1e-4);

	double x1 = w[0] * (exp(-w[0]*tf) - exp(-w[1]*tf))/(w[1] - w[0]);
	ok &= CppAD::NearEqual(x1, xf[1], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[1], 1e-4, 1e-4);

	return ok;
}

bool OdeErrControl(void)
{	bool ok = true;
	ok     &= OdeErrControl_one();
	ok     &= OdeErrControl_two();
	return ok;
}

