#! /bin/bash -e
# $Id:$
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
if [ $0 != "bin/reduce_index.sh" ]
then
	echo "bin/reduce_index.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == '' ]
then
	echo 'usage: bin/reduce_index.sh days'
	echo '	where days is an integer, runs bin/reduce_index.py'
	echo 'on all files checked in within the specified number of days.'
fi
days="$1"
list=`git log --pretty=format: --name-only --since="$days days ago" | sed \
	-e '/^ *$/d'  \
	-e '/^makefile\./d'     -e '/\/makefile\./d' \
	-e '/^CMakeLists.txt/d' -e '/\/CMakeLists.txt/d' \
	-e '/^bin\/reduce_index.py/d'`
for file in $list
do
	if [ -e $file ]
	then
		git checkout $file
		bin/reduce_index.py $file
	fi
done
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
