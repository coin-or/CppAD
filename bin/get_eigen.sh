#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
# {xrst_begin get_eigen.sh}
# {xrst_comment_ch #}
#
# Download and Install Eigen in Build Directory
# #############################################
#
# Syntax
# ******
# ``bin/get_eigen.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# :ref:`Eigen<eigen@Eigen Home Page>`
# in the CppAD ``build`` directory.
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# Source Directory
# ****************
# The Eigen source code is downloaded into the sub-directory
# ``external/eigen.git`` below the distribution directory.
#
# Prefix
# ******
# The :ref:`get_optional.sh@prefix`
# in the file ``bin/get_optional.sh`` is used for this install.
#
# Version
# *******
# This will install the following version of Eigen
# {xrst_spell_off}
# {xrst_code sh}
# This was the mater version on 2024-03-24
# 'a39ade4ccf99df845ec85c580fbbb324f71952fa'
version='a39ade4'
# {xrst_code}
# {xrst_spell_on}
#
# Configuration
# *************
# If the file
#
#     ``external/eigen-`` *version* . ``configured``
#
# exists, the configuration will be skipped.
# Delete this file if you want to re-run the configuration.
#
# {xrst_end get_eigen.sh}
# -----------------------------------------------------------------------------
package='eigen'
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
web_page='https://gitlab.com/libeigen/$package.git'
cppad_dir=`pwd`
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
   prefix="$cppad_dir/$prefix"
fi
echo "prefix=$prefix"
# -----------------------------------------------------------------------------
configured_flag="external/$package-${version}.configured"
echo "Executing get_$package.sh"
if [ -e "$configured_flag" ]
then
   echo "Skipping configuration because $configured_flag exits"
   echo_eval cd external/$package.git/build
   echo_eval make -j $n_job install
   if [ -e $prefix/include/Eigen ]
   then
      echo_eval rm $prefix/include/Eigen
   fi
   echo_eval ln -s $prefix/include/eigen3/Eigen $prefix/include/Eigen
   echo "get_$package.sh: OK"
   exit 0
fi
# -----------------------------------------------------------------------------
if [ ! -d external ]
then
   echo_eval mkdir external
fi
echo_eval cd external
# -----------------------------------------------------------------------------
if [ ! -e $package.git ]
then
   echo_eval git clone $web_page $package.git
fi
# -----------------------------------------------------------------------------
echo_eval cd $package.git
echo_eval git checkout master
echo_eval git checkout --quiet $version
if [ ! -e build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
echo_eval cmake -D CMAKE_INSTALL_PREFIX=$prefix ..
echo_eval make -j $n_job install
if [ -e $prefix/include/Eigen ]
then
   echo_eval rm $prefix/include/Eigen
fi
echo_eval ln -s $prefix/include/eigen3/Eigen $prefix/include/Eigen
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$configured_flag
echo "get_$package.sh: OK"
