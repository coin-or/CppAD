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
# Build the introduction/* directory tests 
#
ADD_SUBDIRECTORY(exp_apx)
ADD_SUBDIRECTORY(get_started)
ADD_CUSTOM_TARGET(check_introduction DEPENDS 
	check_introduction_exp_apx 
	check_introduction_get_started
)
