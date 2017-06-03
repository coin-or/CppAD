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
if [ "$0" != "bin/speed_branch.sh" ]
then
	echo "bin/speed_branch.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$2" == '' ]
then
cat << EOF
usage: bin/speed_branch.sh branch_one branch_two [option_1 [option_2 ...] ]
where the possible options are:
	atomic, boolsparsity, colpack, memory, onetape, optimize, revsparsity
EOF
	exit 1
fi
branch_one="$1"
shift
branch_two="$1"
shift
option_list='none'
for option in $*
do
	if [ "$option_list" == 'none' ]
	then
		option_list="$option"
	else
		option_list="${option_list}_$option"
	fi
done
# ----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
if [ ! -d '.git' ]
then
	echo 'speed_branch.sh: only implemented for git repository'
	exit 1
fi
# -----------------------------------------------------------------------------
dir='build/speed/cppad'
for branch in $branch_one $branch_two
do
	if [ -e "build/speed/cppad/$branch.$option_list.out" ]
	then
		echo "Using existing build/speed/cppad/$branch.$option_list.out"
	else
		echo_eval git checkout $branch
		#
		echo "bin/run_cmake.sh --debug_none > $branch.log"
		bin/run_cmake.sh --debug_none > $branch.log
		#
		echo_eval cd $dir
		#
		echo "make check_speed_cppad >> $branch.log"
		make check_speed_cppad >> ../../../$branch.log
		#
		echo "./speed_cppad speed 123 $* > $branch.$option_list.out"
		./speed_cppad speed 123 $* > $branch.$option_list.out
		#
		cd ../../..
	fi
done
# compare the results
echo "	one=$branch_one , two=$branch_two"
bin/speed_diff.sh \
	$dir/$branch_one.$option_list.out $dir/$branch_two.$option_list.out
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
