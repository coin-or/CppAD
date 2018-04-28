#! /bin/bash -e
# $Id$
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
if [ $0 != "bin/speed_temp.sh" ]
then
	echo "bin/speed_temp.sh: must be executed from its parent directory"
	exit 1
fi
old_hash='418ea1'
new_hash='f3018c'
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
# run speed tests
echo "newer speed test results"
./speed_cppad sparse_hessian 125
cd ../../..
#
# change to older version
# get files that different
list=`git diff --name-only $old_hash $new_hash | sed \
	-e '/^omh\//d' \
	-e '/^example\//d' \
	-e '/^bin\//d' \
	-e '/^speed\//d' \
	-e '/^cppad\/speed\//d'`
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
./speed_cppad sparse_hessian 125
cd ../../..
#
echo 'Use git diff to see changes from newer to older results'
echo 'bin/speed_temp.sh: OK'
exit 0
