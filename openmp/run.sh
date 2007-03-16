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
# $syntax%openmp/run.sh %n_thread% %repeat% %openmp% %optimize%$$
#
# $head Purpose$$
# The script file $code openmp/run.sh$$ compiles and runs the openmp
# test and example program.
#
# $head n_thread$$
# If the argument $italic n_thread$$ is $code automatic$$,
# the number of OpenMP threads is automatically determined.
# Otherwise, this argument specifies the number of threads to use
# for the test.
#
# $head repeat$$
# If the argument $italic repeat$$ is $code automatic$$,
# the number of times to repeat the calculation is determined automatically.
# In this case the timing results are reported as execution speeds in
# times per second that the set of zeros are calculated.
# $pre
#
# $$
# If the argument $italic repeat$$ is not $code automatic$$,
# it must be the a positive integer that specifies the number of times
# to repeat the calculation.
# In this case, the total time is represented by the difference in the 
# unix $code date$$ command between when the program is started and 
# when it ends.
#
# $head openmp$$
# The argument $italic openmp$$ is either $code true$$ or $code false$$.
# If it is true, the program will be compiled with OpenMP multi-threading.
# Otherwise it is complied to run without multi-threading. Thus you
# can easily test the speed for both cases on your system.
#
# $head optimize$$
# The argument $italic optimize$$ is either $code true$$ or $code false$$.
# If it is true, the program will be compiled with optimization flags.
# Otherwise it is complied with debugging flags.
#
# $head Restrictions$$
# Current this script only runs under the bash shell; i.e., it will not
# run in an MSDOS box.
#
# $head Example$$
# The two example runs below were run in quick succession on a machine
# with $code g++ --version$$ equal to
# $codep
#	g++ (GCC) 4.1.1 20070105 (Red Hat 4.1.1-51)
# $$
#
#
# $subhead Without OpenMP$$
# The following is an example $code run.sh$$ command 
# with $italic openmp$$ false and $italic optimize$$ true:
# $codep
#	openmp/run.sh automatic automatic false true
# $$
# The following is the corresponding output
# $codep
#	g++ -I.. multi_newton.cpp -o multi_newton  -DNDEBUG -O2
#	./multi_newton
#	_OPENMP is not defined, running in single tread mode
#	n_grid           = { 20, 40 }
#	Execution Speed  = { 2729, 1823 }
#	Correctness Test Passed
#	rm multi_newton
# $$	
#
# $subhead With OpenMP$$
# The following is an example $code run.sh$$ command 
# with $italic openmp$$ true and $italic optimize$$ true:
# $codep
#	openmp/run.sh automatic automatic true true
# $$
# The following is the corresponding output
# $codep
#	g++ -I.. multi_newton.cpp -o multi_newton -fopenmp -DNDEBUG -O2
#	./multi_newton
#	OpenMP: version = 200505, max number of threads = 2
#	n_grid           = { 20, 40 }
#	Execution Speed  = { 3791, 2470 }
#	Correctness Test Passed
#	rm multi_newton
# $$	
#
# $end
if [ ! -e openmp/run.sh ]
then
	echo "must execute this program from the CppAD distribution directory"
	exit 1
fi
cd openmp
#
n_thread="$1"
repeat="$2"
openmp="$3"
optimize="$4"
for flag in "$openmp" "$optimize"
do
	if [ "$flag" != "true"  ] && [ "$flag" != "false" ]
	then
		echo "usage: openmp/run.sh n_thread repeat openmp optimize"
		echo "n_thread: number of threads to use (or \"automatic\")"
		echo "repeat:   repeat factor (or \"automatic\")"
		echo "openmp:   true (use openmp) or false (do not use), and"
		echo "optimize: true (optimized compile) or false (debugging)"
		exit 1
	fi
done
if [ "$openmp" == "true"  ]
then
	flags=-fopenmp
else
	flags=""
fi
if [ "$optimize" == "true" ]
then
	flags="$flags -DNDEBUG -O2"
else
	flags="$flags -g"
fi
#
echo "g++ -I.. multi_newton.cpp -o multi_newton $flags"
g++ -I.. multi_newton.cpp -o multi_newton $flags
#
n_zero="10"
n_grid="40"
n_sum="10"
if [ "$repeat" != "automatic" ]
then
	date
fi
echo "./multi_newton $n_thread $repeat $n_zero $n_grid $n_sum"
if ! ./multi_newton $n_thread $repeat $n_zero $n_grid $n_sum
then
	echo "Error in multi_newton."
	exit 1
fi
if [ "$repeat" != "automatic" ]
then
	date
fi
#
if [ -e "multi_newton.exe" ]
then
	echo "rm multi_newton.exe"
	rm multi_newton.exe
fi
if [ -e "multi_newton" ]
then
	echo "rm multi_newton"
	rm multi_newton
fi
exit 0
