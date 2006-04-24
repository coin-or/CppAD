/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
/*
$begin ExpApxAD.cpp$$
$spell
	ExpApxAD
	cout
	du
	dv
	dw
	endl
	hpp
	http
	iostream
	org
	std
	www
	CppAD
$$

$section Computing ExpApx Derivatives with CppAD$$.

$head Source Code$$
$codep */

# include <iostream>         // C++ standard input/output
# include <vector>           // standard vector
# include <CppAD/CppAD.h>    // http://www.coin-or.org/CppAD/ 
# include "ExpApx.hpp"       // our example exponential function approximation
int main(void)
{	using CppAD::AD;

	// domain space vector
	size_t n = 2;
	std::vector< AD<double> > U(n);
	U[0] = .5;  // value of x for this operation sequence
	U[1] = .2;  // value of e for this operation sequence

	// declare independent variables and start tape recording
	CppAD::Independent(U);

	// evaluate our exponential approximation
	AD<double> x   = U[0];
	AD<double> e   = U[1];
	AD<double> apx = ExpApx(x, e);  

	// range space vector
	size_t m = 1;
	std::vector< AD<double> > V(m);
	V[0] = apx;

	// create f: U -> V and stop tape recording
	CppAD::ADFun<double> f(U, V);

	// forward mode sweep that computes partial w.r.t x
	std::vector<double> du(n);
	std::vector<double> dv(m);
	du[0] = 1.;
	du[1] = 0.;
	dv    = f.Forward(1, du);
	std::cout << "Forward mode: partial ExpApx(x, e) w.r.t x is " 
	          << dv[0] << std::endl;
	

	// reverse mode sweep that derivative
	std::vector<double>  w(m);
	std::vector<double> dw(n);
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	std::cout << "Reverse mode: partial ExpApx(x, e) w.r.t x is " 
                  << dw[0] << std::endl;
	std::cout << "Reverse mode: partial ExpApx(x, e) w.r.t e is " 
                  << dw[1] << std::endl;
}
/* $$

$head Program Output$$
$code
$verbatim%Introduction/ExpApxAD.out%$$
$$

$end
*/
