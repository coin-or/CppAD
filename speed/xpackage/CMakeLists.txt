# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the speed/xpackage directory tests
# Inherit build type from ../CMakeList.txt

# Set the install prefix for this package
SET(xpackage_prefix "/usr")

# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_XPACKAGE_SPEED")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}/../src
   ${xpackage_prefix}/include
)

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
set_compile_flags( speed_xpackage "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( speed_xpackage EXCLUDE_FROM_ALL ${source_list} )
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_xpackage
   ${cppad_lib}
   ${colpack_libs}
)
# xpackage is an include file only library
TARGET_LINK_LIBRARIES(speed_xpackage speed_src )

# check_speed_xpackage
add_check_executable(check_speed xpackage "correct 54321")
