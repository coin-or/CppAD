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
# -----------------------------------------------------------------------------
# cd /home/bradbell/repo/cppad.git
# -----------------------------------------------------------------------------
# get newer version of cppad
git checkout --quiet $new_hash
git reset --quiet --hard
# -----------------------------------------------------------------------------
# Simplify the det_lu speed test to isloate what is different
# 1. Remove reverse mode sweep
# 2. Remove forward mode sweep
# 4. Remove creationg of ADFun object
git checkout speed/cppad/det_lu.cpp
sed -i speed/cppad/det_lu.cpp \
	-e '/f\.Reverse/d' \
	-e '/f\.Forward/d' \
	-e 's|f.Dependent(.*|ADScalar::abort_recording();|'
#
# suppress det_lu correctness test
git checkout speed/src/link_det_lu.cpp
sed -i speed/src/link_det_lu.cpp \
	-e 's/return ok;/return true;/'
# -----------------------------------------------------------------------------
# configure new_hash
echo "bin/run_cmake.sh >& /dev/null"
bin/run_cmake.sh >& /dev/null
#
# check speed tests for new_hash
cd build/speed/cppad
echo "make check_speed_cppad > /dev/null"
make check_speed_cppad > /dev/null
#
# run speed test for new_hash
echo "newer speed test results"
./speed_cppad det_lu 125
cd ../../..
#
# necessary files that are different in old_hash
list=`git diff --name-only $old_hash $new_hash | sed \
	-e '/\/atomic_base.hpp$/d' \
	-e '/\/checkpoint.hpp$/d' \
	-e '/\/cond_exp.hpp$/d' \
	-e '/\/discrete.hpp$/d' \
	-e '/\/epsilon.hpp$/d' \
	-e '/\/test_vector.hpp$/d' \
	-e '/\/track_new_del.hpp$/d' \
	-e '/^speed\//d' \
	-e '/^cppad_ipopt\//d' \
	-e '/^omh\//d' \
	-e '/^test_more\//d' \
	-e '/^bin\//d' \
	-e '/^example\//d'`
for file in $list
do
	git show $old_hash:$file > $file
done
# -----------------------------------------------------------------------------
# modify old_hash ad_fun.hpp so do not need old_hash speed directory
cat << EOF > junk.sed
/^# ifndef NDEBUG/! b skip
: loop
N
/\\n# endif/! b loop
s|$|\\nvoid compare_change_count(size_t count)\\n{ return; }|
: skip
EOF
sed -i cppad/local/ad_fun.hpp -f junk.sed
# -----------------------------------------------------------------------------
# check speed tests for old_hash
cd build/speed/cppad
echo "make check_speed_cppad > /dev/null"
make check_speed_cppad > /dev/null
#
# run speed tests for old_hash
echo "older speed test results"
./speed_cppad det_lu 125
cd ../../..
#
# amount of difference from new_hash to old_hash
number_lines=`git diff --unified=0 | grep -v '^@' | wc -l `
number_lines=`expr $number_lines - 4`
echo "Number of lines that are different is $number_lines"
echo 'Use git diff to see the differences'
echo 'bin/speed_temp.sh: OK'
exit 0
