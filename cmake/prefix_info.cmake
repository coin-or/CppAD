# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
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
    print_variable( ${package}_prefix )
    #
    # prefix_info_value
    SET( prefix_info_value "${${package}_prefix}" )
    #
    IF( "${prefix_info_value}" STREQUAL "NOTFOUND" )
        SET(cppad_has_${package} 0)
    ELSE( )
        SET(cppad_has_${package} 1)
        #
        # prefix_info_subdir
        FOREACH(prefix_info_subdir ${cmake_install_includedirs})
            #
            # prefix_info_dir
            SET( prefix_info_dir "${prefix_info_value}/${prefix_info_subdir}" )
            IF(IS_DIRECTORY "${prefix_info_dir}" )
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
                MESSAGE(STATUS "    Found ${prefix_info_dir}")
                LINK_DIRECTORIES( "${prefix_info_dir}" )
            ENDIF( )
        ENDFOREACH()
    ENDIF (  )
ENDMACRO( )
