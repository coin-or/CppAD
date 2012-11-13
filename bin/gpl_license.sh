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
#
version=`grep "^ *AC_INIT(" configure.ac | \
        sed -e 's/[^,]*, *\([^ ,]*\).*/\1/'`
root_dir=`pwd`
dir="cppad-$version"
#
# change into work sub-directory
echo_exec cd work
#
# delete old version of *.gpl.tgz 
if [ -e $dir.gpl.tgz ]
then
	echo_exec rm -f $dir.gpl.tgz
fi
#
# delete old version of directory (if it exists)
if [ -e $dir ]
then
	echo_exec rm -rf $dir
fi
if [ -e $dir ]
then
	echo "bin/gpl_license.sh: cannot remove old $dir directory"
	exit 1
fi
#
# extract from the *.epl.tgz file
echo_exec tar -xzf $dir.epl.tgz
#
# change into the work/epl-distribution directory
echo_exec cd $dir
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
	if grep "GNU General Public License" $dir/$file > /dev/null
	then
		if [ "$name" != "doc.omh" ] && \
		   [ "$name" != "gpl_license.sh" ] && \
		   [ "$name" != "gpl-3.0.txt" ]
		then
			echo "GPL license in initial $dir/$file"
			exit 1
		fi
	fi
	#
	sed -i $dir/$file \
-e 's/Eclipse Public License Version 1.0/GNU General Public License Version 3/' 
	#
	if ! grep "GNU General Public License Version 3" $dir/$file > /dev/null
	then
		if [ "$name" != 'config.h.in' ] && \
		   [ "$name" != "gpl-3.0.txt" ] && \
		   [ "$name" != "epl-v10.txt" ]
		then
			echo "Cannot change EPL to GPL for $dir/$file"
			exit 1
		fi
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x $dir/$file
	fi
done
#
echo "change the COPYING file"
sed -n -i $dir/COPYING -e '/-\{70\}/,/-\{70\}/p'
cat $root_dir/gpl-3.0.txt >> $dir/COPYING
#
echo "change the file epl-v10.txt to the file gpl-3.0.txt"
rm $dir/epl-v10.txt
cp $root_dir/gpl-3.0.txt $dir/gpl-3.0.txt
#
list="
	makefile.am
	makefile.in
	omh/license.omh
"
for file in $list
do
	if grep "gpl-3.0.txt" $dir/$file > /dev/null
	then
		echo bin/"gpl_license.sh: gpl-3.0.txt in initial $dir/$file"
		exit 1
	fi
	sed -i $dir/$file -e 's/epl-v10.txt/gpl-3.0.txt/'
	if ! grep "gpl-3.0.txt" $dir/$file > /dev/null
	then
		msg=bin/"gpl_license.sh: cannot change epl-v10.txt to gpl-3.0.txt"
		echo "$msg for $dir/$file"
		exit 1
	fi
done
#
# change into the work/gpl-distribution directory
echo_exec cd $dir
echo_exec rm -r doc
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
echo_exec mkdir doc
echo_exec cd    doc
cmd='omhelp ../doc.omh -noframe -debug -xml -l http://www.coin-or.org/CppAD/'
echo "$cmd > omhelp.xml.log"
if ! eval $cmd > $root_dir/omhelp.xml.log
then
	grep '^OMhelp Error:' $root_dir/omhelp.xml.log
	echo 'OMhelp could not build the CppAD xml documnentation.'
	echo 'See the complete error message in omhelp.xml.log'
	exit 1
fi
if grep '^OMhelp Warning:' $root_dir/omhelp.xml.log
then
	echo 'See the complete warning message in omhelp.xml.log'
	exit 1
fi
echo_exec cd ..
echo_exec mv doc.omh.save doc.omh
echo_exec cd ..
# ----------------------------------------------------------------------------
#
# create *.gpl.tgz file as copy or work/gpl-distribution directory
echo_exec tar -czf $dir.gpl.tgz $dir
