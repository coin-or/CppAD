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
if [ $0 != "bin/travis.sh" ]
then
	echo 'bin/travis.sh: must be executed from its parent directory'
	exit 1
fi
if [ "$1" == '' ]
then
	echo 'usage: bin/travis.sh test_name'
	exit 1
fi
test_name="$1"
if [ ! -e $test_name ]
then
	if [ "$test_name" != 'all' ]
	then
cat << EOF
usage: bin/travis.sh test_name

Where test_name can be 'all', a directory that contains tests,
or a file that contains one test. A file that contains one test must
have the .cpp extension. Note that travis does not yet support test
that require other packages; e.g., adolc.
EOF
	fi
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
if [ -e 'build' ]
then
	echo_eval rm -r build
fi
echo_eval mkdir build
echo_eval cd build
echo_eval cmake ..
#
if [ "$test_name" == 'all' ]
then
	echo_eval make check
elif [ -d "$test_name" ]
then
	check=`echo $test_name | sed -e 's|/$||' -e 's|/|_|g' -e 's|^|check_|'`
	echo_eval make "$check"
else
	ext=`echo $test_name | sed -e 's|.*/||'`
	if [ "$ext" != '.cpp' ]
	then
		echo "travis.sh: $test_name is not 'all', a directory, or *.cpp file"
		exit 1
	fi
	cd ..
	echo_eval bin/test_one.sh $test_name
fi
# -----------------------------------------------------------------------------
echo 'bin/travis.sh: OK'
exit 0
