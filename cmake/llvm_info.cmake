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
# =============================================================================
# llvm_info(llvm_cxxflags llvm_includedir llvm_libdir)
#
# llvm_cxxflags: (out)
# llvm extra c++ flags to add to compliation.
#
# llvm_includedir: (out)
# llvm include directory.
#
# llvm_libdir: (out)
# llvm library directory.
#
MACRO(llvm_info)
    #
    # llvm_config
    FIND_PROGRAM( llvm_config NAMES "llvm-config")
    #
    # llvm_cxxflags
    EXECUTE_PROCESS(
        COMMAND "${llvm_config}" --cxxflags
        OUTPUT_VARIABLE llvm_cxxflags
    )
    #
    # llvm_includedir
    EXECUTE_PROCESS(
        COMMAND "${llvm_config}" --includedir
        OUTPUT_VARIABLE llvm_includedir
    )
    STRING(REPLACE "-fno-exceptions" "" llvm_cxxflags "${llvm_cxxflags}")
    STRING(REGEX REPLACE "-I[^ ]*" ""   llvm_cxxflags "${llvm_cxxflags}")
    #
    # llvm_libdir
    EXECUTE_PROCESS(
        COMMAND "${llvm_config}" --libdir
        OUTPUT_VARIABLE llvm_libdir
    )
    #
    # Remove newline at end of variables
    FOREACH(variable llvm_cxxflags llvm_includedir llvm_libdir)
        STRING(REPLACE "\n" "" "${variable}" "${${variable}}")
        print_variable( ${variable} )
    ENDFOREACH(variable)
ENDMACRO(llvm_info)
