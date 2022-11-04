# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
IF( use_cplusplus_2017_ok )
   SET(dll_lib "dll_lib.cpp")
ELSE( use_cplusplus_2017_ok )
   SET(dll_lib "")
ENDIF( use_cplusplus_2017_ok )
#
# BEGIN_SORT_THIS_LINE_PLUS_3
SET(source_list
   ${dll_lib}
   check_numeric_type.cpp
   check_simple_vector.cpp
   cppad_vector.cpp
   error_handler.cpp
   index_sort.cpp
   lu_factor.cpp
   lu_invert.cpp
   lu_solve.cpp
   nan.cpp
   near_equal.cpp
   ode_err_control.cpp
   ode_err_maxabs.cpp
   ode_gear.cpp
   ode_gear_control.cpp
   poly.cpp
   pow_int.cpp
   romberg_mul.cpp
   romberg_one.cpp
   rosen_34.cpp
   runge45_1.cpp
   runge_45.cpp
   set_union.cpp
   simple_vector.cpp
   sparse_rc.cpp
   sparse_rcv.cpp
   thread_alloc.cpp
   to_string.cpp
   utility.cpp
   vector_bool.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags( example_utility "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_utility EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_utility
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_utility
add_check_executable(check_example utility)
