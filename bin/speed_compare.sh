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
if [ ! -e "bin/speed_compare.sh" ]
then
	echo "bin/speed_compare.sh: must be executed from its parent directory"
	exit 1
fi
options=""
if [ "$1" == "retape" ] || [ "$1" == "optimize" ]
then
	options="_$1"
elif [ "$1" != "none" ]
then
	echo "usage: bin/speed_compare.sh option_1 option_2"
	echo "       where the option choices are: 'none', 'retape', 'optimize'"
	exit 1
fi
if [ "$2" == "retape" ] || [ "$2" == "optimize" ]
then
	options="${options}_$2"
elif [ "$2" != "none" ]
then
	echo "usage: bin/speed_compare.sh option_1 option_2"
	echo "       where the option choices are: 'none', 'retape', 'optimize'"
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
if [ ! -e work/speed/cppad/cur_speed$options.out ]
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
	echo "cd ../cppad; make clean; make test"
	cd ../cppad; make clean; make test
	#
	# run speed test for the current version
	opt=`echo $options | sed -e 's|_||g'`
	echo "./cppad speed 123 $opt > cur_speed$options.out"
	./cppad speed 123 $opt > cur_speed$options.out
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
if [ ! -e work/speed/cppad/new_speed$options.out ]
then
	#
	# compile and link the new version
	echo "cd work/speed/src; make clean; make"
	cd work/speed/src; make clean; make
	#
	echo "cd ../cppad; make clean; make test"
	cd ../cppad; make clean; make test
	#
	#
	# run speed test for the new version
	opt=`echo $options | sed -e 's|_||g'`
	echo "./cppad speed 123 $opt > new_speed$options.out"
	./cppad speed 123 $opt > new_speed$options.out
	#
	echo "cd ../../.."
	cd ../../..
fi
# compare versions
echo "cd work/speed/cppad"
cd work/speed/cppad
#
echo "sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' \\"
echo "       -e 's|::available|::available_cur|' -e '/::available/p' \\"
echo "	cur_speed$options.out > run.out"
sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' \
	-e 's|::available|::available_cur|' -e '/::available/p' \
	cur_speed$options.out > run.out
#
echo "sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' \\"
echo "       -e 's|::available|::available_new|' -e '/::available/p' \\"
echo "	new_speed$options.out >> run.out"
sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' \
	-e 's|::available|::available_new|' -e '/::available/p' \
	new_speed$options.out >> run.out
#
echo "cat run.out | sort -u"
cat run.out | sort -u
