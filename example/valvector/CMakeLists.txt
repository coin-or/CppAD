# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/valvector directory tests
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list valvector.cpp
   ad_join.cpp
   ad_split.cpp
   ad_sum.cpp
   assign.cpp
   azmul.cpp
   base_require.cpp
   binary_op.cpp
   compare_op.cpp
   compound_op.cpp
   condexp.cpp
   ctor.cpp
   element.cpp
   get_started.cpp
   llsq_obj.cpp
   output.cpp
   pow.cpp
   resize.cpp
   size.cpp
   sum.cpp
   unary_math.cpp
   unary_op.cpp
)
# END_SORT_THIS_LINE_MINUS_2
#
set_compile_flags( example_valvector "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_valvector EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_valvector
   ${cppad_lib}
   ${colpack_libs}
)
# check_example_valvector
add_check_executable(check_example valvector)
