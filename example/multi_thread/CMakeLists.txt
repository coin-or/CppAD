# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------

# initialize list as empty
SET(check_example_multi_thread_depends "")

# Define the operation
# CHECK_LIBRARY_EXISTS (LIBRARY FUNCTION LOCATION VARIABLE)
#	LIBRARY  - the name of the library you are looking for
#	FUNCTION - the name of the function
#	LOCATION - location where the library should be found
#	VARIABLE - variable to store the result
INCLUDE(CheckLibraryExists)

# If openmp found, set OpenMP_CXX_FLAGS and process its subdirectory
#
FIND_PACKAGE(OpenMP)
IF ( OPENMP_FOUND )
	# OpenMP_CXX_FLAGS - flags to add to the CXX compiler for OpenMP support
	ADD_SUBDIRECTORY(openmp)
ENDIF ( OPENMP_FOUND )

# If pthreads found, set pthread_lib and pthread_lib_path and process subdir
#
# find_library(<VAR> name1 [path1 path2 ...])
SET(pthread_lib "pthread")
FIND_LIBRARY(pthread_lib_path pthread)
MESSAGE(STATUS "pthread library path = ${pthread_lib_path}")
IF ( pthread_lib_path )
	CHECK_LIBRARY_EXISTS(
		pthread pthread_barrier_wait ${pthread_lib_path} pthread_ok
	)
	IF ( pthread_ok )
		ADD_SUBDIRECTORY(pthread)
	ENDIF ( pthread_ok )
ENDIF ( pthread_lib_path )

# IF we find a boost multi-threadding library, define the corresponding
# bthread_lib, bthread_lib_path, and process the bthread subdirectory.
IF ( Boost_FOUND )
	SET(bthread_lib NOTFOUND)
	FOREACH(idir ${cmake_install_includedirs})
		FOREACH(ldir ${cmake_install_libdirs})
			FOREACH(lname boost_thread-mt boost_thread )
				# abort this search when find a match
				IF( NOT bthread_lib )
					#
					SET(CMAKE_REQUIRED_INCLUDES  "${boost_prefix}/${idir}")
					SET(CMAKE_REQUIRED_LIBRARIES "${lname}" )
					SET(lpath                    "${boost_prefix}/${ldir}" )
					SET(CMAKE_REQUIRED_FLAGS     "-L${lpath}" )
					# CHECK_CXX_SOURCE_RUNS(source variable)
					SET(source "
# include <boost/thread.hpp>
int main(void)
{	boost::barrier wait(1);
	return 0;
}"					)
					SET(flag ${idir}_${ldir}_${lname}_ok )
					CHECK_CXX_SOURCE_RUNS("${source}" ${flag} )
					IF ( ${flag} )
						SET(bthread_lib      ${lname} )
						SET(bthread_lib_path ${lpath} )
					ENDIF ( ${flag} )
				ENDIF( NOT bthread_lib )
			ENDFOREACH(lname boost_thread boost_thread-mt)
		ENDFOREACH(ldir ${cmake_install_liddirs})
	ENDFOREACH(idir ${cmake_install_includedirs})
	#
	IF( bthread_lib )
		ADD_SUBDIRECTORY(bthread)
		MESSAGE(STATUS
		"boost multi-threading library = ${bthread_lib_path}/${bthread_lib}"
		)
	ELSE( bthread_lib )
		MESSAGE(STATUS "Could not find boost multi-threading library")
	ENDIF( bthread_lib )
ENDIF ( Boost_FOUND )

IF( NOT( "${check_example_multi_thread_depends}" STREQUAL "" ) )
	# check_example_multi_thread
	ADD_CUSTOM_TARGET(check_example_multi_thread DEPENDS ${check_example_multi_thread_depends})
	MESSAGE(STATUS "make check_example_multi_thread: available")

	# Change check depends in parent environment
	add_to_list(check_depends check_example_multi_thread)
	SET(check_depends "${check_depends}" PARENT_SCOPE)
ENDIF( NOT( "${check_example_multi_thread_depends}" STREQUAL "" ) )
