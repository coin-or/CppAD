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
if [ ! -e "bin/check_all.sh" ]
then
	echo "bin/check_all.sh: must be executed from its parent directory"
	exit 1
fi
echo_log_eval() {
	echo $*
	echo $* >> $top_srcdir/check_all.log
	if ! eval $* >> $top_srcdir/check_all.log 2> $top_srcdir/check_all.err
	then
		cat $top_srcdir/check_all.err
		echo 'Error: see check_all.log'
		exit 1
	fi
	msg=`cat $top_srcdir/check_all.err`
	cat $top_srcdir/check_all.err
	rm $top_srcdir/check_all.err
	if [ "$msg" != '' ]
	then
		exit 1
	fi
}
log_eval() {
	echo $* >> $top_srcdir/check_all.log
	if ! eval $* >> $top_srcdir/check_all.log
	then
		echo "Error: check check_all.log"
		exit 1
	fi
}
if [ -e check_all.log ]
then
	echo "rm check_all.log"
	rm check_all.log
fi
top_srcdir=`pwd`
# ---------------------------------------------------------------------------
# circular shift program list and set program to first entry in list
next_program() {
	program_list=`echo "$program_list" | sed -e 's| *\([^ ]*\) *\(.*\)|\2 \1|'`
	program=`echo "$program_list" | sed -e 's| *\([^ ]*\).*|\1|'`
}
# ---------------------------------------------------------------------------
# Create package to run test in
echo "bin/package.sh"
bin/package.sh
# -----------------------------------------------------------------------------
# choose which tarball to use for testing
version=`bin/version.sh get`
echo_log_eval cd build
list=( `ls cppad-$version.*.tgz` )
if [ "${#list[@]}" == '1' ]
then
	tarball="${list[0]}"
	skip="$skip other_tarball"
else
	choice=`echo $RANDOM % 2 | bc`
	tarball="${list[$choice]}"
fi
echo_log_eval rm -r cppad-$version
echo_log_eval tar -xzf $tarball
echo_log_eval cd cppad-$version
# -----------------------------------------------------------------------------
list="
	$HOME/prefix/cppad
	build
"
for name in $list
do
	if [ -e "$name" ]
	then
		echo_log_eval rm -r $name
	fi
done
echo_log_eval bin/run_cmake.sh
echo_log_eval cd build
# -----------------------------------------------------------------------------
echo_log_eval make check 
# -----------------------------------------------------------------------------
skip=''
list='
	cppad_ipopt/example/example_ipopt_nlp
	cppad_ipopt/speed/speed_ipopt_nlp
	cppad_ipopt/test/test_more_ipopt_nlp
	example/example
	example/ipopt_solve/example_ipopt_solve
	introduction/exp_apx/introduction_exp_apx
	introduction/get_started/introduction_get_started
	speed/example/speed_example
	test_more/test_more
'
#
# standard tests
for program in $list
do
	if [ ! -e "$program" ]
	then
		skip="$skip $program"
	else
		echo_log_eval $program 
	fi
done
#
# speed tests
for dir in adolc cppad double fadbad sacado profile
do
	program="speed/$dir/speed_${dir}"
	if [ ! -e "$program" ]
	then
		skip="$skip $program"
	else
		echo_log_eval $program correct 54321 
		echo_log_eval $program correct 54321 retape
	fi
done
#
# multi_thread tests
# ----------------------------------------------------------------------------
program_list=''
for dir in bthread openmp pthread
do
	program="multi_thread/${dir}/${dir}_test"
	if [ ! -e $program ]
	then
		skip="$skip $program"
	else
		program_list="$program_list $program"
		#
		# fast cases, test for all programs
		echo_log_eval ./$program a11c
		echo_log_eval ./$program simple_ad
		echo_log_eval ./$program team_example
	fi
done
if [ "$program_list" != '' ]
then
	# test_time=1,max_thread=4,mega_sum=1
	next_program
	echo_log_eval ./$program harmonic 1 4 1
	# 
	# test_time=2,max_thread=4,num_zero=20,num_sub=30,num_sum=500,use_ad=true
	next_program
	echo_log_eval ./$program multi_newton 2 4 20 30 500 true
	#
	# case that failed in the past
	next_program
	echo_log_eval ./$program multi_newton 1 1 100 700 1 true
	#
	# case that failed in the past
	next_program
	echo_log_eval ./$program multi_newton 1 2 3 12 1 true
fi
#
# print_for test 
if [ ! -e 'print_for/print_for' ]
then
	skip="$skip print_for/print_for"
else
	echo_log_eval print_for/print_for 
	print_for/print_for | sed -e '/^Test passes/,$d' > junk.1.$$
	print_for/print_for | sed -e '1,/^Test passes/d' > junk.2.$$
	if diff junk.1.$$ junk.2.$$
	then
		rm junk.1.$$ junk.2.$$
		echo_log_eval echo "print_for: OK"  
	else
		echo_log_eval echo "print_for: Error"
		exit 1
	fi
fi
#
echo_log_eval make install 
#
if [ "$skip" != '' ]
then
	echo_log_eval echo "check_all.sh: skip = $skip"
	exit 1
fi
#
echo_log_eval echo 'check_all.sh: OK'
exit 0
