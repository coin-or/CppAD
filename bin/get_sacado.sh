#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# {xrst_begin get_sacado.sh}
# {xrst_spell
#     trilinos
# }
# {xrst_comment_ch #}
#
# Download and Install Sacado in Build Directory
# ##############################################
#
# Syntax
# ******
# ``bin/get_sacado.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# :ref:`Sacado<sacado_prefix@Sacado Home Page>`
# in the CppAD ``build`` directory.
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# Source Directory
# ****************
# The Sacado source code is downloaded into the sub-directory
# ``external/trilinos.git`` below the distribution directory.
#
# Prefix
# ******
# The :ref:`get_optional.sh@prefix`
# in the file ``bin/get_optional.sh`` is used for this install.
#
# Version
# *******
# This will install the version of Sacado corresponding to the following
# version of Trilinos:
# {xrst_spell_off}
# {xrst_code sh}
version='14-0-0'
# {xrst_code}
# {xrst_spell_on}
#
# Configuration
# *************
# If the file
#
#     ``external/sacado-`` *version* . ``configured``
#
# exists, the configuration will be skipped.
# Delete this file if you want to re-run the configuration.
#
# {xrst_end get_sacado.sh}
# -----------------------------------------------------------------------------
package='sacado'
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
web_page='https://github.com/trilinos/Trilinos/archive/refs/tags'
release_name="trilinos-release-$version"
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
   echo_eval cd external/$release_name/build
   echo_eval make -j $n_job install
   echo "get_$package.sh: OK"
   exit 0
fi
# -----------------------------------------------------------------------------
# libdir
if [ -e /usr/lib64 ]
then
   libdir='lib64'
else
   libdir='lib'
fi
# -----------------------------------------------------------------------------
# change into external directory
if [ ! -d external ]
then
   echo_eval mkdir external
fi
echo_eval cd external
# -----------------------------------------------------------------------------
# create the trilions source directory and change into it
if [ ! -e $release_name ]
then
   wget "$web_page/$release_name.tar.gz"
   tar -xzf "$release_name.tar.gz"
   mv Trilinos-$release_name $release_name
   #
   # see https://github.com/trilinos/Trilinos/issues/11923
   file="$release_name/packages/teuchos/core/src/Teuchos_BigUIntDecl.hpp"
   if ! grep '^#include <cstdint>' $file > /dev/null
   then
      sed -i $file -e 's|#include <iosfwd>|&\n#include <cstdint>|'
   fi
fi
echo_eval cd $release_name
# -----------------------------------------------------------------------------
# change into build sub-directory
if [ ! -e build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
# -----------------------------------------------------------------------------
cmd="cmake \
   -D CMAKE_BUILD_TYPE=RELEASE \
   -D Trilinos_ENABLE_Sacado=ON \
   -D Sacado_ENABLE_TESTS=OFF \
   -D CMAKE_INSTALL_PREFIX=$prefix \
   -D Trilinos_INSTALL_LIB_DIR=$prefix/$libdir \
   .."
echo $cmd
$cmd
echo_eval make -j $n_job install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$configured_flag
echo "get_$package.sh: OK"
