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
if [ $0 != "bin/dir_copy.sh" ]
then
	echo "bin/dir_copy.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$3" == "" ]
then
	echo "usage: bin/dir_copy.sh file_list direction dir"
	echo "	file_list: a file containing a list of file names to copy,"
	echo "	direction: 'to' or 'from', and dir is a directory."
	exit 1
fi
if [ "$2" != 'from' ] && [ "$2" != 'to' ]
then
	echo "bin/dir_copy.sh: invalid direction=$2"
	exit 1
fi
if [ "$2" == 'to' ] && [ ! -e "$1" ]
then
	echo "bin/dir_copy.sh: can't find file_list=$1"
	exit 1
fi
if [ "$2" == 'from' ] && [ ! -e "$3/$1" ]
then
	echo "bin/dir_copy.sh: can't find file_list=$3/$1"
	exit 1
fi
file_list="$1"
direction="$2"
directory="$3"
# -----------------------------------------------------------------------------
#
if [ "$direction" == 'to' ]
then
	if [ -e "$directory" ]
	then
		echo "bin/dir_copy.sh $file_list $direction directory=$directory"
		echo "the directory already exists, remove it first."
		exit 1
	fi
	echo "mkdir $directory"
	mkdir $directory
	#
	list=`cat $file_list`
	for path in $list
	do
		sub_dir=`echo $path | sed -e 's|[^/]*$||' -e 's|/$||'`
		file=`echo $path | sed -e 's|^.*/||'`
		if [ ! -d "$directory/$sub_dir" ]
		then
			echo "mkdir -p $directory/$sub_dir"
			mkdir -p $directory/$sub_dir
		fi
		echo "cp $path $directory/$path"
		cp $path $directory/$path
	done
	echo "cp $file_list $directory/$file_list"
	cp $file_list $directory/$file_list
else
	list=`cat $directory/$file_list`
	for path in $list
	do
		echo "cp $directory/$path $path"
		cp $directory/$path $path
	done
fi
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
