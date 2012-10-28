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
prefix="$HOME/prefix/cppad"
top_srcdir=`pwd | sed -e 's|.*/||'`
echo_exec cd ..
for dir in $prefix build
do
	if [ -e "$dir" ]
	then
		echo_exec rm -r $dir
	fi
done
echo_exec mkdir build
echo_exec cd build
echo_exec cmake ../$top_srcdir -Dinstall_prefix=$prefix
echo_exec make install
echo 'cmake_run.sh: OK'
exit 0
