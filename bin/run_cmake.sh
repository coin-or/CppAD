#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
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
# ---------------------------------------------------------------------------
# clean all variables in cmake cache
cmake_args='-U .+'
if [ "$1" != "" ]
then
	if [ "$1" == '--verbose' ]
	then
		# echo each command that make executes
		cmake_args="$cmake_args  -D CMAKE_VERBOSE_MAKEFILE=1"
	else
		echo 'usage: bin/run_cmake.sh: [--verbose]'
		exit 1
	fi
fi
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
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
for package in adolc eigen ipopt fadbad sacado
do
	dir=$HOME/prefix/$package
	if [ -d "$dir" ]
	then
		cmake_args="$cmake_args  -D ${package}_prefix=$dir"
	fi
done
#
# cppad_cxx_flags
cmake_args="$cmake_args -D cppad_cxx_flags=\
'-Wall -pedantic-errors -std=c++11 -Wshadow'"
#
# simple options
cmake_args="$cmake_args -D cppad_documentation=YES"
cmake_args="$cmake_args -D cppad_sparse_list=YES"
cmake_args="$cmake_args -D cppad_testvector=cppad"
cmake_args="$cmake_args -D cppad_tape_id_type='int'"
cmake_args="$cmake_args -D cppad_tape_addr_type=int"
cmake_args="$cmake_args -D cppad_max_num_threads=48"
#
echo_eval cmake $cmake_args ..
#
exit 0
