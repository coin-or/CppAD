# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# =============================================================================
# print_variable(variable)
#
# variable: (in)
# The variable name and value is printed
#
MACRO(print_variable variable)
   MESSAGE(STATUS "${variable} = ${${variable}}" )
ENDMACRO(print_variable)
