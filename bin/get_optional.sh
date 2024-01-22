#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# {xrst_begin get_optional.sh}
# {xrst_comment_ch #}
#
# Download and Install The CppAD Optional Packages
# ################################################
#
# Syntax
# ******
# ``bin/get_optional.sh``
#
# Purpose
# *******
# If you are using Unix, this command will download and install
# all of the optional packages that can be used with CppAD.
#
# Distribution Directory
# **********************
# This command must be executed in the
# :ref:`download@Distribution Directory` .
#
# prefix
# ******
# This is the prefix for installing the optional packages.
# It can be changed by editing its setting of *prefix* below
# in the file ``bin/get_optional.sh`` .
# Note that there can only be one setting that is not commented out with
# a  at the start of its line.
#
# Absolute Path
# =============
# If the first character in the prefix is a ``/`` ,
# it is an absolute path; e.g., the following setting:
# {xrst_spell_off}
# {xrst_code sh}
# prefix="$HOME/prefix/cppad"
# {xrst_code}
# {xrst_spell_on}
#
# Relative Path
# =============
# If the first character in the prefix is **not** a ``/`` ,
# it is a path relative to the distribution directory;
# e.g., the following setting:
# {xrst_spell_off}
# {xrst_code sh}
prefix="build/prefix"
# {xrst_code}
# {xrst_spell_on}
#
# Configuration
# =============
# If you do an install and then change the *prefix* ,
# you should delete all the files listed by the following command:
#
#     ``ls external/`` * . ``configured``
#
# get_optional.log
# ****************
# This file contains the standard out output for each of the optional scripts
# in the order that they are executed.
#
# get_optional.err
# ****************
# This file contains the standard error output for each of the optional scripts
# in the order that they are executed.
#
# Contents
# ********
# {xrst_toc_table
#    bin/get_adolc.sh
#    bin/get_cppadcg.sh
#    bin/get_colpack.sh
#    bin/get_eigen.sh
#    bin/get_fadbad.sh
#    bin/get_ipopt.sh
#    bin/get_sacado.sh
# }
#
# {xrst_end get_optional.sh}
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_optional.sh" ]
then
   echo "bin/get_optional.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
if [ -e 'get_optional.log' ]
then
   echo_eval rm get_optional.log
fi
if [ -e 'get_optional.err' ]
then
   echo_eval rm get_optional.err
fi
# -----------------------------------------------------------------------------
list='colpack adolc eigen fadbad ipopt sacado cppadcg'
for package in $list
do
   echo "bin/get_${package}.sh 1>> get_optional.log 2>> get_optional.err"
   if bin/get_${package}.sh 1>> get_optional.log 2>> get_optional.err
   then
      echo "bin/get_${package}.sh: OK"
   else
      echo "bin/get_${package}.sh: Error; try following:"
      echo '  tail ./get_optional.err'
      exit 1
   fi
done
# -----------------------------------------------------------------------------
echo "get_optional: OK"
