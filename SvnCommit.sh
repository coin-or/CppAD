# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Define your subversion commit by editing the definition of 
# log_entry, add_list, change_list, delete_list and copy_branch below.
#
# log_entry
# The contents of this variable will be used as the log entry for the commit.
#
# add_list
# List of files that will be added to the repository during this commit.
# Do not use add_list to add directories; use the following instead:
# 	svn add --non-recursive dir
# 	svn commit directory -m "adding directory dir" dir
# where dir is replaced by the name of the directory. 
# Then use add_list to add the files within that directory.
#
# change_list
# List of files that are currently in repository and change during this commit.
#
# delete_list
# List of files that will be deleted from the repository during this commit.
#
# copy_branch
# You may specify up one other branch that should receive a copy of all that 
# changes that you are making with this commit. 
# This other branch cannot be the trunk and you must have a copy of it
# on your local machine. If copy_branch is empty; i.e., copy_branch="", 
# the changes will not be copied (and commited) into another branch.
#
# ----------------------------------------------------------------------
log_entry="Add the Var2Par function and move Discrete.cpp to TapeIndex.cpp

SvnCommit.sh: file that make this commit.
WhatsNew06.omh: user's view of the changes.
var2par.hpp: file defining new function. 
convert.hpp: file that includes var2par.hpp.
ad.hpp: make Var2Par<Base> a friend of AD<Base>.
declare.hpp: forward declaration of Var2Par.
discrete.hpp: change Discrete.cpp to TapeIndex.cpp.
ExampleList.omh: move Discrete.cpp to TapeIndex.cpp and add Var2Par.cpp.
TapeIndex.cpp: move Discrete.cpp to TapeIndex.cpp.
Var2Par.cpp: example for new function.
Discrete.cpp: move Discrete.cpp to TapeIndex.cpp.
Example.vcproj: move Discrete->TapeIndex and add Var2Par & StackMachine.
StachMachine.cpp: fix warnings with MS compiler.
Example.cpp: move Discrete.cpp to TapeIndex.cpp and add Var2Par.cpp.
Makefile.am: move Discrete.cpp to TapeIndex.cpp and add Var2Par.cpp.
CheckIncludeOmh.sh: fix spelling error in message.
CheckIncludeFile.sh: fix spelling error in message.
"
# 
add_list="
	CppAD/local/var2par.hpp
	Example/TapeIndex.cpp
	Example/Var2Par.cpp
"
#
change_list="
	SvnCommit.sh
	omh/WhatsNew06.omh
	CppAD/local/convert.hpp
	CppAD/local/ad.hpp
	CppAD/local/declare.hpp
	CppAD/local/discrete.hpp
	omh/ExampleList.omh
	Example/Example.vcproj
	Example/StackMachine.cpp
	Example/Example.cpp
	Example/Makefile.am
	CheckIncludeFile.sh
	CheckIncludeOmh.sh
"
#
delete_list="
	Example/Discrete.cpp
"
#
copy_branch="" 
# ----------------------------------------------------------------------
if [ "$copy_branch" != "" ]
then
	if [ ! -d ../branches/$copy_branch/.svn ]
	then
		echo "../branches/$copy_branch/.svn is not a directory"
	fi
fi
this_branch=`pwd | sed -e "s|.*/CppAD/||"`
echo "$this_branch: $log_entry" > SvnCommit.log
count=0
for file in $add_list $change_list
do
	count=`expr $count + 1`
	ext=`echo $file | sed -e "s/.*\././"`
	if \
	[ $file != "CheckIncludeFile"  ] && \
	[ $file != "CheckIncludeOmh"   ] && \
	[ $file != "SvnCommit"         ] && \
	[ $file != "SvnCommit.sed"     ] && \
	[ $ext  != ".vcproj"           ] && \
	[ $ext  != ".sln"              ] && \
	[ $ext  != ".vim"              ]
	then
		# automatic edits and backups
		echo "cp $file junk.$count"
		cp $file junk.$count
		sed -f SvnCommit.sed < junk.$count > $file
		diff junk.$count $file
	fi
done
for file in $add_list
do
	echo "svn add $file ?"
	if [ "$copy_branch" != "" ]
	then
		echo "svn add ../branches/$copy_branch/$file ?"
	fi
done
for file in $change_list
do
	if [ "$copy_branch" != "" ]
	then
		echo "cp $file ../branches/$copy_branch/$file ?"
	fi
done
for file in $delete_list
do
	echo "svn delete $file ?"
	if [ "$copy_branch" != "" ]
	then
		echo "svn delete ../branches/$copy_branch/$file ?"
	fi
done
echo "-------------------------------------------------------------"
cat SvnCommit.log
response=""
while [ "$response" != "c" ] && [ "$response" != "a" ]
do
	read -p "continue [c] or abort [a] ? " response
done
if [ "$response" = "a" ]
then
	echo "aborting commit"
	count=0
	for file in $add_list $change_list
	do
		count=`expr $count + 1`
		ext=`echo $file | sed -e "s/.*\././"`
		if \
		[ $file != "CheckIncludeFile"  ] && \
	   	[ $file != "CheckIncludeOmh"   ] && \
	   	[ $file != "SvnCommit"         ] && \
	   	[ $file != "SvnCommit.sed"     ] && \
	   	[ $ext  != ".vcproj"           ] && \
		[ $ext  != ".sln"              ] && \
	   	[ $ext  != ".vim"              ]
		then
			# undo the automatic edits
			echo "mv junk.$count $file"
			mv junk.$count $file
		fi
	done
	exit
fi
echo "continuing commit"
#
for file in $add_list
do
	svn add $file
done
for file in $delete_list
do
	svn delete $file
done
copy_list=""
if [ "$copy_branch" != "" ]
then
	for file in $add_list
	do
		target="../branches/$copy_branch/$file"
		cp $file $target
		svn add $target
		copy_list="$copy_list $target"
	done
	for file in $change_list
	do
		target="../branches/$copy_branch/$file"
		echo "cp $file $target"
		cp $file $target
		copy_list="$copy_list $target"
	done
	for file in $delete_list
	do
		svn delete $target
		target="../branches/$copy_branch/$file"
		copy_list="$copy_list $target"
	done
	
fi
svn commit --username bradbell --file SvnCommit.log $add_list $change_list $delete_list $copy_list
