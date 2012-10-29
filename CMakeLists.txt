# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Some macros
# ----------------------------------------------------------------------------
MACRO(optional_package prefix_variable description) 
	SET(${prefix_variable} NOTFOUND CACHE PATH "${description}")
	SET(prefix ${${prefix_variable}} )
	MESSAGE(STATUS "${description} = ${prefix}")
	IF ( prefix )
		# List of preprocessor include file search directories
		INCLUDE_DIRECTORIES( ${prefix}/include )
		FOREACH( dir "lib" "lib64" )
			IF ( EXISTS "${prefix}/${dir}" )
				# Paths in which the linker will search for libraries,
				# only applies to targets created after it is called
				LINK_DIRECTORIES( ${prefix}/${dir} )
			ENDIF ( EXISTS "${prefix}/${dir}" )
		ENDFOREACH( dir )
	ENDIF ( prefix )
ENDMACRO(optional_package)
# -----------------------------------------------------------------------------
# Some constants
# ----------------------------------------------------------------------------
# Set the minimum required version of cmake for this project.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Set name of this project and create the variables 
# cppad_BINARY_DIR and cppad_SOURCE_DIR. 
# project(projectname [CXX] [C] [Java])
PROJECT(cppad)

# set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
SET(src_version 20121026 "cppad source code version number")

# include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
INCLUDE_DIRECTORIES( ${cppad_SOURCE_DIR} )
# ----------------------------------------------------------------------------
# cmake command line system configuration
# ----------------------------------------------------------------------------

# cppad_prefix:
# set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
SET(cppad_prefix NOTFOUND CACHE PATH "cppad install prefix" )
MESSAGE(STATUS "cppad install prefix = ${cppad_prefix}")
IF ( NOT cppad_prefix )
	MESSAGE(FATAL_ERROR "cppad_prefix was not set by cmake command")
ENDIF ( NOT cppad_prefix )
SET(CMAKE_INSTALL_PREFIX "${cppad_prefix}" 
	CACHE PATH "value copied from cppad_prefix" FORCE
)

# cppad_postfix:
SET(cppad_postfix NOTFOUND CACHE PATH "cppad install postfix")
MESSAGE(STATUS "cppad install postfix = ${cppad_postfix}")
IF ( cppad_postfix )
	SET(include_dir "include/${cppad_postfix}"
		CACHE INTERNAL "cppad install include file directory" FORCE
	)
ELSE ( cppad_postfix )
	SET(include_dir "include"
		CACHE PATH "include files -> ${cppad_prefix}/${include_dir}" FORCE
	)
ENDIF ( cppad_postfix )

# adolc_prefix
optional_package(adolc_prefix "adolc install prefix")

# eigen_prefix
optional_package(eigen_prefix "eigen install prefix")

# ipopt_prefix
optional_package(ipopt_prefix "ipopt install prefix")

# ----------------------------------------------------------------------------
# automated system configuration
# ----------------------------------------------------------------------------
FIND_PROGRAM(pkg_config pkg-config)
MESSAGE(STATUS "pkg_config = ${pkg_config}")
# ----------------------------------------------------------------------------
# Testing
# enable_testing: Enable testing for current directory and below.
ENABLE_TESTING()
# The CMakeLists.txt file in the specified source directory is processed 
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
ADD_SUBDIRECTORY(example)
ADD_SUBDIRECTORY(print_for)
ADD_SUBDIRECTORY(test_more)
IF ( ipopt_prefix )
	ADD_SUBDIRECTORY(cppad_ipopt/src)
	# ADD_SUBDIRECTORY(cppad_ipopt/example)
	# ADD_SUBDIRECTORY(cppad_ipopt/speed)
	# ADD_SUBDIRECTORY(cppad_ipopt/test)
ENDIF( ipopt_prefix )
# ----------------------------------------------------------------------------
# install procedure
# ----------------------------------------------------------------------------
# During install copy cppad direcrory to CMAKE_INSTALL_PREFIX/include/cppad  
# install(DIRECTORY dirs... DESTINATION <dir>
#	[FILE_PERMISSIONS permissions...]
#	[DIRECTORY_PERMISSIONS permissions...]
#	[USE_SOURCE_PERMISSIONS] [OPTIONAL]
#	[CONFIGURATIONS [Debug|Release|...]]
#	[COMPONENT <component>] [FILES_MATCHING]
#	[[PATTERN <pattern> | REGEX <regex>]
#	[EXCLUDE] [PERMISSIONS permissions...]] [...]
# )
INSTALL( DIRECTORY cppad DESTINATION ${include_dir} )
