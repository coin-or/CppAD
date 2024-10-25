#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
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
# ``external/Ipopt.git`` below the distribution directory.
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
ipopt_version='3.14.16'
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
# ----------------------------------------------------------------------------
ipopt_url='https://github.com/coin-or/Ipopt'
# ----------------------------------------------------------------------------
#
# echo_eval
function echo_eval() {
   echo $*
   eval $*
}
#
# clone_url_name_version
# creates ./name.git from corresponding url and version
# $1 = url
# $2 = name
# $3 = version
function clone_url_name_version() {
   if [ ! -e $2.git ]
   then
      echo_eval git clone $1.git $2.git
   fi
   echo_eval cd $2.git
   echo_eval git reset --hard
   echo_eval git fetch origin
   echo_eval git checkout --quiet $3
   if [ ! -e build ]
   then
      echo_eval mkdir build
   fi
   cd ..
}
# ----------------------------------------------------------------------------
if [ $0 != 'bin/get_ipopt.sh' ]
then
   echo 'bin/get_ipopt.sh: must be executed from its parent directory'
   exit 1
fi
# ----------------------------------------------------------------------------
#
# n_job
if which nproc >& /dev/null
then
   n_job=$(nproc)
else
   n_job=$(sysctl -n hw.ncpu)
fi
#
# configured_flag
configured_flag="external/Ipopt-${ipopt_version}.configured"
echo "Executing get_ipopt.sh"
if [ -e "$configured_flag" ]
then
   echo "Skipping configuration because $configured_flag exits"
   echo_eval cd external/Ipopt.git/build
   make -j $n_job install
   echo "get_ipopt.sh: OK"
   exit 0
fi
if ls external/Ipopt-*.configured > /dev/null
then
   echo_eval rm -rf external/Mumps.git
   echo_eval rm -rf external/ASL.git
   echo_eval rm external/Ipopt-*.configured
fi
#
# prefix
eval `grep '^prefix=' bin/get_optional.sh`
if [[ "$prefix" =~ ^[^/] ]]
then
   prefix="$(pwd)/$prefix"
fi
echo "prefix=$prefix"
#
# external
if [ ! -e external ]
then
   mkdir external
fi
echo_eval cd external
#
# configure_all
configure_all="--disable-dependency-tracking"
configure_all+=" --prefix=$prefix"
configure_all+=" --enable-shared"
#
# external/build_type/Ipopt.git
name='Ipopt'
clone_url_name_version $ipopt_url $name "releases/$ipopt_version"
#
# external/build_type/ASL.git
# external/build_type/mumps.git
for name in 'ASL' 'Mumps'
do
   # clone_url_name_version
   line=$(grep "ThirdParty/$name" 'Ipopt.git/.coin-or/Dependencies')
   url=$(echo $line | awk '{print $2}' )
   version=$(echo $line | awk '{print $3}' )
   clone_url_name_version $url $name $version
   #
   # get.$name
   cd $name.git
   if [ -e "./get.$name" ]
   then
      #
      if [ ! -e "./get.$name.done" ]
      then
         echo_eval ./get.$name
         touch ./get.$name.done
      fi
   fi
   cd ..
done
#
# Install ASL
echo_eval cd ASL.git/build
echo_eval ../configure $configure_all
echo_eval make -j $n_job install
cd ../..
#
# Install Mumps
configure_mumps="$configure_all"
if [ "$(uname)" == 'Darwin' ]
then
   if which brew
   then
      metis_libdir=$(brew --prefix)/lib
      metis_incdir=$(brew --prefix)/include
      if [ ! -e "$metis_libdir/libmetis.dylib" ]
      then
         echo 'MacOS+brew: Cannot find metis library directory'
      fi
      if [ ! -e "$metis_incdir/metis.h" ]
      then
         echo 'MacOS+brew: Cannot find metis include directory'
      fi
      configure_mumps+=" --with-metis-lflags='-L$metis_libdir -lmetis'"
      configure_mumps+=" --with-metis-cflags='-I$metis_incdir'"
   fi
fi
cd Mumps.git/build
echo_eval ../configure $configure_mumps
echo_eval make -j $n_job install
cd ../..
#
# Install Ipopt
cd Ipopt.git/build
echo_eval ../configure $configure_all --disable-java
echo_eval make -j $n_job install
cd ../..
#
# configured_flag
cd ..
touch $configured_flag
# ----------------------------------------------------------------------------
echo 'get_ipopt.sh: OK'
exit 0
