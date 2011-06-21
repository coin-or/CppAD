# ! /bin/bash 
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
# $begin openmp_run.sh$$ $newlinech #$$
# $spell
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
# $$
# $index OpenMP, compile example$$
# $index compile, OpenMP example$$
# $index example, compile OpenMP$$
#
# $section Compile and Run the OpenMP Test$$
#
# $head Syntax$$
# $codei%openmp/run.sh %test_name%$$
#
# $head Purpose$$
# This script file, $code openmp/run.sh$$, compiles and runs the specified
# speed and correctness test for using OpenMP.
# The possible values for $icode test_name$$ are:
# $cref/example_a11c/example_a11c.cpp/$$,
# $cref/sum_i_inv/sum_i_inv.cpp/$$, and
# $cref/multi_newton/multi_newton.cpp/$$.
#
# $head Parameters$$
# The following are a list of parameters in this file that can
# be changed by directly editing the file
# (there are no command line parameters to the script):
#
# $subhead Compiler Command$$
# The following sets the name of the C++ compiler command:
# $codep
compiler="g++"
# $$
#
# $subhead Version Flag$$
# The following compiler flag requests its version information:
# $codep
version_flag="--version"
# $$
#
# $subhead OpenMP Flag$$
# The following compiler flag requests openmp support
# For the g++ compiler, you can use $code "-fopenmp"$$ for this flag.
# You can run these tests without multi-threading by setting this flag to 
# $code ""$$.
# $codep
openmp_flag=""
# $$
# For g++ version 4.1 and higher, you can use "-fopenmp" for this flag.
#
# $subhead Other Flag$$
# The following other flags will be used during compilation:
# $codep
other_flags="-Werror -DNDEBUG -O2 -Wall"
# $$
#
# $subhead Boost Directory$$
# If the $cref/BoostDir/InstallUnix/BoostDir/$$ is specified on the 
# $cref/configure/InstallUnix/Configure/$$ command line,
# you must add the corresponding include directory; e.g.,
# $codep
if [ -d /usr/include/boost ]
then
	other_flags="-DNDEBUG -O2 -Wall -I/usr/include"
fi
# $$
#
# $subhead Number of Repeats$$
# The following specifies the number of times to repeat
# the calculation corresponding to one timing test. 
# If this 
# is equal to 0, the number of repeats is determined automatically.
# If it is not equal to 0, it must be a positive integer.
# $codep
n_repeat="0"
# $$
#
# $subhead Number of Threads$$
# The following determines a set of number of threads to test.
# Each value in the set must be a positive integer or zero
# (the value zero is used for dynamic thread adjustment).
# If the
# $cref/openmp_flag/openmp_run.sh/Parameters/OpenMP Flag/$$ is equal to "",
# this setting is not used.
# $codep
n_thread_set="1 2 3 4 0"
# $$
#
# $subhead example_a11c$$
# The following setting determine the corresponding command line
# arguments for the $cref/example_a11c.cpp/$$ program:
# $codep
example_a11c_size="10000"
# $$
#
# $subhead sum_i_inv$$
# The following setting determine the corresponding command line
# arguments for the $cref/sum_i_inv.cpp/$$ program:
# $codep
sum_i_inv_mega_sum="1"
# $$
#
# $subhead multi_newton$$
# The following settings determine the corresponding command line
# arguments for the $cref/multi_newton/$$ program:
# $codep
multi_newton_n_zero="10"
multi_newton_n_grid="48"
multi_newton_n_sum="5000"
multi_newton_use_ad="true"
# $$
#
# $head Restrictions$$
# Current this script only runs under the bash shell; e.g., it will not
# run in an MSDOS box.
#
# $childtable%
#	openmp/example_a11c.cpp%
#	openmp/sum_i_inv.cpp%
#	openmp/multi_newton.cpp
# %$$
#
# $end
# ****************************************************************************
# exit on any error
set -e
#
if [ ! -e openmp/run.sh ]
then
	echo "must execute this program from the CppAD distribution directory"
	exit 1
fi
case "$1" in
	example_a11c)
	;;
	sum_i_inv)
	;;
	multi_newton)
	;;
	*)
	echo "usage: openmp/run.sh test_name"
	echo "where test_name is example_a11c, sum_i_inv, or multi_newton"
	exit 1
	;;
esac
test_name="$1"
echo "cd openmp"
cd openmp
#
cmd="$compiler $version_flag"
echo "$cmd"
eval $cmd
#
case "$test_name" in
	example_a11c )
	args="$example_a11c_size"
	;;
	multi_newton )
	args="$multi_newton_n_zero $multi_newton_n_grid $multi_newton_n_sum"
	args="$args $multi_newton_use_ad"
	;;
	sum_i_inv )
	args="$sum_i_inv_mega_sum"
	;;
esac
#
# Compile without OpenMP
#
cmd="$compiler $test_name.cpp -o ${test_name}_no_openmp -I.. $other_flags"
echo "$cmd"
eval $cmd
#
# Run without OpenMP
n_thread=0
cmd="./${test_name}_no_openmp $n_thread $n_repeat $args"
echo "$cmd"
eval $cmd | tee temp.$$
no_openmp=`cat temp.$$ | grep 'repeats_per_sec' | sed -e 's|.*=||'`
#
# clean up (this is source directory)
# echo "rm ${test_name}_no_openmp"
rm ${test_name}_no_openmp
#
echo "" # newline
yes_openmp_set=""
if [ "$openmp_flag" != "" ]
then
	#
	# Compile with OpenMP
	#
	cmd="$compiler $test_name.cpp \
		-o ${test_name}_yes_openmp \
		$openmp_flag -I.. $other_flags"
	cmd=`echo $cmd | sed -e 's|[ \t][ \t]*| |'`
	echo "$cmd"
	eval $cmd 
	#
	# Run with OpenMP
	for n_thread in $n_thread_set
	do
		cmd="./${test_name}_yes_openmp $n_thread $n_repeat $args"
		echo "$cmd"
		eval $cmd | tee temp.$$
		temp=`cat temp.$$ | grep 'repeats_per_sec' | sed -e 's|.*=||'`
		yes_openmp_set="$yes_openmp_set $temp"
		echo "" # newline
	done
	#
	# clean up (this is source directory)
	# echo "rm ${test_name}_yes_openmp"
	rm ${test_name}_yes_openmp
fi
# more cleanup
# echo "rm temp.$$"
rm temp.$$
#
# summary
echo "${test_name}_repeats_per_sec_no_openmp  = $no_openmp"
if [ "$openmp_flag" != "" ]
then
	echo "${test_name}_n_thread_yes_openmp        = [ $n_thread_set ]"
	echo "${test_name}_repeats_per_sec_yes_openmp = [ $yes_openmp_set ]"
fi
