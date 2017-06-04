#! /bin/bash -eu
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
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != "bin/check_deprecated.sh" ]
then
	echo "bin/check_deprecated.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$#" != '0' ]
then
	echo 'usage: bin/check_deprecated.sh.sh'
	exit 1
fi
# -----------------------------------------------------------------------------
file_list=`git ls-files example | sed -e '/example\/deprecated\//d'`
# -----------------------------------------------------------------------------
# deprecated ADFun member functions
list_of_void='
	Order
	Memory
	Size
	taylor_size
	use_VecAD
	size_taylor
	capacity_taylor
	CompareChange
'
for file in $file_list
do
	for fun in $list_of_void
	do
		if grep "\\.$fun *( *)" $file > /dev/null
		then
			echo ".$fun() is deprecated and appreas in $file"
			exit 1
		fi
	done
	if grep "\\.Dependent *( *[a-zA-Z_][a-zA-Z_]* *)" $file > /dev/null
	then
			echo ".Dependent(y) is deprecated and appreas in $file"
			exit 1
	fi
done

# -----------------------------------------------------------------------------
echo 'bin/check_deprecated.sh: OK'
exit 0
