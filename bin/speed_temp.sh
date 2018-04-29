#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/speed_temp.sh" ]
then
	echo "bin/speed_temp.sh: must be executed from its parent directory"
	exit 1
fi
old_hash='b4c0e5'
new_hash='611e98'
echo_eval cd $HOME/repo/cppad.git
# -----------------------------------------------------------------------------
# get newer version of cppad
git checkout --quiet $new_hash
git reset --quiet --hard
#
# configure
echo "bin/run_cmake.sh >& /dev/null"
bin/run_cmake.sh >& /dev/null
#
# check speed tests
cd build/speed/cppad
echo "make check_speed_cppad > /dev/null"
make check_speed_cppad > /dev/null
#
# run speed test
echo "newer speed test results"
./speed_cppad det_lu 125
cd ../../..
#
# get files that different in older version
list=`git diff --name-only $old_hash $new_hash | sed \
	-e '/\/atomic_base.hpp$/d' \
	-e '/\/checkpoint.hpp$/d' \
	-e '/\/cond_exp.hpp$/d' \
	-e '/\/discrete.hpp$/d' \
	-e '/\/epsilon.hpp$/d' \
	-e '/\/test_vector.hpp$/d' \
	-e '/\/track_new_del.hpp$/d' \
	-e '/^cppad_ipopt\//d' \
	-e '/^omh\//d' \
	-e '/^test_more\//d' \
	-e '/^bin\//d' \
	-e '/^example\//d'`
for file in $list
do
	git show $old_hash:$file > $file
done
#
# check speed tests
cd build/speed/cppad
echo "make check_speed_cppad > /dev/null"
make check_speed_cppad > /dev/null
#
# run speed tests
echo "older speed test results"
./speed_cppad det_lu 125
cd ../../..
#
number_lines=`git diff --unified=0 | grep -v '^@' | wc -l `
number_lines=`expr $number_lines - 4`
echo "Number of lines that are different is $number_lines"
echo 'Use git diff to see the differences'
echo 'bin/speed_temp.sh: OK'
exit 0
