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
# $section Compile and Run an OpenMP example$$
#
# $head Syntax$$
# $syntax%openmp/run.sh %name% %openmp% %optimize%$$
#
# $head Purpose$$
# The script file $code openmp/run.sh$$ compiles and runs one of the openmp
# example programs.
#
# $head name$$
# The argument $italic name$$ is the name of the example program.
# Currently the valid choice for $italic name$$ is 
# $cref/multi_newton/multi_newton.cpp/$$.
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
#	openmp/run.sh multi_newton false true
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
#	openmp/run.sh multi_newton true true
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
for flag in "$2" "$3"
do
	if [ "$flag" != "true"  ] && [ "$flag" != "false" ]
	then
		echo "usage: openmp/run.sh name openmp optimize"
		echo "name     the C++ file name wthout extension,"
		echo "openmp   true (use openmp) or false (do not use), and"
		echo "optimize true (optimized compile) or false (debugging)"
		exit 1
	fi
done
if [ "$2" == "true"  ]
then
	flags=-fopenmp
else
	flags=""
fi
if [ "$3" == "true" ]
then
	flags="$flags -DNDEBUG -O2"
else
	flags="$flags -g"
fi
#
echo "g++ -I.. $1.cpp -o $1 $flags"
g++ -I.. $1.cpp -o $1 $flags
#
echo "./$1"
if ! ./$1
then
	exit 1
fi
#
if [ -e "$1.exe" ]
then
	echo "rm $1.exe"
	rm $1.exe
fi
if [ -e "$1" ]
then
	echo "rm $1"
	rm $1
fi
