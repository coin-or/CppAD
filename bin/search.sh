#! /bin/bash -e
# $Id: search.sh 2082 2011-08-31 17:50:58Z bradbell $
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
if [ ! -e "bin/search.sh" ]
then
	echo "bin/search.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo 'usage: bin/search.sh pattern'
	echo '       Search for pattern in the cppad source code'
	exit 1
fi
pattern="$1"
extensions='.ac .am .cpp .hpp .in .omh .sh'
directories='
	cppad 
	cppad/local 
	cppad_ipopt/example
	cppad_ipopt/speed
	cppad_ipopt/src
	cppad_ipopt/test
	example 
	introduction/exp_apx
	introduction/get_started
	multi_thread
	multi_thread/bthread
	multi_thread/openmp
	multi_thread/pthread
	omh
	pkgconfig
	print_for
	speed
	speed/adolc
	speed/cppad
	speed/double
	speed/example
	speed/fadbad
	speed/profile
	speed/sacado
	speed/src
	test_more 
'
#
find_files.sh "$pattern" "$extensions" "$directories"
