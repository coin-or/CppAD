# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# prefix_info(package system_include)
#
# package: (in)
# is the name of an optional package.
#
# system_include: (in)
# If this is true (false), the include directories for this package will
# (will not) be treated as system directories (no warnings).
#
# ${package}_prefix: (in)
# is set by this macro to a cache PATH variable initialized as NOTFOUND.
# It can be changed by the cmake command line or gui.
# If it is changed, it holds the install prefix for this package.
#
# cppad_has_${package}: (out)
# is 1 if ${package}_prefix is changed by the cmake command line or gui,
# and 0 otherwise.
#
# If ${package}_prefix is changed, the following are done:
# 1. All the valid include subdirectories are added using INCLUDE_DIRECTORIES.
#    Valid include subdirectories are determined by cmake_install_includedirs.
# 2. All the valid library subdirectories are added using LINK_DIRECTORIES.
#    Valid library subdirectories are determined by cmake_install_libdirs.
#
# This macros uses temporary variables with names that begin with
# prefix_info_.
#
MACRO(prefix_info package system_include)
   #
   # ${package}_prefix
   SET(${package}_prefix NOTFOUND CACHE PATH "${package} install prefix")
   #
   # prefix_info_value
   SET( prefix_info_value "${${package}_prefix}" )
   #
   SET(cppad_has_${package} 1)
   #
   IF( "${prefix_info_value}" STREQUAL "NOTFOUND" )
      SET(cppad_has_${package} 0)
   ENDIF( )
   #
   IF( "${prefix_info_value}" STREQUAL "" )
      SET(cppad_has_${package} 0)
   ENDIF( )
   #
   IF( cppad_has_${package} )
      SET(cppad_has_${package} 0)
      #
      # prefix_info_subdir
      FOREACH(prefix_info_subdir ${cmake_install_includedirs})
         #
         # prefix_info_dir
         SET( prefix_info_dir "${prefix_info_value}/${prefix_info_subdir}" )
         IF(IS_DIRECTORY "${prefix_info_dir}" )
            SET(cppad_has_${package} 1)
            MESSAGE(STATUS "    Found ${prefix_info_dir}")
            IF( ${system_include} )
               INCLUDE_DIRECTORIES( SYSTEM "${prefix_info_dir}" )
            ELSE( )
               INCLUDE_DIRECTORIES( "${prefix_info_dir}" )
            ENDIF( )
         ENDIF( )
      ENDFOREACH( )
      #
      # prefix_info_subdir
      FOREACH(prefix_info_subdir ${cmake_install_libdirs})
         #
         # prefix_info_dir
         SET( prefix_info_dir "${prefix_info_value}/${prefix_info_subdir}" )
         IF(IS_DIRECTORY "${prefix_info_dir}" )
            SET(cppad_has_${package} 1)
            MESSAGE(STATUS "    Found ${prefix_info_dir}")
            LINK_DIRECTORIES( "${prefix_info_dir}" )
         ENDIF( )
      ENDFOREACH()
      IF( NOT cppad_has_${package} )
         MESSAGE(FATAL_ERROR
            "cppad_has_${package} = ${prefix_info_value} "
            "but did not find any include files or libraries there"
         )
      ENDIF( )
   ENDIF( )
   #
ENDMACRO( )
