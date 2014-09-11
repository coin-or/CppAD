# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# add_cppad_cxx_flags(target_name)
#
# target_name: (in)
# For package equal to "eigen", "fadbad", "scaado", if target_name ends in
# _package, the compiler flags for this target are set to 
#	cppad_cxx_flags_package
# Otherwise, the compiler flags for this target are set to
#	cppad_cxx_flags.
#
# Side Effects:
# The variables package and flags are used for scratch space and not
# defined after the completion of this macro.
#
MACRO(add_cppad_cxx_flags target_name) 
	#
	SET(flags ${cppad_cxx_flags})
	FOREACH(package eigen fadbad sacado)
		IF( ${target_name} MATCHES ".*_${package}$" )
			SET(flags "${cppad_cxx_flags_${package}}")
			MESSAGE(STATUS 
				"${target_name} cxx_flags = ${cppad_cxx_flags_${package}}"
			)
		ENDIF( ${target_name} MATCHES ".*_${package}$" )
		IF( ${target_name} MATCHES ".*_${package}_lib$" )
			SET(flags "${cppad_cxx_flags_${package}}")
			MESSAGE(STATUS 
				"${target_name} cxx_flags = ${cppad_cxx_flags_${package}}"
			)
		ENDIF( ${target_name} MATCHES ".*_${package}_lib$" )
	ENDFOREACH(package)
	IF( flags )
		SET_TARGET_PROPERTIES( 
			${target_name} PROPERTIES COMPILE_FLAGS "${flags}"
		)
	ELSE( flags )
		SET_TARGET_PROPERTIES( 
			${target_name} PROPERTIES COMPILE_FLAGS ""
		)
	ENDIF( flags )
ENDMACRO(add_cppad_cxx_flags) 
