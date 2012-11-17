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
# command line arguments
#
# cppad_sparse_list
command_line_arg(cppad_sparse_list YES BOOL 
	"use sparse list for internal sparse set representation"
)
#
# cppad_testvector 
command_line_arg(cppad_testvector cppad STRING
"Namespace of vector used for testing, one of: boost, cppad, eigen, std"
)
#
# cppad_tape_addr_type
command_line_arg(cppad_tape_addr_type "unsigned int" STRING
"type used to identify variables on one tape, size must be <= sizeof(size_t)"
)
#
# cppad_tape_id_type
command_line_arg(cppad_tape_id_type "unsigned int" STRING
	"type used to identify different tapes, size must be <= sizeof(size_t)"
)
#
# cppad_max_num_threads
command_line_arg(cppad_max_num_threads 48 STRING
	"maximum number of threads that CppAD can use use"
)
# -----------------------------------------------------------------------------
# CPPAD_INTERNAL_SPARSE_SET
#
IF( cppad_sparse_list )
	SET(CPPAD_INTERNAL_SPARSE_SET  sparse_list )
ELSE( cppad_sparse_list )
	SET(CPPAD_INTERNAL_SPARSE_SET  sparse_set )
ENDIF( cppad_sparse_list )
MESSAGE(STATUS "CPPAD_INTERNAL_SPARSE_SET = ${CPPAD_INTERNAL_SPARSE_SET}" )
#
# -----------------------------------------------------------------------------
# CPPAD_BOOSTVECTOR, CPPAD_CPPADVECTOR, CPPAD_EIGENVECTOR, CPPAD_STDVECTOR
#
check_match(cppad_testvector boost CPPAD_BOOSTVECTOR)
check_match(cppad_testvector cppad CPPAD_CPPADVECTOR)
check_match(cppad_testvector eigen CPPAD_EIGENVECTOR)
check_match(cppad_testvector std   CPPAD_STDVECTOR)
IF( NOT CPPAD_BOOSTVECTOR )
IF( NOT CPPAD_CPPADVECTOR )
IF( NOT CPPAD_EIGNEVECTOR )
IF( NOT CPPAD_STDVECTOR )
MESSAGE(FATAL_ERROR 
"cppad_testvector not one of following: boost, cppad, eigen, std." 
)
ENDIF( NOT CPPAD_STDVECTOR )
ENDIF( NOT CPPAD_EIGNEVECTOR )
ENDIF( NOT CPPAD_CPPADVECTOR )
ENDIF( NOT CPPAD_BOOSTVECTOR )

IF( CPPAD_BOOSTVECTOR )
	# FIND_PACKAGE(Boost) done by ../CMakeLists.txt
	IF( NOT Boost_FOUND )
		MESSAGE(FATAL_ERROR 
"cppad_testvector == boost but cannot find boost include files"
		)
	ENDIF( NOT Boost_FOUND )
ENDIF( CPPAD_BOOSTVECTOR )
#
IF( CPPAD_EIGENVECTOR )
	IF( NOT eigen_prefix )
		MESSAGE(FATAL_ERROR 
"cppad_testvector == eigen but eigen_prefix is not specified"
		)
	ENDIF( NOT eigen_prefix )
ENDIF( CPPAD_EIGENVECTOR )

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
# CPPAD_TAPE_ADDR_TYPE, CPPAD_TAPE_ID_TYPE
#
FOREACH( cmake_var cppad_tape_id_type cppad_tape_addr_type )
	# ----------------------------------------------------------------------
	STRING(COMPARE EQUAL ${cmake_var} cppad_tape_id_type is_id)
	IF( is_id )
		SET(cppad_var   CPPAD_TAPE_ID_TYPE) 
	ELSE( is_id )
		SET(cppad_var   CPPAD_TAPE_ADDR_TYPE) 
	ENDIF( is_id )
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
# CPPAD_MAX_NUM_THREADS
#
SET(source "
int main(void)
{	const char* number = \"${cppad_max_num_threads}\";
	int value = 0;
	while( '0' <= *number && *number <= '9' && *number != char(0) )
	{	value = 10 * value + (int)(*number - '0');
		number++;
	}
	if( *number != char(0) )
		return 1;
	if( value < 4 )
		return 1;
	return 0;
}
" )
CHECK_CXX_SOURCE_RUNS("${source}" cppad_max_num_threads_is_integer_ge_4)
IF( NOT cppad_max_num_threads_is_integer_ge_4 )
	MESSAGE(FATAL_ERROR 
	"cppad_max_num_threads is not an integer greater than or equal 4"
	)
ENDIF( NOT cppad_max_num_threads_is_integer_ge_4 )
SET(CPPAD_MAX_NUM_THREADS_DEFAULT "${cppad_max_num_threads}")
MESSAGE(STATUS "cppad_max_num_threads = ${cppad_max_num_threads}")
# -----------------------------------------------------------------------------
# Copy a file to another location and modify its contents.
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp.in             
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp
)
