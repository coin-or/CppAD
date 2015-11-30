// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include "ode_run.hpp"
# include "ode_fast.hpp"
# include "ode_check.hpp"

bool ode_fast_check(void)
{	bool ok = true;
	bool retape;
	size_t i;

	// solution vector
	NumberVector x;

	// number of time grid intervals between measurement values
	SizeVector N(Nz + 1);
	N[0] = 0;
	for(i = 1; i <= Nz; i++)
		N[i] = 5;

	for(i = 0; i < 2; i++)
	{	retape = bool(i);
		ipopt_ode_case<FG_fast>(retape, N, x);
		ok &= ode_check(N, x);
	}

	return ok;
}
