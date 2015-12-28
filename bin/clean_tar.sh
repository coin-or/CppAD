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
if [ $0 != "bin/clean_tar.sh" ]
then
	echo "bin/clean_tar.sh: must be executed from its parent directory"
	exit 1
fi
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
list=`find . -name '*.swp'`
for file in $list
do
	response=''
	read -p "Remove the editor temporary file $file [y/n]" response
	if [ "$response" == 'y' ]
	then
		echo_eval rm $file
	fi
done
for dir in build doc doxydoc
do
	if [ -e "$dir" ]
	then
		echo_eval rm -r $dir
	fi
done
list=`find . -name 'new'`
for dir in $list
do
	echo_eval rm -r $dir
done
list=`find . \
	\( -name '*.err' \) -or \
	\( -name '*.log' \) -or \
	\( -name '*.tmp' \) -or \
	\( -name 'junk.*' \) -or \
	\( -name 'temp.*' \) -or \
	\( -name 'test_one.exe' \) -or \
	\( -name 'test_one.sh' \) -or \
	\( -name 'test_one.cpp' \) -or \
	\( -name 'aclocal.m4' \) -or \
	\( -name 'ar-lib' \) -or \
	\( -name 'configure.hpp' \) -or \
	\( -name 'git_commit.sh' \) -or \
	\( -name 'junk' \) -or \
	\( -name 'temp' \) | \
	sed -e '/\.git\//d' -e '/example\/atomic\/test_one.sh/d' `
for file in $list
do
	echo_eval rm $file
done
version=`date +%Y%m%d`
name=`pwd | sed -e 's|.*/||'`
cd ..
cp -r $name $name-$version
echo_eval tar -czf $name-$version.tgz $name-$version
rm -rf $name-$version
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
