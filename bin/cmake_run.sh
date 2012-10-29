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
if [ "$1" != "" ]
then
	args=''
	if [ "$1" == '--verbose' ]
	then
		args="$args  -DCMAKE_VERBOSE_MAKEFILE=1"
	else
		echo 'usage: bin/cmake_run.sh: [--verbose]'
		exit 1
	fi
fi
# -----------------------------------------------------------------------------
top_srcdir=`pwd | sed -e 's|.*/||'`
echo_exec cd ..
list="
	$top_srcdir/cmake_run.log
	$HOME/prefix/cppad
	build
"
for name in $list
do
	if [ -e "$name" ]
	then
		echo_exec rm -r $name
	fi
done
echo_exec mkdir build
echo_exec cd build
log_file="../$top_srcdir/cmake_run.log"
# -----------------------------------------------------------------------------
args="$args  -Dcppad_prefix=$HOME/prefix/cppad"
args="$args  -Dadolc_prefix=$HOME/prefix/adolc"
#
echo "cmake ../$top_srcdir $args >> cmake_run.log"
cmake ../$top_srcdir $args >> $log_file
#
echo "make all >> cmake_run.log"
make all >> $log_file
#
# other test cases
for dir in example test_more
do
	echo "$dir/$dir >> cmake_run.log"
	$dir/$dir >> $log_file
done
#
# print_for is a special case 
echo "print_for/print_for >> cmake_run.log"
print_for/print_for >> $log_file
print_for/print_for | sed -e '/^Test passes/,$d' > junk.1.$$
print_for/print_for | sed -e '1,/^Test passes/d' > junk.2.$$
if diff junk.1.$$ junk.2.$$
then
	rm junk.1.$$ junk.2.$$
	echo "print_for: OK"  >> $log_file
else
	echo "print_for: Error"  >> $log_file
	exit 1
fi
#
echo "make install > cmake_run.log"
make install >> $log_file
#
echo 'cmake_run.sh: OK'
exit 0
