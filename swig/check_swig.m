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
% d_vector
ok  = 1;
vec = swig_cppad.d_vector(2);
% size
ok  = ok & vec.size() == 2;
% resize
vec.resize(4);
ok  = ok & vec.size() == 4;
% data
a     = vec.data();
for i = 0 : 3
	swig_cppad.double_array_setitem(a, i, 3.0);
	value = swig_cppad.double_array_getitem(a, i);
	ok    = ok & value == 3.0;
endfor
if( ok )
	printf('swig_cppad.d_vector: OK\n')
else
	printf('swig_cppad.d_vector: Error\n')
	error_count = error_count + 1
end
% ---------------------------------------------
% return error_count
exit(error_count)
