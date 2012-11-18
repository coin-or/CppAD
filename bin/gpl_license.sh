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
# Bradley M. Bell has given COIN-OR permission to use this script to generate 
# a distribution of CppAD that has "GNU General Public License Version 3"
# in place of "Eclipse Public License Version 1.0." in all occurrences
# of the message above.
# -----------------------------------------------------------------------------
if [ ! -e "bin/gpl_license.sh" ]
then
	echo "bin/gpl_license.sh: must be executed from its parent directory"
	exit 1
fi
echo_exec() {
     echo $* 
     eval $*
}
# ----------------------------------------------------------------------------
if [ "$3" == '' ]
then
	echo 'usage: bin/gpl_license.sh version_dir archive_dir work_dir'
	exit 1
fi
version_dir="$1"
archive_dir="$2"
work_dir="$3"
# ----------------------------------------------------------------------------
start_dir=`pwd`
#
# change into work directory
echo_exec cd $work_dir
#
# delete old version of tarball (if it exists)
if [ -e $version_dir.gpl.tgz ]
then
	echo_exec rm $version_dir.gpl.tgz
fi
#
# delete old version of directory (if it exists)
if [ -e $version_dir ]
then
	echo_exec rm -r $version_dir
fi
if [ -e $version_dir ]
then
	echo "bin/gpl_license.sh: cannot remove old $version_dir directory"
	exit 1
fi
#
# extract from the EPL version
echo_exec tar -xzf $start_dir/$archive_dir/$version_dir.epl.tgz
#
# remove documentation directory (if it exists)
if [ -e $version_dir/doc ]
then
	echo_exec rm -r $version_dir/doc
fi
#
# change into the directory
echo_exec cd $version_dir
#
# files that need changing
list=`find . \
	\( -name '*.ac'  \) -or \
	\( -name '*.am'  \) -or \
	\( -name '*.c'   \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.in'  \) -or \
	\( -name '*.omh' \) -or \
	\( -name '*.pc' \)  -or \
	\( -name '*.py' \)  -or \
	\( -name '*.sh' \)  -or \
	\( -name '*.txt'  \)`
#
# change back up to work directory (to be safe)
echo_exec cd ..
#
echo "Changing license from EPL to GPL"
for file in $list
do
	file=`echo $file | sed -e 's|^\./||'`
	name=`echo $file | sed -e 's|.*\/||'`
	ext=`echo $name  | sed -e 's|.*\.|\.|'`
	if grep "GNU General Public License" $version_dir/$file > /dev/null
	then
		if [ "$name" != "doc.omh" ] && \
		   [ "$name" != "gpl_license.sh" ] && \
		   [ "$name" != "gpl-3.0.txt" ]
		then
			echo "GPL license in initial $version_dir/$file"
			exit 1
		fi
	fi
	#
	sed -i $version_dir/$file \
-e 's/Eclipse Public License Version 1.0/GNU General Public License Version 3/' 
	#
	if ! grep "GNU General Public License Version 3" $version_dir/$file > /dev/null
	then
		if [ "$name" != "config.h.in" ] && \
		   [ "$name" != "gpl-3.0.txt" ] && \
		   [ "$name" != "epl-v10.txt" ]
		then
			echo "Cannot change EPL to GPL for $version_dir/$file"
			exit 1
		fi
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x $version_dir/$file
	fi
done
#
echo "change the COPYING file"
sed -n -i $version_dir/COPYING -e '/-\{70\}/,/-\{70\}/p'
cat $start_dir/gpl-3.0.txt >> $version_dir/COPYING
#
echo "change the file epl-v10.txt to the file gpl-3.0.txt"
rm $version_dir/epl-v10.txt
cp $start_dir/gpl-3.0.txt $version_dir/gpl-3.0.txt
#
list="
	makefile.am
	makefile.in
	omh/license.omh
"
for file in $list
do
	if grep "gpl-3.0.txt" $version_dir/$file > /dev/null
	then
		echo bin/"gpl_license.sh: gpl-3.0.txt in initial $version_dir/$file"
		exit 1
	fi
	sed -i $version_dir/$file -e 's/epl-v10.txt/gpl-3.0.txt/'
	if ! grep "gpl-3.0.txt" $version_dir/$file > /dev/null
	then
		msg=bin/"gpl_license.sh: cannot change epl-v10.txt to gpl-3.0.txt"
		echo "$msg for $version_dir/$file"
		exit 1
	fi
done
#
# change into the version directory
echo_exec cd $version_dir
#
# Only include the *.xml verison of the documentation in distribution
# So remove the table at the top (but save the original doc.omh file).
if ! grep < doc.omh > /dev/null \
	'This comment is used to remove the table below' 
then
	echo "Missing comment expected in doc.omh"
	exit 1
fi
echo "sed -i.save doc.omh ..."
sed -i.save doc.omh \
	-e '/This comment is used to remove the table below/,/$tend/d'
#
# This command creates omhelp.xml.log in current directory (and says so)
echo "bin/run_omhelp.sh xml"
if ! bin/run_omhelp.sh xml
then
	mv omhelp.xml.log ../..
	echo_exec mv doc.omh.save doc.omh
	exit 1
fi
# Move the log to the directory where the start directory
echo_exec mv omhelp.xml.log $start_dir
#
echo_exec mv doc.omh.save doc.omh
echo_exec cd ..
# ----------------------------------------------------------------------------
#
# create GPL tarball file as copy or the version directory
echo_exec tar -czf $version_dir.gpl.tgz $version_dir
