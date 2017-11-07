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
if [ ! -d new ]
then
	echo 'speed_new.sh: the directory ./new does not exist.'
	echo 'It should contain the new source code.'
	exit 1
fi
if [ "$1" == 'none' ]
then
	shift
fi
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
# -------------------------------------------------------------------------
#
file_cur="cur.$option_list.out"
if [ ! -e build/speed/cppad/$file_cur ]
then
	# revert cppad source code to the current version
	git reset --hard
	#
	# compile and link the current version
	echo 'cd build; make check_speed_cppad; cd speed/cppad'
	cd build; make check_speed_cppad; cd speed/cppad
	#
	# run speed test for the current version
	echo "./speed_cppad speed 123 $* > $file_cur"
	./speed_cppad speed 123 $* > "$file_cur"
	#
	echo 'cd ../../..'
	cd ../../..
fi
#
# Convert cppad source to the new version
echo 'git_new.sh from'
git_new.sh from
#
file_new="new.$option_list.out"
if [ ! -e buid/speed/cppad/$file_new ]
then
	# compile and link the new version
	echo 'cd build; make check_speed_cppad; cd speed/cppad'
	cd build; make check_speed_cppad; cd speed/cppad
	#
	# run speed test for the new version
	echo "./speed_cppad speed 123 $* > $file_new"
	./speed_cppad speed 123 $* > "$file_new"
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
echo "	$file_cur > run.out"
sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' \
	-e 's|available|available_cur|' -e '/available_cur/p' \
	$file_cur > run.out
#
echo "sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' \\"
echo "       -e 's|available|available_new|' -e '/available_new/p' \\"
echo "	$file_new >> run.out"
sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' \
	-e 's|available|available_new|' -e '/available_new/p' \
	$file_new >> run.out
#
echo "cat run.out | sort -u"
cat run.out | sort -u
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
