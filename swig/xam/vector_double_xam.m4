header_(vector_double_xam)
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
c_ std::vector<double>
c_ -----------------------------------------------------------------------------
begin_bool_fun_0_(ok, vector_double_xam)
	new_var_(n, 4)
	new_var_new_(vec, module_fun_1_(vector_double, n))
	c_
	c_ check size
	and_assign_(ok, member_fun_0_(vec, size) == n)
	c_
	c_ setting elements
	begin_for_(i, var_(n) )
		vec_set_(vec, var_(i), 2.0 * var_(i))
	end_
	c_ getting elements
	begin_for_(i, var_(n) )
		new_var_(element, vec_get_(vec, var_(i)))
		and_assign_(ok, var_(element) == 2.0 * var_(i))
	end_
	return_(var_(ok))
end_
