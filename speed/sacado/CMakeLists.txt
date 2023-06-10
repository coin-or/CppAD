# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the speed/sacado directory tests
# Inherit build type from ../CMakeList.txt

# assert sacado_prefix is defined
assert( sacado_prefix )

# Avoid the following warning:
# The installed Kokkos configuration does not support CXX extensions.
SET(CMAKE_CXX_EXTENSIONS Off)

# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_SACADO_SPEED -DRAD_AUTO_AD_Const")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list ../main.cpp
   det_lu.cpp
   det_minor.cpp
   mat_mul.cpp
   ode.cpp
   poly.cpp
   sparse_hessian.cpp
   sparse_jacobian.cpp
)
set_compile_flags( speed_sacado "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( speed_sacado EXCLUDE_FROM_ALL ${source_list} )
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_sacado
   ${cppad_lib}
   ${colpack_libs}
)
# determine libraries necessary to link Sacado
FOREACH(dir ${cmake_install_libdirs})
   IF( IS_DIRECTORY ${sacado_prefix}/${dir}/cmake/Trilinos )
      SET(Trilinos_DIR "${sacado_prefix}/${dir}/cmake/Trilinos" )
   ENDIF( IS_DIRECTORY ${sacado_prefix}/${dir}/cmake/Trilinos )
ENDFOREACH( dir )
FIND_PACKAGE(Trilinos CONFIG)
print_variable(Sacado_LIBRARIES)
#
# include the dl library if it is in the system path
CHECK_LIBRARY_EXISTS( dl dlopen "${CMAKE_SYSTEM_LIBRARY_PATH}" dl_found )
IF( dl_found )
   TARGET_LINK_LIBRARIES( speed_sacado speed_src ${Sacado_LIBRARIES} dl )
ELSE( dl_found )
   TARGET_LINK_LIBRARIES( speed_sacado speed_src ${Sacado_LIBRARIES} )
ENDIF( dl_found )
#
# the check_speed_sacado
add_check_executable(check_speed sacado "correct 54321")
