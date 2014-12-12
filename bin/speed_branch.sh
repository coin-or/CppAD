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
if [ "$0" != 'bin/speed_branch.sh' ]
then
	echo 'bin/speed_branch.sh: must be executed from its parent directory'
	exit 1
fi
local_dir=`pwd | sed -e 's|.*/||'`
if [ "$local_dir" != 'trunk' ]
then
	echo 'bin/speed_branch.sh: must be executed from the trunk'
	exit 1
fi
if [ "$1" == '' ]
then
cat << EOF
usage: bin/speed_branch.sh branch_name option_1 option_2 ...
where the possible options are:
	onetape, colpack, optimize, atomic, memory, boolsparsity
Use the special value 'none' for no options
EOF
	exit 1
fi
branch_name="$1"
branch_dir="../branches/$branch_name"
original_dir=`pwd`
speed_dir='build/speed/cppad'
shift
if [ ! -d "$branch_dir" ]
then
	echo "speed_branch.sh: directory $branch_dir does not exist."
	exit 1
fi
#
if [ ! -e $speed_dir/speed.out ]
then
	# compile and link this version
	echo "cd build; make check_speed_cppad; cd speed/cppad"
	cd build; make check_speed_cppad; cd speed/cppad
	#
	# run speed test for the current version
	echo "./speed_cppad speed 123 $tmp $* > speed.out"
	./speed_cppad speed 123 $* > speed.out
	#
	echo "cd $original_dir"
	cd $original_dir
fi
#
if [ ! -e $branch_dir/$speed_dir/speed.out ]
then
	# compile and link the new version
	echo "cd $branch_dir/build; make check_speed_cppad; cd speed/cppad"
	cd $branch_dir/build; make check_speed_cppad; cd speed/cppad
	#
	# run speed test for the new version
	echo "./speed_cppad speed 123 $* > speed.out"
	./speed_cppad speed 123 $* > speed.out
	#
	echo "cd $original_dir"
	cd $original_dir
fi
# compare versions
#
cat << EOF
sed -n -e 's|_rate|_rate_trunk|' -e '/_rate_/p' \\
	-e 's|available|available_trunk|' -e '/available_trunk/p' \\
	$speed_dir/speed.out > $speed_dir/speed_branch.out
EOF
sed -n -e 's|_rate|_rate_trunk|' -e '/_rate_/p' \
	-e 's|available|available_trunk|' -e '/available_trunk/p' \
	$speed_dir/speed.out > $speed_dir/speed_branch.out
cat << EOF
sed -n -e 's|_rate|_rate_branch|' -e '/_rate_/p' \\
	-e 's|available|available_branch|' -e '/available_branch/p' \\
	$branch_dir/$speed_dir/speed.out >> $speed_dir/speed_branch.out
EOF
sed -n -e 's|_rate|_rate_branch|' -e '/_rate_/p' \
	-e 's|available|available_branch|' -e '/available_branch/p' \
	$branch_dir/$speed_dir/speed.out >> $speed_dir/speed_branch.out
#
echo "cat $speed_dir/speed_branch.out | sort -u"
cat $speed_dir/speed_branch.out | sort -u
