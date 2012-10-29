#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/cmake_run.sh" ]
then
	echo "bin/cmake_run.sh: must be executed from its parent directory"
	exit 1
fi
echo_exec() {
     echo $* 
     eval $*
}
# -----------------------------------------------------------------------------
top_srcdir=`pwd | sed -e 's|.*/||'`
echo_exec cd ..
for dir in build $HOME/prefix/cppad
do
	if [ -e "$dir" ]
	then
		echo_exec rm -r $dir
	fi
done
echo_exec mkdir build
echo_exec cd build
#
args="../$top_srcdir"
args="$args  -Dcppad_prefix=$HOME/prefix/cppad"
args="$args  -Dadolc_prefix=$HOME/prefix/adolc"
args="$args  -DCMAKE_VERBOSE_MAKEFILE=1"
#
echo "cmake $args > cmake_run.log"
cmake $args > ../$top_srcdir/cmake_run.log
#
echo "make all >> cmake_run.log"
make all >> ../$top_srcdir/cmake_run.log
#
echo "example/example > cmake_run.log"
example/example >> ../$top_srcdir/cmake_run.log
#
echo "make install > cmake_run.log"
make install >> ../$top_srcdir/cmake_run.log
#
echo 'cmake_run.sh: OK'
exit 0
