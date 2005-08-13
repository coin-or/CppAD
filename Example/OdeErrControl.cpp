// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
$begin OdeErrControl.cpp$$
$spell
	Runge
$$

$section OdeErrControl: Example and Test$$

$index OdeErrControl, example$$
$index example, OdeErrControl$$
$index test, OdeErrControl$$

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
The example tests OdeErrControl using the relations above:

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/OdeErrControl.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cstddef>              // for size_t
# include <cmath>                // for exp
# include <CppAD/OdeErrControl.h>   // CppAD::OdeErrControl
# include <CppAD/NearEqual.h>    // CppAD::NearEqual
# include <CppAD/CppAD_vector.h> // CppAD::vector
# include <CppAD/Runge45.h>      // CppAD::Runge45

namespace {
	// --------------------------------------------------------------
	class Fun {
	private:
		 CppAD::vector<double> w;
	public:
		// constructor
		Fun(const CppAD::vector<double> &w_) : w(w_)
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
	class Method {
	private:
		Fun F;
	public:
		// constructor
		Method(const CppAD::vector<double> &w_) : F(w_)
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

	CppAD::vector<double> w(2);
	w[0] = 10.;
	w[1] = 1.;
	Method method(w);

	CppAD::vector<double> xi(2);
	xi[0] = 1.;
	xi[1] = 0.;

	CppAD::vector<double> eabs(2);
	eabs[0] = 1e-4;
	eabs[1] = 1e-4;

	CppAD::vector<double> ef(2);
	CppAD::vector<double> xf(2);

	double ti   = 0.;
	double tf   = 1.;
	double smin = 1e-4;
	double smax = 1.;
	double scur = .5;
	double erel = 0.;
	
	xf = OdeErrControl(method,
		ti, tf, xi, smin, smax, scur, eabs, erel, ef);

	double x0 = exp(-w[0]*tf);
	ok &= CppAD::NearEqual(x0, xf[0], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[0], 1e-4, 1e-4);

	double x1 = w[0] * (exp(-w[0]*tf) - exp(-w[1]*tf))/(w[1] - w[0]);
	ok &= CppAD::NearEqual(x1, xf[1], 1e-4, 1e-4);
	ok &= CppAD::NearEqual(0., ef[1], 1e-4, 1e-4);

	return ok;
}

// END PROGRAM
