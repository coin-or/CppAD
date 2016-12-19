% load the module
addpath('build')
example
% initialze exit status as OK
error_count = 0;
% ---------------------------------------------
% factorial_by_val
if (example.factorial_by_val(4) == 24)
	printf('example.factorial_by_val: OK\n')
else
	printf('example.factorial_by_val: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% message_of_void
if (example.message_of_void() == 'OK' )
	printf('example.message_of_void: OK\n')
else
	printf('example.message_of_void: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% add_by_ptr: pointer_class(int, int_class)
obj = example.int_class();
example.add_by_ptr(3, 4, obj)
if( obj.value() == 7 )
	printf('example.add_by_ptr: OK\n')
else
	printf('example.add_by_ptr: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% max_array_ptr: array_functions(int, int_array_ptr)
n   = 10;
array_ptr = example.new_int_array_ptr(n);
for i = 0 : (n-1)
	example.int_array_ptr_setitem(array_ptr, i, 2 * i);
endfor
if( example.max_array_by_ptr(n, array_ptr) == 18 )
	printf('example.max_array_by_ptr: pointer:  OK\n')
else
	printf('example.max_array_by_ptr: pointer:  Error\n')
	error_count = error_count + 1;
end
example.delete_int_array_ptr(array_ptr);
% ---------------------------------------------
% max_array_ptr: array_class(int, int_array_class)
n   = 10;
array_obj = example.int_array_class(n);
for i = 0 : (n-1)
	array_obj(i) = 2 * i;
endfor
if( example.max_array_by_ptr(n, array_obj) == 18 )
	printf('example.max_array_by_ptr: class:  OK\n')
else
	printf('example.max_array_by_ptr: class:  Error\n')
	error_count = error_count + 1;
end
# --------------------------------------------
try
	example.raise_exception('test message');
	message = '';
catch
	message = example.raise_exception('');
end_try_catch
if( message == 'test message' )
	printf('example.example.raise_exception: OK\n')
else
	printf('example.raise_exception.message_of_void: Error\n')
	error_count = error_count + 1
end
# --------------------------------------------
two   = example.my_class(2);
three = example.my_class(3);
five  = two + three;
ok       = five == example.my_class(5);
ok       = ok & (4 < five.value() ) & (five.value() < 6);
if( ok )
	printf('example.my_class: OK\n')
else
	printf('example.my_class: Error\n')
	error_count = error_count + 1
end
% ---------------------------------------------
% return error_count
exit(error_count)
