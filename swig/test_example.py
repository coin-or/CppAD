# $Id
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# load the modules
import sys
#
sys.path.append( 'build' )
import example
#
# initialze exit status as OK
error_count = 0
# --------------------------------------------
# factorial_by_val
if example.factorial_by_val(4) == 24 :
	print('example.fact: OK')
else :
	print('example.factorial_by_val: Error')
	error_count = error_count + 1
# --------------------------------------------
# message_of_void
if example.message_of_void() == 'OK' :
	print('example.message_of_void: OK')
else :
	print('example.message_of_void: Error')
	error_count = error_count + 1
# --------------------------------------------
# add_by_ptr: pointer_class(int, int_class)
obj = example.int_class()
example.add_by_ptr(3, 4, obj)
if obj.value() == 7 :
	print('example.add_by_ptr: OK')
else :
	print('example.add_by_ptr: Error')
	error_count = error_count + 1
# --------------------------------------------
# max_array_ptr: array_functions(int, int_array_ptr)
n   = 10
array_ptr = example.new_int_array_ptr(n)
for i in range(n) :
	example.int_array_ptr_setitem(array_ptr, i, 2 * i)
#
if example.max_array_by_ptr(n, array_ptr) == 18 :
	print('example.max_array_by_ptr: pointer: OK')
else :
	print('example.max_array_by_ptr: pointer: Error')
	error_count = error_count + 1
example.delete_int_array_ptr(array_ptr)
# --------------------------------------------
# max_array_ptr: array_class(int, int_array_class)
n   = 10
array_obj = example.int_array_class(n)
for i in range(n) :
	array_obj[i] = 2 * i;
#
if example.max_array_by_ptr(n, array_obj) == 18 :
	print('example.max_array_by_ptr: class: OK')
else :
	print('example.max_array_by_ptr: class: Error')
	error_count = error_count + 1
# --------------------------------------------
try :
	example.raise_exception('test message');
	message = ''
except :
	message = example.raise_exception('');
if message == 'test message' :
	print('example.example.raise_exception: OK')
else :
	print('example.raise_exception.message_of_void: Error')
	error_count = error_count + 1
# --------------------------------------------
two   = example.my_class(2);
three = example.my_class(3);
five  = two + three
ok       = five == example.my_class(5)
ok       = ok and 4 < five.value()  and five.value() < 6
if ok :
	print('example.my_class: OK')
else :
	print('example.my_class: Error')
	error_count = error_count + 1
# --------------------------------------------
# return error_count
sys.exit(error_count)
