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
log_entry="Move ipopt_cppad* to cppad_ipopt*.

This is to facillitate splitting up cppad_ipopt_nlp.* into multiple
files and having CPPAD; see whats_new_10.omh entry for 01-20.
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
check_include_file.sh
check_include_omh.sh
test_more/makefile.am
M       test_more/makefile.in
M       test_more/test_more.cpp
A  +    test_more/cppad_ipopt.cpp
D       test_more/ipopt_cppad.cpp
D       ipopt_cppad
D       ipopt_cppad/speed
D       ipopt_cppad/speed/speed.cpp
D       ipopt_cppad/speed/makefile.in
D       ipopt_cppad/speed/ode_speed.cpp
D       ipopt_cppad/speed/makefile.am
D       ipopt_cppad/src
D       ipopt_cppad/src/ipopt_cppad_nlp.hpp
D       ipopt_cppad/src/ipopt_cppad_nlp.cpp
D       ipopt_cppad/example
D       ipopt_cppad/example/ode_run.hpp
D       ipopt_cppad/example/makefile.in
D       ipopt_cppad/example/ode_check.cpp
D       ipopt_cppad/example/example.sln
D       ipopt_cppad/example/get_started.cpp
D       ipopt_cppad/example/ode_simple.hpp
D       ipopt_cppad/example/makefile.am
D       ipopt_cppad/example/example_windows.bat
D       ipopt_cppad/example/ode1.omh
D       ipopt_cppad/example/ode_fast.hpp
D       ipopt_cppad/example/ode2.omh
D       ipopt_cppad/example/example.cpp
D       ipopt_cppad/example/example.vcproj
D       ipopt_cppad/example/ode_problem.hpp
A  +    cppad_ipopt
M  +    cppad_ipopt/speed/speed.cpp
M  +    cppad_ipopt/speed/makefile.in
M  +    cppad_ipopt/speed/ode_speed.cpp
M  +    cppad_ipopt/speed/makefile.am
D  +    cppad_ipopt/src/ipopt_cppad_nlp.hpp
A  +    cppad_ipopt/src/cppad_ipopt_nlp.hpp
D  +    cppad_ipopt/src/ipopt_cppad_nlp.cpp
A  +    cppad_ipopt/src/cppad_ipopt_nlp.cpp
M  +    cppad_ipopt/example/ode_run.hpp
M  +    cppad_ipopt/example/makefile.in
M  +    cppad_ipopt/example/ode_check.cpp
M  +    cppad_ipopt/example/get_started.cpp
M  +    cppad_ipopt/example/ode_simple.hpp
M  +    cppad_ipopt/example/makefile.am
M  +    cppad_ipopt/example/example_windows.bat
M  +    cppad_ipopt/example/ode1.omh
M  +    cppad_ipopt/example/ode_fast.hpp
M  +    cppad_ipopt/example/ode2.omh
M  +    cppad_ipopt/example/ode_problem.hpp
M       configure.ac
M       check_include_def.sh
M       build.sh
M       makefile.am
M       makefile.in
M       omh/whats_new_10.omh
M       omh/wish_list.omh
M       omh/example_list.omh
M       omh/whats_new_08.omh
M       omh/install_unix.omh.in
M       cppad/local/cppad_assert.hpp
M       cppad/config.h
M       check_svn_dist.sh
M       check_example.sh
diff configure-local configure-subversion
5690c5690
< ac_config_files="$ac_config_files cppad/configure.hpp doxyfile doc.omh example/test_one.sh omh/install_unix.omh omh/install_windows.omh test_more/test_one.sh makefile example/makefile cppad_ipopt/example/makefile cppad_ipopt/speed/makefile introduction/get_started/makefile introduction/exp_apx/makefile print_for/makefile speed/adolc/makefile speed/cppad/makefile speed/double/makefile speed/example/makefile speed/fadbad/makefile speed/profile/makefile speed/sacado/makefile test_more/makefile"
---
> ac_config_files="$ac_config_files cppad/configure.hpp doxyfile doc.omh example/test_one.sh omh/install_unix.omh omh/install_windows.omh test_more/test_one.sh makefile example/makefile ipopt_cppad/example/makefile ipopt_cppad/speed/makefile introduction/get_started/makefile introduction/exp_apx/makefile print_for/makefile speed/adolc/makefile speed/cppad/makefile speed/double/makefile speed/example/makefile speed/fadbad/makefile speed/profile/makefile speed/sacado/makefile test_more/makefile"
6443,6444c6443,6444
<     "cppad_ipopt/example/makefile") CONFIG_FILES="$CONFIG_FILES cppad_ipopt/example/makefile" ;;
<     "cppad_ipopt/speed/makefile") CONFIG_FILES="$CONFIG_FILES cppad_ipopt/speed/makefile" ;;
---
>     "ipopt_cppad/example/makefile") CONFIG_FILES="$CONFIG_FILES ipopt_cppad/example/makefile" ;;
>     "ipopt_cppad/speed/makefile") CONFIG_FILES="$CONFIG_FILES ipopt_cppad/speed/makefile" ;;
diff AUTHORS-local AUTHORS-subversion
diff configure.ac-local configure.ac-subversion
315,316c315,316
< 	cppad_ipopt/example/makefile
< 	cppad_ipopt/speed/makefile
---
> 	ipopt_cppad/example/makefile
> 	ipopt_cppad/speed/makefile
diff cppad/config.h-local cppad/config.h-subversion
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
#
response=""
while [ "$response" != "y" ] && [ "$response" != "n" ]
do
	read -p "run ./check_svn_dist.sh [y/n] ? " response
done
if [ "$response" = "y" ]
then
	if ! ./check_svn_dist.sh
	then
		echo "svn_commit.sh: error running ./check_svn_dist.sh."
		exit 1
	fi
fi
exit 0
