# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build and install the cppad_lib shared library
#
# string(REGEX REPLACE <regular_expression>
#        <replace_expression> <output variable>
#        <input> [<input>...])
#
# (year - 2000) dot (remove leading 0 from mmdd) dot (release)
# Note that the dot (release) is optional
# Also name that when mmdd is 0000 get (year - 2000) dot dot (release)
STRING(REGEX REPLACE
	"20([0-9][0-9])0*([0-9]*)([.]?[0-9]*)"
	"\\1.\\2\\3"
	soversion
	${cppad_version}
)
# In case where mmdd is 0000, dot (release) is present. Convert to
# (year - 2000) dot 0 dot (release)
STRING(REGEX REPLACE
	"([.][.])"
	".0."
	soversion
	${soversion}
)
MESSAGE(STATUS "soversion=${soversion}")
#
#  add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL]
#	source1 source2 ... sourceN)
# )
ADD_LIBRARY( cppad_lib SHARED
	cppad_colpack.cpp
)
# set_target_properties(target1 target2 ...
# PROPERTIES prop1 value1 prop2 value2 ...)
SET_TARGET_PROPERTIES( cppad_lib PROPERTIES SOVERSION ${soversion} )
#
# install(TARGETS myExe mySharedLib myStaticLib
#	RUNTIME DESTINATION bin
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib/static)
INSTALL(TARGETS cppad_lib DESTINATION ${cppad_abs_libdir})
