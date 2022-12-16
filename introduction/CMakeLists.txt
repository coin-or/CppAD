# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the introduction/exp_apx directory tests

# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   exp_2.cpp
   exp_2.hpp
   exp_2_cppad.cpp
   exp_2_for0.cpp
   exp_2_for1.cpp
   exp_2_for2.cpp
   exp_2_rev1.cpp
   exp_2_rev2.cpp
   exp_eps.cpp
   exp_eps.hpp
   exp_eps_cppad.cpp
   exp_eps_for0.cpp
   exp_eps_for1.cpp
   exp_eps_for2.cpp
   exp_eps_rev1.cpp
   exp_eps_rev2.cpp
   introduction.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags( introduction "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( introduction EXCLUDE_FROM_ALL ${source_list} )
#
TARGET_LINK_LIBRARIES(introduction ${cppad_lib} ${colpack_libs})
#
# check_introduction
add_check_executable(check introduction)
