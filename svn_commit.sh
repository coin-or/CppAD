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
# Define your subversion commit by editing the definition of 
# log_entry, add_list, delete_list, and change_list below:
# 
log_entry="
" 
add_list="
"
delete_list="
"
old_list="
"
#
change_list="
"
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
# old_list
# List of old file names for files that have already been moved in local copy. 
# The new file names must be in the change_list.
#
# change_list
# List of files that are currently in repository and change during this commit.
# ----------------------------------------------------------------------
#
check=\
`echo "$add_list$delete_list$old_list$change_list" | sed -e 's/[ \t\n]*//g'`
if [ "$check" = "" ] 
then
	echo "svn_commit.sh: no files are specified in the argument lists"
	exit 1
fi
for file in $delete_list $old_list 
do
	if [ -e $file ]
	then
		echo "$file exists and is in delete_list or old_list"
		exit 1
	fi
done
for file in $add_list $change_list
do
	if [ ! -e $file ]
	then
		echo "$file does not exist and is in add_list or change_list"
		exit 1
	fi
done
#
this_branch=`pwd | sed -e "s|.*/CppAD/||"`
echo "$this_branch: $log_entry" > svn_commit.log
count=0
for file in $add_list $change_list
do
	count=`expr $count + 1`
	ext=`echo $file | sed -e "s/.*\././"`
	# If you change the conditions below, you must also change the 
	# conditions in the abort section (below in this script).
	if \
	[ -f $file            ]      &&   \
	[ $file != "svn_commit.sh" ] &&   \
	[ $file != "configure" ]     &&   \
	[ $ext  != ".in"     ]       &&   \
	[ $ext  != ".sed"     ]      &&   \
	[ $ext  != ".sln"     ]      &&   \
	[ $ext  != ".vcproj"  ]
	then
		# automatic edits and backups
		echo "cp $file junk.$count"
		cp $file junk.$count
		if [ "$file" != "configure" ]
		then
			sed -f svn_commit.sed < junk.$count > $file
		fi
		diff junk.$count $file
		if [ "$ext" == ".sh" ]
		then
			chmod +x $file
		fi
	fi
done
for file in $add_list
do
	echo "svn add $file ?"
done
for file in $delete_list $old_list
do
	echo "svn delete $file ?"
done
echo "-------------------------------------------------------------"
cat svn_commit.log
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
		[ -f $file            ]      &&   \
		[ $file != "svn_commit.sh" ] &&   \
		[ $file != "configure" ]     &&   \
		[ $ext  != ".in"     ]       &&   \
		[ $ext  != ".sed"     ]      &&   \
		[ $ext  != ".sln"     ]      &&   \
		[ $ext  != ".vcproj"  ]
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
#
svn commit --username bradbell --file svn_commit.log \
	$add_list $change_list $delete_list $old_list
