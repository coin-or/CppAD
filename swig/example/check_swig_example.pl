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
use pl_example;
#
# initialze error_count
$error_count = 0;
# --------------------------------------------
# factorial_by_val
if( pl_example::factorial_by_val(4) == 24 )
{	print "pl_example::factorial_by_val: OK\n"; }
else
{	print "pl_example::factorial_by_val: Error\n";
	$error_count = $error_count + 1;
}
# --------------------------------------------
# message_of_void
if( pl_example::message_of_void() == "OK" )
{	print "pl_example::message_of_void: OK\n"; }
else
{	print "pl_example::message_of_void: Error\n";
	$error_count = $error_count + 1;
}
# --------------------------------------------
# add_by_ptr: pointer_class(int, int_class)
$obj = new pl_example::int_class();
pl_example::add_by_ptr(3, 4, $obj);
if( $obj->value() == 7 )
{	print "pl_example::add_by_ptr: OK\n"; }
else
{	print "pl_example::add_by_ptr: Error\n";
	$error_count = $error_count + 1;
}
# --------------------------------------------
# max_array_ptr: array_functions(int, int_array_ptr)
$n = 10;
$array_ptr = pl_example::new_int_array_ptr($n);
for($i = 0; $i < $n; $i++)
{	pl_example::int_array_ptr_setitem($array_ptr, $i, 2 * $i); }
#
if( pl_example::max_array_by_ptr($n, $array_ptr) == 18 )
{	print "pl_example::max_array_by_ptr: pointer: OK\n"; }
else
{	print "pl_example::max_array_by_ptr: pointer: Error\n";
	$error_count = $error_count + 1;
}
pl_example::delete_int_array_ptr($array_ptr);
# --------------------------------------------
# max_array_ptr: array_class(int, int_array_class)
$n         = 10;
$array_obj = new pl_example::int_array_class($n);
for($i = 0; $i < $n; $i++)
{	$array_obj[$i] = 2 * i; }
if( pl_example::max_array_by_ptr($n, $array_obj) == 18 )
{	print "pl_example::max_array_by_ptr: class: OK\n"; }
else
{	print "pl_example::max_array_by_ptr: class: Error\n";
	$error_count = $error_count + 1;
}
# ------------------------------------------------
# max_std_vector_double: std::vector<double>
$n   = 10;
$vec = new pl_example::vector_double($n);
for($i = 0; $i < $n; $i++)
{	$vec->set($i, 2.0 * $i); }
#
if( pl_example::max_std_vector_double($vec) == 18.0 )
{	print "pl_example::max_std_vector_double: class: OK\n"; }
else
{	print "pl_example::max_std_vector_double: class: Error\n";
	$error_count = $error_count + 1;
}
# --------------------------------------------
$message = "";
eval { pl_example::raise_exception("test message\n"); };
if( $@ )
{	$message = pl_example::raise_exception(""); }
if( $message == "test message" )
{	print "pl_example::pl_example::raise_exception: OK\n"; }
else
{	print "pl_example::raise_exception.message_of_void: Error\n";
	$error_count = $error_count + 1;
}
# --------------------------------------------
$two   = new pl_example::normal_class(2);
$three = new pl_example::normal_class(3);
$five  = $two + $three;
$check = new pl_example::normal_class(5);
$ok    = $five == $check;
$ok    = $ok and 4 < $five->value()  and $five->value() < 6;
if( ok )
{	print "pl_example::normal_class: OK\n"; }
else
{	print "pl_example::normal_class: Error\n";
	$error_count = $error_count + 1;
}
# --------------------------------------------
# double instantiation of a template class
$two   = new pl_example::double_class(2.0);
$three = new pl_example::double_class(3.0);
$five  = $two + $three;
$check = new pl_example::double_class(5.0);
$ok    = $five == $check;
$ok    = $ok and 4.5 < $five->value()  and $five->value() < 5.5;
if( ok )
{	print "pl_example::double_class: OK\n"; }
else
{	print "pl_example::double_class: Error\n";
	$error_count = $error_count + 1;
}
# --------------------------------------------
# return error_count
if( $error_count == 0 )
{	exit 0; }
exit 1;
