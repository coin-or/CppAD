# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# =============================================================================
# join(variable sep item_1 ... item_n )
#
# variable: (out)
# The variable that well be set to the value
# ${item_1}${sep}$ ...${sep}${item_n} .
#
# sep: (in)
# Is the text placed between items. For example,
# it could be a space or a comma.
#
# item_j: (in)
# is the j-th item placed in the variable.
#
# convert semicolons to spaces
MACRO(join)
	SET(argv ${ARGV})
	LIST(GET argv 0 var)
	LIST(GET argv 1 sep)
	LIST(REMOVE_AT argv 0 1)
	SET(result "")
	FOREACH(entry ${argv} )
		IF(result STREQUAL "" )
			SET(result "${entry}")
		ELSE(result STREQUAL "" )
			SET(result "${result}${sep}${entry}")
		ENDIF(result STREQUAL "" )
	ENDFOREACH(entry ${argv} )
ENDMACRO(join)
