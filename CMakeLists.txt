# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# =============================================================================
# Some constants
# =============================================================================
#
# Set the minimum required version of cmake for this project.
# see http://www.cmake.org/pipermail/cmake/2013-January/053213.html
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
#
# Only interpret if() arguments as variables or keywords when unquoted.
IF( POLICY CMP0054 )
  CMAKE_POLICY(SET CMP0054 NEW)
ENDIF( POLICY CMP0054 )
#
# cppad_version is used by version.sh to get the version number.
SET(cppad_version "20240000.5")
SET(cppad_url          "https://coin-or.github.io/CppAD" )
SET(cppad_description  "Differentiation of C++ Algorithms" )
IF( NOT DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "NOTFOUND")
ENDIF( NOT DEFINED CMAKE_BUILD_TYPE)
#
# Set name of this project and create the variables
# cppad_BINARY_DIR and cppad_SOURCE_DIR.
# project(projectname [CXX] [C] [Java])
PROJECT(cppad)
#
# Add the include sub-directory to the list of C++ preprocessor
# include directories for the entire project.  The SYSTEM flag is not included
# so warnings will be geenreated for this directory.
INCLUDE_DIRECTORIES( ${cppad_SOURCE_DIR}/include )
#
# =============================================================================
# Some system cmake language extensions
# =============================================================================
# CHECK_CXX_SOURCE_COMPILES(source variable)
# Checks whether the code given in source will compile, link and run and
# return zero status. You can set
# CMAKE_REQUIRED_LIBRARIES, CMAKE_REQUIRED_FLAGS and CMAKE_REQUIRED_INCLUDES
# accordingly if additional libraries or compiler flags are required.
INCLUDE(CheckCXXSourceCompiles)
# ============================================================================
# Some local cmake language
# ============================================================================
# assert_value_in_set(value element_1 ... element_n)
INCLUDE(cmake/assert_value_in_set.cmake)
#
# dos_path_to_unix(dos_path unix_path)
INCLUDE(cmake/dos_path_to_unix.cmake)
#
# add_to_list(variable_list constant_value)
INCLUDE(cmake/add_to_list.cmake)
#
# command_line_arg(variable default type description)
INCLUDE(cmake/command_line_arg.cmake)
#
# prefix_info(package description)
INCLUDE(cmake/prefix_info.cmake)
#
# compile_source_test(source variable)
INCLUDE(cmake/compile_source_test.cmake)
#
# assert(variable)
INCLUDE(cmake/assert.cmake)
#
# print_variable(variable)
INCLUDE(cmake/print_variable.cmake)
#
# set_compile_flags( program_name debug_which source_list)
INCLUDE(cmake/set_compile_flags.cmake)
#
# pkgconfig_info(name, system)
INCLUDE(cmake/pkgconfig_info.cmake)
#
# add_check_executable(parent_target short_name)
INCLUDE(cmake/add_check_executable.cmake)
# =============================================================================
# command line arguments
# =============================================================================
# Arguments that are no longer used
#
IF( debug_which )
   MESSAGE(FATAL_ERROR
      "Using debug_which, use cppad_debug_which or CMAKE_BUILD_TYPE instead"
   )
ENDIF( debug_which )
#
IF( cppad_sparse_list )
   MESSAGE(FATAL_ERROR
      "cppad_sparse_list has been removed from cmake command"
   )
ENDIF( cppad_sparse_list )
#
IF( cppad_deprecated )
   MESSAGE(FATAL_ERROR "cppad_deprecated has been removed from cmake command" )
ENDIF( cppad_deprecated )
#
IF( cmake_install_prefix )
   MESSAGE(FATAL_ERROR
      "cmake_install_prefix has been changed to cppad_prefix"
   )
ENDIF( cmake_install_prefix )
#
IF( cmake_install_postfix )
   MESSAGE(FATAL_ERROR
      "cmake_install_postfix has been changed to cppad_postfix"
   )
ENDIF( cmake_install_postfix )
#
FOREACH(pkg adolc ipopt cppadcg)
   IF( ${pkg}_prefix )
      MESSAGE(FATAL_ERROR
         "-D ${pkg}_prefix=value has been changed to -D include_${pkg}=true"
      )
   ENDIF( ${pkg}_prefix )
ENDFOREACH(pkg adolc eigen ipopt)
#
IF( cmake_needs_dot_slash )
   MESSAGE(FATAL_ERROR
      "cmake_needs_dot_slash was removed; see release notes for 2023-06-01"
   )
ENDIF( cmake_needs_dot_slash )
# -----------------------------------------------------------------------------
# Current Arguments
#
# cmake_install_datadir
command_line_arg(cmake_install_datadir share STRING
   "directory, below prefix, where cmake installs cppad data files"
)
#
# cmake_install_docdir
command_line_arg(cmake_install_docdir NOTFOUND STRING
   "directory, below prefix, where cmake installs cppad documentation files"
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
# cmake_defined_ok
command_line_arg(cmake_defined_ok TRUE BOOL
   "If false, check that some symbols only get defined once"
   "If true, you can use CMakeCache.txt to store previous settings"
)
#
# cppad_prefix
command_line_arg(cppad_prefix "${CMAKE_INSTALL_PREFIX}" PATH
   "cppad install prefix"
)
#
# cppad_postfix
command_line_arg(cppad_postfix NOTFOUND STRING
   "cppad install postfix"
)
#
# cppad_cxx_flags
command_line_arg(cppad_cxx_flags "" STRING
   "compile flags used with cppad (besides debug, release, and profile flags)"
)
#
# cppad_profile_flag
command_line_arg(cppad_profile_flag NOTFOUND STRING
   "compile flag used to compile and link a profile version of a program"
)
#
# cppad_testvector
command_line_arg(cppad_testvector cppad STRING
   "Namespace of vector used for testing, one of: boost, cppad, eigen, std"
)
assert_value_in_set(cppad_testvector boost cppad eigen std)
#
# cppad_max_num_threads
command_line_arg(cppad_max_num_threads 48 STRING
   "maximum number of threads that CppAD can use use"
)
IF( "${cppad_max_num_threads}" LESS "4" )
   MESSAGE(FATAL_ERROR
      "cppad_max_num_threads is not an integer greater than or equal 4"
   )
ENDIF( "${cppad_max_num_threads}" LESS "4" )
#
# cppad_tape_id_type
command_line_arg(cppad_tape_id_type "unsigned int" STRING
   "type used to identify different tapes, size must be <= sizeof(size_t)"
)
#
# cppad_tape_addr_type
command_line_arg(cppad_tape_addr_type "unsigned int" STRING
"type used to identify variables on one tape, size must be <= sizeof(size_t)"
)
#
# cppad_static_lib
command_line_arg(cppad_static_lib FALSE BOOL
   "If true (false) the cppad library will be static (shared)"
)
#
# cppad_debug_and_release
command_line_arg(cppad_debug_and_release TRUE BOOL
   "If true the cppad library and tests will be able to mix debug and release"
)
# ----------------------------------------------------------------------------
#
# Ensure c++11 support
SET(CMAKE_REQUIRED_DEFINITIONS "")
SET(CMAKE_REQUIRED_FLAGS       "${cppad_cxx_flags}")
SET(CMAKE_REQUIRED_INCLUDES    "")
SET(CMAKE_REQUIRED_LIBRARIES   "")
SET(source "
int main(void)
{  static_assert( __cplusplus >= 201103 , \"c++11 is supported\" );
   return 0;
}"
)
compile_source_test(${cmake_defined_ok} "${source}" minimal_cplusplus )
IF( NOT minimal_cplusplus  )
   MESSAGE(STATUS "setting C++ standard to 2011")
   SET(CMAKE_CXX_STANDARD 11)
   SET(CMAKE_CXX_STANDARD_REQUIRED ON)
ENDIF( NOT minimal_cplusplus  )
#
# use_cplusplus_2017_ok
SET(source "
int main(void)
{  static_assert( __cplusplus >= 201703 , \"c++17 is supported\" );
   return 0;
}"
)
compile_source_test(${cmake_defined_ok} "${source}" use_cplusplus_2017_ok )
# ----------------------------------------------------------------------------
#
# cppad_debug_and_release_01
IF (cppad_debug_and_release )
   SET(cppad_debug_and_release_01 1 )
ELSE (cppad_debug_and_release )
   SET(cppad_debug_and_release_01 0 )
ENDIF ( )
#
# cppad_debug_which
# CMAKE_BUILD_TYPE
SET(debug_even_or_odd FALSE)
IF( NOT ${cppad_debug_which} STREQUAL "" )
   assert_value_in_set(
      cppad_debug_which debug_even debug_odd debug_all debug_none
   )
ENDIF( NOT ${cppad_debug_which} STREQUAL "" )
IF( CMAKE_BUILD_TYPE )
   IF( NOT ${cppad_debug_which} STREQUAL "" )
      print_variable(CMAKE_BUILD_TYPE)
      print_variable(cppad_debug_which)
      MESSAGE(FATAL_ERROR
         "Both CMAKE_BUILD_TYPE and cppad_debug_which specified"
      )
   ENDIF( )
   STRING( TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_lower )
   IF(cmake_build_type_lower MATCHES debug)
      SET(cppad_debug_which "debug_all")
   ELSEIF(cmake_build_type_lower MATCHES release)
      SET(cppad_debug_which "debug_none")
   ELSEIF(cmake_build_type_lower MATCHES relwithdebInfo)
      SET(cppad_debug_which "debug_none")
   ELSEIF(cmake_build_type_lower MATCHES minsizerel)
      SET(cppad_debug_which "debug_none")
   ELSE( )
      MESSAGE(FATAL_ERROR
         "Unknonw CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}"
      )
   ENDIF( )
ELSE( ) # CMAKE_BUILD_TYPE
   IF( "${cppad_debug_which}" STREQUAL debug_all)
      SET(CMAKE_BUILD_TYPE Debug)
   ELSEIF( "${cppad_debug_which}" STREQUAL debug_none)
      SET(CMAKE_BUILD_TYPE Release)
   ELSEIF( "${cppad_debug_which}" STREQUAL debug_odd)
      SET(CMAKE_BUILD_TYPE Debug)
      SET(debug_even_or_odd TRUE)
   ELSEIF( "${cppad_debug_which}" STREQUAL debug_even)
      SET(CMAKE_BUILD_TYPE Release)
      SET(debug_even_or_odd TRUE)
   ENDIF( "${cppad_debug_which}" STREQUAL debug_all)
ENDIF( ) # CMAKE_BUILD_TYPE
IF( debug_even_or_odd )
   IF ( NOT cppad_debug_and_release )
      MESSAGE(FATAL_ERROR
"cppad_debug_and_release is false and cppad_debug_wich = ${cppad_debug_which}"
      )
   ENDIF ( NOT cppad_debug_and_release )
   IF( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
      MESSAGE(FATAL_ERROR
      "Cannot have cppad_debug_which equal ${cppad_debug_which} on Windows."
      )
   ENDIF( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
ENDIF( debug_even_or_odd )
print_variable(CMAKE_SYSTEM_NAME)
print_variable(cppad_debug_which)
print_variable(CMAKE_BUILD_TYPE)
MESSAGE(STATUS "make check: avialable")
# -----------------------------------------------------------------------------
# CMAKE_INSTALL_PREFIX
SET(CMAKE_INSTALL_PREFIX "${cppad_prefix}"
   CACHE PATH "value copied from cppad_prefix" FORCE
)
# -----------------------------------------------------------------------------
# Optional package information
SET(system_include TRUE)
#
# eigen
IF( ${include_eigen} )
   pkgconfig_info(eigen ${system_include})
   IF( DEFINED eigen_prefix )
      MESSAGE(FATAL_ERROR "include_eigen is true and eigen_prefix is defined")
   ENDIF( )
ELSE( )
   prefix_info(eigen ${system_include} )
ENDIF( )
#
# adolc
pkgconfig_info(adolc ${system_include})
#
# ipopt
pkgconfig_info(ipopt ${system_include})
#
# colpack_prefix
prefix_info(colpack ${system_include} )
#
# sacado_prefix
prefix_info(sacado ${system_include} )
#
# fadbad_prefix
prefix_info(fadbad ${system_include} )
#
# include_cpapdcg
SET( include_cppadcg FALSE CACHE BOOL "include cppadcg" )
IF( include_cppadcg )
   #
   # cppad_has_cppadcg
   SET(cppad_has_cppadcg 1)
   #
   # Assume bin/get_cppadcg.sh puts include files in this directory
   SET(cppadcg_include_dir "${CMAKE_BINARY_DIR}/prefix/include" )
   print_variable(cppadcg_include_dir)
   INCLUDE_DIRECTORIES( SYSTEM ${cppadcg_include_dir})
ELSE( )
   # cppad_has_cppadcg
   SET(cppad_has_cppadcg 0)
ENDIF( include_cppadcg )
# =============================================================================
# cppad_lib
# Perhaps in the future cppad_lib will depend on cmake header only flag ?
SET( cppad_lib    "cppad_lib" )
LINK_DIRECTORIES( ${cppad_BINARY_DIR}/cppad_lib )
# =============================================================================
# colpack_libs
#
IF( cppad_has_colpack )
   SET( colpack_libs "ColPack" )
ELSE( cppad_has_colpack )
   SET( colpack_libs "" )
ENDIF( cppad_has_colpack )
# =============================================================================
# automated system configuration
# =============================================================================
# CMAKE_CXX_FLAGS
IF( "${cppad_debug_which}" STREQUAL "debug_all" )
   print_variable(CMAKE_CXX_FLAGS_DEBUG)
ELSEIF( "${cppad_debug_which}" STREQUAL "debug_none" )
   print_variable(CMAKE_CXX_FLAGS_RELEASE)
ELSE( "${cppad_debug_which}" )
   print_variable(CMAKE_CXX_FLAGS_DEBUG)
   print_variable(CMAKE_CXX_FLAGS_RELEASE)
ENDIF( "${cppad_debug_which}" STREQUAL "debug_all" )
# -----------------------------------------------------------------------------
# cppad_abs_includedir, cppad_abs_libdir, cppad_abs_datadir, cppad_abs_docdir
#
# for dir_types = includedirs, libdirs, datadir, docdir
FOREACH(dir_types includedirs libdirs datadir docdir)
   # set dir_type = dir_types with  "dirs" -> "dir"
   STRING(REGEX REPLACE "dirs" "dir" dir_type ${dir_types})
   #
   # set dir_name to first directory in cmake_install_${dir_types}
   SET(dir_name NOTFOUND)
   FOREACH(dir ${cmake_install_${dir_types}})
      IF( NOT dir_name )
         SET(dir_name ${dir})
      ENDIF( NOT dir_name )
   ENDFOREACH(dir ${cmake_install_${dir_types}})
   #
   # set cppad_abs_${dir_type}
   SET(cppad_abs_${dir_type} "${cppad_prefix}/${dir_name}" )
   #
   # check if we need to add a postfix to it
   IF( cppad_postfix )
      SET(cppad_abs_${dir_type} ${cppad_abs_${dir_type}}/${cppad_postfix} )
   ENDIF( cppad_postfix )
ENDFOREACH(dir_types includedirs libdirs datadir docdir)
# -----------------------------------------------------------------------------
# OpenMP_CXX_FOUND, OpenMP_CXX_FLAGS
IF( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
   SET(OpenMP_CXX_FOUND FALSE)
   MESSAGE(STATUS
      "Skipping OpenMP on Darwin (Mac) systems. 2DO: fix this"
   )
ELSE( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
   FIND_PACKAGE(OpenMP)
ENDIF( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
# -----------------------------------------------------------------------------
# boost_prefix
# Only need components for object libraries, not include libraries.
SET(CMAKE_REQUIRED_FLAGS "")  # 2DO: why is this necessary with clang ?
FIND_PACKAGE(Boost COMPONENTS thread)
SET(CMAKE_REQUIRED_FLAGS "${cppad_cxx_flags}")
SET(cppad_has_boost 0)
IF ( Boost_FOUND )
   SET(cppad_has_boost 1)
   #
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
   print_variable(boost_prefix)
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
# -----------------------------------------------------------------------------
# ipopt_LIBRARIES and ipopt_LIBRARY_DIRS
IF( cppad_has_ipopt )
   #
   # Set the system environment variable PKG_CONFIG_PATH
   FOREACH(dir ${cmake_install_libdirs})
      IF(EXISTS "${ipopt_prefix}/${dir}/pkgconfig/ipopt.pc")
         SET( ENV{PKG_CONFIG_PATH} ${ipopt_prefix}/${dir}/pkgconfig )
      ENDIF(EXISTS "${ipopt_prefix}/${dir}/pkgconfig/ipopt.pc")
   ENDFOREACH(dir)
   #
   # pkg_check_modules(<PREFIX> [REQUIRED] <MODULE> [<MODULE>]*)
   # ipopt_LIBRARIES      ... only the libraries (w/o the '-l')
   # ipopt_LIBRARY_DIRS   ... the paths of the libraries (w/o the '-L')
   pkg_check_modules(ipopt ipopt)
   IF( NOT ipopt_FOUND )
      MESSAGE(FATAL_ERROR
"For all directories dir in cmake_install_libdirs, cannot find the file
   ipopt_prefix/dir/pkgconfig/ipopt.pc
where
   ipopt_prefix = ${ipopt_prefix}
   cmake_install_libdirs = ${cmake_install_libdirs}
"
      )
   ENDIF( NOT ipopt_FOUND )
ENDIF( cppad_has_ipopt )
# =============================================================================
# Currently building tests as normal executables
# =============================================================================
# The CMakeLists.txt file in the specified source directory is processed
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
#
#
# is_cppad_lib_dynamic
STRING( REGEX MATCH "^MSYS"    is_msys     "${CMAKE_SYSTEM_NAME}" )
STRING( REGEX MATCH "^CYGWIN"  is_cygwin   "${CMAKE_SYSTEM_NAME}" )
STRING( REGEX MATCH "^Windows" is_windows  "${CMAKE_SYSTEM_NAME}" )
IF( is_msys OR is_cygwin OR is_windows )
   IF( NOT cppad_static_lib )
      MESSAGE(WARNING
"This systems uses windows dynamic libraries.
The cppad library must be static for this case.
Ignoring fact that cppad_static_lib is FALSE.
"
      )
   ENDIF( )
   SET(is_cppad_lib_dynamic 0)
ELSE( )
   IF( cppad_static_lib )
      SET(is_cppad_lib_dynamic 0)
   ELSE( )
      SET(is_cppad_lib_dynamic 1)
   ENDIF( )
ENDIF( )
print_variable(is_cppad_lib_dynamic)

#
# Initialize list of tests as empty
SET(check_depends "")
#
# directories with no check depends entries
ADD_SUBDIRECTORY(include/cppad)
ADD_SUBDIRECTORY(pkgconfig)
ADD_SUBDIRECTORY(cppad_lib)
#
IF( NOT ( "${check_depends}" STREQUAL "" ) )
   MESSAGE(FATAL_ERROR "Error in CMakeLists.txt scripts")
ENDIF( NOT ( "${check_depends}" STREQUAL "" ) )
#
# directories with check depends entries
ADD_SUBDIRECTORY(val_graph)
ADD_SUBDIRECTORY(example)
ADD_SUBDIRECTORY(introduction)
ADD_SUBDIRECTORY(test_more)
ADD_SUBDIRECTORY(speed)
IF( cppad_has_ipopt )
   ADD_SUBDIRECTORY(cppad_ipopt)
ENDIF( cppad_has_ipopt )
#
# check, test
ADD_CUSTOM_TARGET(check DEPENDS ${check_depends})
ADD_CUSTOM_TARGET(test DEPENDS ${check_depends})
# ============================================================================
# Copy a file to another location and modify its contents.
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(
   ${CMAKE_CURRENT_SOURCE_DIR}/bin/test_one.sh.in
   ${CMAKE_CURRENT_SOURCE_DIR}/bin/test_one.sh
)
# =============================================================================
# install procedure
# =============================================================================
# install(DIRECTORY dirs... DESTINATION <dir>
#  [FILE_PERMISSIONS permissions...]
#  [DIRECTORY_PERMISSIONS permissions...]
#  [USE_SOURCE_PERMISSIONS] [OPTIONAL]
#  [CONFIGURATIONS [Debug|Release|...]]
#  [COMPONENT <component>] [FILES_MATCHING]
#  [[PATTERN <pattern> | REGEX <regex>]
#  [EXCLUDE] [PERMISSIONS permissions...]] [...]
# )
# Note a trailing / in the source directory name drops the source directory
# name during the copy.
#
# During install copy all the cppad include files to
# ${cppad_abs_includedir}/cppad
INSTALL(
   DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/cppad/"
   DESTINATION ${cppad_abs_includedir}/cppad
   FILES_MATCHING PATTERN "*.hpp" PATTERN "xrst" EXCLUDE
)
#
# During install copy doc direcrory to cppad_abs_docdir/cppad
IF ( cmake_install_docdir )
   # ADD_CUSTOM_TARGET(docdir COMMAND
   #    xrst
   #    --group_list default app
   #    --config_file "${CMAKE_CURRENT_SOURCE_DIR}/xrst.toml"
   # )
   INSTALL(
      DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/html/"
      DESTINATION ${cppad_abs_docdir}/cppad
   )
ENDIF ( cmake_install_docdir )

# =============================================================================
# uninstall procedure
# =============================================================================
ADD_CUSTOM_TARGET(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cppad_uninstall.cmake)
