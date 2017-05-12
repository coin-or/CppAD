#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
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
echo_eval() {
     echo $*
     eval $*
}
# ----------------------------------------------------------------------------
if [ "$3" == '' ]
then
	echo 'usage: bin/gpl_license.sh archive_name archive_dir work_dir'
	exit 1
fi
archive_name="$1"
archive_dir="$2"
work_dir="$3"
# ----------------------------------------------------------------------------
start_dir=`pwd`
if [[ "$archive_name" =~ .*/.* ]]
then
	echo "bin/gpl_version.sh: archive_name = $archive_name"
	echo "                    archive_name cannot contain a / character"
	exit 1
fi
# change archive_dir to an absolute path so can use after changing directories
first_character=`echo "$archive_dir" | sed -e 's/\(.\).*/\1/'`
if [ "$first_character" != '/' ]
then
	archive_dir="$start_dir/$archive_dir"
fi
#
# delete old gpl version of tarball (if it exists)
if [ -e $archive_dir/$archive_name.gpl.tgz ]
then
	echo_eval rm $archive_dir/$archive_name.gpl.tgz
fi
#
# change into work directory
echo_eval cd $work_dir
#
# delete old version of directory (if it exists)
if [ -e $archive_name ]
then
	echo_eval rm -r $archive_name
fi
if [ -e $archive_name ]
then
	echo "bin/gpl_license.sh: cannot remove old $archive_name directory"
	exit 1
fi
#
# extract from the EPL version
echo_eval tar -xzf $archive_dir/$archive_name.epl.tgz
#
# remove documentation directory (if it exists)
if [ -e $archive_name/doc ]
then
	echo_eval rm -r $archive_name/doc
fi
#
# change into the directory
echo_eval cd $archive_name
#
# files that need removing
echo_eval rm epl-v10.html epl-v10.txt
#
# files that need changing
list=`find . \
	\( -name '*.ac'  \) -or \
	\( -name '*.am'  \) -or \
	\( -name '*.c'   \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.h' \)   -or \
	\( -name '*.in'  \) -or \
	\( -name '*.omh' \) -or \
	\( -name '*.py' \)  -or \
	\( -name '*.sh' \)  -or \
	\( -name '*.txt'  \)`
#
# change back up to work directory (to be safe)
echo_eval cd ..
#
echo "Changing license from EPL to GPL"
for file in $list
do
	file=`echo $file | sed -e 's|^\./||'`
	name=`echo $file | sed -e 's|.*\/||'`
	ext=`echo $name  | sed -e 's|.*\.|\.|'`
	#
	if [ "$name" == 'gpl-3.0.txt' ]
	then
		echo "gpl-3.0.txt is in $archive_dir/$archive_name.epl.tgz"
		exit 1
	fi
	if [ "$name" == 'gpl_license.sh' ]
	then
		echo "gpl_license.sh is in $archive_dir/$archive_name.epl.tgz"
		exit 1
	fi
	if grep "GNU General Public License" $archive_name/$file > /dev/null
	then
		if [ "$name" != "doc.omh" ]      && \
		   [ "$name" != 'download.omh' ] && \
		   [ "$name" != 'proj_desc.py' ]
		then
			echo "gpl_license.sh: name=$name"
			echo "GPL license in initial $archive_name/$file"
			exit 1
		fi
	fi
	#
	sed -i $archive_name/$file \
-e 's/Eclipse Public License Version 1.0/GNU General Public License Version 3/'
	#
	if ! grep "GNU General Public License Version 3" $archive_name/$file \
		> /dev/null
	then
		# one line (deprecaed) files do not have copyright.
		lines=`cat $archive_name/$file | wc -l`
		if [ "$lines" != '1' ] && [ "$name" != 'colpack.sh' ]
		then
			echo "Cannot change EPL to GPL for $archive_name/$file"
			exit 1
		fi
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x $archive_name/$file
	fi
done
#
echo "change the COPYING file"
sed -n -i $archive_name/COPYING -e '/-\{70\}/,/-\{70\}/p'
cat $start_dir/gpl-3.0.txt >> $archive_name/COPYING
#
echo "change the file epl-v10.txt to the file gpl-3.0.txt"
cp $start_dir/gpl-3.0.txt $archive_name/gpl-3.0.txt
#
list="
	makefile.am
	makefile.in
	omh/appendix/license.omh
"
for file in $list
do
	if grep "gpl-3.0.txt" $archive_name/$file > /dev/null
	then
		echo bin/"gpl_license.sh: gpl-3.0.txt in initial $archive_name/$file"
		exit 1
	fi
	sed -i $archive_name/$file -e 's/epl-v10.txt/gpl-3.0.txt/'
	if ! grep "gpl-3.0.txt" $archive_name/$file > /dev/null
	then
		msg=bin/"gpl_license.sh: cannot change epl-v10.txt to gpl-3.0.txt"
		echo "$msg for $archive_name/$file"
		exit 1
	fi
done
#
# change into the version directory
echo_eval cd $archive_name
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
	echo_eval mv doc.omh.save doc.omh
	exit 1
fi
# Move the log to the directory where the start directory
echo_eval mv omhelp.xml.log $start_dir
#
echo_eval mv doc.omh.save doc.omh
echo_eval cd ..
# ----------------------------------------------------------------------------
#
# create GPL tarball file as copy or the version directory
echo_eval tar -czf $archive_dir/$archive_name.gpl.tgz $archive_name

# if we reach here, no error occurred
echo "bin/gpl_license.sh: OK"
