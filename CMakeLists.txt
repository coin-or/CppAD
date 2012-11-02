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
# Check for package_prefix and if it is specified, set correspodong
# include and link directories.
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

# set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
SET(cppad_version      "20121026" )
SET(cppad_url          "http://www.coin-or.org/CppAD" )
SET(cppad_description  "Differentiation of C++ Algorithms" )


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

#
# cppad_* settings
#

# cppad_prefix: This is the only required command line argument.
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
SET(cppad_postfix NOTFOUND CACHE STRING "cppad install postfix")
MESSAGE(STATUS "cppad install postfix = ${cppad_postfix}")

#
# cmake_install_* settings
#

# cmake_install_datadir:
SET(cmake_install_datadir NOTFOUND CACHE STRING 
	"dirctory, below prefix, where cmake installs data files"
)
MESSAGE(STATUS "cmake install datadir = ${cmake_install_datadir}")

# cmake_install_includedir:
SET(cmake_install_includedir NOTFOUND CACHE STRING 
	"directory, below prefix, where cmake installs include files"
)
MESSAGE(STATUS "cmake install includedir = ${cmake_install_includedir}")

#
# External package prefixes
#
# adolc_prefix
optional_package(adolc_prefix "adolc install prefix")

# eigen_prefix
optional_package(eigen_prefix "eigen install prefix")

# fadbad_prefix
optional_package(fadbad_prefix "fadbad install prefix")

# ipopt_prefix
optional_package(ipopt_prefix "ipopt install prefix")

# sacado_prefix
optional_package(sacado_prefix "sacado install prefix")

# ----------------------------------------------------------------------------
# automated system configuration
# ----------------------------------------------------------------------------

# cppad_includedir 
IF ( cppad_postfix )
	SET(cppad_includedir 
		"${cppad_prefix}/${cmake_install_includedir}/${cppad_postfix}" 
		CACHE INTERNAL "cppad install include file directory" FORCE
	)
ELSE ( cppad_postfix )
	SET(cppad_includedir 
		"${cppad_prefix}/${cmake_install_includedir}" 
		CACHE PATH "cppad install include file directory" FORCE
	)
ENDIF ( cppad_postfix )

# cppad_datadir 
IF ( cppad_postfix )
	SET(cppad_datadir 
		"${cppad_prefix}/${cmake_install_datadir}/${cppad_postfix}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
ELSE ( cppad_postfix )
	SET(cppad_datadir 
		"${cppad_prefix}/${cmake_install_datadir}" 
		CACHE PATH "cppad install data file directory" FORCE
	)
ENDIF ( cppad_postfix )

# ----------------------------------------------------------------------------
# Currently building tests as normal executables
# ----------------------------------------------------------------------------
# The CMakeLists.txt file in the specified source directory is processed 
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
#
ADD_SUBDIRECTORY(example)
ADD_SUBDIRECTORY(multi_thread)
ADD_SUBDIRECTORY(pkgconfig)
ADD_SUBDIRECTORY(print_for)
ADD_SUBDIRECTORY(test_more)
ADD_SUBDIRECTORY(speed)
IF ( ipopt_prefix )
	ADD_SUBDIRECTORY(cppad_ipopt)
ENDIF( ipopt_prefix )
#
# You can comment out some of the ADD_SUBDIRECTORY comamnds above 
# to speed up testing the other sub-directories. In this case, the script
#	bin/run_cmake.sh
# runs tests that are present, reports which are not present, and fail.
#
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
INSTALL( DIRECTORY cppad DESTINATION ${cppad_includedir} )
