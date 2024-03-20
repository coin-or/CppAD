#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# {xrst_begin get_fadbad.sh}
# {xrst_comment_ch #}
#
# Download and Install Fadbad in Build Directory
# ##############################################
#
# Syntax
# ******
# ``bin/get_fadbad.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# :ref:`Fadbad<fadbad_prefix@Fadbad Home Page>`
# in the CppAD ``build`` directory.
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# Source Directory
# ****************
# The Fadbad source code is downloaded into the sub-directory
# ``external/FADBAD++`` below the distribution directory.
#
# Prefix
# ******
# The :ref:`get_optional.sh@prefix`
# in the file ``bin/get_optional.sh`` is used for this install.
#
# Version
# *******
# This will install the following version of Fadbad
# {xrst_spell_off}
# {xrst_code sh}
version='2.1'
# {xrst_code}
# {xrst_spell_on}
#
# {xrst_end get_fadbad.sh}
# -----------------------------------------------------------------------------
package='fadbad'
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
web_page='http://uning.dk//download'
cppad_dir=`pwd`
# -----------------------------------------------------------------------------
# prefix
eval `grep '^prefix=' bin/get_optional.sh`
if [[ "$prefix" =~ ^[^/] ]]
then
   prefix="$cppad_dir/$prefix"
fi
echo "prefix=$prefix"
# -----------------------------------------------------------------------------
if [ ! -d external ]
then
   echo_eval mkdir external
fi
echo_eval cd external
# -----------------------------------------------------------------------------
if [ ! -e "FADBAD++-$version.tar.gz" ]
then
   echo_eval wget --no-check-certificate $web_page/FADBAD++-$version.tar.gz
fi
if [ -e "FADBAD++" ]
then
   echo_eval rm -r FADBAD++
fi
echo_eval tar -xzf FADBAD++-$version.tar.gz
if [ ! -e "$prefix/include" ]
then
   echo_eval mkdir -p "$prefix/include"
fi
if [ -e "$prefix/include/FADBAD++" ]
then
   echo_eval rm -r "$prefix/include/FADBAD++"
fi
echo_eval cp -r FADBAD++ "$prefix/include/FADBAD++"
# -----------------------------------------------------------------------------
echo "get_$package.sh: OK"
