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

# cppad_version is used by set_version.sh to get the version number.
SET(cppad_version      "20121120" )
SET(cppad_url          "http://www.coin-or.org/CppAD" )
SET(cppad_description  "Differentiation of C++ Algorithms" )


# Set name of this project and create the variables 
# cppad_BINARY_DIR and cppad_SOURCE_DIR. 
# project(projectname [CXX] [C] [Java])
PROJECT(cppad)

# include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
INCLUDE_DIRECTORIES( ${cppad_SOURCE_DIR} )
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
# Set the unix_variable to the dos_path but with \ replaced by /
MACRO(dos_path_to_unix dos_constant unix_variable )
	STRING(REGEX REPLACE "[\\]" "/" ${unix_variable} "${dos_constant}" )
ENDMACRO(dos_path_to_unix dos_path unix_path)
#
# Set the the list to include the value at the end. An empty original list
# is denoted by the string value "" (the value may not be empty).
MACRO(add_to_list variable_list constant_value )
	IF( "${${variable_list}}" STREQUAL "" )
		SET( ${variable_list} ${constant_value} )
	ELSE( "${${variable_list}}" STREQUAL "" )
		SET( ${variable_list} ${${variable_list}} ${constant_value} )
	ENDIF( "${${variable_list}}" STREQUAL "" )
ENDMACRO(add_to_list) 
#
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
		FOREACH(dir ${cmake_install_includedirs})
			IF(IS_DIRECTORY ${prefix}/${dir} )
				INCLUDE_DIRECTORIES( ${prefix}/${dir} )
				MESSAGE(STATUS "Found ${prefix}/${dir}")
			ENDIF(IS_DIRECTORY ${prefix}/${dir} )
		ENDFOREACH(dir)
		# Paths in which the linker will search for libraries,
		# only applies to targets created after it is called
		FOREACH(dir ${cmake_install_libdirs})
			IF(IS_DIRECTORY ${prefix}/${dir} )
				LINK_DIRECTORIES( ${prefix}/${dir} )
				MESSAGE(STATUS "Found ${prefix}/${dir}")
			ENDIF(IS_DIRECTORY ${prefix}/${dir} )
		ENDFOREACH(dir)
	ENDIF ( prefix )
ENDMACRO(optional_package)
#
# Add C++ compiler flags for a target, checking for special cases where
# -Wshadow is left out.
MACRO(add_cppad_cxx_flags target_name) 
	SET(no_shadow NO)
	IF( ${target_name} STREQUAL example_eigen )
		SET(no_shadow YES)
	ENDIF( ${target_name} STREQUAL example_eigen )
	IF( ${target_name} STREQUAL test_more_eigen )
		SET(no_shadow YES)
	ENDIF( ${target_name} STREQUAL test_more_eigen )
	IF( ${target_name} STREQUAL speed_fadbad )
		SET(no_shadow YES)
	ENDIF( ${target_name} STREQUAL speed_fadbad )
	IF( no_shadow )
		SET(cxx_flags "${cppad_cxx_flags_skip_Wshadow}")
	ELSE( no_shadow )
		SET(cxx_flags "${cppad_cxx_flags}")
	ENDIF( no_shadow )
	IF( cxx_flags )
		SET_TARGET_PROPERTIES( 
			${target_name} PROPERTIES COMPILE_FLAGS "${cxx_flags}"
		)
	ENDIF( cxx_flags )
ENDMACRO(add_cppad_cxx_flags) 
# ----------------------------------------------------------------------------
# command line arguments
# ----------------------------------------------------------------------------
#
# cmake_install_datadir
command_line_arg(cmake_install_datadir share STRING 
	"directory, below prefix, where cmake installs cppad data files"
)
#
# cmake_install_includedirs
command_line_arg(cmake_install_includedirs include STRING 
	"directories, below prefix, where cmake installs include files"
)
#
# cmake_install_libdirs
command_line_arg(cmake_install_libdirs lib STRING 
	"directories, below prefix, where cmake installs library files"
)
#
# cmake_install_prefix: 
command_line_arg(cmake_install_prefix /usr PATH 
	"cppad install prefix"
)
SET(CMAKE_INSTALL_PREFIX "${cmake_install_prefix}" 
	CACHE PATH "value copied from cmake_install_prefix" FORCE
)
#
# cmake_install_postfix:
command_line_arg(cmake_install_postfix NOTFOUND STRING 
	"cppad install postfix"
)
#
# cppad_documentation:
command_line_arg(cppad_documentation NO BOOL 
	"install cppad documentation"
)
#
# cppad_cxx_flags
command_line_arg(cppad_cxx_flags NOTFOUND STRING 
	"extra flags used with cppad source (besides debug and release flags)"
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
#
# cppad_install_includedir
SET(cppad_install_includedir NOTFOUND)
FOREACH(dir ${cmake_install_includedirs})
	IF( NOT cppad_install_includedir )
		SET(cppad_install_includedir ${dir})
	ENDIF( NOT cppad_install_includedir )
ENDFOREACH(dir ${cmake_install_includedirs})
#
# cppad_install_libdir
SET(cppad_install_libdir NOTFOUND)
FOREACH(dir ${cmake_install_libdirs})
	IF( NOT cppad_install_libdir )
		SET(cppad_install_libdir ${dir})
	ENDIF( NOT cppad_install_libdir )
ENDFOREACH(dir ${cmake_install_libdirs})
#
# boost_prefix:
FIND_PACKAGE(Boost)
IF ( Boost_FOUND )
	# Extract the Boost prefix from Boost_INCLUDE_DIRS
	#
	# convert to using unix directory separator
	dos_path_to_unix("${Boost_INCLUDE_DIRS}" boost_include_dirs)
	#
	# convert to just one directory
	STRING(REGEX REPLACE
		"([^ ]+).*" "\\1"
		boost_include_dir "${boost_include_dirs}"
	)
	#
	# extract part before last backslash
	STRING(REGEX REPLACE 
		"([^ ]*)/[^/ ]*" "\\1" 
		boost_prefix "${boost_include_dir}" 
	) 
	MESSAGE(STATUS "boost_prefix = ${boost_prefix}")
	#
	# add boost include directories
	FOREACH(dir ${cmake_install_includedirs})
		IF( IS_DIRECTORY ${boost_prefix}/${dir} )
			INCLUDE_DIRECTORIES( ${boost_prefix}/${dir} )
			MESSAGE(STATUS "Found ${boost_prefix}/${dir}")
		ENDIF( IS_DIRECTORY ${boost_prefix}/${dir} )
	ENDFOREACH( dir )
	#
	# add boost link directories
	FOREACH(dir ${cmake_install_libdirs})
		IF( IS_DIRECTORY ${boost_prefix}/${dir} )
			LINK_DIRECTORIES( ${boost_prefix}/${dir} )
			MESSAGE(STATUS "Found ${boost_prefix}/${dir}")
		ENDIF( IS_DIRECTORY ${boost_prefix}/${dir} )
	ENDFOREACH( dir )
ENDIF ( Boost_FOUND )
#
# cppad_cxx_flags_skip_Wshadow
STRING(REGEX REPLACE 
	"(.*)-Wshadow(.*)"         "\\1 \\2" 
	cppad_cxx_flags_skip_Wshadow "${cppad_cxx_flags}"
)
MESSAGE(STATUS "cppad_cxx_flags_skip_Wshadow = ${cppad_cxx_flags_skip_Wshadow}")
#
# cppad_includedir, cppad_libdir, cppad_datadir, cppad_docdir
IF ( cmake_install_postfix )
	SET(cppad_includedir 
"${cmake_install_prefix}/${cppad_install_includedir}/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install include file directory" FORCE
	)
	SET(cppad_libdir 
"${cmake_install_prefix}/${cmake_install_libdir}/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install library file directory" FORCE
	)
	SET(cppad_datadir 
"${cmake_install_prefix}/${cmake_install_datadir}/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
	SET(cppad_docdir 
"${cmake_install_prefix}/${cmake_install_datadir}/doc/${cmake_install_postfix}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
ELSE ( cmake_install_postfix )
	SET(cppad_includedir 
		"${cmake_install_prefix}/${cppad_install_includedir}" 
		CACHE INTERNAL "cppad install include file directory" FORCE
	)
	SET(cppad_libdir 
		"${cmake_install_prefix}/${cmake_install_libdir}" 
		CACHE INTERNAL "cppad install library file directory" FORCE
	)
	SET(cppad_datadir 
		"${cmake_install_prefix}/${cmake_install_datadir}" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
	SET(cppad_docdir 
		"${cmake_install_prefix}/${cmake_install_datadir}/doc" 
		CACHE INTERNAL "cppad install data file directory" FORCE
	)
ENDIF ( cmake_install_postfix )
# ----------------------------------------------------------------------------
# Currently building tests as normal executables
# ----------------------------------------------------------------------------
# The CMakeLists.txt file in the specified source directory is processed 
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
#
# Initialize list of tests as empty
SET(list_check "")
#
ADD_SUBDIRECTORY(cppad)
ADD_SUBDIRECTORY(pkgconfig)
#
ADD_SUBDIRECTORY(compare_c)
add_to_list(list_check check_compare_c)
#
ADD_SUBDIRECTORY(example)
add_to_list(list_check check_example)
#
ADD_SUBDIRECTORY(print_for)
add_to_list(list_check check_print_for)
#
ADD_SUBDIRECTORY(test_more)
add_to_list(list_check check_test_more)
#
ADD_SUBDIRECTORY(multi_thread)
add_to_list(list_check check_multi_thread)
#
ADD_SUBDIRECTORY(speed)
add_to_list(list_check check_speed)
#
IF ( ipopt_prefix )
	ADD_SUBDIRECTORY(cppad_ipopt)
	add_to_list(list_check check_ipopt)
ENDIF( ipopt_prefix )
#
ADD_CUSTOM_TARGET(check DEPENDS ${list_check})
#
# ----------------------------------------------------------------------------
# install procedure
# ----------------------------------------------------------------------------
# install(DIRECTORY dirs... DESTINATION <dir>
#	[FILE_PERMISSIONS permissions...]
#	[DIRECTORY_PERMISSIONS permissions...]
#	[USE_SOURCE_PERMISSIONS] [OPTIONAL]
#	[CONFIGURATIONS [Debug|Release|...]]
#	[COMPONENT <component>] [FILES_MATCHING]
#	[[PATTERN <pattern> | REGEX <regex>]
#	[EXCLUDE] [PERMISSIONS permissions...]] [...]
# )
# Note a trailing / in the source directory name drops the source directory
# name during the copy.
#
# During install copy cppad direcrory to ${cppad_includedir}/cppad  
INSTALL( DIRECTORY "cppad/" DESTINATION ${cppad_includedir}/cppad )
#
# During install copy doc direcrory to cppad_docdir/cppad-${cppad_version}  
IF ( cppad_documentation )
	INSTALL( DIRECTORY "doc/" DESTINATION 
		${cppad_docdir}/cppad-${cppad_version} 
	)
ENDIF ( cppad_documentation )
