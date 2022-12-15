# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
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
