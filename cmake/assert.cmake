# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# =============================================================================
# assert(variable)
#
# variable: (in)
# The variable is checked to make sure it is true, and if it is not true
# a fatal error message is printed.
#
MACRO(assert variable)
     IF( NOT ${variable} )
      MESSAGE(FATAL_ERROR
         "Error: ${variable} is false in ${CMAKE_CURRENT_LIST_FILE}"
      )
     ENDIF( NOT ${variable} )
ENDMACRO(assert)
