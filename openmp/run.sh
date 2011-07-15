#! /bin/bash -e 
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# $OMhelpKeyCharacter=@
# @begin openmp_run.sh@@ @newlinech #@@
# @spell
#	std
#	Wshadow
#	num
#	Werror
#	fi
#	usr
#	Dir
#	mega
#	inv
#	openmp
#	vec
#	exe
#	rm
#	false false
#	true true
#	fopenmp
#	cpp
# @@
# @index OpenMP, compile example@@
# @index compile, OpenMP example@@
# @index example, compile OpenMP@@
#
# @section Compile and Run the OpenMP Test@@
#
# @head Syntax@@
# @codei%openmp/run.sh %test_name%@@
#
# @head Purpose@@
# This script file, @code openmp/run.sh@@, compiles and runs the specified
# example, and correctness test for using OpenMP.
# In addition, some of the tests are also speed tests.
# @table
# @icode test_name@@                      @cnext @icode speed_test@@  @rnext
# @cref/example_a11c/example_a11c.cpp/@@  @cnext no                   @rnext
# @cref/parallel_ad/parallel_ad.cpp/@@    @cnext no                   @rnext
# @cref/sum_i_inv/sum_i_inv.cpp/@@,       @cnext yes                  @rnext
# @cref/multi_newton/multi_newton.cpp/@@  @cnext yes
# @tend
#
# @head Parameters@@
# Below are a list of parameters in this file that can
# be changed by directly editing the file
# (there are no command line arguments to @code openmp/run.sh@@):
#
# @head Compiler Command@@
# The variable @code compiler@@ specifies the C++ compiler command:
# @codep
compiler="g++"
# @@
#
# @head OpenMP Flag@@
# The variable @code openmp_flag@@ is a compiler flag
# that requests openmp support.
# If it is equal to @code ""@@, the tests will be run without OpenMP.
# For g++ version 4.1 and higher, 
# if it is equal to "-fopenmp", will compile the program with OpenMP support.
# @codep
openmp_flag="-fopenmp"
# @@
#
# @head Other Flag@@
# The variable @code speed_flags@@ specifies the other compiler flags
# for the tests @code sum_i_inv@@ and @code multi_newton@@.
# The variable @code correct_flags@@ specifies the other compiler flags
# for the tests @code example_a11c@@ and @code parallel_ad@@.
# @codep
speed_flags="-DNDEBUG -O2 -Wall -ansi -pedantic-errors -std=c++98 -Wshadow"
correct_flags="-g -Wall -ansi -pedantic-errors -std=c++98 -Wshadow"
# @@
#
# @head Number of Repeats@@
# The variable @code n_repeat@@ specifies the number of times to repeat
# the test (only used for the speed tests).
# It must be a non-negative integer.
# If it is 0, the number of repeats is determined automatically.
# @codep
n_repeat="0"
# @@
#
# @head Maximum Number of Threads@@
# The variable @code max_num_threads@@ specifies
# the maximum number of threads for this test.
# This value must be a positive-negative integer.
# (It has no effect when the tests are run with OpenMP; see
# @cref/OpenMP flag/openmp_run.sh/OpenMP Flag/@@.)
# @codep
max_num_threads="2"
# @@
#
# @head sum_i_inv@@
# The variable @code sum_i_inv_mega_sum@@ determines the corresponding 
# command line arguments for the @cref/sum_i_inv.cpp/@@ program:
# @codep
sum_i_inv_mega_sum="1"
# @@
#
# @head multi_newton@@
# The following variables determine the corresponding command line
# arguments for the @cref/multi_newton/@@ program:
# @codep
multi_newton_n_zero="20"
multi_newton_n_sum="500"
multi_newton_use_ad="true"
let multi_newton_n_grid="$max_num_threads*15"
# @@
#
# @head Restrictions@@
# Current this script only runs under the bash shell; e.g., it will not
# run in an MSDOS box.
#
# @childtable%
#	openmp/example_a11c.cpp%
#	openmp/parallel_ad.cpp%
#	openmp/sum_i_inv.cpp%
#	openmp/multi_newton.cpp
# %@@
#
# @end
# ****************************************************************************
if [ "$0" != openmp/run.sh ]
then
	echo "openmp/run.sh: must be executed from its parent directory"
	exit 1
fi
case "$1" in
	example_a11c)
	other_flags="$correct_flags"
	args=""
	;;
	parallel_ad)
	other_flags="-I.. $correct_flags"
	args=""
	;;
	sum_i_inv)
	other_flags="-I.. $speed_flags"
	args="$n_repeat $sum_i_inv_mega_sum"
	;;
	multi_newton)
	other_flags="-I.. $speed_flags"
	args="$n_repeat $multi_newton_n_zero $multi_newton_n_grid"
	args="$args $multi_newton_n_sum $multi_newton_use_ad"
	;;
	*)
	echo "usage: openmp/run.sh test_name"
	echo "where test_name is one of the following:"
	echo "	example_a11c, parallel_ad, sum_i_inv, or multi_newton"
	exit 1
	;;
esac
test_name="$1"
#
echo "cd openmp"
cd openmp
#
error_occurred="no"
# ----------------------------------------------------------------------------
# Compile without OpenMP
#
cmd="$compiler $test_name.cpp -o ${test_name}_no_openmp $other_flags"
echo "$cmd"
eval $cmd
#
n_thread=0
args_plus="$n_thread $args"
#
# Run without OpenMP
cmd="./${test_name}_no_openmp $args_plus"
echo "$cmd"
if ! $cmd > temp.$$
then
	error_occurred="yes"
fi
cat temp.$$
no_openmp=`cat temp.$$ | grep 'repeats_per_sec' | sed -e 's|.*=||'`
#
# clean up (this is a source directory)
# echo "rm ${test_name}_no_openmp"
rm ${test_name}_no_openmp
# echo "rm temp.$$"
rm temp.$$
if [ "$error_occurred" = "yes" ]
then
	exit 1
fi
if [ "$openmp_flag" == "" ]
then
	exit 0
fi
# ---------------------------------------------------------------------------
# Compile with OpenMP
cmd="$compiler $test_name.cpp \
	-o ${test_name}_yes_openmp \
	$openmp_flag $other_flags"
cmd=`echo $cmd | sed -e 's|[ \t][ \t]*| |'`
echo "$cmd"
eval $cmd 
#
cmd="./${test_name}_yes_openmp $args_plus"
echo "$cmd"
if ! $cmd > temp.$$
then
	error_occurred="yes"
fi
cat temp.$$
dynamic_openmp=`cat temp.$$ | grep 'repeats_per_sec' | sed -e 's|.*=||'`
dynamic_n_thread=`cat temp.$$ | grep 'n_thread' | sed -e 's|.*=||'`
#
if [ "$test_name" == "parallel_ad" ]  || \
   [ "$test_name" == "example_a11c" ] || \
   [ "$error_occurred" == "yes" ]
then
	# clean up (this is a source directory)
	# echo "rm ${test_name}_yes_openmp"
	rm ${test_name}_yes_openmp
	# echo "rm temp.$$"
	rm temp.$$
	if [ "$error_occurred" = "yes" ]
	then
		exit 1
	fi
	exit 0
fi
# ---------------------------------------------------------------------------
# Specify number of threads
#
# n_thread_set = 1 2 ... max_num_threads
n_thread_set=""
n_thread=1
while (($n_thread <= "$max_num_threads" ))
do
	n_thread_set="$n_thread_set $n_thread"
	let n_thread=$n_thread+1
done
echo "" # newline
yes_openmp_set=""
ok_set=""
#
# all cases up to max_num_threads
for n_thread in $n_thread_set
do
	cmd="./${test_name}_yes_openmp $n_thread $args"
	echo "$cmd"
	if ! $cmd > temp.$$
	then
		error_occurred="yes"
	fi
	cat temp.$$
	if [ "$error_occurred" = "yes" ]
	then
		#
		# clean up (this is source directory)
		# echo "rm ${test_name}_yes_openmp"
		rm ${test_name}_yes_openmp
		# echo "rm temp.$$"
		rm temp.$$
		exit 1
	fi
	temp=`cat temp.$$ | grep 'repeats_per_sec' | sed -e 's|.*=||'`
	if [ "$temp" != "" ]
	then
		yes_openmp_set="$yes_openmp_set $temp"
		ok_set="$ok_set $n_thread"
	fi
	echo "" # newline
done
#
# clean up (this is source directory)
# echo "rm ${test_name}_yes_openmp"
rm ${test_name}_yes_openmp
# echo "rm temp.$$"
rm temp.$$
# ---------------------------------------------------------------------------
# Summary
echo "repeats_per_sec_${test_name}_no_openmp  = $no_openmp"
echo "n_thread_${test_name}_dynamic_openmp        = $dynamic_n_thread"
echo "repeats_pre_sec_${test_name}_dynamic_openmp = $dynamic_openmp"
echo "n_thread_${test_name}_yes_openmp            = [ $ok_set ]"
echo "repeats_per_sec_${test_name}_yes_openmp     = [ $yes_openmp_set ]"
