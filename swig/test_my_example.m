% load the module
addpath('build')
my_example
% initialze exit status as OK
error_count = 0;
% ---------------------------------------------
% factorial_by_val
if (my_example.factorial_by_val(4) == 24)
	printf('my_example.factorial_by_val: OK\n')
else
	printf('my_example.factorial_by_val: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% message_of_void
if (my_example.message_of_void() == 'OK' )
	printf('my_example.message_of_void: OK\n')
else
	printf('my_example.message_of_void: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% add_by_ptr: pointer_class(int, int_class)
obj = my_example.int_class();
my_example.add_by_ptr(3, 4, obj)
if( obj.value() == 7 )
	printf('my_example.add_by_ptr: OK\n')
else
	printf('my_example.add_by_ptr: Error\n')
	error_count = error_count + 1;
end
% ---------------------------------------------
% max_array_ptr: array_functions(int, int_array_ptr)
n   = 10;
array_ptr = my_example.new_int_array_ptr(n);
for i = 0 : (n-1)
	my_example.int_array_ptr_setitem(array_ptr, i, 2 * i);
endfor
if( my_example.max_array_by_ptr(n, array_ptr) == 18 )
	printf('my_example.max_array_by_ptr: pointer:  OK\n')
else
	printf('my_example.max_array_by_ptr: pointer:  Error\n')
	error_count = error_count + 1;
end
my_example.delete_int_array_ptr(array_ptr);
% ---------------------------------------------
% max_array_ptr: array_class(int, int_array_class)
n   = 10;
array_obj = my_example.int_array_class(n);
for i = 0 : (n-1)
	array_obj(i) = 2 * i;
endfor
if( my_example.max_array_by_ptr(n, array_obj) == 18 )
	printf('my_example.max_array_by_ptr: class:  OK\n')
else
	printf('my_example.max_array_by_ptr: class:  Error\n')
	error_count = error_count + 1;
end
# --------------------------------------------
try
	my_example.raise_exception('test message');
	message = '';
catch
	message = my_example.raise_exception('');
end_try_catch
if( message == 'test message' )
	printf('my_example.my_example.raise_exception: OK\n')
else
	printf('my_example.raise_exception.message_of_void: Error\n')
	error_count = error_count + 1
end
# --------------------------------------------
my_two   = my_example.my_class(2);
my_three = my_example.my_class(3);
my_five  = my_two + my_three;
if( my_five == my_example.my_class(5) )
	printf('my_example.my_class: OK\n')
else
	printf('my_example.my_class: Error\n')
	error_count = error_count + 1
end
% ---------------------------------------------
% return error_count
exit(error_count)
