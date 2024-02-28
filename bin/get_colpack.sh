#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# {xrst_begin get_colpack.sh}
# {xrst_comment_ch #}
#
# Download and Install ColPack in Build Directory
# ###############################################
#
# Syntax
# ******
# ``bin/get_colpack.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# :ref:`Colpack<colpack_prefix@Colpack Home Page>`
# in the CppAD ``build`` directory.
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# Source Directory
# ****************
# The ColPack source code is downloaded into the sub-directory
# ``external/colpack.git`` below the distribution directory.
#
# Prefix
# ******
# The :ref:`get_optional.sh@prefix`
# in the file ``bin/get_optional.sh`` is used for this install.
#
# Version
# *******
# This will install the following version of ColPack
# {xrst_spell_off}
# {xrst_code sh}
version='1.0.10'
# {xrst_code}
# {xrst_spell_on}
#
# Configuration
# *************
# If the file
#
#     ``external/colpack-`` *version* . ``configured``
#
# exists, the configuration will be skipped.
# Delete this file if you want to re-run the configuration.
#
# {xrst_end get_colpack.sh}
# -----------------------------------------------------------------------------
package='colpack'
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
web_page='https://github.com/CSCsw/ColPack.git'
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
   echo_eval cd external/$package.git
   echo_eval make -j $n_job install
   echo "get_$package.sh: OK"
   exit 0
fi
# --------------------------------------------------------------------------
if [ -e /usr/lib64 ]
then
   libdir='lib64'
else
   libdir='lib'
fi
# -----------------------------------------------------------------------------
if [ ! -d external ]
then
   echo_eval mkdir external
fi
echo_eval cd external
# -----------------------------------------------------------------------------
if [ ! -e "$package.git" ]
then
   echo_eval git clone $web_page $package.git
fi
# -----------------------------------------------------------------------------
echo_eval cd $package.git
echo_eval git checkout --quiet v$version
# -----------------------------------------------------------------------------
if which autoconf >& /dev/null
then
   echo_eval libtoolize
   echo_eval autoreconf --install --force
else
   echo "Error: autoconf and libtool required for this script"
   exit 1
fi
# -----------------------------------------------------------------------------
system_name=`uname | sed -e 's|\(......\).*|\1|'`
if [ "$system_name" == 'CYGWIN' ]
then
   lib_type='--enable-static --disable-shared'
   echo_eval ls -l ./configure
   echo_eval chmod +x ./configure
else
   lib_type='--enable-static --enable-shared'
fi
echo_eval ./configure \
   --prefix=$prefix \
   --libdir=$prefix/$libdir \
   $lib_type
#
echo_eval touch $cppad_dir/$configured_flag
echo_eval make -j $n_job install
# -----------------------------------------------------------------------------
echo "get_$package: OK"
