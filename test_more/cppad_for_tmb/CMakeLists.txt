# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# Name for this text
SET(name cppad_for_tmb)
#
# Add extra compiler flags to cppad_cxx_flags so set_compile_flags uses them.
# This change only affects the cppad_for_tmb tests (NO PARENT_SCOPE).
SET(cppad_cxx_flags "${cppad_cxx_flags} ${OpenMP_CXX_FLAGS} -DCPPAD_FOR_TMB")
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   ${name}.cpp
   implicit_ctor.cpp
   multi_atomic_three.cpp
   multi_atomic_two.cpp
   multi_chkpoint_one.cpp
   multi_chkpoint_two.cpp
   perfer_reverse.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(
   test_more_${name} "${cppad_debug_which}" "${source_list}"
)
#
# now that we have the properties, add the executable
ADD_EXECUTABLE( test_more_${name} EXCLUDE_FROM_ALL ${source_list} )
TARGET_LINK_LIBRARIES(test_more_${name} PRIVATE
   ${cppad_lib}
   ${colpack_libs}
   ${OpenMP_CXX_LIBRARIES}
)
#
# Extra flags used by linker for openmp support
SET(CMAKE_EXE_LINKER_FLAGS ${OpenMP_CXX_FLAGS} )
#
# check_test_more_${name}
add_check_executable(check_test_more ${name})
