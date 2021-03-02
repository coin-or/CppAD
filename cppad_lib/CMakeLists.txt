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
    SET(llvm_source_list
        llvm/llvm_ir.cpp
        llvm/llvm_link.cpp
    )
    # llvm_config
    FIND_PROGRAM(
        llvm_config
        NAMES "llvm-config"
    )
    # llvm_cxxflags
    EXECUTE_PROCESS(
        COMMAND "${llvm_config}" --cxxflags
        OUTPUT_VARIABLE llvm_cxxflags
    )
    STRING(REPLACE "-fno-exceptions" "" llvm_cxxflags "${llvm_cxxflags}")
    #
    # llvm_libs
    EXECUTE_PROCESS(
        COMMAND "${llvm_config}" --libs
        OUTPUT_VARIABLE llvm_libs
    )
    STRING(REPLACE "-l" "" llvm_libs "${llvm_libs}")
    #
    # llvm_dirs
    EXECUTE_PROCESS(
        COMMAND "${llvm_config}" --libdir
        OUTPUT_VARIABLE llvm_libdir
    )
    STRING(REPLACE "-l" "" llvm_libs "${llvm_libs}")
    #
    # Remove newline at end of variables
    FOREACH(variable llvm_config, llvm_cxxflags llvm_libs llvm_libdir)
        STRING(REPLACE "\n" "" "${variable}" "${${variable}}")
    ENDFOREACH(variable)
    #
    print_variable(llvm_config)
    print_variable(llvm_cxxflags)
    print_variable(llvm_libs)
    print_variable(llvm_libdir)
    #
    # add llvm link directory
    LINK_DIRECTORIES( ${llvm_dir} )
    #
    # add llvm compiler flags
    SET(cppad_cxx_flags "${cppad_cxx_flags} ${llvm_cxxflags}" )
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
ELSE( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
    MESSAGE( STATUS "Not Windows system so building shared cppad_lib")
    ADD_LIBRARY( cppad_lib SHARED ${source_list} )
    SET_TARGET_PROPERTIES( cppad_lib PROPERTIES SOVERSION ${soversion} )
    #
    FIND_LIBRARY(dl_LIBRARY dl)
    IF( dl_LIBRARY )
        TARGET_LINK_LIBRARIES(cppad_lib ${dl_LIBRARY})
    ENDIF( dl_LIBRARY )
ENDIF( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
IF( cppad_has_llvm )
    TARGET_LINK_LIBRARIES(cppad_lib ${llvm_libs} )
ENDIF( cppad_has_llvm )
#
#
# install(TARGETS myExe mySharedLib myStaticLib
#   RUNTIME DESTINATION bin
#   LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib/static)
INSTALL(TARGETS cppad_lib DESTINATION ${cppad_abs_libdir})
