#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
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
standard='c++11'
debug_speed='no'
deprecated='no'
profile_speed='no'
clang='no'
no_colpack='no'
no_ipopt='no'
no_sparse_list='no'
no_documentation='no'
testvector='boost'
while [ "$1" != "" ]
do
	if [ "$1" == '--help' ]
	then
		cat << EOF
usage: bin/run_cmake.sh: \\
	[--help] \\
	[--verbose] \\
	[--c++98] \\
	[--debug_speed] \\
	[--deprecated] \\
	[--profile_speed] \\
	[--clang ] \\
	[--no_colpack] \\
	[--no_ipopt] \\
	[--no_sparse_list] \\
	[--no_documentation] \\
	[--<package>_vector]
The --help option just prints this message and exits.
The value <package> above must be one of: cppad, boost, or eigen.

EOF
		exit 0
	elif [ "$1" == '--verbose' ]
	then
		verbose='yes'
	elif [ "$1" == '--c++98' ]
	then
		standard='c++98'
	elif [ "$1" == '--debug_speed' ]
	then
		debug_speed='yes'
		profile_speed='no'
	elif [ "$1" == '--deprecated' ]
	then
		deprecated='yes'
	elif [ "$1" == '--profile_speed' ]
	then
		profile_speed='yes'
		debug_speed='no'
	elif [ "$1" == '--clang' ]
	then
		clang='yes'
	elif [ "$1" == '--no_colpack' ]
	then
		no_colpack='yes'
	elif [ "$1" == '--no_ipopt' ]
	then
		no_ipopt='yes'
	elif [ "$1" == '--no_sparse_list' ]
	then
		no_sparse_list='yes'
	elif [ "$1" == '--no_documentation' ]
	then
		no_documentation='yes'
	elif [ "$1" == '--cppad_vector' ]
	then
		testvector='cppad'
	elif [ "$1" == '--boost_vector' ]
	then
		testvector='boost'
	elif [ "$1" == '--eigen_vector' ]
	then
		testvector='eigen'
	else
		echo "$1 is an invalid option, try bin/run_cmake.sh --help"
		exit 1
	fi
	shift
done
# ---------------------------------------------------------------------------
if [ "$debug_speed" == 'yes' ]
then
	sed -e 's|^SET(CMAKE_BUILD_TYPE .*|SET(CMAKE_BUILD_TYPE DEBUG)|' \
	    -e 's|^# SET(CMAKE_CXX_FLAGS_DEBUG|SET(CMAKE_CXX_FLAGS_DEBUG|' \
		-i  speed/CMakeLists.txt
else
	sed -e 's|^SET(CMAKE_BUILD_TYPE .*|SET(CMAKE_BUILD_TYPE RELEASE)|' \
	    -e 's|^SET(CMAKE_CXX_FLAGS_DEBUG|# SET(CMAKE_CXX_FLAGS_DEBUG|' \
		-i speed/CMakeLists.txt
fi
# ---------------------------------------------------------------------------
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
if [ -e CMakeCache.txt ]
then
	echo_eval rm CMakeCache.txt
fi
# ---------------------------------------------------------------------------
# clean all variables in cmake cache
cmake_args='-U .+'
#
if [ "$verbose" == 'yes' ]
then
	# echo each command that make executes
	cmake_args="$cmake_args  -D CMAKE_VERBOSE_MAKEFILE=YES"
fi
# -----------------------------------------------------------------------------
# cppad_prefix
cmake_args="$cmake_args  -D cppad_prefix=$HOME/prefix/cppad"
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
# cmake_install_docdir
if [ -d '/usr/share' ] && [ "$no_documentation" == 'no' ]
then
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
package_list='fadbad adolc eigen sacado'
if [ "$no_colpack" == 'no' ]
then
	package_list="$package_list colpack"
fi
if [ "$no_ipopt" == 'no' ]
then
	package_list="$package_list ipopt"
fi
for package in $package_list
do
	dir=$HOME/prefix/$package
	if [ -d "$dir" ]
	then
		cmake_args="$cmake_args  -D ${package}_prefix=$dir"
	fi
done
#
# sparse_list
if [ "$no_sparse_list" == 'yes' ]
then
	cmake_args="$cmake_args -D cppad_sparse_list=NO"
else
	cmake_args="$cmake_args -D cppad_sparse_list=YES"
fi
#
# cppad_cxx_flags
cppad_cxx_flags="-Wall -pedantic-errors -std=$standard"
if [ "$testvector" != 'eigen' ]
then
	cppad_cxx_flags="$cppad_cxx_flags -Wshadow"
fi
cmake_args="$cmake_args -D cppad_cxx_flags='$cppad_cxx_flags'"
#
# clang
if [ "$clang" == 'yes' ]
then
	cmake_args="$cmake_args -D CMAKE_C_COMPILER=clang"
	cmake_args="$cmake_args -D CMAKE_CXX_COMPILER=clang++"
fi
#
# profile
if [ "$profile_speed" == 'yes' ]
then
	cmake_args="$cmake_args -D cppad_profile_flag=-pg"
fi
#
# deprecated
if [ "$deprecated" == 'yes' ]
then
	cmake_args="$cmake_args -D cppad_deprecated=YES"
else
	cmake_args="$cmake_args -D cppad_deprecated=NO"
fi
#
# simple options
cmake_args="$cmake_args -D cppad_testvector=$testvector"
cmake_args="$cmake_args -D cppad_tape_id_type='int32_t'"
cmake_args="$cmake_args -D cppad_tape_addr_type=int32_t"
cmake_args="$cmake_args -D cppad_max_num_threads=48"
#
echo_eval cmake $cmake_args ..
#
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
