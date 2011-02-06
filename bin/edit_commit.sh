#! /bin/bash -e
# $Id$
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
if [ $0 != "bin/edit_commit.sh" ]
then
	echo "bin/edit_commit.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo "usage: bin/edit_commit.sh file1, file2, ..."
	echo "runs the sed script bin/commit.sed on the list of files"
	exit 1
fi
for file in $*
do
	if [ ! -f $file ]
	then
		echo "Not doing any editing because $file is not a file."
		exit 1
	fi
	if ! chmod +w $file
	then
		echo "Not doing any editing because cannot change $file"
		exit 1
	fi
done
for file in $*
do
	echo "sed -f bin/commit.sed < $file > $$.tmp"
	      sed -f bin/commit.sed < $file > $$.tmp
	#
	if ! diff $file $$.tmp > /dev/null
	then
		echo "$file"
		if diff $file $$.tmp
		then
			echo "bin/edit_commit.sh: program error"
			exit 1
		fi
	fi
	mv $$.tmp $file
	ext=`echo $file | sed -e 's|.*\.||'`
	if [ "$ext" == "sh" ]
	then
		echo "chmod +x $file"
		      chmod +x $file
	fi 
done
