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
# $syntax%openmp/run.sh %program% %n_thread% %repeat% %openmp% %optimize%$$
#
# $head Purpose$$
# The script file $code openmp/run.sh$$ compiles and runs two openmp
# test programs. 
# One test is Example A.1.1.1c from the OpenMP 2.5 standard
# document. The other test is a parallel version of Newton's method
# that uses OpenMP together with CppAD.
#
# $head program$$
# The argument $italic program$$ is either 
# $cref/example_a11c/example_a11c.cpp/$$ 
# (for example A.1.1.c of the OpenMP 2.5 standards document)
# or it is $xref/multi_newton/$$.
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
#	openmp/run.sh multi_newton automatic automatic false true
# $$
# The following is the corresponding output
# $codep
#	g++ -I.. multi_newton.cpp -o multi_newton  -DNDEBUG -O2
#	./multi_newton automatic automatic 10 40 10
#	_OPENMP is not defined, running in single tread mode
#	n_grid           = 40
#	repeats per sec  = 1804
#	Correctness Test Passed
#	rm multi_newton
# $$	
#
# $subhead With OpenMP$$
# The following is an example $code run.sh$$ command 
# with $italic openmp$$ true and $italic optimize$$ true:
# $codep
#	openmp/run.sh multi_newton automatic automatic true true
# $$
# The following is the corresponding output
# $codep
#	g++ -I.. multi_newton.cpp -o multi_newton -fopenmp -DNDEBUG -O2
#	./multi_newton automatic automatic 10 40 10
#	OpenMP: version = 200505, max number of threads = 2
#	n_grid           = 40
#	repeats per sec  = 2615
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
program="$1"
n_thread="$2"
repeat="$3"
openmp="$4"
optimize="$5"
ok="true"
if [ "$program" != "multi_newton" ]  && [ "$program" != "example_a11c" ]
then
	echo "Error: program=$program"
	ok="false"
fi
if [ "$openmp" != "true" ]  && [ "$openmp" != "false" ]
then
	echo "Error: openmp=$openmp"
	ok="false"
fi
if [ "$optimize" != "true" ]  && [ "$optimize" != "false" ]
then
	echo "Error: optimize=$optimize"
	ok="false"
fi
if [ "$ok" == "false"  ] 
then
	echo "usage:    openmp/run.sh program n_thread repeat openmp optimize"
	echo "program:  is either example_a11c or multi_newton"
	echo "n_thread: number of threads to use (or \"automatic\")"
	echo "repeat:   repeat factor (or \"automatic\")"
	echo "openmp:   true (use openmp) or false (do not use), and"
	echo "optimize: true (optimized compile) or false (debugging)"
	exit 1
fi
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
echo "g++ -I.. $program.cpp -o $program $flags"
g++ -I.. $program.cpp -o $program $flags
#
n_zero="10"
n_grid="40"
n_sum="10"
size="10000"
#
if [ "$repeat" != "automatic" ]
then
	date
fi
if [ "$program" == "multi_newton" ]
then
	echo "./multi_newton $n_thread $repeat $n_zero $n_grid $n_sum"
	if ! ./multi_newton $n_thread $repeat $n_zero $n_grid $n_sum
	then
		echo "Error in multi_newton."
		exit 1
	fi
else 
	echo "./example_a11c $n_thread $repeat $size"
	if ! ./example_a11c $n_thread $repeat $size
	then
		echo "Error in example_a11c."
		exit 1
	fi
fi
if [ "$repeat" != "automatic" ]
then
	date
fi
#
for file in multi_newton.exe multi_newton example_a11c.exe example_a11c
do
	if [ -e "$file" ]
	then
		echo "rm $file"
		rm $file
	fi
done
exit 0
