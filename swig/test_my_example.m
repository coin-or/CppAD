% load the module
addpath('build')
my_example
% initialze exit status as OK
error_count = 0;
% ---------------------------------------------
% integer return value, integer arguments
if (my_example.my_fact(4) == 24)
	printf("my_example.my_fact: OK\n")
else
	printf("my_example.my_fact: Error\n")
	error_count = error_count + 1;
end
% ---------------------------------------------
% c string return value
if (my_example.my_message() == "OK" )
	printf("my_example.my_message: OK\n")
else
	printf("my_example.my_message: Error\n")
	error_count = error_count + 1;
end
% ---------------------------------------------
% integer pointer argument to one integer value
obj = my_example.int_class();
my_example.my_add(3, 4, obj)
if( obj.value() == 7 )
	printf("my_example.my_add: OK\n")
else
	printf("my_example.my_add: Error\n")
	error_count = error_count + 1;
end
% ---------------------------------------------
% integer pointer argument to array of integer values
n   = 10;
array_ptr = my_example.new_int_array_ptr(n);
for i = 0 : (n-1)
	my_example.int_array_ptr_setitem(array_ptr, i, 2 * i);
endfor
if( my_example.my_max(n, array_ptr) == 18 )
	printf("my_example.my_max: OK\n")
else
	printf("my_example.my_max: Error\n")
	error_count = error_count + 1;
end
my_example.delete_int_array_ptr(array_ptr);
% ---------------------------------------------
% return error_count
exit(error_count)
