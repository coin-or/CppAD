
% This file can be automatically generaeted using the following command
% m4 ../octave.m4 ../xam/a_fun_xam.m4 > a_fun_xam.m
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
% std::vector<a_double>
% -----------------------------------------------------------------------------
function ok = a_fun_xam()
	%
	% load the cppad swig library
	m_cppad
	%
	% initialize return variable
	ok = true;
	n = 2;
	m = 1;
	%
	% create ax
	x = m_cppad.vector_double(n);
	for i = [ 0 :(n -1) ]
		x(i) = i + 1.0;
	ax = m_cppad.independent(x);
	%
	% create af
	ax0 = ax(0);
	ax1 = ax(1);
	ay = m_cppad.vector_ad(1);
	ay(0) = ax0 + ax0 - ax1;
	af = m_cppad.a_fun(ax, ay);
	%
	% zero order forward
	x(0) = 3.0;
	x(1) = 1.0;
	y = af.forward(0, x);
	ok = ok && y(0) == 5.0;
	%
	% first order forward
	x(0) = 0.0;
	x(1) = 1.0;
	y = af.forward(1, x);
	ok = ok && y(0) == -1.0;
	%
	return;
end
