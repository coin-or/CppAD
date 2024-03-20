#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# {xrst_begin get_ipopt.sh}
# {xrst_spell
#     blas
#     lapack
# }
# {xrst_comment_ch #}
#
# Download and Install Ipopt in Build Directory
# #############################################
#
# Syntax
# ******
# ``bin/get_ipopt.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# `Ipopt <https://coin-or.github.io/Ipopt/>`_ in the
# CppAD ``build`` directory.
#
# Requirements
# ************
# This It is assumed that a copy of the Blas and Lapack is installed on
# the system.
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# Source Directory
# ****************
# The Ipopt source code is downloaded and compiled in the sub-directory
# ``external/Ipopt-`` *version* below the distribution directory.
#
# Prefix
# ******
# The :ref:`get_optional.sh@prefix`
# in the file ``bin/get_optional.sh`` is used for this install.
#
# Version
# *******
# This will install the following version of Ipopt
# {xrst_spell_off}
# {xrst_code sh}
version='3.13.2'
# {xrst_code}
# {xrst_spell_on}
#
# Configuration
# *************
# If the file
#
#     ``external/ipopt-`` *version* . ``configured``
#
# exists, the configuration will be skipped.
# Delete this file if you want to re-run the configuration.
#
# {xrst_end get_ipopt.sh}
# -----------------------------------------------------------------------------
package='ipopt'
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
coinbrew='https://raw.githubusercontent.com/coin-or/coinbrew/master/coinbrew'
cppad_dir=`pwd`
# -----------------------------------------------------------------------------
# n_proc
if which nproc >& /dev/null
then
   n_job=$(nproc)
else
   n_job=$(sysctl -n hw.ncpu)
fi
# -----------------------------------------------------------------------------
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
   echo_eval cd external
   ./coinbrew -j $n_job install Ipopt --no-prompt
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
if [ ! -e coinbrew ]
then
   echo_eval wget $coinbrew
   echo_eval chmod +x coinbrew
fi
if [ ! -e Ipoot ]
then
   ./coinbrew fetch Ipopt@$version --no-prompt
fi
# -----------------------------------------------------------------------------
# klugde necessary until coin or mumps fixes this problem
cat << EOF > junk.f
      program junk
      print*, "Hello World"
      end
EOF
if gfortran -c -fallow-argument-mismatch junk.f >& /dev/null
then
   echo 'Adding -fallow-argument-mismatch to Mumps fortran compiler flags'
   ADD_FCFLAGS='ADD_FCFLAGS=-fallow-argument-mismatch'
else
   ADD_FCFLAGS=''
fi
# -----------------------------------------------------------------------------
echo_eval ./coinbrew -j $n_job build Ipopt@$version \
   --prefix=$prefix --test --no-prompt --verbosity=3 $ADD_FCFLAGS
echo_eval ./coinbrew -j $n_job install Ipopt@$version \
   --no-prompt
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$configured_flag
echo "get_$package.sh: OK"
