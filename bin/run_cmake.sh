#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
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
# -----------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------
verbose='no'
testvector='boost'
debug_speed='no'
profile_speed='no'
while [ "$1" != "" ]
do
	if [ "$1" == '--verbose' ]
	then
		verbose='yes'
	elif [ "$1" == '--cppad_vector' ]
	then
		testvector='cppad'
	elif [ "$1" == '--boost_vector' ]
	then
		testvector='boost'
	elif [ "$1" == '--eigen_vector' ]
	then
		testvector='eigen'
	elif [ "$1" == '--debug_speed' ]
	then
		debug_speed='yes'
		profile_speed='no'
	elif [ "$1" == '--profile_speed' ]
	then
		profile_speed='yes'
		debug_speed='no'
	else
		options='[--verbose] [--<package>_vector]'
		options="$options [--debug_speed] [--profile_speed']"
		echo "usage: bin/run_cmake.sh: $options"
		echo 'where <package> is cppad, boost, or eigen'
		exit 1
	fi
	shift
done
# ---------------------------------------------------------------------------
if [ "$debug_speed" == 'yes' ]
then
	sed -e 's|^SET(CMAKE_BUILD_TYPE .*|SET(CMAKE_BUILD_TYPE DEBUG)|' \
		-i  speed/CMakeLists.txt
elif [ "$profile_speed" == 'yes' ]
then
	sed -e 's|^SET(CMAKE_BUILD_TYPE .*|SET(CMAKE_BUILD_TYPE MinSizeRel)|' \
		-i  speed/CMakeLists.txt
else
	sed -e 's|^SET(CMAKE_BUILD_TYPE .*|SET(CMAKE_BUILD_TYPE RELEASE)|' \
		-i speed/CMakeLists.txt
fi
# ---------------------------------------------------------------------------
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
# ---------------------------------------------------------------------------
# clean all variables in cmake cache
cmake_args='-U .+'
#
if [ "$verbose" == 'yes' ]
then
	# echo each command that make executes
	cmake_args="$cmake_args  -D CMAKE_VERBOSE_MAKEFILE=1"
fi
# -----------------------------------------------------------------------------
# cmake_install_prefix
cmake_args="$cmake_args  -D cmake_install_prefix=$HOME/prefix/cppad"
#
# cmake_install_includedirs
if [ -d '/usr/include' ]
then
	cmake_args="$cmake_args -D cmake_install_includedirs=include"
fi
#
# cmake_install_datadir
if [ -d '/usr/share' ]
then
	cmake_args="$cmake_args -D cmake_install_datadir=share"
	cmake_args="$cmake_args -D cmake_install_docdir=share/doc"
fi
#
# cmake_install_libdirs
if [ -d '/usr/lib64' ]
then
	cmake_args="$cmake_args -D cmake_install_libdirs='lib64;lib'"
elif [ -d '/usr/lib' ]
then
	cmake_args="$cmake_args -D cmake_install_libdirs='lib;lib64'"
fi
#
# {package}_prefix
for package in adolc colpack eigen ipopt fadbad sacado
do
	dir=$HOME/prefix/$package
	if [ -d "$dir" ]
	then
		cmake_args="$cmake_args  -D ${package}_prefix=$dir"
	fi
done
#
# cppad_cxx_flags
cmake_args="$cmake_args -D cppad_cxx_flags='-Wall -pedantic-errors -std=c++11'"
if [ "$testvector" != 'eigen' ]
then
 	cmake_args="$cmake_args -Wshadow"
fi
#
# simple options
cmake_args="$cmake_args -D cppad_implicit_ctor_from_any_type_from_any_type=NO"
cmake_args="$cmake_args -D cppad_sparse_list=YES"
cmake_args="$cmake_args -D cppad_testvector=$testvector"
cmake_args="$cmake_args -D cppad_tape_id_type='int'"
cmake_args="$cmake_args -D cppad_tape_addr_type=int"
cmake_args="$cmake_args -D cppad_max_num_threads=48"
#
echo_eval cmake $cmake_args ..
#
exit 0
