# ----------------------------------------------------------------------------
# pkgconfig_info(name system)
#
# Inputs:
# name:     is the name of the package config file without the .pc extension
# system:   if true (false) include directory will (will not) be
#           treated like a system directory; i.e., no warnings.
#
# Outputs:
# ${name}_LIBRARIES:     list of libraries necessary to use this package.
# ${name}_LIBRARY_DIRS:  directories containing library files for this package.
# ${name}_INCLUDE_DIRS:  directories containing include files for this package.
#
# Side Effects:
# The include directors are passed to INCLUDE_DIRECTORIES.
# The link directories are passed to LINK_DIRECTORIES,
# and the SYSTEM flag is inclued if system is TRUE.
#
# Assumptions:
# FIND_PACKAGE(PkgConfig) was successful
# It is a fatal error if ${name}.pc is not in PKG_CONFIG_PATH
MACRO(pkgconfig_info name system)
    IF( NOT PKG_CONFIG_FOUND )
        FIND_PACKAGE(PkgConfig REQUIRED)
    ENDIF( NOT PKG_CONFIG_FOUND )
    pkg_check_modules( ${name} QUIET ${name} )
    IF( ${name}_FOUND )
        MESSAGE(STATUS "Found ${name}.pc file")
    ELSE( ${name}_FOUND )
        MESSAGE(STATUS "Cannot find ${name}.pc file")
        MESSAGE(FATAL_ERROR  "PKG_CONFIG_PATH=$ENV{PKG_CONFIG_PATH}")
    ENDIF( ${name}_FOUND )
    #
    # INLCUDE_DIRECTORIES
    IF( system )
        INCLUDE_DIRECTORIES( SYSTEM ${${name}_INCLUDE_DIRS} )
    ELSE( system  )
        INCLUDE_DIRECTORIES( ${${name}_INCLUDE_DIRS} )
    ENDIF( system )
    #
    # LINK_DIRECTORIES
    LINK_DIRECTORIES( ${${name}_LIBRARY_DIRS} )
ENDMACRO(pkgconfig_info)
