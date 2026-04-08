# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-26 Bradley M. Bell
# ----------------------------------------------------------------------------
# pkgconfig_info(package system_include)
#
# package: (in)
# is the name of an optional package.
#
# system_include: (in)
# If this is true (false), the include directories for this package will
# (will not) be treated as system directories (no warnings).
#
# include_${package}: (in)
# is set by this macro to a cache BOOL variable initialized as false.
# It can be changed by the cmake command line or gui.
# If it is true, there must be a pkg-config file for this package and
# the corresponding include directories added using INCLUDE_DIRECTORIES.
#
# cppad_has_${package}: (out)
# is 1 (0) if include_${package} is true (false).
#
# ${package}_LINK_LIBRARIES: (out)
# is a list of absolute paths to libraries necessary to use this package.
#
# This macros uses temporary variables with names that begin with
# pkgconfig_info_ and ${package}_.
#
MACRO(pkgconfig_info package system_include)
    #
    # include_${package}
    SET(
        include_${package}
        FALSE CACHE BOOL "include ${package} using its pkgconfig file"
    )
    print_variable( include_${package} )
    IF( NOT include_${package} )
        SET(cppad_has_${package} 0)
    ELSE( )
        SET(cppad_has_${package} 1)
        #
        # check for pkg-config
        IF( NOT PKG_CONFIG_FOUND )
            FIND_PACKAGE(PkgConfig REQUIRED)
        ENDIF( )
        #
        # ${package} info
        pkg_check_modules(
            ${package} QUIET ${package} )
        #
        IF( ${package}_FOUND )
            MESSAGE(STATUS "Found ${package} pkg-config file")
        ELSE( )
            MESSAGE(FATAL_ERROR
            "Can't find ${package} pkg-config file or one of its requirements."
            "\nPKG_CONFIG_PATH=$ENV{PKG_CONFIG_PATH}"
        )
        ENDIF( )
        #
        IF( ${package} STREQUAL eigen )
            SET( eigen_LIBRARIES "${eigen3_LIBRARIES}" )
        ENDIF()
        #
        # pkgconfig_info_dirs
        STRING(
            REGEX REPLACE "/coin-or" ""
            pkgconfig_info_dirs "${${package}_INCLUDE_DIRS}"
        )
        #
        # INCLUDE_DIRECTORIES
        IF( ${system_include} )
            INCLUDE_DIRECTORIES( SYSTEM ${pkgconfig_info_dirs} )
        ELSE( )
            INCLUDE_DIRECTORIES( ${pkgconfig_info_dirs} )
        ENDIF( )
    ENDIF( )
    #
    print_variable( cppad_has_${package} )
ENDMACRO(pkgconfig_info)
