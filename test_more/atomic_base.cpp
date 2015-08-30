// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>
namespace {
	using CppAD::vector;

	// test pack_sparsity_enum
	class atomic_pack : public CppAD::atomic_base<double>
	{
	public:
		atomic_pack(const std::string& name) :
		CppAD::atomic_base<double>(name, pack_sparsity_enum )
		{ }
	private:
		virtual bool forward(
			size_t                    p ,
			size_t                    q ,
			const vector<bool>&      vx ,
			vector<bool>&            vy ,
			const vector<double>&    tx ,
			vector<double>&          ty
		)
		{	size_t n = tx.size() / (q + 1);
			size_t m = ty.size() / (q + 1);
			assert( n == 3 );
			assert( m == 2 );

			// only order zero
			bool ok = q == 0;
			if( ! ok )
				return ok;

			// check for defining variable information
			if( vx.size() > 0 )
			{	ok   &= vx.size() == n;
				vy[0] = vx[2];
				vy[1] = vx[0] || vx[1];
			}

			// Order zero forward mode.
			// y[0] = x[2], y[1] = x[0] * x[1]
			if( p <= 0 )
			{	ty[0] = tx[2];
				ty[1] = tx[0] * tx[1];
			}
			return ok;
		}
	};
}
bool atomic_base(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// Create the atomic get_started object
	atomic_pack afun("atomic_pack");

	size_t n = 3;
	size_t m = 2;
	vector< AD<double> > ax(n), ay(m);
	for(size_t j = 0; j < n; j++)
		ax[j] = double(j + 1);

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// call user function
	afun(ax, ay);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f;
	f.Dependent (ax, ay);  // f(x) = x

	// check function value
	ok &= NearEqual(ay[0] , ax[2],  eps, eps);
	ok &= NearEqual(ay[1] , ax[0] * ax[1],  eps, eps);

	return ok;
}
