/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin OdeErrControl.cpp$$
$spell
$$

$section OdeErrControl: Example and Test$$

$index OdeErrControl, example$$
$index example, OdeErrControl$$
$index test, OdeErrControl$$

Define 
$latex X : \R \rightarrow \R^2$$ by
$latex \[
\begin{array}{rcl}
	X_0 (0)       & = & 1  \\
	X_1 (0)       & = & 0  \\
	X_0^{(1)} (t) & = & - \alpha X_0 (t)  \\
	X_1^{(1)} (t) & = &  1 / X_0 (t)
\end{array}
\] $$
It follows that
$latex \[
\begin{array}{rcl}
X_0 (t) & = &  \exp ( - \alpha t )  \\
X_1 (t) & = & [ \exp( \alpha t ) - 1 ] / \alpha
\end{array}
\] $$
This example tests OdeErrControl using the relations above.

$head Nan$$
Note that $latex X_0 (t) > 0$$ for all $latex t$$ and that the
ODE goes through a singularity between $latex X_0 (t) > 0$$
and $latex X_0 (t) < 0$$.
If $latex X_0 (t) < 0$$,
we return $code nan$$ in order to inform 
$code OdeErrControl$$ that its is taking to large a step.


$code
$verbatim%example/ode_err_control.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cstddef>                     // for size_t
# include <cmath>                       // for exp
# include <cppad/ode_err_control.hpp>   // CppAD::OdeErrControl
# include <cppad/near_equal.hpp>        // CppAD::NearEqual
# include <cppad/vector.hpp>            // CppAD::vector
# include <cppad/runge_45.hpp>          // CppAD::Runge45
# include <cppad/nan.hpp>               // for nan

namespace {
	// --------------------------------------------------------------
	class Fun {
	private:
		const double alpha_;
	public:
		// constructor
		Fun(double alpha) : alpha_(alpha)
		{ } 

		// set f = x'(t)
		void Ode(
			const double                &t, 
			const CppAD::vector<double> &x, 
			CppAD::vector<double>       &f)
		{	f[0] = - alpha_ * x[0];
			f[1] = 1. / x[0];	
			// case where ODE does not make sense
			if( x[0] < 0. )
				f[1] = CppAD::nan(0.);
		}

	};

	// --------------------------------------------------------------
	class Method {
	private:
		Fun F;
	public:
		// constructor
		Method(double alpha) : F(alpha)
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
{	bool ok = true;     // initial return value

	double alpha = 10.;
	Method method(alpha);

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
	double scur = 1.;
	double erel = 0.;

	// outputs
	CppAD::vector<double> ef(2);
	CppAD::vector<double> xf(2);
	CppAD::vector<double> maxabs(2);
	size_t nstep;

	
	xf = OdeErrControl(method,
		ti, tf, xi, smin, smax, scur, eabs, erel, ef, maxabs, nstep);

	double x0 = exp(-alpha*tf);
	ok &= CppAD::NearEqual(x0, xf[0], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[0], 1e-4, 1e-4);

	double x1 = (exp(alpha*tf) - 1) / alpha;
	ok &= CppAD::NearEqual(x1, xf[1], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[1], 1e-4, 1e-4);

	return ok;
}

// END PROGRAM
