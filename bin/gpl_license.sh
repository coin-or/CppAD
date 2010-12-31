#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/gpl_license.sh" ]
then
	echo "bin/gpl_license.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
#
# Bradley M. Bell has given COIN-OR permission to use this script to generate 
# a distribution of CppAD that has "GNU General Public License Version 2"
# in place of "Common Public License Version 1.0." in all occurrences
# of the message above.
#
version=`grep "^ *AC_INIT(" configure.ac | \
        sed -e 's/[^,]*, *\([^ ,]*\).*/\1/'`
root_dir=`pwd`
dir="cppad-$version"
#
echo "cd work"
cd work
#
# delete old version of *.gpl.tgz 
if [ -e $dir.gpl.tgz ]
then
	echo "rm -f $dir.gpl.tgz"
	rm -f $dir.gpl.tgz
fi
#
# delete old version of directory (if it exists)
if [ -e $dir ]
then
	echo "rm -rf $dir"
	rm -rf $dir
fi
if [ -e $dir ]
then
	echo "bin/gpl_license.sh: cannot remove old $dir directory"
	exit 1
fi
#
# extract from the *.cpl.tgz file
echo "tar -xzf $dir.cpl.tgz"
tar -xzf $dir.cpl.tgz
#
# make sure can change into new directory 
echo "cd $dir"
cd $dir
#
# files that need changing
list=`find . \
	\( -name 'COPYING' \) -or \
	\( -name '*.am'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.bat' \) -or \
	\( -name '*.sh'  \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.in'  \) -or \
	\( -name '*.omh' \)`
#
# change back up to original directory (to be safe)
echo "cd .."
cd ..
#
echo "Changing license from CPL to GPL"
for file in $list
do
	file=`echo $file | sed -e 's|^\./||'`
	name=`echo $file | sed -e 's|.*\/||'`
	ext=`echo $name  | sed -e 's|.*\.|\.|'`
	if grep "GNU General Public License" $dir/$file > /dev/null
	then
		if [ "$name" != "doc.omh.in" ] && [ "$name" != "doc.omh" ]
		then
			echo "GPL license in initial $dir/$file"
			exit 1
		fi
	fi
	#
	sed -i $dir/$file \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' 
	#
	if ! grep "GNU General Public License Version 2" $dir/$file > /dev/null
	then
		if [ "$name" != config.h.in ]
		then
			echo "Cannot change CPL to GPL for $dir/$file"
			exit 1
		fi
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x $dir/$file
	fi
done
#
# change the COPYING file
sed -n -i $dir/COPYING -e '/-\{70\}/,/-\{70\}/p'
cat $root_dir/gpl2.txt >> $dir/COPYING
#
# change the file cpl1.0.txt to the file gpl2.txt
rm $dir/cpl1.0.txt
cp $root_dir/gpl2.txt $dir/gpl2.txt
#
list="
	makefile.am
	makefile.in
	omh/license.omh
"
for file in $list
do
	if grep "gpl2.txt" $dir/$file > /dev/null
	then
		echo bin/"gpl_license.sh: gpl2.txt in initial $dir/$file"
		exit 1
	fi
	sed -i $dir/$file -e 's/cpl1.0.txt/gpl2.txt/'
	if ! grep "gpl2.txt" $dir/$file > /dev/null
	then
		msg=bin/"gpl_license.sh: cannot change cpl1.0.txt to gpl2.txt"
		echo "$msg for $dir/$file"
		exit 1
	fi
done
#
# Rerun omhelp to change documentation version of license from CPL to GPL 
echo "cd $dir"
cd $dir
#
echo "./configure > /dev/null"
./configure > /dev/null
#
echo "bin/run_omhelp.sh doc xml"
bin/run_omhelp.sh doc xml
#
if [ -e "doc/error.wrd" ]
then
	echo "rm -rf doc/error.wrd"
	rm -rf doc/error.wrd
fi
# 
echo "cd .."
cd ..
#
# create *.gpl.tgz file
echo "tar -czf $dir.gpl.tgz $dir"
tar -czf $dir.gpl.tgz $dir
