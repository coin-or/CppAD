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
# ----------------------------------------------------------------------------
# cmake command line system configuration
# ----------------------------------------------------------------------------
# install_prefix:
# option(<option_variable> "help string describing option" [initial value])
OPTION(install_prefix "cppad install prefix" OFF)
IF ( install_prefix )
	MESSAGE(STATUS "install_prefix = ${install_prefix}")
ELSE ( install_prefix )
	MESSAGE(FATAL_ERROR "install_prefix was not set by cmake command")
ENDIF ( install_prefix )
# set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
SET(CMAKE_INSTALL_PREFIX "${install_prefix}" 
	CACHE PATH "default copied from install_prefix" FORCE
)
#
# install_postfix:
OPTION(install_postfix "cppad install postfix" OFF)
IF ( install_postfix )
	SET(include_dir "include/${install_postfix}"
		CACHE PATH "include files -> ${install_prefix}/${include_dir}" FORCE
	)
ELSE ( install_postfix )
	SET(include_dir "include"
		CACHE PATH "include files -> ${install_prefix}/${include_dir}" FORCE
	)
ENDIF ( install_postfix )
MESSAGE(STATUS "install_postfix = ${install_postfix}")
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
