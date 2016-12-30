
% This file can be automatically generaeted using the following command
% m4 ../octave.m4 ../xam/vector_ad_xam.m4 > vector_ad_xam.m
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
function ok = vector_ad_xam()
	%
	% load the cppad swig library
	m_cppad
	%
	% initialize return variable
	ok = true;
	n = 4;
	a_vec = m_cppad.vector_ad(n);
	%
	% check size
	ok = ok && a_vec.size() == n;
	%
	% setting elements
	for i = [ 0 :(n -1) ]
		ad = m_cppad.a_double(2.0 * i);
		a_vec(i) = ad;
	end
	% getting elements
	for i = [ 0 :(n -1) ]
		a_element = a_vec(i);
		ok = ok && a_element.value() == 2.0 * i;
	end
	return;
end
