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
import py_example
#
# initialze error_count
error_count = 0
# --------------------------------------------
# factorial_by_val
if py_example.factorial_by_val(4) == 24 :
	print('py_example.factorial_by_val: OK')
else :
	print('py_example.factorial_by_val: Error')
	error_count = error_count + 1
# --------------------------------------------
# message_of_void
if py_example.message_of_void() == 'OK' :
	print('py_example.message_of_void: OK')
else :
	print('py_example.message_of_void: Error')
	error_count = error_count + 1
# --------------------------------------------
# add_by_ptr: pointer_class(int, int_class)
obj = py_example.int_class()
py_example.add_by_ptr(3, 4, obj)
if obj.value() == 7 :
	print('py_example.add_by_ptr: OK')
else :
	print('py_example.add_by_ptr: Error')
	error_count = error_count + 1
# --------------------------------------------
# max_array_ptr: array_functions(int, int_array_ptr)
n   = 10
array_ptr = py_example.new_int_array_ptr(n)
for i in range(n) :
	py_example.int_array_ptr_setitem(array_ptr, i, 2 * i)
#
if py_example.max_array_by_ptr(n, array_ptr) == 18 :
	print('py_example.max_array_by_ptr: pointer: OK')
else :
	print('py_example.max_array_by_ptr: pointer: Error')
	error_count = error_count + 1
py_example.delete_int_array_ptr(array_ptr)
# --------------------------------------------
# max_array_ptr: array_class(int, int_array_class)
n   = 10
array_obj = py_example.int_array_class(n)
for i in range(n) :
	array_obj[i] = 2 * i
#
if py_example.max_array_by_ptr(n, array_obj) == 18 :
	print('py_example.max_array_by_ptr: class: OK')
else :
	print('py_example.max_array_by_ptr: class: Error')
	error_count = error_count + 1
# --------------------------------------------
# max_std_vector_double: std::vector<double>
n   = 10
vec = py_example.vector_double(n)
for i in range(n) :
	vec[i] = 2.0 * i;
#
if py_example.max_std_vector_double(vec) == 18.0 :
	print('py_example.max_std_vector_double: class: OK')
else :
	print('py_example.max_std_vector_double: class: Error')
	error_count = error_count + 1
# --------------------------------------------
try :
	py_example.raise_exception('test message')
	message = ''
except :
	message = py_example.raise_exception('')
if message == 'test message' :
	print('py_example.py_example.raise_exception: OK')
else :
	print('py_example.raise_exception.message_of_void: Error')
	error_count = error_count + 1
# --------------------------------------------
two   = py_example.normal_class(2)
three = py_example.normal_class(3)
five  = two + three
ok       = five == py_example.normal_class(5)
ok       = ok and 4 < five.value()  and five.value() < 6
if ok :
	print('py_example.normal_class: OK')
else :
	print('py_example.normal_class: Error')
	error_count = error_count + 1
# --------------------------------------------
# double instantiation of a template class
two   = py_example.double_class(2.0)
three = py_example.double_class(3.0)
five  = two + three
ok       = five == py_example.double_class(5.0)
ok       = ok and 4.5 < five.value()  and five.value() < 5.5
if ok :
	print('py_example.double_class: OK')
else :
	print('py_example.double_class: Error')
	error_count = error_count + 1
# --------------------------------------------
# return error_count
sys.exit(error_count)
