# $Id$
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
# =============================================================================
# print_variable(variable)
#
# variable: (in)
# The variable name and value is printed
#
MACRO(print_variable variable)
	MESSAGE(STATUS "${variable} = ${${variable}}" )
ENDMACRO(print_variable)
