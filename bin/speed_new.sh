#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/speed_new.sh" ]
then
	echo "bin/speed_new.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == '' ]
then
cat << EOF
usage: bin/speed_new.sh option_1 option_2 ...
where the possible options are:
	onetape, colpack, optimize, atomic, memory, boolsparsity, colpack
Use the special value 'none' for no options
EOF
	exit 1
fi
if [ "$1" == 'none' ]
then
	shift
fi
if [ ! -d new ]
then
	echo 'speed_new.sh: the directory ./new does not exist.'
	echo 'It should contain the new source code.'
	exit 1
fi
# -------------------------------------------------------------------------
#
if [ ! -e build/speed/cppad/cur_speed.out ]
then
	# revert cppad source code to the current version
	git reset --hard
	#
	# compile and link the current version
	echo 'cd build; make check_speed_cppad; cd speed/cppad'
	cd build; make check_speed_cppad; cd speed/cppad
	#
	# run speed test for the current version
	echo './speed_cppad speed 123 $* > cur_speed.out'
	./speed_cppad speed 123 $* > cur_speed.out
	#
	echo 'cd ../../..'
	cd ../../..
fi
#
# Convert cppad source to the new version
echo 'git_new.sh from'
git_new.sh from
#
if [ ! -e build/speed/cppad/new_speed.out ]
then
	# compile and link the new version
	echo 'cd build; make check_speed_cppad; cd speed/cppad'
	cd build; make check_speed_cppad; cd speed/cppad
	#
	# run speed test for the new version
	echo './speed_cppad speed 123 $* > new_speed.out'
	./speed_cppad speed 123 $* > new_speed.out
	#
	echo 'cd ../../..'
	cd ../../..
fi
# compare versions
echo 'cd build/speed/cppad'
cd build/speed/cppad
#
echo "sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' \\"
echo "       -e 's|available|available_cur|' -e '/available_cur/p' \\"
echo "	cur_speed.out > run.out"
sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' \
	-e 's|available|available_cur|' -e '/available_cur/p' \
	cur_speed.out > run.out
#
echo "sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' \\"
echo "       -e 's|available|available_new|' -e '/available_new/p' \\"
echo "	new_speed.out >> run.out"
sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' \
	-e 's|available|available_new|' -e '/available_new/p' \
	new_speed.out >> run.out
#
echo "cat run.out | sort -u"
cat run.out | sort -u
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
