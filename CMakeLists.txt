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
# Some cmake language extensions
# ----------------------------------------------------------------------------
# CHECK_CXX_SOURCE_COMPILES(source variable)
# Checks whether the code given in source will compile, link and run and
# return zero status. You can set 
# CMAKE_REQUIRED_LIBRARIES, CMAKE_REQUIRED_FLAGS and CMAKE_REQUIRED_INCLUDES 
# accordingly if additional libraries or compiler flags are required. 
INCLUDE(CheckCXXSourceRuns)
# -----------------------------------------------------------------------------
# Some macros
# ----------------------------------------------------------------------------
# Define a command line argument and set its default value
# also echo it in the cmake status output.
MACRO(command_line_arg variable default type description)
	SET(${variable} "${default}" CACHE $type "${description}")
	MESSAGE(STATUS "${variable} = ${${variable}}")
ENDMACRO( command_line_arg )
#
# Check for package_prefix and if it is specified, set correspodong
# include and link directories.
MACRO(optional_package prefix_variable description) 
	SET(${prefix_variable} NOTFOUND CACHE PATH "${description}")
	SET(prefix ${${prefix_variable}} )
	MESSAGE(STATUS "${prefix_variable} = ${prefix}")
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

# cppad_version is used by set_version.sh to get the version number.
SET(cppad_version      "20121109" )
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
# command line arguments
# ----------------------------------------------------------------------------
#
# cmake_install_datadir
command_line_arg(cmake_install_datadir share STRING 
	"directory, below prefix, where cmake installs data files"
)
#
# cmake_install_includedir
command_line_arg(cmake_install_includedir include STRING 
	"directory, below prefix, where cmake installs include files"
)
#
# cmake_install_libdir
command_line_arg(cmake_install_libdir lib STRING 
	"directory, below prefix, where cmake installs library files"
)
#
# cppad_prefix: 
command_line_arg(cppad_prefix /usr PATH 
	"cppad install prefix"
)
SET(CMAKE_INSTALL_PREFIX "${cppad_prefix}" 
	CACHE PATH "value copied from cppad_prefix" FORCE
)
#
# cppad_postfix:
command_line_arg(cppad_postfix NOTFOUND STRING 
	"cppad install postfix"
)
#
# External package prefixes
#
# adolc_prefix
optional_package(adolc_prefix "adolc install prefix")
#
# eigen_prefix
optional_package(eigen_prefix "eigen install prefix")
#
# fadbad_prefix
optional_package(fadbad_prefix "fadbad install prefix")
#
# ipopt_prefix
optional_package(ipopt_prefix "ipopt install prefix")
#
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
ADD_SUBDIRECTORY(cppad)
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
