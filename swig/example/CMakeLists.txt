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
# ----------------------------------------------------------------------------
# use debug build type for initial swig implementation
SET(CMAKE_BUILD_TYPE DEBUG)

# initialize check_swig_example_depends
SET(check_swig_example_depends "")
# ----------------------------------------------------------------------------
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
	#
	FIND_PACKAGE(PerlLibs)
	IF( PERLLIBS_FOUND )
		SET( use_swig TRUE )
	ENDIF( PERLLIBS_FOUND )
ENDIF( SWIG_FOUND )
# ----------------------------------------------------------------------------
IF( use_swig )
	# include files in this source directory
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

	# include the swig specific macros
	INCLUDE( ${CMAKE_ROOT}/Modules/UseSWIG.cmake )

	# treat like a C++ files
	SET_SOURCE_FILES_PROPERTIES(swig_example.i PROPERTIES CPLUSPLUS ON)

	# swig flags when processing this file
	SET_SOURCE_FILES_PROPERTIES( swig_example.i PROPERTIES SWIG_FLAGS "-Wall" )
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
	SWIG_ADD_MODULE(py_example python swig_example.i swig_example.cpp)

	# SWIG_LINK_LIBRARIES(name [ libraries ])
	SWIG_LINK_LIBRARIES(py_example ${PYTHON_LIBRARIES})

	# example use of CMAKE_SWIG_OUTDIR (not necessary in this case)
	SET(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}")

	# copy test program to directory with swig module is located
	ADD_CUSTOM_TARGEt(check_swig_example_py
		"${CMAKE_COMMAND}" -E copy
		"${CMAKE_CURRENT_SOURCE_DIR}/check_swig_example.py"
		"${CMAKE_CURRENT_BINARY_DIR}/check_swig_example.py"
		DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/check_swig_example.py"
	)

	# check_swig_example_python
	ADD_CUSTOM_TARGET(check_swig_example_python
		python check_swig_example.py
		DEPENDS ${SWIG_MODULE_py_example_REAL_NAME} check_swig_example_py
	)
	MESSAGE(STATUS "make check_swig_example_python: available")

	# add to list for all checks
	add_to_list(check_swig_example_depends check_swig_example_python)

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
	SWIG_ADD_MODULE(m_example octave swig_example.i swig_example.cpp)
	#
	# SWIG_LINK_LIBRARIES(name [ libraries ])
	SWIG_LINK_LIBRARIES(m_example
		${octave_liboctinterp} ${octave_liboctave}
	)
	ADD_CUSTOM_TARGET(check_swig_example_m
		${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/check_swig_example.m
		${CMAKE_CURRENT_BINARY_DIR}/check_swig_example.m
		DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/check_swig_example.m
	)
	# check_swig_example_octave
	ADD_CUSTOM_TARGET(check_swig_example_octave
		octave check_swig_example.m
		DEPENDS ${SWIG_MODULE_m_example_REAL_NAME} check_swig_example_m
	)
	MESSAGE(STATUS "make check_swig_example_octave: available")

	# add to list for all checks
	add_to_list(check_swig_example_depends check_swig_example_octave)

ENDIF( SWIG_FOUND AND octave_config )
# ----------------------------------------------------------------------------
# perl
IF( SWIG_FOUND AND PERLLIBS_FOUND )
	#
	# include directory determined by find PythonLibs
	INCLUDE_DIRECTORIES(${PERL_INCLUDE_PATH})
	#
	# SWIG_ADD_MODULE(name language [ files ])
	SWIG_ADD_MODULE(pl_example perl swig_example.i swig_example.cpp)
	#
	# SWIG_LINK_LIBRARIES(name [ libraries ])
	SWIG_LINK_LIBRARIES(pl_example ${PERL_LIBRARY})
	#
	# example use of CMAKE_SWIG_OUTDIR (not necessary in this case)
	SET(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}")
	#
	# copy test program to directory with swig module is located
	ADD_CUSTOM_TARGEt(check_swig_example_pl
		"${CMAKE_COMMAND}" -E copy
		"${CMAKE_CURRENT_SOURCE_DIR}/check_swig_example.pl"
		"${CMAKE_CURRENT_BINARY_DIR}/check_swig_example.pl"
		DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/check_swig_example.pl"
	)
	#
	# check_swig_example_perl
	ADD_CUSTOM_TARGET(check_swig_example_perl
		perl check_swig_example.pl
		DEPENDS ${SWIG_MODULE_pl_example_REAL_NAME} check_swig_example_pl
	)
	MESSAGE(STATUS "make check_swig_example_perl: available")
	#
	# add to list for all checks
	add_to_list(check_swig_example_depends check_swig_example_perl)
	#
ENDIF( SWIG_FOUND AND PERLLIBS_FOUND )
# ----------------------------------------------------------------------------
IF( NOT( "${check_swig_example_depends}" STREQUAL "" ) )
	# Add the check_swig_example target
	ADD_CUSTOM_TARGET(check_swig_example DEPENDS ${check_swig_example_depends})
	MESSAGE(STATUS "make check_swig_example: available")

	# Change check depends in parent environment
	add_to_list(check_swig_depends check_swig_example)
	SET(check_swig_depends "${check_swig_depends}" PARENT_SCOPE)
ENDIF( NOT( "${check_swig_example_depends}" STREQUAL "" ) )
