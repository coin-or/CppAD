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
# Build the cppad_ipopt/* directory tests 
# Inherit environment from ../CMakeList.txt

# Initialize list of tests as empty
SET(list_ipopt "")

# assert ipopt_prefix is defined
IF ( NOT ipopt_prefix )
	MESSAGE(FATAL_ERROR 
		"cppad_ipopt/speed/CMakeLists.txt: ipopt_prefix = ${ipopt_prefix}"
	)
ENDIF ( NOT ipopt_prefix )

FIND_PACKAGE(PkgConfig)
IF( NOT PKG_CONFIG_FOUND )
	MESSAGE(FATAL_ERROR "Using ipopt_prefix but cannot find pkg-config")
ENDIF( NOT PKG_CONFIG_FOUND )

# Set the system environment variable 
# 	PKG_CONFIG_PATH=${ipopt_prefix}/lib/pkgconfig
SET(ipopt_config ${ipopt_prefix}/lib/pkgconfig )
SET( ENV{PKG_CONFIG_PATH} ${ipopt_config} )

# pkg_check_modules(<PREFIX> [REQUIRED] <MODULE> [<MODULE>]*)
# ipopt_LIBRARIES      ... only the libraries (w/o the '-l')
# ipopt_LIBRARY_DIRS   ... the paths of the libraries (w/o the '-L')
pkg_check_modules(ipopt ipopt)
IF( NOT ipopt_FOUND )
	MESSAGE(FATAL_ERROR 
		"Missing ipopt pkg-config info in ${ipopt_config}"
	)
ENDIF( NOT ipopt_FOUND )

# The CMakeLists.txt file in the specified source directory is processed 
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
ADD_SUBDIRECTORY(src)
#
ADD_SUBDIRECTORY(example)
add_to_list(list_ipopt check_ipopt_example)
#
ADD_SUBDIRECTORY(speed)
add_to_list(list_ipopt check_ipopt_speed)
#
ADD_SUBDIRECTORY(test)
add_to_list(list_ipopt check_ipopt_test_more)

# Add the check_ipopt target
ADD_CUSTOM_TARGET(check_ipopt DEPENDS ${list_ipopt} )
