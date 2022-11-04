# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the speed/cppadcg directory tests
# Inherit build type from ../CMakeList.txt
#
assert ( include_cppadcg )
#
# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_CPPADCG_SPEED")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

SET(source_list ../main.cpp
   det_lu.cpp
   det_minor.cpp
   mat_mul.cpp
   ode.cpp
   poly.cpp
   sparse_hessian.cpp
   sparse_jacobian.cpp
)
set_compile_flags( speed_cppadcg "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(speed_cppadcg EXCLUDE_FROM_ALL ${source_list} )
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_cppadcg
   ${cppad_lib}
   ${colpack_libs}
)
# cppadcg is an include file only library
TARGET_LINK_LIBRARIES(speed_cppadcg
   speed_src
   ${cppad_lib}
   ${colpack_libs}
)


# check_speed_cppadcg
SET(random_seed 54321)
add_check_executable(check_speed cppadcg "correct ${random_seed} onetape")
