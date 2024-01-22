#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# {xrst_begin get_adolc.sh}
# {xrst_spell
#     nov
# }
# {xrst_comment_ch #}
#
# Download and Install Adolc in Build Directory
# #############################################
#
# Syntax
# ******
# ``bin/get_adolc.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# :ref:`Adolc<adolc@Adolc Home Page>`
# in the CppAD ``build`` directory.
#
# Requirements
# ************
# You must first use :ref:`get_colpack.sh-name` to download and install
# the ColPack coloring algorithms (used for sparse matrix derivatives).
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# Source Directory
# ****************
# The Adolc source code is downloaded into the sub-directory
# ``external/adolc.git`` below the distribution directory.
#
# Prefix
# ******
# The :ref:`get_optional.sh@prefix`
# in the file ``bin/get_optional.sh`` is used for this install.
#
# Version
# *******
# This will install the following version of Adolc
# {xrst_spell_off}
# {xrst_code sh}
version='e1fe476'
# {xrst_code}
# {xrst_spell_on}
# This corresponds to the git master on Nov 13, 2020.
#
# Configuration
# *************
# If the file
#
#     ``external/adolc-`` *version* . ``configured``
#
# exists, the configuration will be skipped.
# Delete this file if you want to re-run the configuration.
#
# {xrst_end get_adolc.sh}
# -----------------------------------------------------------------------------
package='adolc'
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
web_page='https://github.com/coin-or/ADOL-C.git'
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
echo_eval cd $package.git
echo_eval git reset --hard
echo_eval git checkout --quiet $version
# -----------------------------------------------------------------------------
system=`uname | tr [A-Z] [a-z] | sed -e 's|\([a-z][a-z]*\).*|\1|'`
# -----------------------------------------------------------------------------
if which autoconf >& /dev/null
then
   echo_eval autoreconf --install --force
fi
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
# -----------------------------------------------------------------------------
flags="--prefix=$prefix --with-colpack=$prefix --libdir=$prefix/$libdir"
flags="$flags --enable-static --enable-shared --enable-atrig-erf"
#
echo_eval ../configure $flags
echo_eval make -j $n_job install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$configured_flag
echo "get_$package: OK"
