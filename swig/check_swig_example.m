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
swig_example
%
% initialze exit status as OK
error_count = 0;
% ---------------------------------------------
% factorial_by_val
if (swig_example.factorial_by_val(4) == 24)
	printf('swig_example.factorial_by_val: OK\n')
else
	printf('swig_example.factorial_by_val: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% message_of_void
if (swig_example.message_of_void() == 'OK' )
	printf('swig_example.message_of_void: OK\n')
else
	printf('swig_example.message_of_void: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% add_by_ptr: pointer_class(int, int_class)
obj = swig_example.int_class();
swig_example.add_by_ptr(3, 4, obj)
if( obj.value() == 7 )
	printf('swig_example.add_by_ptr: OK\n')
else
	printf('swig_example.add_by_ptr: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% max_array_ptr: array_functions(int, int_array_ptr)
n   = 10;
array_ptr = swig_example.new_int_array_ptr(n);
for i = 0 : (n-1)
	swig_example.int_array_ptr_setitem(array_ptr, i, 2 * i);
endfor
if( swig_example.max_array_by_ptr(n, array_ptr) == 18 )
	printf('swig_example.max_array_by_ptr: pointer:  OK\n')
else
	printf('swig_example.max_array_by_ptr: pointer:  Error\n')
	error_count = error_count + 1;
end
swig_example.delete_int_array_ptr(array_ptr);
% ---------------------------------------------
% max_array_ptr: array_class(int, int_array_class)
n   = 10;
array_obj = swig_example.int_array_class(n);
for i = 0 : (n-1)
	array_obj(i) = 2 * i;
endfor
if( swig_example.max_array_by_ptr(n, array_obj) == 18 )
	printf('swig_example.max_array_by_ptr: class:  OK\n')
else
	printf('swig_example.max_array_by_ptr: class:  Error\n')
	error_count = error_count + 1;
end
# --------------------------------------------
try
	swig_example.raise_exception('test message');
	message = '';
catch
	message = swig_example.raise_exception('');
end_try_catch
if( message == 'test message' )
	printf('swig_example.swig_example.raise_exception: OK\n')
else
	printf('swig_example.raise_exception.message_of_void: Error\n')
	error_count = error_count + 1
end
# --------------------------------------------
two   = swig_example.normal_class(2);
three = swig_example.normal_class(3);
five  = two + three;
ok       = five == swig_example.normal_class(5);
ok       = ok & (4 < five.value() ) & (five.value() < 6);
if( ok )
	printf('swig_example.normal_class: OK\n')
else
	printf('swig_example.normal_class: Error\n')
	error_count = error_count + 1
end
# --------------------------------------------
two   = swig_example.double_class(2.0);
three = swig_example.double_class(3.0);
five  = two + three;
ok       = five == swig_example.double_class(5.0);
ok       = ok & (4.5 < five.value() ) & (five.value() < 5.5);
if( ok )
	printf('swig_example.double_class: OK\n')
else
	printf('swig_example.double_class: Error\n')
	error_count = error_count + 1
end
% ---------------------------------------------
% return error_count
exit(error_count)
