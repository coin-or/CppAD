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
if [ ! -e "bin/check_example.sh" ]
then
	echo "bin/check_example.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Checking that all examples are in omh/example_list.omh"
echo "-------------------------------------------------------"
file_list=`bin/ls_files.sh | sed -n \
	-e '/^example\/deprecated\//d' \
	-e '/^example\//p'`
#
sed < omh/example_list.omh > check_example.$$ \
	-n -e '/\$begin ListAllExamples\$\$/,/\$end/p'
#
# Make sure all example names are of the form *.cpp or *.hpp
check=`sed -n -e '/$rref [0-9a-zA-Z_]*\.[hc]pp/d' \
	-e '/$rref/p' check_example.$$`
if [ "$check" != '' ]
then
	echo $check
	echo 'Not all examples are for *.hpp or *.cpp files'
	exit 1
fi
ok="yes"
for file in $file_list
do
	# only files in example directory with $begin *.cpp or *.hpp
	# e.g., example/multi_thread/harmonic.omh has $begin harmonic.cpp$$ in it
	name=`grep '$begin *[0-9a-zA-Z_]*\.[hc]pp' $file |
		sed -e 's/.*$begin *//' -e 's/ *$$.*//'`
	if [ "$name" != "" ]
	then
		if ! grep "$name" check_example.$$ > /dev/null
		then
			echo "$name is missing from omh/example_list.omh"
			ok="no"
		fi
	fi
done
rm check_example.$$
echo "-------------------------------------------------------"
if [ "$ok" = "yes" ]
then
	echo "Ok: nothing is between the two dashed lines above"
        exit 0
else
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
fi
