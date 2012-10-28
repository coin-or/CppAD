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
# Force user to choose an install prefix
# option(<option_variable> "help string describing option" [initial value])
OPTION(install_prefix "cppad install prefix" "not_set")
IF ( install_prefix EQUAL "not_set" )
	MESSAGE(FATAL "'install_prefix' was not set by cmake command")
ELSE ( install_prefix EQUAL "not_set" )
	MESSAGE(STATUS "install_prefix = ${install_prefix}")
ENDIF ( install_prefix EQUAL "not_set" )
# set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
SET(CMAKE_INSTALL_PREFIX "${install_prefix}" 
	CACHE FILEPATH "default copied from install_prefix" FORCE
)
# ----------------------------------------------------------------------------
# automated system configuration
# ----------------------------------------------------------------------------
FIND_PROGRAM(pkg_config pkg-config)
MESSAGE(STATUS "pkg_config = ${pkg_config}")
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
INSTALL( DIRECTORY cppad DESTINATION include )
