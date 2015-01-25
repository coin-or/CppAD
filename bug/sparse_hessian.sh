#! /bin/bash -e
# $Id:$
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
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo_eval cd build
echo_eval cmake ../..
#
echo_eval sed -e "'s|\\(CPPAD_EXTRA_RUN_BEFORE_TIMING\\) *[01]|\\1 0|'" \
	-i ../../cppad/time_test.hpp
echo_eval make check_speed_cppad
echo 'speed/cppad/speed_cppad sparse_hessian 123  > no_extra.out'
speed/cppad/speed_cppad sparse_hessian 123  > no_extra.out
#
echo_eval sed -e "'s|\\(CPPAD_EXTRA_RUN_BEFORE_TIMING\\) *[01]|\\1 1|'" \
	-i ../../cppad/time_test.hpp
echo_eval make check_speed_cppad
echo 'speed/cppad/speed_cppad sparse_hessian 123  > yes_extra.out'
speed/cppad/speed_cppad sparse_hessian 123  > yes_extra.out
#
echo 'cppad/time_test.hpp: # define CPPAD_EXTRA_RUN_BEFORE_TIMING 0'
cat no_extra.out
echo 'cppad/time_test.hpp: # define CPPAD_EXTRA_RUN_BEFORE_TIMING 1'
cat yes_extra.out
