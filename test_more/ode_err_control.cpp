// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT

/*
Test relative error with zero initial conditions.
(Uses minimum step size to integrate).
*/
// BEGIN PROGRAM

# include <cstddef>                 // for size_t
# include <cppad/ode_err_control.hpp>   // CppAD::OdeErrControl
# include <cppad/near_equal.hpp>       // CppAD::NearEqual
# include <cppad/vector.hpp>    // CppAD::vector
# include <cppad/runge_45.hpp>         // CppAD::Runge45

namespace {
	// --------------------------------------------------------------
	class Fun {
	private:
		 size_t n;   // dimension of the state space
	public:
		// constructor
		Fun(size_t n_) : n(n_)
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
	class Method {
	private:
		Fun F;
	public:
		// constructor
		Method(size_t n_) : F(n_)
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

bool OdeErrControl(void)
{	bool   ok = true;     // initial return value

	// Runge45 should yield exact results for x_i (t) = t^(i+1), i < 4
	size_t  n = 6;        
	
	// construct method for n component solution
	Method method(n);

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

// END PROGRAM
