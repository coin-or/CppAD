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
if [ $0 != "bin/speed_compare.sh" ]
then
	echo "bin/speed_compare.sh: must be executed from its parent directory"
	exit 1
fi
if [ ! -d cppad/new ]
then
	echo "speed_comapre.sh: the directory cppad/new does not exist."
	echo "It contains new source code for the cppad directory (can be empty)."
	exit 1
fi
if [ ! -d cppad/local/new ]
then
	echo "speed_comapre.sh: the directory cppad/local/new does not exist"
	echo "It contains new source code for the cppad/local (can be empty)."
	exit 1
fi
# Source code files that are changing 
new_cppad=`cd cppad/new ; ls`
new_local=`cd cppad/local/new ; ls`
if [ "$new_cppad" == "" ] && [ "$new_local" == "" ]
then
	echo "speed_compare.sh: both cppad/new and cppad/local/new are empty"
	exit 1
fi
# -------------------------------------------------------------------------
#
# check if we already have results for current version
if [ ! -e work/speed/cppad/speed_cur.out ]
then
	echo "svn list cppad"
	list_cppad=`svn list cppad`
	# revert cppad source code to the current version
	if [ "$new_cppad" != "" ]
	then
		for file in $new_cppad
		do
			if (echo $list_cppad | grep $file > /dev/null)
			then
				echo "svn revert cppad/$file"
				svn revert cppad/$file
			fi
		done
	fi
	echo "svn list cppad/local"
	list_local=`svn list cppad/local`
	# revert cppad/local source code to the current version
	if [ "$new_local" != "" ]
	then
		for file in $new_local
		do
			if (echo $list_local | grep $file > /dev/null)
			then
				echo "svn revert cppad/local/$file"
				svn revert cppad/local/$file
			fi
		done
	fi
	#
	# compile and link the current version
	echo "cd work/speed/src; make clean; make"
	cd work/speed/src; make clean; make
	#
	echo "cd ../cppad; make clean; make test.sh"
	cd ../cppad; make clean; make test.sh
	#
	# run speed test for the current version
	echo "./cppad speed 123 retape > speed_cur.out"
	./cppad speed 123 retape > speed_cur.out
	#
	echo "cd ../../.."
	cd ../../..
fi
#
# Convert cppad source to the new version
for file in $new_cppad
do
	echo "cp cppad/new/$file cppad/$file"
	cp cppad/new/$file cppad/$file
done
#
# Convert local source to the new version
for file in $new_local
do
	echo "cp cppad/local/new/$file cppad/local/$file"
	cp cppad/local/new/$file cppad/local/$file
done
#
# compile and link the new version
echo "cd work/speed/src; make clean; make"
cd work/speed/src; make clean; make
#
echo "cd ../cppad; make clean; make test.sh"
cd ../cppad; make clean; make test.sh
#
# run speed test for the new version
echo "./cppad speed 123 retape > speed_new.out"
./cppad speed 123 retape > speed_new.out
#
# compare versions
echo "sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' < speed_cur.out > run.out"
sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' < speed_cur.out > run.out
#
echo "sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' < speed_new.out >> run.out"
sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' < speed_new.out >> run.out
#
echo "cat run.out | sort -u"
cat run.out | sort -u
