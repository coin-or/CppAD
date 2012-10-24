/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_print_optimize$$
$spell
	cppad
	CppAD
	bool
	std::cout
	const
	var
$$

$section Speed Test Utility: Optimize Operation Sequences and Print Results$$
$index print_optimize, cppad speed test$$
$index optimize, print cppad speed test$$

$head Syntax$$
$codei%print_optimize(%f%, %print%, %label%, %test_id%)%$$

$head Purpose$$
This routine $cref/optimizes/optimize/$$ the operation sequences 
and prints the results of the optimization.
It is used by the CppAD speed tests when the 
$cref/optimize/speed_main/option_list/optimize/$$ option is true.
This helps when improving optimization of operation sequences.

$head f$$
The argument $icode f$$ has prototype
$codei%
	ADFun<double>& %f%
%$$
It contains the operation sequences that is 
$cref/optimized/optimize/$$.

$head print$$
The argument $icode print$$ has prototype
$codei%
	bool %print%
%$$
If it is true, the following values are printed on $code std::cout$$:
$codei%
	%label% = [ %test_id%, %size_before%, %size_after% ]
%$$
Where $icode size_before$$ and $icode size_after$$ are the value
corresponding to $icode%f%.size_var()%$$ before and after the optimization.

$head label$$
The argument $icode label$$ has prototype
$codei%
	const char* %label%
%$$
It is the label that is used when printing the optimization results.

$head test_id$$
The argument $icode test_id$$ has prototype
$codei%
	size_t %test_id%
%$$
and is an identifier for this speed test.

$end
*/
# include <cppad/cppad.hpp>

void print_optimize(
	CppAD::ADFun<double>& f  ,
	bool           print     ,
	const char*    label     ,
	size_t         test_id   )
{	size_t before = f.size_var();
	f.optimize();
	size_t after = f.size_var();
	if( print )
	{	std::cout << label        << " = [ ";
		std::cout << int(test_id) << ", ";
		std::cout << int(before)  << ", ";
		std::cout << int(after)   << " ]";
		std::cout << std::endl;
	}
}
