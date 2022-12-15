# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# dos_path_to_unix(dos_path unix_path)
#
# dos_path: (in)
# is the value of the path we are converting to unix format; i.e.,
# all \ characters are replaced by / characters.
#
# unix_path: (out)
# is the variable where the result of the conversion is placed.
#
MACRO(dos_path_to_unix dos_path unix_path)
   STRING(REGEX REPLACE "[\\]" "/" ${unix_path} "${dos_path}" )
ENDMACRO(dos_path_to_unix dos_path unix_path)
