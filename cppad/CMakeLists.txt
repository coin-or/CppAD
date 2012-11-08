#CppAD $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the cppad/configure.hpp file.
# Inherit environment from ../CMakeLists.txt
#
# Note that this file breaks the convention that all local definitions
# are in lower case (cmake definitions are in upper case).
# -----------------------------------------------------------------------------
MACRO(check_match match_variable match_constant output_variable) 
	STRING(COMPARE EQUAL ${${match_variable}} ${match_constant} match_flag )
	IF( match_flag )
		SET(${output_variable} 1)
	ELSE( match_flag )
		SET(${output_variable} 0)
	ENDIF( match_flag )
	MESSAGE(STATUS "${output_variable} = ${${output_variable}}" )
ENDMACRO(check_match)
# -----------------------------------------------------------------------------
# CPPAD_INTERNAL_SPARSE_SET
SET(sparse_list YES CACHE BOOL 
	"use sparse list for internal sparse set representation"
)
IF( sparse_list )
	SET(CPPAD_INTERNAL_SPARSE_SET  sparse_list )
ELSE( sparse_list )
	SET(CPPAD_INTERNAL_SPARSE_SET  sparse_set )
ENDIF( sparse_list )
MESSAGE(STATUS "CPPAD_INTERNAL_SPARSE_SET = ${CPPAD_INTERNAL_SPARSE_SET}" )
#
# -----------------------------------------------------------------------------
# CPPAD_BOOSTVECTOR, CPPAD_CPPADVECTOR, CPPAD_EIGENVECTOR, CPPAD_STDVECTOR
#
SET(vector_namespace cppad CACHE STRING
	"Namespace of vector used for testing (one of boost, cppad, eigen, std)"
)
check_match(test_vector_namespace boost CPPAD_BOOSTVECTOR)
check_match(test_vector_namespace cppad CPPAD_CPPADVECTOR)
check_match(test_vector_namespace eigne CPPAD_EIGENVECTOR)
check_match(test_vector_namespace std   CPPAD_STDTVECTOR)
IF( NOT CPPAD_BOOSTVECTOR )
IF( NOT CPPAD_CPPADVECTOR )
IF( NOT CPPAD_EIGNEVECTOR )
IF( NOT CPPAD_STDVECTOR )
MESSAGE(FATAL_ERROR 
"test_vector_namespace not one of the following: boost, cppad, eigen, std." 
)
ENDIF( NOT CPPAD_STDVECTOR )
ENDIF( NOT CPPAD_EIGNEVECTOR )
ENDIF( NOT CPPAD_CPPADVECTOR )
ENDIF( NOT CPPAD_BOOSTVECTOR )
# -----------------------------------------------------------------------------
# CPPAD_GETTIMEOFDAY
#
# CHECK_CXX_SOURCE_RUNS(source variable)
SET(CMAKE_REQUIRED_INCLUDES  "")
SET(CMAKE_REQUIRED_LIBRARIES "")
SET(CMAKE_REQUIRED_FLAGS     "")
SET(source "
# include<sys/time.h>
int main(void)
{	struct timeval time;
	gettimeofday(&time, 0);
	return 0;
}" )
CHECK_CXX_SOURCE_RUNS("${source}" gettimeofday_ok)
IF( gettimeofday_ok )
	SET(CPPAD_GETTIMEOFDAY 1)
ELSE( gettimeofday_ok )
	SET(CPPAD_GETTIMEOFDAY 0)
ENDIF( gettimeofday_ok )
MESSAGE(STATUS "CPPAD_GETTIMEOFDAY = ${CPPAD_GETTIMEOFDAY}" )
# -----------------------------------------------------------------------------
# CPPAD_SIZE_T_SAME_UNSIGNED_INT
#
# CHECK_CXX_SOURCE_RUNS(source variable)
SET(CMAKE_REQUIRED_INCLUDES  "")
SET(CMAKE_REQUIRED_LIBRARIES "")
SET(CMAKE_REQUIRED_FLAGS     "")
SET(source "
# include<cstring>
int main(void)
{	int not_same = sizeof(size_t) != sizeof(unsigned int); 	
	return not_same;
}" )
CHECK_CXX_SOURCE_RUNS("${source}" size_t_same_unsigned_int)
IF( size_t_same_unsigned_int )
	SET(CPPAD_SIZE_T_SAME_UNSIGNED_INT 1)
ELSE( size_t_same_unsigned_int )
	SET(CPPAD_SIZE_T_SAME_UNSIGNED_INT 0)
ENDIF( size_t_same_unsigned_int )
MESSAGE(STATUS 
	"CPPAD_SIZE_T_SAME_UNSIGNED_INT = ${CPPAD_SIZE_T_SAME_UNSIGNED_INT}" 
)
# -----------------------------------------------------------------------------
# Copy a file to another location and modify its contents.
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
# (using configure.hpp.tmp until all entries have been converted)
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp.in             
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp.tmp 
)
