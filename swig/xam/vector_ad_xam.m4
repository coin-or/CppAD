header_(vector_ad_xam)
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
begin_bool_fun_0_(ok, vector_ad_xam)
	new_var_(n, 4)
	new_var_new_(a_vec, module_fun_1_(vector_ad, n))
	c_
	c_ check size
	and_assign_(ok, member_fun_0_(a_vec, size) == n)
	c_
	c_ setting elements
	begin_for_(i, var_(n) )
		new_var_new_(ad, module_fun_1_(a_double, 2.0 * i))
		vec_set_(a_vec, var_(i), var_(ad))
	end_
	c_ getting elements
	begin_for_(i, var_(n) )
		new_var_(a_element, vec_get_(a_vec, var_(i)))
		and_assign_(ok, member_fun_0_(a_element, value) == 2.0 * var_(i))
	end_
	return_(var_(ok))
end_
