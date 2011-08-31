#! /bin/bash -e
# \$Id:\$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/grep_source.sh" ]
then
	echo "bin/grep_source.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo "bin/grep_source.sh [-i] pattern"
	exit 1
else
	if [ "$1" == "-i" ] && [ "$2" == "" ]
	then
		echo "bin/grep_source.sh [-i] pattern"
		exit 1
	fi
fi
list=`find . \
       \( -name '*.am' \)  -or \
       \( -name '*.cpp' \) -or \
       \( -name '*.hpp' \) -or \
       \( -name '*.omh' \) -or \
       \( -name '*.sh' \)      | \
	sed -e 's|\./||' -e '/^work\//d' -e '/^svn_dist\//d' `
#
list="$list 
	configure.ac
"
if [ "$1" == "-i" ]
then
	flag="-i"
	pattern="$2"
else
	flag=""
	pattern="$1"
fi
for file in $list
do
	if grep "$pattern" $file > /dev/null
	then
		echo
		echo "$file"
		grep $flag "$pattern" $file
	fi
done
