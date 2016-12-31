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
# used debug during initial development of swig interface
SET(CMAKE_BUILD_TYPE DEBUG)

assert( SWIG_FOUND )
FIND_PROGRAM(octave_config NAMES octave-config)
IF( octave_config )
	# create check_all.m for this directory
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/check_all.m.in
		${CMAKE_CURRENT_BINARY_DIR}/check_all.m
	)

	# treat like a C++ files
	SET_SOURCE_FILES_PROPERTIES(
		${swig_control_file} PROPERTIES CPLUSPLUS ON
	)

	# swig -octave warns that it does not support +=, -=, *=, or /=,
	# (but is does and the code works) so supress these warnings.
	SET(CMAKE_SWIG_FLAGS -Wextra -w365,366,367,368 )

	# octave include directories
	EXECUTE_PROCESS(
		COMMAND ${octave_config} -p OCTINCLUDEDIR
		OUTPUT_VARIABLE  octave_include_dirs
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	# unix style paths
	STRING(REGEX REPLACE
		"/octave$" "" octave_include_dirs ${octave_include_dirs}
	)
	# windows styple paths
	STRING(REGEX REPLACE
		"\\octave$" "" octave_include_dirs ${octave_include_dirs}
	)

	# octave library dcirectories
	EXECUTE_PROCESS(
		COMMAND ${octave_config} -p OCTLIBDIR
		OUTPUT_VARIABLE  octave_library_dirs
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# liboctinterp
	FIND_LIBRARY(octave_liboctinterp
		NAMES octinterp liboctinterp
		PATHS "${octave_library_dirs}"
	)

	# liboctave
	FIND_LIBRARY(octave_liboctave
		NAMES octave liboctave
		PATHS "${octave_library_dirs}"
	)

	# include directory determined by find octive-config
	INCLUDE_DIRECTORIES(${octave_include_dirs})

	# SWIG_ADD_MODULE(name language [ files ])
	SWIG_ADD_MODULE(m_cppad octave ${swig_control_file} )

	# SWIG_LINK_LIBRARIES(name [ libraries ])
	SWIG_LINK_LIBRARIES(m_cppad
		cppad_swig_lib ${octave_liboctinterp} ${octave_liboctave}
	)

	# check_swig_octave
	ADD_CUSTOM_TARGET(check_swig_octave
		octave check_all.m
		DEPENDS
		${SWIG_MODULE_m_cppad_REAL_NAME}
		${CMAKE_CURRENT_BINARY_DIR}/check_all.m
	)
	MESSAGE(STATUS "make check_swig_octave: available")

	# Change check depends in parent environment
	add_to_list(check_swig_depends check_swig_octave)
	SET(check_swig_depends "${check_swig_depends}" PARENT_SCOPE)

ENDIF( octave_config )
