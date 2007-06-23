# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
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
#	fi
#	usr
#	Dir
#	mega
#	inv
#       automatic automatic
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
# $code openmp/run.sh$$
#
# $head Purpose$$
# This script file, $code openmp/run.sh$$, compiles and runs the 
# speed and correctness tests for using OpenMP.
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
# You can run these tests with a compiler that does not support OpenMP
# by setting this flag to "".
# $codep
openmp_flag=""
# $$
# For g++ version 4.1 and higher, you can use "-fopenmp" for this flag.
#
# $subhead Other Flag$$
# The following other flags will be used during compilation:
# $codep
other_flags="-DNDEBUG -O2 -Wall"
# $$
#
# $subhead Boost Directory$$
# If the $cref/BoostDir/InstallUnix/BoostDir/$$ is specified on the 
# $cref/configure/InstallUnix/Configure/$$ command line,
# you must add the corresponding include directory; e.g.,
# $codep
if [ -d /usr/include/boost-1_33_1 ]
then
	other_flags="-DNDEBUG -O2 -Wall -I/usr/include/boost-1_33_1"
fi
# $$
#
# $subhead Number of Repeats$$
# The following specifies the number of times to repeat
# the calculation corresponding to one timing test. 
# If this 
# is equal to "automatic", the number of repeats is determined automatically.
# If it is not equal to "automatic", it must be a positive integer.
# $codep
n_repeat="automatic"
# $$
#
# $subhead Number of Threads$$
# The following determines a set of number of threads to test.
# Each value in the set must be a positive integer or zero
# (the value zero is used for dynamic thread adjustment).
# If the
# $cref/openmp_flag/openmp_run.sh/Purpose/OpenMP Flag/$$ is equal to "",
# this setting is not used.
# $codep
n_thread_set="0 1 2 3 4"
# $$
#
# $subhead example_a11c$$
# The following setting determine the corresponding command line
# arguments for the $cref/example_a11c.cpp/$$ program:
# $codep
example_a11c_size="10000"
# $$
#
# $subhead multi_newton$$
# The following settings determine the corresponding command line
# arguments for the $cref/multi_newton/$$ program:
# $codep
multi_newton_n_zero="10"
multi_newton_n_grid="40"
multi_newton_n_sum="10"
# $$
#
# $subhead sum_i_inv$$
# The following setting determine the corresponding command line
# arguments for the $cref/sum_i_inv.cpp/$$ program:
# $codep
sum_i_inv_mega_sum="1"
# $$
#
# $head Restrictions$$
# Current this script only runs under the bash shell; e.g., it will not
# run in an MSDOS box.
#
# $childtable%
#	openmp/example_a11c.cpp%
#	openmp/multi_newton.cpp%
#	openmp/sum_i_inv.cpp
# %$$
#
# $end
# ****************************************************************************
if [ ! -e openmp/run.sh ]
then
	echo "must execute this program from the CppAD distribution directory"
	exit 1
fi
cd openmp
#
cmd="$compiler $version_flag"
echo "$cmd"
$cmd
#
for name in \
	example_a11c \
	multi_newton \
	sum_i_inv 
do
	case "$name" in
	example_a11c )
	args="$example_a11c_size"
	;;
	multi_newton )
	args="$multi_newton_n_zero $multi_newton_n_grid $multi_newton_n_sum"
	;;
	sum_i_inv )
	args="$sum_i_inv_mega_sum"
	;;
	esac
	#
	# Compile without OpenMP
	#
	cmd="$compiler $name.cpp -o ${name}_no_openmp -I.. $other_flags"
	echo "$cmd"
	$cmd
	#
	# Run without OpenMP
	cmd="./${name}_no_openmp 0 $n_repeat $args"
	echo "$cmd"
	if ! $cmd
	then
		echo "Error in ${name}_no_openmp."
		exit 1
	fi
	echo "" # newline
	if [ "$openmp_flag" != "" ]
	then
		#
		# Compile with OpenMP
		#
		cmd="$compiler $name.cpp \
			-o ${name}_yes_openmp \
			$openmp_flag -I.. $other_flags"
		cmd=`echo $cmd | sed -e 's|[ \t][ \t]*| |'`
		echo "$cmd"
		$cmd
		#
		# Run without OpenMP
		#
		for n_thread in $n_thread_set
		do
			cmd="./${name}_yes_openmp $n_thread $n_repeat $args"
			echo "$cmd"
			if ! $cmd
			then
				echo "Error in ${name}_yes_openmp."
				exit 1
			fi
			echo "" # newline
		done
	fi
done
