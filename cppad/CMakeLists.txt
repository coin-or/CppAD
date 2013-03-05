#CppAD $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
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
#
# cppad_testvector 
command_line_arg(cppad_testvector cppad STRING
"Namespace of vector used for testing, one of: boost, cppad, eigen, std"
)
#
# cppad_max_num_threads
command_line_arg(cppad_max_num_threads 48 STRING
	"maximum number of threads that CppAD can use use"
)
# cppad_sparse_list
command_line_arg(cppad_sparse_list YES BOOL 
	"use sparse list for internal sparse set representation"
)
#
# cppad_tape_id_type
command_line_arg(cppad_tape_id_type "unsigned int" STRING
	"type used to identify different tapes, size must be <= sizeof(size_t)"
)
#
# cppad_tape_addr_type
command_line_arg(cppad_tape_addr_type "unsigned int" STRING
"type used to identify variables on one tape, size must be <= sizeof(size_t)"
)
# -----------------------------------------------------------------------------
# cppad_internal_sparse_set
#
IF( cppad_sparse_list )
	SET(cppad_internal_sparse_set  sparse_list )
ELSE( cppad_sparse_list )
	SET(cppad_internal_sparse_set  sparse_set )
ENDIF( cppad_sparse_list )
MESSAGE(STATUS "cppad_internal_sparse_set = ${cppad_internal_sparse_set}" )
#
# -----------------------------------------------------------------------------
# cppad_boostvector, cppad_cppadvector, cppad_eigenvector, cppad_stdvector
#
check_match(cppad_testvector boost cppad_boostvector)
check_match(cppad_testvector cppad cppad_cppadvector)
check_match(cppad_testvector eigen cppad_eigenvector)
check_match(cppad_testvector std   cppad_stdvector)
IF( NOT cppad_boostvector )
IF( NOT cppad_cppadvector )
IF( NOT cppad_eigenvector )
IF( NOT cppad_stdvector )
MESSAGE(FATAL_ERROR 
"cppad_testvector not one of following: boost, cppad, eigen, std." 
)
ENDIF( NOT cppad_stdvector )
ENDIF( NOT cppad_eigenvector )
ENDIF( NOT cppad_cppadvector )
ENDIF( NOT cppad_boostvector )

IF( cppad_boostvector )
	# FIND_PACKAGE(Boost) done by ../CMakeLists.txt
	IF( NOT Boost_FOUND )
		MESSAGE(FATAL_ERROR 
"cppad_testvector == boost but cannot find boost include files"
		)
	ENDIF( NOT Boost_FOUND )
ENDIF( cppad_boostvector )
#
IF( cppad_eigenvector )
	IF( NOT eigen_prefix )
		MESSAGE(FATAL_ERROR 
"cppad_testvector == eigen but eigen_prefix is not specified"
		)
	ENDIF( NOT eigen_prefix )
ENDIF( cppad_eigenvector )

# -----------------------------------------------------------------------------
# cppad_has_nullptr
#
# CHECK_CXX_SOURCE_RUNS(source variable)
SET(CMAKE_REQUIRED_INCLUDES  "")
SET(CMAKE_REQUIRED_LIBRARIES "")
SET(CMAKE_REQUIRED_FLAGS     ${cppad_cxx_flags})
SET(source "
int main(void)
{	char *c = nullptr;
	return 0;
}" )
CHECK_CXX_SOURCE_RUNS("${source}" nullptr_ok)
IF( nullptr_ok )
	SET(cppad_has_nullptr 1)
ELSE( nullptr_ok )
	SET(cppad_has_nullptr 0)
ENDIF( nullptr_ok )
MESSAGE(STATUS "cppad_has_nullptr = ${cppad_has_nullptr}" )

# -----------------------------------------------------------------------------
# cppad_has_gettimeofday
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
	SET(cppad_has_gettimeofday 1)
ELSE( gettimeofday_ok )
	SET(cppad_has_gettimeofday 0)
ENDIF( gettimeofday_ok )
MESSAGE(STATUS "cppad_has_gettimeofday = ${cppad_has_gettimeofday}" )
# -----------------------------------------------------------------------------
# cppad_size_t_same_unsigned_int
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
	SET(cppad_size_t_same_unsigned_int 0)
ELSE( size_t_not_unsigned_int )
	SET(cppad_size_t_same_unsigned_int 1)
ENDIF( size_t_not_unsigned_int )
MESSAGE(STATUS 
	"cppad_size_t_same_unsigned_int = ${cppad_size_t_same_unsigned_int}" 
)
# -----------------------------------------------------------------------------
# cppad_tape_addr_type, cppad_tape_id_type
#
FOREACH( cmake_var cppad_tape_id_type cppad_tape_addr_type )
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
ENDFOREACH( cmake_var )
# -----------------------------------------------------------------------------
# cppad_max_num_threads
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

# -----------------------------------------------------------------------------
# Copy a file to another location and modify its contents.
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp.in             
	${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp
)
