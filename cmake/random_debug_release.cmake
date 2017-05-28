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
# random_debug_release(program_name source_list)
#
# program_name: (in)
# Is the name of the program that we are building. This is only used to
# report which files have debug and which have release properties.
#
# source_list: (in)
# is a list of source files that randomly get set to have debug or release
# compile flags. The cppad_cxx_flags compile flags always get included.
#
# Case ${random_choice_in_0123} = 0:
# The files with an even (odd) index in source_list have debug (release) flags.
# In addition the compiler flag -DCPPAD_DEBUG_AND_RELEASE is added.
#
# Case ${random_choice_in_0123} = 1:
# The files with an odd (even) index in source_list have debug (release) flags.
# In addition the compiler flag -DCPPAD_DEBUG_AND_RELEASE is added.
#
# Case ${random_choice_in_0123} = 2:
# All the files have debug flags.
#
# Case ${random_choice_in_0123} = 3:
# All the the files have release flags.
#
FUNCTION(random_debug_release program_name source_list)
	# debug compile flags
	SET(debug_flags "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_DEBUG}")
	# relese compile flags
	SET(release_flags "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_RELEASE}")
	#
	# set alternate, report random number result,
	# set compile flags property when not alternating.
	IF( ${random_choice_in_0123} STREQUAL 0 )
		SET(alternate TRUE)
	ELSEIF( ${random_choice_in_0123} STREQUAL 1 )
		SET(alternate TRUE)
	ELSEIF( ${random_choice_in_0123} STREQUAL 2 )
		SET(alternate FALSE)
		SET_SOURCE_FILES_PROPERTIES(
			${source_list} PROPERTIES COMPILE_FLAGS "${debug_flags}"
		)
	ELSEIF( ${random_choice_in_0123} STREQUAL 3 )
		SET(alternate FALSE)
		SET_SOURCE_FILES_PROPERTIES(
			${source_list} PROPERTIES COMPILE_FLAGS "${release_flags}"
		)
	ELSE( ${random_choice_in_0123} STREQUAL 4 )
		MESSAGE(ERROR "random_choice_in_0123 = ${random_choice_in_0123}")
	ENDIF( ${random_choice_in_0123} STREQUAL 0 )
	#
	IF( alternate )
		SET(debug_list "")
		SET(release_list "")
		SET(count_mod_2 "${random_choice_in_0123}")
		FOREACH(source ${source_list})
			MATH(EXPR count_mod_2 "(${count_mod_2} + 1) % 2")
			IF( count_mod_2 )
				add_to_list(debug_list ${source})
			ELSE( count_mod_2 )
				add_to_list(release_list ${source})
			ENDIF( count_mod_2 )
		ENDFOREACH(source ${source_list})
		SET_SOURCE_FILES_PROPERTIES(
			${debug_list} PROPERTIES COMPILE_FLAGS
			"${debug_flags} -DCPPAD_DEBUG_AND_RELEASE"
		)
		SET_SOURCE_FILES_PROPERTIES(
			${release_list} PROPERTIES COMPILE_FLAGS
			"${release_flags} -DCPPAD_DEBUG_AND_RELEASE"
		)
	ENDIF( alternate )
ENDFUNCTION(random_debug_release program_name source_list)
