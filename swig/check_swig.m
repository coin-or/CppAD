% $Id
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
% load the module
swig_cppad
%
% initialze exit status as OK
error_count = 0;
% ---------------------------------------------
% std::vector<double>
% ---------------------------------------------
ok  = true;
vec = swig_cppad.vector_double(2);
% size
ok  = ok & vec.size() == 2;
% resize
vec.resize(4);
ok  = ok & vec.size() == 4;
% setting elements
for i = 0 : (vec.size()-1)
	vec(i) = 2.0 * i;
endfor
% getting elements
for i = 0 : (vec.size()-1)
	ok = ok & vec(i) == 2.0 * i;
endfor
if( ok )
	printf('swig_cppad.vector_double: OK\n')
else
	printf('swig_cppad.vector_double: Error\n')
	error_count = error_count + 1
end
% --------------------------------------------
% a_double
% --------------------------------------------
ok    = true;
two   = swig_cppad.a_double(2.0);
three = swig_cppad.a_double(3.0);
five  = two + three;
ok    = ok & 4.5 < five.value() & five.value() < 5.5;
if( ok )
	printf('swig_cppad.a_double: OK\n')
else
	printf('swig_cppad.a_double: Error\n')
	error_count = error_count + 1
end
% ---------------------------------------------
% return error_count
exit(error_count)
