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

# Initialize list of tests as empty
SET(check_swig_depends "")

# files used to build the module for each language
SET(swig_files swig_cppad.i
	a_double.hpp
	a_fun.hpp
	function.hpp
	a_double.cpp
	a_fun.cpp
	function.cpp
)

# subdirectories
ADD_SUBDIRECTORY(example)
# -----------------------------------------------------------------------------
# This directory
#
# check if we are using swig
SET( use_swig FALSE )
FIND_PACKAGE(SWIG)
IF( SWIG_FOUND )
	FIND_PACKAGE(PythonInterp)
	FIND_PACKAGE(PythonLibs)
	IF ( PythonLibs_FOUND AND PythonInterp_FOUND )
		SET( use_swig TRUE )
	ENDIF ( PythonLibs_FOUND AND PythonInterp_FOUND )
	#
	FIND_PROGRAM(octave_config NAMES octave-config)
	IF( octave_config )
		SET( use_swig TRUE )
	ENDIF( octave_config )
ENDIF( SWIG_FOUND )
# ----------------------------------------------------------------------------
IF( use_swig )
	# include the swig specific macros
	INCLUDE( ${CMAKE_ROOT}/Modules/UseSWIG.cmake )

	# SWIG_MODULE_name_REAL_NAME
	# To get the actual name of the swig module

	# CMAKE_SWIG_OUTDIR
	# where to write all the swig generated module (swig -outdir option)

	# a global used to add special flags to all swig calls.
	SET(CMAKE_SWIG_FLAGS "")

	# treat like a C++ files
	SET_SOURCE_FILES_PROPERTIES(swig_cppad.i PROPERTIES CPLUSPLUS ON)

	# use swig flags when processing this file
	SET_SOURCE_FILES_PROPERTIES(
		swig_cppad.i PROPERTIES SWIG_FLAGS ""
	)
ENDIF( use_swig )
# ----------------------------------------------------------------------------
# python
IF ( SWIG_FOUND AND PythonLibs_FOUND AND PythonInterp_FOUND)
	# which version of python did we find
	SET(lib    "${PYTHONLIBS_VERSION_STRING}" )
	SET(interp "${PYTHON_VERSION_STRING}" )
	IF( NOT ( "${lib}" STREQUAL "${interp}" ) )
		MESSAGE(FATAL_ERROR,
			"cmake python library ${lib} not same as interpertor ${interp}"
		)
	ENDIF( NOT ( "${lib}" STREQUAL "${interp}" ) )

	# include directory determined by find PythonLibs
	INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

	# SWIG_ADD_MODULE(name language [ files ])
	SWIG_ADD_MODULE(swig_cppad python ${swig_files})

	# SWIG_LINK_LIBRARIES(name [ libraries ])
	SWIG_LINK_LIBRARIES(swig_cppad ${PYTHON_LIBRARIES})

	# copy test program to directory with swig module is located
	ADD_CUSTOM_TARGET(check_swig_py
		${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/check_swig.py
		${CMAKE_CURRENT_BINARY_DIR}/check_swig.py
		DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/check_swig.py
	)

	# check_swig_python (note underbar in front of swig_cppad)
	ADD_CUSTOM_TARGET(check_swig_python
		python check_swig.py
		DEPENDS _swig_cppad check_swig_py
	)
	MESSAGE(STATUS "make check_swig_python: available")

	# add to list for all checks
	add_to_list(check_swig_depends check_swig_python)

ENDIF ( SWIG_FOUND AND PythonLibs_FOUND AND PythonInterp_FOUND)
# ----------------------------------------------------------------------------
# octave
IF( SWIG_FOUND AND octave_config )
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
	SWIG_ADD_MODULE(swig_cppad octave ${swig_files})
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

	# add to list for all checks
	add_to_list(check_swig_depends check_swig_octave)

ENDIF( SWIG_FOUND AND octave_config )

# -----------------------------------------------------------------------------
IF( NOT( "${check_swig_depends}" STREQUAL "" ) )
	# Add the check_swig target
	ADD_CUSTOM_TARGET(check_swig DEPENDS ${check_swig_depends})
	MESSAGE(STATUS "make check_swig: available")

	# Change check depends in parent environment
	add_to_list(check_depends check_swig)
	SET(check_depends "${check_depends}" PARENT_SCOPE)
ENDIF( NOT( "${check_swig_depends}" STREQUAL "" ) )
