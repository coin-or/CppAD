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
if [ ! -e "bin/run_cmake.sh" ]
then
	echo "bin/run_cmake.sh: must be executed from its parent directory"
	exit 1
fi
echo_exec() {
     echo $* 
     eval $*
}
args=''
run_tests='no'
if [ "$1" != "" ]
then
	if [ "$1" == '--verbose' ]
	then
		args="$args  -DCMAKE_VERBOSE_MAKEFILE=1"
	elif [ "$1" == '--run_tests' ]
	then
		run_tests='yes'
	else
		echo 'usage: bin/run_cmake.sh: [--verbose] [--run_tests]'
		exit 1
	fi
fi
# -----------------------------------------------------------------------------
top_srcdir=`pwd | sed -e 's|.*/||'`
echo_exec cd ..
list="
	$top_srcdir/run_cmake.log
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
log_file="../$top_srcdir/run_cmake.log"
# -----------------------------------------------------------------------------
for package in cppad adolc eigen ipopt
do
	args="$args  -D${package}_prefix=$HOME/prefix/$package"
done
#
echo "cmake ../$top_srcdir $args >> run_cmake.log"
cmake ../$top_srcdir $args >> $log_file
#
echo "make all >> run_cmake.log"
make all >> $log_file
#
if [ "$run_tests" == 'yes' ]
then
	#
	# standard test cases
	echo "example/example >> run_cmake.log"
	example/example >> $log_file
	#
	echo "test_more/test_more >> run_cmake.log"
	test_more/test_more >> $log_file
	# 
	echo "cppad_ipopt/example/ipopt_example >> run_cmake.log"
	cppad_ipopt/example/ipopt_example >> $log_file
	#
	# print_for is a special case 
	echo "print_for/print_for >> run_cmake.log"
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
fi
#
echo "make install > run_cmake.log"
make install >> $log_file
#
echo 'run_cmake.sh: OK'
exit 0
