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
# assert(variable)
#
# variable: (in)
# The variable is checked to make sure it is true, and if it is not true
# a fatal error message is printed.
#
MACRO(assert variable)
     IF( NOT ${variable} )
		MESSAGE(FATAL_ERROR
			"Error: ${variable} is false in ${CMAKE_CURRENT_LIST_FILE}"
		)
     ENDIF( NOT ${variable} )
ENDMACRO(assert)
