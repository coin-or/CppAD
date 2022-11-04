# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------

# Initialize list of tests as empty
SET(check_cppad_ipopt_depends "")

# assert include_ipopt
IF ( NOT include_ipopt )
   MESSAGE(FATAL_ERROR
      "include_ipopt is not on the cmake command line"
   )
ENDIF ( NOT include_ipopt )

# The CMakeLists.txt file in the specified source directory is processed
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(example)
ADD_SUBDIRECTORY(speed)
ADD_SUBDIRECTORY(test)

# Add the check_cppad_ipopt target
ADD_CUSTOM_TARGET(check_cppad_ipopt DEPENDS ${check_cppad_ipopt_depends} )
MESSAGE(STATUS "make check_cppad_ipopt: available")

# Change check depends in parent environment
add_to_list(check_depends check_cppad_ipopt)
SET(check_depends "${check_depends}" PARENT_SCOPE)
