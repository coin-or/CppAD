header_(a_double_xam)
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
c_ a_double
c_ -----------------------------------------------------------------------------
begin_bool_fun_0_(ok, a_double_xam)
	new_var_new_(two,   module_fun_1_(a_double, 2.0))
	new_var_new_(three,  module_fun_1_(a_double, 3.0))
	c_
	new_var_(five,       var_(two) + var_(three))
	new_var_(six,        var_(two) * var_(three))
	new_var_(neg_one,    var_(two) - var_(three))
	new_var_(two_thirds, var_(two) / var_(three))
	c_
	and_assign_(ok, member_fun_0_(five,     value) == 5.0)
	and_assign_(ok, member_fun_0_(six,      value) == 6.0)
	and_assign_(ok, member_fun_0_(neg_one,  value) == -1.0)
	and_assign_(ok, member_fun_0_(neg_one,  value) == -1.0)
	and_assign_(ok, 0.5 < member_fun_0_(two_thirds,  value))
	and_assign_(ok, member_fun_0_(two_thirds,  value) < 1.0)
	and_assign_(ok, five < six)
	c_
	return_(var_(ok))
end_
