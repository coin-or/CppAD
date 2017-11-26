#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/check_all.sh' ]
then
	echo "bin/check_all.sh: must be executed from its parent directory"
	exit 1
fi
debug_all='no'
if [ "$1" != '' ]
then
	debug_all='yes'
	if [ "$1" != 'debug_all' ]
	then
		echo 'usage: bin/check_all.sh [debug_all]'
		exit 1
	fi
fi
echo_log_eval() {
	echo $*
	echo $* >> $top_srcdir/check_all.log
	echo $* >  $top_srcdir/check_all.err
	if ! eval $* >> $top_srcdir/check_all.log 2>> $top_srcdir/check_all.err
	then
		cat $top_srcdir/check_all.err
		echo 'Error: see check_all.log'
		exit 1
	fi
	count=`wc -l $top_srcdir/check_all.err | sed -e 's|\([0-9]*\) .*|\1|'`
	if [ "$count" != '1' ]
	then
		cat "$top_srcdir/check_all.err"
		echo 'Warning: see check_all.err'
		exit 1
	fi
	rm $top_srcdir/check_all.err
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
echo "top_srcdir = $top_srcdir"
#
if ! random_zero_one=`expr $RANDOM % 2`
then
	# expr exit status is 1 when the expression result is zero
	# supress shell exit in this case
	:
fi
echo "random_zero_one = $random_zero_one"
# ---------------------------------------------------------------------------
# circular shift program list and set program to first entry in list
next_program() {
	program_list=`echo "$program_list" | sed -e 's| *\([^ ]*\) *\(.*\)|\2 \1|'`
	program=`echo "$program_list" | sed -e 's| *\([^ ]*\).*|\1|'`
}
# ---------------------------------------------------------------------------
if [ -e "$HOME/prefix/cppad" ]
then
	echo_log_eval rm -r $HOME/prefix/cppad
fi
# ---------------------------------------------------------------------------
# Create package to run test in
echo "bin/package.sh"
bin/package.sh
# -----------------------------------------------------------------------------
# choose which tarball to use for testing
skip=''
version=`bin/version.sh get`
echo_log_eval cd build
list=( `ls cppad-$version.*.tgz` )
if [ "${#list[@]}" == '1' ]
then
	tarball="${list[0]}"
	skip="$skip other_tarball"
else
	tarball="${list[$random_zero_one]}"
fi
echo_log_eval rm -rf cppad-$version
echo_log_eval tar -xzf $tarball
echo_log_eval cd cppad-$version
# -----------------------------------------------------------------------------
if [ "$debug_all" == 'yes' ]
then
	echo_log_eval bin/run_cmake.sh --cppad_vector --debug_all
elif [ "$random_zero_one" == '0' ]
then
	echo_log_eval bin/run_cmake.sh --boost_vector --debug_odd
else
	echo_log_eval bin/run_cmake.sh --eigen_vector --debug_even
fi
echo_log_eval cd build
# -----------------------------------------------------------------------------
echo_log_eval make check
# -----------------------------------------------------------------------------
for package in adolc eigen ipopt fadbad sacado
do
	dir=$HOME/prefix/$package
	if [ ! -d "$dir" ]
	then
		skip="$skip $package"
	fi
done
#
# extra speed tests not run with option specified
for option in onetape colpack optimize atomic memory boolsparsity
do
	echo_eval speed/cppad/speed_cppad correct 432 $option
done
echo_eval speed/adolc/speed_adolc correct         432 onetape
echo_eval speed/adolc/speed_adolc sparse_jacobian 432 onetape colpack
echo_eval speed/adolc/speed_adolc sparse_hessian  432 onetape colpack
#
# ----------------------------------------------------------------------------
# extra multi_thread tests
program_list=''
for threading in bthread openmp pthread
do
	program="example/multi_thread/${threading}"
	program="$program/example_multi_thread_${threading}"
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
	# test_time=1,max_thread=4,num_solve=100
	next_program
	echo_log_eval ./$program multi_atomic 1 4 100
	#
	# test_time=2,max_thread=4,num_zero=20,num_sub=30,num_sum=50,use_ad=true
	next_program
	echo_log_eval ./$program multi_newton 2 4 20 30 50 true
	#
fi
#
# print_for test
program='example/print_for/example_print_for'
if [ ! -e "$program" ]
then
	skip="$skip $program"
else
	echo_log_eval $program
	$program | sed -e '/^Test passes/,$d' > junk.1.$$
	$program | sed -e '1,/^Test passes/d' > junk.2.$$
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
# ----------------------------------------------------------------------------
echo "$0: OK" >> $top_srcdir/check_all.log
echo "$0: OK"
exit 0
