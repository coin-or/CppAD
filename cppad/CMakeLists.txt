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
# cppad_sparse_list, CPPAD_INTERNAL_SPARSE_SET
SET(cppad_sparse_list YES CACHE BOOL 
	"use sparse list for internal sparse set representation"
)
IF( cppad_sparse_list )
	SET(CPPAD_INTERNAL_SPARSE_SET  sparse_list )
ELSE( cppad_sparse_list )
	SET(CPPAD_INTERNAL_SPARSE_SET  sparse_set )
ENDIF( cppad_sparse_list )
MESSAGE(STATUS "CPPAD_INTERNAL_SPARSE_SET = ${CPPAD_INTERNAL_SPARSE_SET}" )
#
# -----------------------------------------------------------------------------
# cppad_test_vector_namespace,
# CPPAD_BOOSTVECTOR, CPPAD_CPPADVECTOR, CPPAD_EIGENVECTOR, CPPAD_STDVECTOR
#
SET(cppad_test_vector_namespace cppad CACHE STRING
	"Namespace of vector used for testing (one of boost, cppad, eigen, std)"
)
check_match(cppad_test_vector_namespace boost CPPAD_BOOSTVECTOR)
check_match(cppad_test_vector_namespace cppad CPPAD_CPPADVECTOR)
check_match(cppad_test_vector_namespace eigen CPPAD_EIGENVECTOR)
check_match(cppad_test_vector_namespace std   CPPAD_STDVECTOR)
IF( NOT CPPAD_BOOSTVECTOR )
IF( NOT CPPAD_CPPADVECTOR )
IF( NOT CPPAD_EIGNEVECTOR )
IF( NOT CPPAD_STDVECTOR )
MESSAGE(FATAL_ERROR 
"cppad_test_vector_namespace not one of following: boost, cppad, eigen, std." 
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
# include <cstring>
template <class T> inline bool is_pod(void)               { return false; }
template <>        inline bool is_pod<unsigned int>(void) { return true;  }  
template <>        inline bool is_pod<size_t>(void)       { return true;  }  
int main(void)
{	return 0; }
" )
CHECK_CXX_SOURCE_RUNS("${source}" size_t_not_unsigned_int)
IF( size_t_not_unsigned_int )
	SET(CPPAD_SIZE_T_SAME_UNSIGNED_INT 0)
ELSE( size_t_not_unsigned_int )
	SET(CPPAD_SIZE_T_SAME_UNSIGNED_INT 1)
ENDIF( size_t_not_unsigned_int )
MESSAGE(STATUS 
	"CPPAD_SIZE_T_SAME_UNSIGNED_INT = ${CPPAD_SIZE_T_SAME_UNSIGNED_INT}" 
)
# -----------------------------------------------------------------------------
# cppad_tape_addr_type, CPPAD_TAPE_ADDR_TYPE
# cppad_tape_id_type,   CPPAD_TAPE_ID_TYPE
#
FOREACH( cmake_var cppad_tape_id_type cppad_tape_addr_type )
	# ----------------------------------------------------------------------
	STRING(COMPARE EQUAL ${cmake_var} cppad_tape_id_type is_id)
	IF( is_id )
		SET(cppad_var   CPPAD_TAPE_ID_TYPE) 
		SET(description   
"type used to identify different tapes, size must be <= sizeof(size_t)"
		)
	ELSE( is_id )
		SET(cppad_var   CPPAD_TAPE_ADDR_TYPE) 
		SET(description 
"type used to identify variables on the tape, size must be <= sizeof(size_t)"
		)
	ENDIF( is_id )
	#
	SET(${cmake_var} "unsigned int" CACHE STRING "${description}")
	# ----------------------------------------------------------------------
	SET(source "
# include <limits>
int main(void)
{	bool is_signed = std::numeric_limits<${${cmake_var}}>::is_signed;
	return int(! is_signed);
}
" )
	CHECK_CXX_SOURCE_RUNS("${source}" ${cmake_var}_is_signed)
	IF( ${cmake_var}_is_signed  )
		MESSAGE(STATUS 
"Warning: using a signed ${cmake_var} (for CppAD developers not users)" 
	)
	ENDIF( ${cmake_var}_is_signed  )
	# ----------------------------------------------------------------------
	SET(${cppad_var} "${${cmake_var}}")
	MESSAGE(STATUS "${cppad_var} = ${${cmake_var}}" )
ENDFOREACH( cmake_var )
# -----------------------------------------------------------------------------
# cppad_max_num_threads, CPPAD_MAX_NUM_THREADS
#
SET(cppad_max_num_threads 48 CACHE STRING
	"maximum number of threads that CppAD can use use"
)
SET(source "
int main(void)
{	const char* number = \"${cppad_max_num_threads}\";
	while( '0' <= *number && *number <= '9' && *number != char(0) )
		number++;
	if( *number == char(0) )
		return 0;
	return 1;
}
" )
CHECK_CXX_SOURCE_RUNS("${source}" cppad_max_num_threads_is_integer)
IF( NOT cppad_max_num_threads_is_integer )
	MESSAGE(FATAL_ERROR 
	"cppad_max_num_threads contains a character that is not a decimal digit"
	)
ENDIF( NOT cppad_max_num_threads_is_integer )
SET(CPPAD_MAX_NUM_THREADS "${cppad_max_num_threads}")
MESSAGE(STATUS "CPPAD_MAX_NUM_THREADS = ${CPPAD_MAX_NUM_THREADS}")
# -----------------------------------------------------------------------------
# Copy a file to another location and modify its contents.
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp.in             
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp
)
