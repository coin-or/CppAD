# load the modules
import sys
#
sys.path.append( 'build' )
import my_example
#
# initialze exit status as OK
error_count = 0
# --------------------------------------------
# factorial_by_val
if my_example.factorial_by_val(4) == 24 :
	print('my_example.fact: OK')
else :
	print('my_example.factorial_by_val: Error')
	error_count = error_count + 1
# --------------------------------------------
# message_of_void
if my_example.message_of_void() == 'OK' :
	print('my_example.message_of_void: OK')
else :
	print('my_example.message_of_void: Error')
	error_count = error_count + 1
# --------------------------------------------
# add_by_ptr: pointer_class(int, int_class)
obj = my_example.int_class()
my_example.add_by_ptr(3, 4, obj)
if obj.value() == 7 :
	print('my_example.add_by_ptr: OK')
else :
	print('my_example.add_by_ptr: Error')
	error_count = error_count + 1
# --------------------------------------------
# max_array_ptr: array_functions(int, int_array_ptr)
n   = 10
array_ptr = my_example.new_int_array_ptr(n)
for i in range(n) :
	my_example.int_array_ptr_setitem(array_ptr, i, 2 * i)
#
if my_example.max_array_by_ptr(n, array_ptr) == 18 :
	print('my_example.max_array_by_ptr: pointer: OK')
else :
	print('my_example.max_array_by_ptr: pointer: Error')
	error_count = error_count + 1
my_example.delete_int_array_ptr(array_ptr)
# --------------------------------------------
# max_array_ptr: array_class(int, int_array_class)
n   = 10
array_obj = my_example.int_array_class(n)
for i in range(n) :
	array_obj[i] = 2 * i;
#
if my_example.max_array_by_ptr(n, array_obj) == 18 :
	print('my_example.max_array_by_ptr: class: OK')
else :
	print('my_example.max_array_by_ptr: class: Error')
	error_count = error_count + 1
# --------------------------------------------
try :
	my_example.raise_exception('test message');
	message = ''
except :
	message = my_example.raise_exception('');
if message == 'test message' :
	print('my_example.my_example.raise_exception: OK')
else :
	print('my_example.raise_exception.message_of_void: Error')
	error_count = error_count + 1
# --------------------------------------------
my_two   = my_example.my_class(2);
my_three = my_example.my_class(3);
my_five  = my_two + my_three
ok       = my_five == my_example.my_class(5)
ok       = ok and 4 < my_five.value()  and my_five.value() < 6
if ok :
	print('my_example.my_class: OK')
else :
	print('my_example.my_class: Error')
	error_count = error_count + 1
# --------------------------------------------
# return error_count
sys.exit(error_count)
