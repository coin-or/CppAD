# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# sources_libs_define(prefix_name sources libs define)
#
# prefix_name: (in)
# If the variable ${prefix_name}_prefix is NOTFOUND,
# sources and libs are set to the empty string "" and no definition is added.
#
# Otherwise ${prefix_name}_sources is set to ${sources},
# ${prefix_name}_libs is set to ${libs}. If ${define} is not empty "",
# the defintion -DCPPAD_${define} is added; i.e.,
#	ADD_DEFINITIONS("-DCPPAD_${define}") 
#
MACRO(sources_libs_define prefix_name soruces libs define)
	IF ( ${prefix_name}_prefix )
		SET( ${prefix_name}_sources ${sources} )
		SET( ${prefix_name}_libs    ${libs}    )
		IF( NOT "${define}" STREQUAL "" )
			ADD_DEFINITIONS("-DCPPAD_${define}")
			MESSAGE(STATUS "-DCPPAD_${define}" )
		ENDIF( NOT "${define}" STREQUAL "" )
	ELSE ( ${prefix_name}_prefix )
		SET( ${prefix_name}_sources "" )
		SET( ${prefix_name}_libs    "" )
	ENDIF ( ${prefix_name}_prefix )
ENDMACRO( sources_libs_define )
