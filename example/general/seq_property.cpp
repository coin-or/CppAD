/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin seq_property.cpp$$
$spell
	op
	arg
	abs
	var
	VecAD
$$

$section ADFun Sequence Properties: Example and Test$$
$mindex Domain Range Parameter size_var size_par size_op_arg size_VecAD$$




$code
$srcfile%example/general/seq_property.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool seq_property(void)
{	bool ok = true;
	using CppAD::AD;

	// Use nvar to track the number of variables in the operation sequence.
	// Start with one for the phantom variable at tape address zero.
	size_t nvar = 1;

	// Use npar to track the number of parameters in the operation sequence.
	size_t npar = 0;

	// Start with one for operator corresponding to phantom variable
	size_t nop  = 1;

	// Start with one for operator corresponding to phantom argument
	size_t narg = 1;

	// Use ntext to track the number of characters used to label
	// output generated using PrintFor commands.
	size_t ntext = 0;

	// Use nvecad to track the number of VecAD vectors, plus the number
	// of VecAD vector elements, in the operation sequence.
	size_t nvecad = 0;

	// a VecAD vector
	CppAD::VecAD<double> v(2);
	v[0]     = 0; // requires the parameter 0, when becomes a variable
	v[1]     = 1; // requires the parameter 1, when becomes a variable

	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) x(n);
	x[0]     = 0.;
	x[1]     = 1.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);
	nvar    += n;
	nop     += n;

	// a computation that adds to the operation sequence
	AD<double> I = 0;
	v[I]         = x[0];
	nvecad      +=   3;  // one for vector, two for its elements
	npar        +=   2;  // need parameters 0 and 1 for initial v
	nop         +=   1;  // operator for storing in a VecAD object
	narg        +=   3;  // the three arguments are v, I, and x[0]

	// some operations that do not add to the operation sequence
	AD<double> u = x[0];  // use same variable as x[0]
	AD<double> w = x[1];  // use same variable as x[1]

	// a computation that adds to the operation sequence
	w      = w * (u + w);
	nop   += 2;   // requires two new operators, an add and a multiply
	nvar  += 2;   // each operator results in its own variable
	narg  += 4;   // each operator has two arguments

	// range space vector
	size_t m = 3;
	CPPAD_TESTVECTOR(AD<double>) y(m);

	// operations that do not add to the operation sequence
	y[0]   = 1.;  // re-use the parameter 1
	y[1]   = u;   // use same variable as u

	// a computation that adds to the operation sequence
	y[2]   = w + 2.;
	nop   += 1;   // requires a new add operator
	npar  += 1;   // new parameter 2 is new, so it must be included
	nvar  += 1;   // variable corresponding to the result
	narg  += 2;   // operator has two arguments

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);
	nop   += 1;   // special operator for y[0] because it is a parameter
	nvar  += 1;   // special variable for y[0] because it is a parameter
	narg  += 1;   // identifies which parameter corresponds to y[0]
	nop   += 1;   // special operator at the end of operation sequence

	// check the sequence property functions
	ok &= f.Domain()      == n;
	ok &= f.Range()       == m;
	ok &= f.Parameter(0)  == true;
	ok &= f.Parameter(1)  == false;
	ok &= f.Parameter(2)  == false;
	ok &= f.size_var()    == nvar;
	ok &= f.size_op()     == nop;
	ok &= f.size_op_arg() == narg;
	ok &= f.size_par()    == npar;
	ok &= f.size_text()   == ntext;
	ok &= f.size_VecAD()  == nvecad;
	size_t sum = 0;
	sum += nop    * sizeof(CppAD::local::OpCode);
	sum += narg   * sizeof(CPPAD_TAPE_ADDR_TYPE);
	sum += npar   * sizeof(double);
	sum += ntext  * sizeof(char);
	sum += nvecad * sizeof(CPPAD_TAPE_ADDR_TYPE);
	sum += nop    * sizeof(CPPAD_TAPE_ADDR_TYPE) * 3;
	ok &= f.size_op_seq() == sum;

	return ok;
}

// END C++
