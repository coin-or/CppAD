#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
# {xrst_begin get_cppadcg.sh}
# {xrst_spell
#     doxygen
#     gen
# }
# {xrst_comment_ch #}
#
# Download and Install CppADCodeGen in Build Directory
# ####################################################
#
# Syntax
# ******
# ``bin/get_cppadcg.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# `cppadcg <https://github.com/joaoleal/CppADCodeGen>`_
# in the CppAD ``build`` directory.
#
# Requirements
# ************
# You must first use :ref:`get_eigen.sh-name` to download and install Eigen.
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# Source Directory
# ****************
# The Cppadcg source code is downloaded into the sub-directory
# ``external/cppadcg.git`` below the distribution directory.
#
# Prefix
# ******
# The :ref:`get_optional.sh@prefix`
# in the file ``bin/get_optional.sh`` is used for this install.
#
# Git Hash
# ********
# This will install the commit of Cppadcg with the following git hash
# {xrst_spell_off}
# {xrst_code sh}
git_hash='fde3752'
# {xrst_code}
# {xrst_spell_on}
# The date corresponding to this commit was 2022-08-14.
#
# Configuration
# *************
# If the file
#
#     ``external/cppadcg-`` *git_hash* . ``configured``
#
# exists, the configuration will be skipped.
# Delete this file if you want to re-run the configuration.
#
# Documentation
# *************
# If you change the setting for ``CREATE_DOXYGEN_DOC`` to ``ON`` ,
# the doxygen documentation for CppADCodeGen will be installed in the directory
#
#     *prefix* / ``share/doc/cppadcg/html``
#
# where *prefix* has the value specified in the
# :ref:`get_optional.sh<get_optional.sh@prefix>` file.
#
# {xrst_end get_cppadcg.sh}
# -----------------------------------------------------------------------------
package='cppadcg'
if [ $0 != "bin/get_$package.sh" ]
then
   echo "bin/get_$package.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
web_page='https://github.com/joaoleal/CppADCodeGen'
cppad_repo=$(pwd)
# -----------------------------------------------------------------------------
# n_job
if which nproc > /dev/null
then
   n_job=$(nproc)
else
   n_job=$(sysctl -n hw.ncpu)
fi
# ----------------------------------------------------------------------------
# prefix
eval `grep '^prefix=' bin/get_optional.sh`
if [[ "$prefix" =~ ^[^/] ]]
then
   prefix="$cppad_repo/$prefix"
fi
echo "prefix=$prefix"
# -----------------------------------------------------------------------------
configured_flag="external/$package-${git_hash}.configured"
echo "Executing get_$package.sh"
if [ -e "$configured_flag" ]
then
   echo "Skipping configuration because $configured_flag exits"
   echo_eval cd external/$package.git/build
   echo_eval make -j $n_job install
   echo "get_$package.sh: OK"
   exit 0
fi
# -----------------------------------------------------------------------------
# Create build/cppad_lib/libcppad_lib.* to aid in cppadcg install
if [ ! -d build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
if [ -e CMakeCache.txt ]
then
   echo_eval rm CMakeCache.txt
fi
echo_eval cmake ..
echo_eval make
echo_eval cd ..
# -----------------------------------------------------------------------------
# Change into external
if [ ! -d external ]
then
   echo_eval mkdir external
fi
echo_eval cd external
# -----------------------------------------------------------------------------
# cppadcg.git
if [ ! -e $package.git ]
then
   echo_eval git clone $web_page $package.git
fi
echo_eval cd $package.git
echo_eval git reset --hard
echo_eval git checkout --quiet $git_hash
#
# cmake/FindCppAD.cmake
# Modify FindCppAD.cmake so can use git repository
# version of CppAD (not yet installed).
cat << EOF > temp.sed
s|IF *( *DEFINED *CPPAD_HOME *)|IF (DEFINED CPPAD_GIT_REPO)\\
   # This setting is used for testing before installing CppAD.\\
   # CPPAD_GIT_REPO is the a CppAD git repository. It is assumed that\\
   # cmake and make have been executed in CPPAD_GIT_REPO/build.\\
   SET(CPPAD_INCLUDE_DIR "\${CPPAD_GIT_REPO}/include" )\\
   SET(CPPAD_LIBRARIES\\
      "\${CPPAD_GIT_REPO}/build/cppad_lib"\\
   )\\
   INCLUDE_DIRECTORIES(\\
      "\${CPPAD_INCLUDE_DIR}"\\
   )\\
   #\\
   IF( NOT EXISTS "\${CPPAD_INCLUDE_DIR}/cppad/cppad.hpp" )\\
      MESSAGE(FATAL_ERROR\\
         "Cannot find CPPAD_GIT_REPO/include/cppad/cppad.hpp"\\
      )\\
   ENDIF()\\
   IF( NOT EXISTS "\${CPPAD_INCLUDE_DIR}/cppad/configure.hpp" )\\
      MESSAGE(FATAL_ERROR\\
         "Cannot find CPPAD_GIT_REPO/include/cppad/configure.hpp"\\
      )\\
   ENDIF()\\
   #\\
   FIND_LIBRARY( CPPAD_LIB_PATH\\
      cppad_lib\\
      PATHS \${CPPAD_LIBRARIES}\\
      NO_DEFAULT_PATH\\
   )\\
   IF( NOT CPPAD_LIB_PATH  )\\
      MESSAGE(FATAL_ERROR\\
         "Cannot find \${library} library below CPPAD_GIT_REPO="\\
         "{CPPAD_GIT_REPO}"\\
      )\\
   ENDIF()\\
   #\\
   SET(CPPAD_FOUND TRUE)\\
\\
ELSEIF (DEFINED CPPAD_HOME)|
EOF
echo_eval git checkout  cmake/FindCppAD.cmake
echo_eval sed -i cmake/FindCppAD.cmake -f temp.sed
#
# include/cppad/cg/base_float.hpp
cat << EOF > temp.sed
/template *<> *\$/! b skip
: loop1
N
/\\n};\$/! b loop1
#
: loop2
N
/\\n}\$/! b loop2
s|.*|CPPAD_NUMERIC_LIMITS(float, cg::CG<float>)|
#
: skip
EOF
echo_eval sed -i include/cppad/cg/base_float.hpp -f temp.sed
#
# include/cppad/cg/base_double.hpp
cat << EOF > temp.sed
/template *<> *\$/! b skip
: loop1
N
/\\n};* *\$/! b loop1
#
: loop2
N
/\\n};* *\$/! b loop2
s|.*|CPPAD_NUMERIC_LIMITS(double, cg::CG<double>)|
#
: skip
EOF
echo_eval sed -i include/cppad/cg/base_double.hpp -f temp.sed
# -----------------------------------------------------------------------------
#  make install
if [ ! -e build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
echo_eval cmake \
   -D CPPAD_GIT_REPO="$cppad_repo" \
   -D CMAKE_INSTALL_PREFIX=$prefix \
   -D EIGEN3_INCLUDE_DIR=$prefix/include \
   -D GOOGLETEST_GIT=ON \
   -D CREATE_DOXYGEN_DOC=OFF \
   ..
echo_eval make -j $n_job install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_repo/$configured_flag
echo "get_$package.sh: OK"
