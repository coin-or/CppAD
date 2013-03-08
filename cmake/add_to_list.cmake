# $Id$
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
# =============================================================================
# add_to_list(variable_list constant_value)
#
# variables_list: (in/out)
# The variable containing the list of values. 
# The original list may be ""; i.e., the empty list.
#
# constant_value: (in)
# Is the value we are adding to the list. This value cannot be empty.
#
MACRO(add_to_list variable_list constant_value )
     IF( "${${variable_list}}" STREQUAL "" )
          SET( ${variable_list} ${constant_value} )
     ELSE( "${${variable_list}}" STREQUAL "" )
          SET( ${variable_list} ${${variable_list}} ${constant_value} )
     ENDIF( "${${variable_list}}" STREQUAL "" )
ENDMACRO(add_to_list)
