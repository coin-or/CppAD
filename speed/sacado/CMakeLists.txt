# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the speed/sacado directory tests 
# Inherit environment from ../CMakeList.txt
# CMAKE_BUILD_TYPE determined by parent directory

# assert sacado_prefix is defined
IF ( NOT sacado_prefix )
	MESSAGE(FATAL_ERROR 
		"speed/sacado/CMakeLists.txt: sacado_prefix = ${sacado_prefix}"
	)
ENDIF ( NOT sacado_prefix )

# Adds flags to the compiler command line for sources in the current directory 
# and below. This command can be used to add any flags, but it was originally 
# intended to add preprocessor definitions. 
ADD_DEFINITIONS("-DCPPAD_SACADO_SPEED -DRAD_AUTO_AD_Const")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(speed_sacado EXCLUDE_FROM_ALL ../main.cpp 
	det_lu.cpp
	det_minor.cpp
	mat_mul.cpp
	ode.cpp
	poly.cpp
	sparse_hessian.cpp
	sparse_jacobian.cpp
)

# Compiler flags for cppad source
add_cppad_cxx_flags( speed_sacado )

# Find name used for teuchos library
SET(teuchos_lib NOTFOUND)
FOREACH(dir ${cmake_install_libdirs})
	FILE(GLOB file_list ${sacado_prefix}/${dir}/libteuchoscore.*)
	IF(file_list)
		SET(teuchos_lib teuchoscore)
	ENDIF(file_list)
	FILE(GLOB file_list ${sacado_prefix}/${dir}/libteuchos.*)
	IF(file_list)
		SET(teuchos_lib teuchos)
	ENDIF(file_list)
ENDFOREACH(dir)

# List of libraries to be linked into the specified target
IF(teuchos_lib)
	TARGET_LINK_LIBRARIES(speed_sacado speed_src sacado ${teuchos_lib})
ELSE(teuchos_lib)
	MESSAGE(FATAT_ERROR
"
For all dir in cmake_install_libdirs, cannot find either
	sacado_prefix/dir/teuchos
	sacado_prefix/dir/teuchoscore
where
	sacado_prefix = ${sacado_prefix}
	cmake_install_libdirs = ${cmake_install_libdirs}
	teuchos_lib= ${teuchos_lib}
"
	)
ENDIF(teuchos_lib)
#
# Add the check_speed_sacado target
ADD_CUSTOM_TARGET(check_speed_sacado 
	speed_sacado correct 54321
	DEPENDS speed_sacado speed_src
)
