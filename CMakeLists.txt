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
# option(<option_variable> "help string describing option" [initial value])
OPTION(cppad_prefix "cppad install prefix" OFF)
IF ( cppad_prefix )
	MESSAGE(STATUS "cppad_prefix = ${cppad_prefix}")
ELSE ( cppad_prefix )
	MESSAGE(FATAL_ERROR "cppad_prefix was not set by cmake command")
ENDIF ( cppad_prefix )
# set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
SET(CMAKE_INSTALL_PREFIX "${cppad_prefix}" 
	CACHE PATH "default copied from cppad_prefix" FORCE
)

# cppad_postfix:
OPTION(cppad_postfix "cppad install postfix" OFF)
IF ( cppad_postfix )
	SET(include_dir "include/${cppad_postfix}"
		CACHE PATH "include files -> ${cppad_prefix}/${include_dir}" FORCE
	)
ELSE ( cppad_postfix )
	SET(include_dir "include"
		CACHE PATH "include files -> ${cppad_prefix}/${include_dir}" FORCE
	)
ENDIF ( cppad_postfix )
MESSAGE(STATUS "cppad_postfix = ${cppad_postfix}")

# adolc_prefix
OPTION(adolc_prefix "adolc install prefix" OFF)
MESSAGE(STATUS "adolc_prefix = ${adolc_prefix}")

# adolc include and link directories
IF ( adolc_prefix )
	# List of preprocessor include file search directories
	INCLUDE_DIRECTORIES( ${adolc_prefix}/include )
	FOREACH( dir "lib" "lib64" )
		IF ( EXISTS "${adolc_prefix}/${dir}" )
			# Paths in which the linker will search for libraries,
			# only applies to targets created after it is called
			LINK_DIRECTORIES( ${adolc_prefix}/${dir} )
		ENDIF ( EXISTS "${adolc_prefix}/${dir}" )
	ENDFOREACH( dir )
ENDIF ( adolc_prefix )

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
