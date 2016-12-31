divert(-1)
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

# some simple constants
define(ext_, py)
define(module_, py_cppad)
define(true_, True)
define(false_, Flase)
define(and_, and)
define(end_, `#')
define(c_, `#')

# header_(language)
define(header_,
c_ This file can be automatically generaeted using the following command
`#' m4 ../python.m4 ../xam/$1.m4 > $1.ext_)

# begin_bool_fun_0_(return_variable, fun_name)
define(begin_bool_fun_0_,
def $2() :
	c_
	c_ load the cppad swig library
	import module_
	c_
	c_ initialize return variable
	$1 = true_)

# module_fun_1_(fun_name, argument)
define(module_fun_1_, module_.$1($2))

# module_fun_2_(fun_name, argument1, argument2)
define(module_fun_2_, module_.$1($2, $3))

# var_(variable)
define(var_, $1)

# new_var_(variable, value)
define(new_var_, $1 = $2)

# new_var_new_(variable, value)
define(new_var_new_, $1 = $2)

# assign_(variable, value)
define(assign_, $1 = $2)

# and_assign_(variable, value)
define(and_assign_, $1 = $1 and_ $2)

# member_fun_0_(variable, member_fun)
define(member_fun_0_, $1.$2())

# member_fun_1_(variable, member_fun, argument)
define(member_fun_1_, $1.$2($3))

# member_fun_2_(variable, member_fun, argument1, argument2)
define(member_fun_2_, $1.$2($3, $4))

# vec_set_(vector, index, value)
define(vec_set_, $1[$2] = $3)

# vec_get_(vector, index)
define(vec_get_, $1[$2])

# begin_for_(variable, upper)
define(begin_for_, for $1 in range( $2 ) :)

# print_text_(text)
define(print_text_, print('$1'))

# return_(expression)
define(return_, return( $1 ))

divert
