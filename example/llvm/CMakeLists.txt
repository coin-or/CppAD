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
# -----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
    llvm.cpp
    link.cpp
    algo2adfun.cpp
    load_obj_file.cpp
    llvm_ir.cpp
    from_to_graph.cpp
    algo2adfun_xam.cpp
    optimize.cpp
    llvm_link.cpp
)
# END_SORT_THIS_LINE_MINUS_2

SET(cppad_cxx_flags "${cppad_cxx_flags} ${llvm_cxxflags}" )
set_compile_flags(example_llvm "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_llvm EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_llvm
    ${cppad_lib}
    ${colpack_libs}
    ${llvm_libs}
)
LINK_DIRECTORIES( ${llvm_dir} )
#
# Add the check_example_llvm target
ADD_CUSTOM_TARGET(check_example_llvm
    example_llvm
    DEPENDS example_llvm
)
MESSAGE(STATUS "make check_example_llvm: available")
#
# add to check check_example_depends in parent environment
add_to_list(check_example_depends check_example_llvm)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
