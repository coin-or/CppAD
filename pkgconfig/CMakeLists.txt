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
# Build the cppad pkg-config information files.
# Inherit environment from ../CMakeLists.txt
#
# Copy a file to another location and modify its contents.
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(cmake.cppad.pc             cppad.pc )
CONFIGURE_FILE(cmake.cppad-uninstalled.pc cppad-uninstalled.pc)

# During install copy cppad.pc to CMAKE_INSTALL_PREFIX/
# install(FILES files... DESTINATION <dir>
#	[PERMISSIONS permissions...]
#	[CONFIGURATIONS [Debug|Release|...]]
#	[COMPONENT <component>]
#	[RENAME <name>] [OPTIONAL])
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/cppad.pc DESTINATION ${cppad_datadir})

