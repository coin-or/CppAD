
% This file can be automatically generaeted using the following command
% m4 ../octave.m4 ../xam/a_double_xam.m4 > a_double_xam.m
% -----------------------------------------------------------------------------
% CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
%
% CppAD is distributed under multiple licenses. This distribution is under
% the terms of the
%                     Eclipse Public License Version 1.0.
%
% A copy of this license is included in the COPYING file of this distribution.
% Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
% -----------------------------------------------------------------------------
% a_double
% -----------------------------------------------------------------------------
function ok = a_double_xam()
	%
	% load the cppad swig library
	m_cppad
	%
	% initialize return variable
	ok = true;
	two = m_cppad.a_double(2.0);
	three = m_cppad.a_double(3.0);
	%
	five = two + three;
	six = two * three;
	neg_one = two - three;
	two_thirds = two / three;
	%
	ok = ok && five.value() == 5.0;
	ok = ok && six.value() == 6.0;
	ok = ok && neg_one.value() == -1.0;
	ok = ok && neg_one.value() == -1.0;
	ok = ok && 0.5 < two_thirds.value();
	ok = ok && two_thirds.value() < 1.0;
	ok = ok && five < six;
	%
	return;
end
