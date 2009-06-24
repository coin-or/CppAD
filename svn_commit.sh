# ! /bin/bash 
# $Id$
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
log_entry="Merge in changes from branches/sweep.
" 
add_list="
"
delete_list="
"
old_list="
"
#
change_list="
	configure
	svn_status.sh
	check_doxygen.sh
	AUTHORS
	svn_commit.sh
	check_include_file.sh
	example/makefile.in
	example/example.cpp
	doc.omh.in
	svn_merge.sh
	test_more/makefile.in
	test_more/test_more.cpp
	test_more/ipopt_cppad.cpp
	print_for/makefile.in
	ipopt_cppad/makefile.in
	ipopt_cppad/ipopt_cppad_ode.cpp
	ipopt_cppad/ipopt_cppad_simple.cpp
	configure.ac
	build.sh
	makefile.am
	introduction/exp_apx/makefile.in
	introduction/exp_apx/main.cpp
	introduction/get_started/makefile.in
	makefile.in
	omh/whats_new_04.omh
	omh/whats_new_06.omh
	omh/whats_new_09.omh
	omh/install_unix.omh.in
	cppad/speed_test.hpp
	cppad/local/ad_fun.hpp
	cppad/local/player.hpp
	cppad/local/abs_op.hpp
	cppad/local/for_jac_sweep.hpp
	cppad/local/reverse.hpp
	cppad/local/rev_sparse_jac.hpp
	cppad/local/unary_op.hpp
	cppad/local/forward.hpp
	cppad/local/op_code.hpp
	cppad/local/rev_sparse_hes.hpp
	cppad/local/config.h.in
	cppad/local/forward0sweep.hpp
	cppad/local/independent.hpp
	cppad/local/recorder.hpp
	cppad/local/forward_sweep.hpp
	cppad/local/cppad_assert.hpp
	cppad/local/rev_jac_sweep.hpp
	cppad/local/vec_ad.hpp
	cppad/local/define.hpp
	cppad/local/fun_construct.hpp
	cppad/local/rev_hes_sweep.hpp
	cppad/local/dependent.hpp
	cppad/local/op.hpp
	cppad/local/reverse_sweep.hpp
	cppad/config.h
	cppad/configure.hpp.in
	cppad/error_handler.hpp
	cppad/cppad.hpp
	cppad/configure.hpp
	openmp/example_a11c.cpp
	openmp/multi_newton.cpp
	openmp/sum_i_inv.cpp
	openmp/run.sh
	speed/profile/makefile.in
	speed/fadbad/makefile.in
	speed/adolc/makefile.in
	speed/main.cpp
	speed/cppad/makefile.in
	speed/double/makefile.in
	speed/sacado/makefile.in
	speed/example/makefile.in
	speed/example/example.cpp
	dev.omh
"
#
# log_entry
# The contents of this variable will be used as the log entry for the commit.
#
# add_list
# List of files that will be added to the repository during this commit.
# The corresponding files will have svn:keywords "Id" property set on.
# Do not use add_list to add directories; use the following instead:
# 	svn add --non-recursive dir
# 	svn commit directory -m "adding directory dir" dir
# where dir is replaced by the name of the directory. 
# Then use add_list to add the files within that directory.
# If you do not want the Id property set, use svn add before running this
# script and then include the file in the svn_change list.
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
	name=`echo $file | sed -e "s|.*/||"`
	# If you change the conditions below, you must also change the 
	# conditions in the abort section (below in this script).
	if \
	[ -f $file            ]      &&   \
	[ $file != "svn_commit.sh" ] &&   \
	[ $file != "configure" ]     &&   \
	[ $name != "makefile.in"  ]  &&   \
	[ $ext  != ".sed"     ]      &&   \
	[ $ext  != ".sln"     ]      &&   \
	[ $ext  != ".vcproj"  ]
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
done
for file in $add_list
do
	echo "svn add $file ; svn propset svn:keywords "Id" $file ?"
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
		name=`echo $file | sed -e "s|.*/||"`
		if \
		[ -f $file            ]      &&   \
		[ $file != "svn_commit.sh" ] &&   \
		[ $file != "configure" ]     &&   \
		[ $name != "makefile.in"  ]  &&   \
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
	svn propset svn:keywords "Id" $file
done
for file in $delete_list
do
	svn delete $file
done
#
svn commit --username bradbell --file svn_commit.log \
	$add_list $change_list $delete_list $old_list
