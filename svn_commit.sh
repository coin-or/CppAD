# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
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
# delete_list
# List of files that will be deleted from the repository during this commit.
#
# move_list
# List of files that have been moved in local copy, just the old names. 
# The new file names should go in the change_list.
#
# change_list
# List of files that are currently in repository and change during this commit.
#
# copy_branch
# You may specify up one other branch that should receive a copy of all that 
# changes that you are making with this commit. 
# This other branch cannot be the trunk and you must have a copy of it
# on your local machine. If copy_branch is empty; i.e., copy_branch="", 
# the changes will not be copied (and commited) into another branch.
#
# ----------------------------------------------------------------------
log_entry="Change pow_int so that it is no longer deprecated.

svn_commit.sh: file that made this commit.
whats_new_07.omh: user's view of the changes.
pow.cpp: use pow_int when base is less than zero and exponent is an integer.
makefile.am: include pow_int.hpp in distribution.
include_deprecated.omh: remove pow_int from deprecated list.
library.omh: add library.omh to library children.
whats_new_06.omh: change PowInt to pow_int.
pow.hpp: compare with use of instead.
PowInt.h: just include pow_int.hpp.
cppad.hpp: add pow_int.hpp to list (and sort library list).
pow_int.hpp: move PowInt.h to here.
" 
add_list="
"
delete_list="
"
move_list="
"
#
change_list="
	svn_commit.sh
	omh/whats_new_07.omh
	test_more/pow.cpp
	makefile.am
	omh/include_deprecated.omh
	omh/library.omh
	omh/whats_new_06.omh
	cppad/local/pow.hpp
	cppad/PowInt.h
	cppad/cppad.hpp
	cppad/pow_int.hpp
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
echo "$this_branch: $log_entry" > svn_commit.log
count=0
bad_name=""
for file in $add_list $change_list
do
	count=`expr $count + 1`
	ext=`echo $file | sed -e "s/.*\././"`
	if \
	[ -f $file            ]      &&   \
	[ $file != "svn_commit.sh" ] &&   \
	[ $ext  != ".sed"     ]      &&   \
	[ $ext  != ".vcproj"  ]      &&   \
	[ $ext  != ".sln"     ]      &&   \
	[ $ext  != ".vim"     ]
	then
		# automatic edits and backups
		echo "cp $file junk.$count"
		cp $file junk.$count
		sed -f svn_commit.sed < junk.$count > $file
		diff junk.$count $file
		if [ "$ext" == ".sh" ]
		then
			chmod +x $file
		fi
	fi
	if [ ! -e $file ]
	then
		bad_name="$file"
		echo "$file is not a file or directory not exist"
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
for file in $delete_list $move_list
do
	echo "svn delete $file ?"
	if [ "$copy_branch" != "" ]
	then
		echo "svn delete ../branches/$copy_branch/$file ?"
	fi
done
echo "-------------------------------------------------------------"
cat svn_commit.log
response=""
if [ "$bad_name" != "" ]
then
	echo "bad file or directory name: $bad_name"
	response="a"
else
	while [ "$response" != "c" ] && [ "$response" != "a" ]
	do
		read -p "continue [c] or abort [a] ? " response
	done
fi
if [ "$response" = "a" ]
then
	echo "aborting commit"
	count=0
	for file in $add_list $change_list
	do
		count=`expr $count + 1`
		ext=`echo $file | sed -e "s/.*\././"`
		if \
		[ -f $file            ] && \
		[ $ext  != ".sh"      ] && \
		[ $ext  != ".sed"     ] && \
		[ $ext  != ".vcproj"  ] && \
		[ $ext  != ".sln"     ] && \
		[ $ext  != ".vim"     ]
		then
			# undo the automatic edits
			echo "mv junk.$count $file"
			mv junk.$count $file
			if [ "$ext" == ".sh" ]
			then
				chmod +x $file
			fi
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
	for file in $delete_list $move_list
	do
		svn delete $target
		target="../branches/$copy_branch/$file"
		copy_list="$copy_list $target"
	done
	
fi
svn commit --username bradbell --file svn_commit.log $add_list $change_list $delete_list $move_list $copy_list
