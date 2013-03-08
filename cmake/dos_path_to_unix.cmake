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
