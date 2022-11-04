# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build and install the cppad_lib shared library
#
# split cppad_version into year;month;day;release
STRING(REGEX REPLACE
   "([0-9][0-9][0-9][0-9])([0-9][0-9])([0-9][0-9])[.]*([0-9]*)"
   "\\1;\\2;\\3;\\4"
   version_list
   ${cppad_version}
)
LIST(GET version_list 0 year)
LIST(GET version_list 1 month)
LIST(GET version_list 2 day)
LIST(GET version_list 3 release)
#
# soversion: dynamic library version number
MATH(EXPR major "${day} - 1 + 31 * ( ${month} - 1 + 12 * ( ${year} - 2019))")
IF( "${release}" STREQUAL "" )
   SET(soversion "${major}")
ELSE( "${release}" STREQUAL "" )
   SET(soversion "${major}.${release}")
ENDIF( "${release}" STREQUAL "" )
print_variable(soversion)
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   cpp_graph_op.cpp
   cppad_colpack.cpp
   csrc_writer.cpp
   json_lexer.cpp
   json_parser.cpp
   json_writer.cpp
   link_dll_lib.cpp
   temp_file.cpp
)
# END_SORT_THIS_LINE_MINUS_2
IF( cppad_has_cppadcg )
   SET(source_list ${source_list} code_gen_fun.cpp)
ENDIF( cppad_has_cppadcg )
#
set_compile_flags(cppad_lib "${cppad_debug_which}" "${source_list}" )
#
# is_windows
IF( "${is_cppad_lib_dynamic}" STREQUAL "0" )
   MESSAGE( STATUS "Windows system so building static cppad_lib")
   ADD_LIBRARY( cppad_lib STATIC ${source_list} )
ELSEIF( "${is_cppad_lib_dynamic}" STREQUAL "1" )
   MESSAGE( STATUS "Not Windows system so building shared cppad_lib")
   ADD_LIBRARY( cppad_lib SHARED ${source_list} )
   SET_TARGET_PROPERTIES( cppad_lib PROPERTIES SOVERSION ${soversion} )
   #
   FIND_LIBRARY(dl_LIBRARY dl)
   IF( dl_LIBRARY )
      TARGET_LINK_LIBRARIES(cppad_lib ${dl_LIBRARY})
   ENDIF( dl_LIBRARY )
ELSE( "${is_cppad_lib_dynamic}" == "0" )
   MESSAGE(FATAL_ERROR "CMkakeLists.txt Error: invalid is_cppad_lib_dynamic")
ENDIF( )
#
# install(TARGETS myExe mySharedLib myStaticLib
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#  ARCHIVE DESTINATION lib/static)
INSTALL(TARGETS cppad_lib DESTINATION ${cppad_abs_libdir})
