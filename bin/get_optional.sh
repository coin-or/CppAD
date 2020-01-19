#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
# $OMhelpKeyCharacter=@
# @begin get_optional.sh@@ @newlinech #@@
# @spell
#   CppAD
# @@
#
# @section Download and Install The CppAD Optional Packages@@
#
# @head Syntax@@
# @code bin/get_optional.sh@@
#
# @head Purpose@@
# If you are using Unix, this command will download and install
# all of the optional packages that can be used with CppAD.
#
# @head Distribution Directory@@
# This command must be executed in the
# @cref/distribution directory/download/Distribution Directory/@@.
#
# @head prefix@@
# The prefix for installing the optional packages is
# @srccode%sh%
prefix="$HOME/prefix/cppad"
# %@@
# It can be changed by editing the line above in the file
# @code bin/get_optional.sh@@
#
# $head Package Scripts$$
# The scripts for installing the optional packages are listed below.
# The output for each of these scripts is suppressed.
# You can get the output for an particular script running it.
#
# @childtable%
#   bin/get_colpack.sh%
#   bin/get_adolc.sh%
#   bin/get_eigen.sh%
#   bin/get_fadbad.sh%
#   bin/get_ipopt.sh%
#   bin/get_sacado.sh
# %@@
#
# @end
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
# -----------------------------------------------------------------------------
list='colpack adolc fadbad eigen ipopt sacado'
for package in $list
do
    echo "bin/get_${package}.sh"
    bin/get_${package}.sh 1> /dev/null 2> /dev/null
done
# -----------------------------------------------------------------------------
echo "get_optional: OK"
