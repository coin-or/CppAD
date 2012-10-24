#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Use this shell script when a new test is added to initially create 
# the source code files */test_name.cpp which return false (for not available).
#
echo "Change this script so that it automates the omhelp commands"
echo "at the beginning of the created files."
echo exit 1
#
if [ "$1" == "" ]
then
	echo "usage: new_test.sh test_name"
	echo "where test_name is the name of the new test being added"
	exit 1
fi
test_name="$1"
if [ ! -e "link_$test_name.cpp" ]
then
	echo "The file ./link_$test_name.cpp does not yet exist."
	echo "It must first be created before executing this script."
	exit 1
fi
if ! grep "speed\/link_$test_name.cpp" main.cpp
then
	echo "link_$test_name.cpp has not yet been added to main.cpp"
	exit 1
fi
list="
	adolc
	cppad
	double
	fadbad
	sacado
"
for dir in profile $list
do
	if grep "link_$test_name.cpp" $dir/makefile.am
	then
		echo "$test_name.cpp is already in $dir/makefile.am"
		exit 1
	fi
	if [ -e $dir/$test_name.cpp ]
	then
		echo "The file $dir/$test_name.cpp already exists."
		exit 1
	fi
done
#
sed -i main.cpp -e "s/speed\/link[^%]*\$/&%\n\tspeed\/link_$test_name.cpp/"
#
copy=`sed -n ../COPYING -e '/^\/\*/,/\*\/$/p'`
link=`sed -n link_$test_name.cpp -e "/^ *extern *bool *link_$test_name/,/^);/p"`
fun=`echo "$link" | sed -e 's/extern */\n/' -e 's/^);/)\n{\n\treturn false;\n}/'`
for dir in $list
do
	echo "$copy$fun" > $dir/$test_name.cpp
	sed -i $dir/makefile.am \
		-e "s/\/main.cpp.*/&\n\t..\/link_$test_name.cpp \\\\/" \
		-e "s/\/link_$test_name.cpp.*/&\n\t$test_name.cpp \\\\/"
done
sed -i profile/makefile.am \
	-e "s/\/main.cpp.*/&\n\t..\/link_$test_name.cpp \\\\/" \
	-e "s/\/link_$test_name.cpp.*/&\n\t..\/cppad\/$test_name.cpp \\\\/"
