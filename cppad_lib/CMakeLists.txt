# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
# Build and install the cppad_lib shared library
#
# string(REGEX REPLACE <regular_expression>
#        <replace_expression> <output variable>
#        <input> [<input>...])
#
# (year - 2000) dot (remove leading 0 from mmdd) dot (release)
# Note that the dot (release) is optional
# Also name that when mmdd is 0000 get (year - 2000) dot dot (release)
STRING(REGEX REPLACE
    "20([0-9][0-9])0*([0-9]*)([.]?[0-9]*)"
    "\\1.\\2\\3"
    soversion
    ${cppad_version}
)
# In case where mmdd is 0000, dot (release) is present. Convert to
# (year - 2000) dot 0 dot (release)
STRING(REGEX REPLACE
    "([.][.])"
    ".0."
    soversion
    ${soversion}
)
print_variable(soversion)
# )
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
    cppad_colpack.cpp
    json_lexer.cpp
    json_parser.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(cppad_lib "${cppad_debug_which}" "${source_list}" )
#
IF( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    MESSAGE( STATUS "Windows system so building static cpapd_lib")
    ADD_LIBRARY( cppad_lib STATIC ${source_list} )
ELSE( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    MESSAGE( STATUS "Not Windows system so building shared cpapd_lib")
    ADD_LIBRARY( cppad_lib SHARED ${source_list} )
    SET_TARGET_PROPERTIES( cppad_lib PROPERTIES SOVERSION ${soversion} )
ENDIF( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
#
# install(TARGETS myExe mySharedLib myStaticLib
#   RUNTIME DESTINATION bin
#   LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib/static)
INSTALL(TARGETS cppad_lib DESTINATION ${cppad_abs_libdir})
