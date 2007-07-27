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
log_entry="Merge in changes from test_vector branch

whats_new_07.omh: user's view of the changes.
" 
add_list="
"
delete_list="
"
move_list="
	cppad/local/cppad_vector.hpp
"
#
change_list="
	svn_commit.sh
	example/unary_plus.cpp
	example/atan_2.cpp
	example/reverse_two.cpp
	example/div_eq.cpp
	example/unary_minus.cpp
	example/lu_vec_ad_ok.cpp
	example/complex_poly.cpp
	example/reverse_one.cpp
	example/value.cpp
	example/tape_index.cpp
	example/log.cpp
	example/vec_ad.cpp
	example/reverse_any.cpp
	example/atan.cpp
	example/acos.cpp
	example/rev_two.cpp
	example/sin.cpp
	example/sub.cpp
	example/rosen_34.cpp
	example/mul.cpp
	example/integer.cpp
	example/rev_one.cpp
	example/abs.cpp
	example/pow.cpp
	example/jacobian.cpp
	example/hes_lu_det.cpp
	example/erf.cpp
	example/var_2par.cpp
	example/copy_ad.cpp
	example/mul_level_adolc.cpp
	example/par_var.cpp
	example/compare_change.cpp
	example/jac_minor_det.cpp
	example/cond_exp.cpp
	example/sinh.cpp
	example/ode_stiff.cpp
	example/bender_quad.cpp
	example/add_eq.cpp
	example/equal_op_seq.cpp
	example/hes_times_dir.cpp
	example/hes_minor_det.cpp
	example/for_two.cpp
	example/for_sparse_jac.cpp
	example/div.cpp
	example/tan.cpp
	example/cos.cpp
	example/rev_sparse_jac.cpp
	example/for_one.cpp
	example/add.cpp
	example/jac_lu_det.cpp
	example/sqrt.cpp
	example/asin.cpp
	example/cosh.cpp
	example/seq_property.cpp
	example/ode_taylor.cpp
	example/ode_taylor_adolc.cpp
	example/forward.cpp
	example/eq.cpp
	example/ode_gear.cpp
	example/ode_gear_control.cpp
	example/rev_sparse_hes.cpp
	example/piecewise.cpp
	example/lu_ratio.cpp
	example/sub_eq.cpp
	example/pow_int.cpp
	example/lu_vec_ad.cpp
	example/log_10.cpp
	example/mul_eq.cpp
	example/poly.cpp
	example/mul_level.cpp
	example/compare.cpp
	example/hessian.cpp
	example/exp.cpp
	example/independent.cpp
	test_more/cond_exp.cpp
	test_more/sinh.cpp
	test_more/cond_exp_ad.cpp
	test_more/atan_2.cpp
	test_more/div_eq.cpp
	test_more/neg.cpp
	test_more/copy.cpp
	test_more/reverse.cpp
	test_more/value.cpp
	test_more/add_eq.cpp
	test_more/for_sparse_jac.cpp
	test_more/log.cpp
	test_more/vec_ad.cpp
	test_more/div_zero_one.cpp
	test_more/div.cpp
	test_more/atan.cpp
	test_more/cos.cpp
	test_more/acos.cpp
	test_more/rev_sparse_jac.cpp
	test_more/add_zero.cpp
	test_more/add.cpp
	test_more/sin.cpp
	test_more/sqrt.cpp
	test_more/sub.cpp
	test_more/asin.cpp
	test_more/cosh.cpp
	test_more/rosen_34.cpp
	test_more/mul.cpp
	test_more/sparse_vec_ad.cpp
	test_more/forward.cpp
	test_more/abs.cpp
	test_more/pow.cpp
	test_more/for_hess.cpp
	test_more/rev_sparse_hes.cpp
	test_more/erf.cpp
	test_more/base_adolc.cpp
	test_more/sub_eq.cpp
	test_more/pow_int.cpp
	test_more/log_10.cpp
	test_more/mul_eq.cpp
	test_more/poly.cpp
	test_more/vec_ad_par.cpp
	test_more/runge_45.cpp
	test_more/sub_zero.cpp
	test_more/compare_change.cpp
	test_more/vec_unary.cpp
	test_more/exp.cpp
	test_more/mul_zero_one.cpp
	test_more/sin_cos.cpp
	print_for/print_for.cpp
	makefile.am
	omh/mul_level.omh
	omh/example.omh
	omh/wish_list.omh
	omh/fun_deprecated.omh
	omh/install_unix.omh
	omh/whats_new_04.omh
	omh/whats_new_05.omh
	omh/whats_new_07.omh
	omh/faq.omh
	grep_source.sh
	doc.omh
	cppad/local/test_vector.hpp
	cppad/local/bender_quad.hpp
	cppad/local/preprocessor.hpp
	cppad/config.h
	cppad/cppad.hpp
	speed/fadbad/poly.cpp
	speed/adolc/poly.cpp
	speed/cppad/det_minor.cpp
	speed/cppad/poly.cpp
	speed/cppad/det_lu.cpp
	speed/example/det_by_lu.cpp
	speed/example/det_by_minor.cpp
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
