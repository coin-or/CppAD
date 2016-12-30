
% This file can be automatically generaeted using the following command
% m4 ../octave.m4 ../xam/vector_double_xam.m4 > vector_double_xam.m
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
% std::vector<double>
% -----------------------------------------------------------------------------
function ok = vector_double_xam()
	%
	% load the cppad swig library
	m_cppad
	%
	% initialize return variable
	ok = true;
	n = 4;
	vec = m_cppad.vector_double(n);
	%
	% check size
	ok = ok && vec.size() == n;
	%
	% setting elements
	for i = [ 0 :(n -1) ]
		vec(i) = 2.0 * i;
	end
	% getting elements
	for i = [ 0 :(n -1) ]
		element = vec(i);
		ok = ok && element == 2.0 * i;
	end
	return;
end
