# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
# eigen_info()
#
# cppad_has_eigen: (out)
# is 1 (0) if eigen is found (not found)
# If this is 1, the include directory for eigen will automatically be
# added using INCLUDE_DIRECTORIES with the SYSTEM flag.
#
# eigen_LINK_LIBRARIES: (out)
# if cppad_has_eigen is 1,
# is a list of absolute paths to libraries necessary to use eigen
# (should be empty becasuse eigen is a header only library).
#
# This macro may use variables with the name eigen_*
MACRO(eigen_info)
   #
   # check for pkg-config
   IF( NOT PKG_CONFIG_FOUND )
      FIND_PACKAGE(PkgConfig REQUIRED)
   ENDIF( )
   #
   #
   IF( NOT ${use_cplusplus_2014_ok} )
      MESSAGE(MESSAGE "Eigen not supportedL: because c++14 not supported")
      SET(cppad_has_eigen 0)
   ELSE( )
      #
      # eigen_*
      pkg_check_modules(eigen QUIET eigen3 )
      #
      IF( eigen_FOUND )
         MESSAGE(MESSAGE "Eigen found")
         SET(cppad_has_eigen 1)
         INCLUDE_DIRECTORIES( SYSTEM ${eigen_INCLUDE_DIRS} )
      ELSE( )
         MESSAGE(MESSAGE "Eigen not Found: eigen3.pc not in PKG_CONFIG_PATH")
         SET(cppad_has_eigen 0)
      ENDIF( )
   ENDIF( )
   #
ENDMACRO(eigen_info)
