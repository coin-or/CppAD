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
m_example
%
% initialze error count
error_count = 0;
% ---------------------------------------------
% factorial_by_val
if (m_example.factorial_by_val(4) == 24)
	printf('m_example.factorial_by_val: OK\n')
else
	printf('m_example.factorial_by_val: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% message_of_void
if (m_example.message_of_void() == 'OK' )
	printf('m_example.message_of_void: OK\n')
else
	printf('m_example.message_of_void: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% add_by_ptr: pointer_class(int, int_class)
obj = m_example.int_class();
m_example.add_by_ptr(3, 4, obj)
if( obj.value() == 7 )
	printf('m_example.add_by_ptr: OK\n')
else
	printf('m_example.add_by_ptr: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% max_array_ptr: array_functions(int, int_array_ptr)
n   = 10;
array_ptr = m_example.new_int_array_ptr(n);
for i = 0 : (n-1)
	m_example.int_array_ptr_setitem(array_ptr, i, 2 * i);
endfor
if( m_example.max_array_by_ptr(n, array_ptr) == 18 )
	printf('m_example.max_array_by_ptr: pointer:  OK\n')
else
	printf('m_example.max_array_by_ptr: pointer:  Error\n')
	error_count = error_count + 1;
end
m_example.delete_int_array_ptr(array_ptr);
% ---------------------------------------------
% max_array_ptr: array_class(int, int_array_class)
n   = 10;
array_obj = m_example.int_array_class(n);
for i = 0 : (n-1)
	array_obj(i) = 2 * i;
endfor
if( m_example.max_array_by_ptr(n, array_obj) == 18 )
	printf('m_example.max_array_by_ptr: class:  OK\n')
else
	printf('m_example.max_array_by_ptr: class:  Error\n')
	error_count = error_count + 1;
end
# --------------------------------------------
try
	m_example.raise_exception('test message');
	message = '';
catch
	message = m_example.raise_exception('');
end_try_catch
if( message == 'test message' )
	printf('m_example.m_example.raise_exception: OK\n')
else
	printf('m_example.raise_exception.message_of_void: Error\n')
	error_count = error_count + 1
end
# --------------------------------------------
two   = m_example.normal_class(2);
three = m_example.normal_class(3);
five  = two + three;
ok       = five == m_example.normal_class(5);
ok       = ok & (4 < five.value() ) & (five.value() < 6);
if( ok )
	printf('m_example.normal_class: OK\n')
else
	printf('m_example.normal_class: Error\n')
	error_count = error_count + 1
end
# --------------------------------------------
two   = m_example.double_class(2.0);
three = m_example.double_class(3.0);
five  = two + three;
ok       = five == m_example.double_class(5.0);
ok       = ok & (4.5 < five.value() ) & (five.value() < 5.5);
if( ok )
	printf('m_example.double_class: OK\n')
else
	printf('m_example.double_class: Error\n')
	error_count = error_count + 1
end
% ---------------------------------------------
% return error_count
exit(error_count)
