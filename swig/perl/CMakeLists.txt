# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# used debug during initial development of swig interface
SET(CMAKE_BUILD_TYPE DEBUG)

assert( SWIG_FOUND )
FIND_PACKAGE(PerlLibs)
# ----------------------------------------------------------------------------
IF ( PERLLIBS_FOUND )
	# create check_all.pl for this directory
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/check_all.pl.in
		${CMAKE_CURRENT_BINARY_DIR}/check_all.pl
	)

	# treat like a C++ files
	SET_SOURCE_FILES_PROPERTIES(
		${swig_control_file} PROPERTIES CPLUSPLUS ON
	)

	# swig -perl warns that it does not support *=, or /=,
	# supress these warnings.
	SET(CMAKE_SWIG_FLAGS -Wextra -w367,368 )

	# include directory determined by find PerlLibs
	INCLUDE_DIRECTORIES(${PERL_INCLUDE_PATH})

	# SWIG_ADD_MODULE(name language [ files ])
	SWIG_ADD_MODULE(pl_cppad perl ${swig_control_file} )

	# SWIG_LINK_LIBRARIES(name [ libraries ])
	SWIG_LINK_LIBRARIES(pl_cppad cppad_swig_lib ${PYTHON_LIBRARIES})

	# check_swig_perl
	ADD_CUSTOM_TARGET(check_swig_perl
		perl check_all.pl
		DEPENDS
		${SWIG_MODULE_pl_cppad_REAL_NAME}
		${CMAKE_CURRENT_BINARY_DIR}/check_all.pl
	)
	MESSAGE(STATUS "make check_swig_perl: available")

	# Change check depends in parent environment
	add_to_list(check_swig_depends check_swig_perl)
	SET(check_swig_depends "${check_swig_depends}" PARENT_SCOPE)

ENDIF ( PERLLIBS_FOUND )
