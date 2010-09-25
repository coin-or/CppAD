# ! /bin/bash 
# $Id: build.sh 1705 2010-09-23 11:46:10Z bradbell $
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# exit on error
set -e
# -----------------------------------------------------------------------------
# These values can be changed.
#
# prefix director where CppAD will be install
CPPAD_DIR=$HOME/prefix/cppad
#
# prefix directory where boost, adolc, fadbad, sacado, and ipopt are installed
BOOST_DIR=/usr/include
ADOLC_DIR=$HOME/prefix/adolc
FADBAD_DIR=$HOME/prefix/fadbad
SACADO_DIR=$HOME/prefix/sacado
IPOPT_DIR=$HOME/prefix/ipopt
#
# library path for the ipopt and adolc
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$ADOLC_DIR/lib:$IPOPT_DIR/lib"
# -----------------------------------------------------------------------------
if [ "$1" != "" ]
then
     echo "$1 is not a valid option"
fi
cat << EOF
usage: build.sh option_1 option_2 ...

options
-------
EOF
exit 1
