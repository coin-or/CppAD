# $Id$
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
# Run the cppad_ipopt/speed directory tests 
# Inherit environment from ../CMakeList.txt

# The CMakeLists.txt file in the specified source directory is processed 
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(cppad)
ADD_SUBDIRECTORY(double)
IF ( adolc_prefix )
	ADD_SUBDIRECTORY(adolc)
ENDIF( adolc_prefix )
IF ( fadbad_prefix )
	ADD_SUBDIRECTORY(fadbad)
ENDIF( fadbad_prefix )
IF ( sacado_prefix )
	ADD_SUBDIRECTORY(sacado)
ENDIF( sacado_prefix )
