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
log_entry="Merge in changes from brancehs/optimize.

Made minor changes to these files:
	cppad/local/optimize.hpp
	cppad/local/cap_taylor.hpp
	omh/forward.omh
	omh/seq_property.omh
	omh/whats_new_09.omh
" 
add_list="
"
delete_list="
"
old_list="
"
#
# M     .
change_list="
	check_doxygen.sh
	check_makefile.sh
	example/makefile.am
	example/makefile.in
	example/seq_property.cpp
	example/optimize.cpp
	example/example.cpp
	svn_merge.sh
	test_more/makefile.am
	test_more/makefile.in
	test_more/test_more.cpp
	test_more/optimize.cpp
	build.sh
	makefile.am
	makefile.in
	omh/forward.omh
	omh/example_list.omh
	omh/whats_new_06.omh
	omh/whats_new_09.omh
	omh/seq_property.omh
	cppad/local/ad_fun.hpp
	cppad/local/player.hpp
	cppad/local/prototype_op.hpp
	cppad/local/optimize.hpp
	cppad/local/op_code.hpp
	cppad/local/forward0sweep.hpp
	cppad/local/cap_taylor.hpp
	cppad/local/recorder.hpp
	cppad/local/forward_sweep.hpp
	cppad/local/rev_jac_sweep.hpp
	cppad/local/fun_construct.hpp
	cppad/local/sparse_unary_op.hpp
	cppad/local/reverse_sweep.hpp
	cppad/speed/ode_evaluate.hpp
	speed/fadbad/det_minor.cpp
	speed/fadbad/poly.cpp
	speed/fadbad/ode.cpp
	speed/fadbad/sparse_hessian.cpp
	speed/link_poly.cpp
	speed/adolc/det_minor.cpp
	speed/adolc/poly.cpp
	speed/adolc/ode.cpp
	speed/adolc/sparse_hessian.cpp
	speed/link_ode.cpp
	speed/link_sparse_hessian.cpp
	speed/main.cpp
	speed/cppad/det_minor.cpp
	speed/cppad/poly.cpp
	speed/cppad/ode.cpp
	speed/cppad/sparse_hessian.cpp
	speed/cppad/det_lu.cpp
	speed/double/det_minor.cpp
	speed/double/poly.cpp
	speed/double/ode.cpp
	speed/double/sparse_hessian.cpp
	speed/sacado/det_minor.cpp
	speed/sacado/poly.cpp
	speed/sacado/ode.cpp
	speed/link_det_lu.cpp
	speed/example/ode_evaluate.cpp
	speed/link_det_minor.cpp
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
# List of files that will be deleted from the local copy
# and from the repository during this commit.
#
# old_list
# List of old file names for files that have already been deleted from the
# local copy, but have not yet been deleted from the repository.
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
	if [ "$file" == "svn_commit.sh" ]
	then
		echo "Cannot include svn_commit.sh in list of files to commit."
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
if ! svn commit --username bradbell --file svn_commit.log \
	$add_list $change_list $delete_list $old_list
then
	echo "svn_commit.sh: error during commit."
	exit 1
fi
echo "svn revert svn_commit.sh"
svn revert svn_commit.sh
