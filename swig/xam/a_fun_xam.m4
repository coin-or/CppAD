header_(a_fun_xam)
c_ -----------------------------------------------------------------------------
c_ CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
c_
c_ CppAD is distributed under multiple licenses. This distribution is under
c_ the terms of the
c_                     Eclipse Public License Version 1.0.
c_
c_ A copy of this license is included in the COPYING file of this distribution.
c_ Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
c_ -----------------------------------------------------------------------------
c_ std::vector<a_double>
c_ -----------------------------------------------------------------------------
begin_bool_fun_0_(ok, a_fun_xam)
	new_var_(n, 2)
	new_var_(m, 1)
	c_
	c_ create ax
	new_var_new_(x, module_fun_1_(vector_double, n))
	begin_for_(i, var_(n) )
		vec_set_(x, var_(i), var_(i) + 1.0)
	new_var_(ax, module_fun_1_(independent, x))
	c_
	c_ create af
	new_var_(ax0, vec_get_(ax, 0))
	new_var_(ax1, vec_get_(ax, 1))
	new_var_new_(ay, module_fun_1_(vector_ad, 1))
	vec_set_(ay, 0, var_(ax0) + var_(ax0) - var_(ax1))
	new_var_new_(af, module_fun_2_(a_fun, var_(ax), var_(ay)))
	c_
	c_ zero order forward
	vec_set_(x, 0, 3.0)
	vec_set_(x, 1, 1.0)
	new_var_(y, member_fun_2_(af, forward, 0, var_(x)))
	and_assign_(ok, vec_get_(y, 0) == 5.0)
	c_
	c_ first order forward
	vec_set_(x, 0, 0.0)
	vec_set_(x, 1, 1.0)
	assign_(y, member_fun_2_(af, forward, 1, var_(x)))
	and_assign_(ok, vec_get_(y, 0) == -1.0)
	c_
	return_(var_(ok))
end_
