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
log_entry="Merge from branches/openmp to trunk.

svn_commit.sh: file that made this commit.
" 
add_list="
"
delete_list="
"
move_list="
	example/mul_tape.cpp
	omh/mul_tape.omh
	speed/det_by_minor.hpp
	speed/det_grad_33.hpp
	speed/uniform_01.hpp
	speed/det_of_minor.hpp
	speed/det_by_lu.hpp
"
#
change_list="
	svn_commit.sh
	check_include_file.sh
	example/lu_vec_ad_ok.cpp
	example/makefile.am
	example/hes_lu_det.cpp
	example/par_var.cpp
	example/jac_minor_det.cpp
	example/hes_minor_det.cpp
	example/jac_lu_det.cpp
	example/example.vcproj
	example/mul_level.cpp
	example/fun_check.cpp
	example/example.cpp
	check_include_omh.sh
	test_more/test_more.cpp
	test_more/makefile.am
	test_more/fun_check.cpp
	configure.ac
	dos_format.sh
	check_include_def.sh
	build.sh
	makefile.am
	omh/forward.omh
	omh/speed_utility.omh
	omh/mul_level.omh
	omh/uniform_01_hpp.omh
	omh/glossary.omh
	omh/wish_list.omh
	omh/fun_deprecated.omh
	omh/det_by_minor_hpp.omh
	omh/det_grad_33_hpp.omh
	omh/det_of_minor_hpp.omh
	omh/install_subversion.omh
	omh/example_list.omh
	omh/whats_new_03.omh
	omh/whats_new_04.omh
	omh/whats_new_05.omh
	omh/whats_new_06.omh
	omh/whats_new_07.omh
	omh/faq.omh
	omh/det_by_lu_hpp.omh
	doc.omh
	cppad/speed_test.hpp
	cppad/local/exp_op.hpp
	cppad/local/par_var.hpp
	cppad/local/mul_op.hpp
	cppad/local/ad_fun.hpp
	cppad/local/ad_tape.hpp
	cppad/local/cond_exp.hpp
	cppad/local/abs_op.hpp
	cppad/local/copy_base.hpp
	cppad/local/atan2.hpp
	cppad/local/bender_quad.hpp
	cppad/local/equal_op_seq.hpp
	cppad/local/add_eq.hpp
	cppad/local/div_op.hpp
	cppad/local/div.hpp
	cppad/local/ad.hpp
	cppad/local/preprocessor.hpp
	cppad/local/omp_max_thread.hpp
	cppad/local/asin_op.hpp
	cppad/local/add.hpp
	cppad/local/near_equal_ext.hpp
	cppad/local/declare.hpp
	cppad/local/undef.hpp
	cppad/local/discrete.hpp
	cppad/local/bool_fun.hpp
	cppad/local/eq.hpp
	cppad/local/lu_ratio.hpp
	cppad/local/sub_eq.hpp
	cppad/local/atan_op.hpp
	cppad/local/mul_eq.hpp
	cppad/local/fun_check.hpp
	cppad/local/compare.hpp
	cppad/local/sqrt_op.hpp
	cppad/local/independent.hpp
	cppad/local/cap_taylor.hpp
	cppad/local/print_for.hpp
	cppad/local/div_eq.hpp
	cppad/local/value.hpp
	cppad/local/vec_ad.hpp
	cppad/local/user_ad.hpp
	cppad/local/add_op.hpp
	cppad/local/sub.hpp
	cppad/local/define.hpp
	cppad/local/fun_construct.hpp
	cppad/local/mul.hpp
	cppad/local/integer.hpp
	cppad/local/output.hpp
	cppad/local/abs.hpp
	cppad/local/pow.hpp
	cppad/local/dependent.hpp
	cppad/local/tape_link.hpp
	cppad/local/erf.hpp
	cppad/local/op.hpp
	cppad/local/copy_ad.hpp
	cppad/local/log_op.hpp
	cppad/local/std_math_unary.hpp
	cppad/local/acos_op.hpp
	cppad/local/sub_op.hpp
	cppad/config.h
	cppad/error_handler.hpp
	cppad/cppad.hpp
	cppad/track_new_del.hpp
	cppad/speed
	cppad/speed/det_by_minor.hpp
	cppad/speed/det_grad_33.hpp
	cppad/speed/uniform_01.hpp
	cppad/speed/det_of_minor.hpp
	cppad/speed/det_by_lu.hpp
	openmp
	openmp/multi_newton.cpp
	openmp/multi_newton.hpp
	openmp/run.sh
	dev.omh
	speed/fadbad/det_minor.cpp
	speed/fadbad/poly.cpp
	speed/fadbad/det_lu.cpp
	speed/adolc/det_minor.cpp
	speed/adolc/poly.cpp
	speed/adolc/det_lu.cpp
	speed/main.cpp
	speed/cppad/det_minor.cpp
	speed/cppad/poly.cpp
	speed/cppad/det_lu.cpp
	speed/example/det_of_minor.cpp
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
