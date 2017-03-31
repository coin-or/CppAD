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
if [ "$0" != "bin/valgrind.sh" ]
then
	echo "bin/valgrind.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$#" != '0' ]
then
	echo 'usage: bin/valgrind.sh.sh'
	exit 1
fi
# -----------------------------------------------------------------------------
list=`find build -perm -700 -type f | \
    sed -e '/^build\/cppad-*/d'  -e '/\/CMakeFiles\//d'`
for program in $list
do
	arguments=''
	if echo "$program" | grep '\/speed\/' > /dev/null
	then
		arguments='correct none'
	fi
	echo "valgrind $program $arguments 2> valgrind.log"
	valgrind $program $arguments 2> valgrind.log
	if ! grep 'ERROR SUMMARY: 0 errors' valgrind.log
	then
		grep 'ERROR SUMMARY' valgrind.log
		exit 1
	fi
done
# -----------------------------------------------------------------------------
echo 'bin/valgrind.sh: OK'
exit 0
