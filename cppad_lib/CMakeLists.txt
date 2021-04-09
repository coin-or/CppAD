# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell
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
# -----------------------------------------------------------------------------
SET(llvm_soruce_list "")
IF( cppad_has_llvm )
    # BEGIN_SORT_THIS_LINE_PLUS_2
    SET(llvm_source_list
        llvm/ir_ctor.cpp
        llvm/ir_from_graph.cpp
        llvm/ir_optimize.cpp
        llvm/ir_print.cpp
        llvm/ir_to_graph.cpp
        llvm/ir_to_object_file.cpp
        llvm/llvm_link.cpp
        llvm/dynamic_lib.cpp
        llvm/error_msg.cpp
    )
    # END_SORT_THIS_LINE_MINUS_2
ENDIF( cppad_has_llvm )
# ---------------------------------------------------------------------------
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
# source_list
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
    cpp_graph_op.cpp
    cppad_colpack.cpp
    json_lexer.cpp
    json_parser.cpp
    json_writer.cpp
    ${llvm_source_list}
)
# END_SORT_THIS_LINE_MINUS_2
IF( cppad_has_cppadcg )
    SET(source_list ${source_list} code_gen_fun.cpp)
ENDIF( cppad_has_cppadcg )
#
# set_compile_flags
set_compile_flags(cppad_lib "${cppad_debug_which}" "${source_list}" )
#
# create cppad_lib
IF( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    MESSAGE( STATUS "Windows system so building static cppad_lib")
    ADD_LIBRARY( cppad_lib STATIC ${source_list} )
    IF( cppad_has_llvm )
        ADD_LIBRARY( cppad_link STATIC llvm/cppad_link.cpp )
    ENDIF( cppad_has_llvm )
ELSE( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    MESSAGE( STATUS "Not Windows system so building shared cppad_lib")
    ADD_LIBRARY( cppad_lib SHARED ${source_list} )
    SET_TARGET_PROPERTIES( cppad_lib PROPERTIES SOVERSION ${soversion} )
    IF( cppad_has_llvm )
        ADD_LIBRARY( cppad_link SHARED llvm/cppad_link.cpp )
        SET_TARGET_PROPERTIES(
            cppad_link PROPERTIES SOVERSION ${soversion}
        )
    ENDIF( cppad_has_llvm )
    #
    FIND_LIBRARY(dl_LIBRARY dl)
    IF( dl_LIBRARY )
        TARGET_LINK_LIBRARIES(cppad_lib ${dl_LIBRARY})
    ENDIF( dl_LIBRARY )
ENDIF( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
#
# cppad_link is used for runtime (JIT) linking
IF( cppad_has_llvm )
    # The llvm library is called LLVM
    TARGET_LINK_LIBRARIES(cppad_lib "LLVM")
ENDIF( cppad_has_llvm )
#
#
# install(TARGETS myExe mySharedLib myStaticLib
#   RUNTIME DESTINATION bin
#   LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib/static)
INSTALL(TARGETS cppad_lib DESTINATION ${cppad_abs_libdir})
IF( cppad_has_llvm )
    INSTALL(TARGETS cppad_link DESTINATION ${cppad_abs_libdir})
ENDIF( cppad_has_llvm )
