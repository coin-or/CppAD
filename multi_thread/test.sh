#! /bin/sh -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# script used by */makefile.am to run a default case for all the the tests
# 
# cases that have failed in the past
./openmp_test multi_newton 1 1 100 700 1 true
./openmp_test multi_newton 1 2 3 12 1 true
#
# standard test cases
for program in openmp_test pthread_test bthread_test
do
	if [ -e "$program" ]
	then
		./$program a11c
		echo
		./$program simple_ad
		echo
		# test_time=1 max_thread=4, mega_sum=1
		./$program harmonic 1 4 1
		echo
		# test_time= 2 max_thread=4, 
		# num_zero=20, num_sub=30, num_sum=500, use_ad=true
		./$program multi_newton 2 4 20 30 500 true
	fi
done
