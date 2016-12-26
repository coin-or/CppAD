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
# used debug during initial development of swig interface
SET(CMAKE_BUILD_TYPE DEBUG)

FIND_PACKAGE(SWIG)
IF( SWIG_FOUND )
	FIND_PROGRAM(octave_config NAMES octave-config)
ENDIF( SWIG_FOUND )
IF( SWIG_FOUND AND octave_config )

	# include the swig specific macros
	INCLUDE( ${CMAKE_ROOT}/Modules/UseSWIG.cmake )

	# SWIG_MODULE_name_REAL_NAME
	# To get the actual name of the swig module

	# CMAKE_SWIG_OUTDIR
	# where to write all the swig generated module (swig -outdir option)

	# treat like a C++ files
	SET_SOURCE_FILES_PROPERTIES(
		${swig_control_file} PROPERTIES CPLUSPLUS ON
	)

	# swig flags when processing this file
	SET_SOURCE_FILES_PROPERTIES(
		${swig_control_file} PROPERTIES SWIG_FLAGS "-Wall"
	)
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
	#
	# octave library dcirectories
	EXECUTE_PROCESS(
		COMMAND ${octave_config} -p OCTLIBDIR
		OUTPUT_VARIABLE  octave_library_dirs
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	#
	# liboctinterp
	FIND_LIBRARY(octave_liboctinterp
		NAMES octinterp liboctinterp
		PATHS "${octave_library_dirs}"
	)
	#
	# liboctave
	FIND_LIBRARY(octave_liboctave
		NAMES octave liboctave
		PATHS "${octave_library_dirs}"
	)
	#
	# include directory determined by find octive-config
	INCLUDE_DIRECTORIES(${octave_include_dirs})
	#
	# SWIG_ADD_MODULE(name language [ files ])
	SWIG_ADD_MODULE(swig_cppad octave ${swig_module_files} )

	# use current binary directory for swig output files
	SET(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}")
	#
	# SWIG_LINK_LIBRARIES(name [ libraries ])
	SWIG_LINK_LIBRARIES(swig_cppad
		${octave_liboctinterp} ${octave_liboctave}
	)
	#
	# copy test program to directory with swig module is located
	ADD_CUSTOM_TARGET(check_swig_m
		${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/check_swig.m
		${CMAKE_CURRENT_BINARY_DIR}/check_swig.m
		DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/check_swig.m
	)
	# check_swig_octave (no underbar in front of swig_cppad)
	ADD_CUSTOM_TARGET(check_swig_octave
		octave check_swig.m
		DEPENDS swig_cppad check_swig_m
	)
	MESSAGE(STATUS "make check_swig_octave: available")

	# Change check depends in parent environment
	add_to_list(check_swig_depends check_swig_octave)
	SET(check_swig_depends "${check_swig_depends}" PARENT_SCOPE)

ENDIF( SWIG_FOUND AND octave_config )
