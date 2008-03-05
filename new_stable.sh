# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
copy_from_trunk="keep"     # do, redo, or keep
trunk_revision="1104"      # trunk revision number that stable corresponds to
yyyy_mm_dd="2007-12-10"    # Date corresponding to this trunk revision
stable_version="2.2"       # stable version number
test_stable="true"         # true or false
# -----------------------------------------------------------------------------
echo "copy_from_trunk=$copy_from_trunk"
echo "trunk_revision=$trunk_revision"
echo "yyyy_mm_dd=$yyyy_mm_dd"
echo "stable_version=$stable_version"
echo "test_stable=$test_stable"
echo
if [ "$test_stable" = "true" ]
then
	if [ "$copy_from_trunk" != "keep" ]
	then
		echo "cannot test until copy_from_trunk = keep"
		exit 1
	fi
fi
yyyymmdd=`echo $yyyy_mm_dd | sed -e 's/-//g'`
repository="https://projects.coin-or.org/svn/CppAD"
rep_trunk="$repository/trunk"
rep_stable="$repository/stable/$stable_version"
#
# check initial working directory
dir=`pwd | sed -e 's|.*/||'`
if [ "$dir" != "trunk" ]
then
	echo "must execute this script in the trunk"
	exit 1
fi
echo "cd .."
cd ..
dir=`pwd`
if [ "$copy_from_trunk" = "redo" ] 
then
	# delete old copy of branch
	msg="Replacing old stable $stable_version."
	echo "svn delete $rep_stable -m \"$msg\""
	if ! svn delete $rep_stable -m "$msg"
	then
		"Cannot remove $rep_stable"
		exit 1
	fi
fi
if [ "$copy_from_trunk" = "do" ] || [ "$copy_from_trunk" = "redo" ]
then
	#
	# create the new stable verison
	temp_1="Create stable version $stable_version"
	temp_2="from trunk revision $trunk_revision."
	msg="$temp_1 $temp_2"
	echo "svn copy -r $trunk_revision $rep_trunk $rep_stable -m \"$msg\""
	if ! svn copy -r $trunk_revision $rep_trunk $rep_stable -m "$msg"
	then
		"Cannot create $rep_stable"
		exit 1
	fi
	if [ ! -d stable ]
	then
		if ! mkdir stable
		then
			echo "Cannot create the directory $dir/stable"
			exit 1
		fi
	fi
fi
if [ -e "stable/$stable_version" ]
then
	echo "rm -rf stable/$stable_version"
	if ! rm -rf stable/$stable_version
	then
		echo "cannot remove old copy of $dir/stable/$stable_version"
		exit 1
	fi
fi
#
# retrieve stable verison from repository ------------------------------------
#
echo "svn checkout --quiet $rep_stable stable/$stable_version"
svn checkout --quiet $rep_stable stable/$stable_version
#
echo "cd stable/$stable_version"
if ! cd stable/$stable_version
then
	echo "Cannot create $dir/stable/$stable_version"
	exit 1
fi
#
# ----------------------------------------------------------------------------
if [ "$test_stable" = "false" ]
then
	# Automatic editing ------------------------------------------------ 
	#
	sed < AUTHORS > AUTHORS.$$ \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/"
	#
	sed < configure.ac > configure.ac.$$\
	-e "s/(CppAD, [0-9]\{8\} *,/(CppAD, $yyyymmdd,/" 
	#
	sed < build.sh > build.sh.$$ \
	-e "s/yyyymmdd=.*/yyyymmdd=\"$yyyymmdd\"/" \
	-e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/" 
	#
	sed < svn_status.sh > svn_status.sh.$$ \
	-e "s/yyyymmdd=.*/yyyymmdd=\"$yyyymmdd\"/" \
	-e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/"  \
	-e '/^[\t ]*cppad\/config.h$/d'
	#
	list="
		AUTHORS
		configure.ac
		build.sh
		svn_status.sh
	"
	for name in $list 
	do
		echo "diff $name $name.$$"
		diff $name $name.$$
		echo "mv   $name.$$ $name"
		if ! mv   $name.$$ $name
		then
			echo "Cannot set the version in $name"
			exit 1
		fi
	done
	#
	# Build all sources ------------------------------------------------ 
	echo "aclocal"
	if ! aclocal
	then
		echo "aclocal failed"
		exit 1
	fi
	#
	echo "autoheader"
	if ! autoheader
	then
		echo "autoheader failed"
		exit 1
	fi
	#
	echo "autoconf"
	if ! autoconf
	then
		echo "autoconf failed"
		exit 1
	fi
	#
	echo "automake --add-missing"
	if ! automake --add-missing
	then
		echo "automake failed"
		exit 1
	fi
	#
	echo "Change directory: cd ../stable/$stable_version."
	echo "Review differences: ./svn_status.sh."
	echo "If not correct, edit CppAD/trunk/new_stable.sh and repeat."
	echo "If correct, commit the changes and then run"
	echo "CppAD/trunk/new_stable.sh with test_stable equal to true."
	echo 
	echo "If errors occur during the test, commit changes to fix them"
	echo "in stable/$stable_version and then repeat the test."
	echo
	exit 0
fi
# -------------------------------------------------------------------------
# check include files
cmd="./check_include_def.sh"
echo "$cmd" 
echo "$cmd" > $dir/trunk/new_stable.log
if ! $cmd  >> $dir/trunk/new_stable.log
then
	echo "Error detected."
	exit 1
fi
cmd="./check_include_file.sh"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
cmd="./check_include_omh.sh"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
# -------------------------------------------------------------------------
# check creating documentation 
cmd="mkdir doc"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
cmd="cd doc"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
cmd="omhelp ../doc.omh -noframe -debug -l http://www.coin-or.org/CppAD/"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
cmd="omhelp ../doc.omh -noframe -debug -l http://www.coin-or.org/CppAD/ -xml"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
#
cmd="cd .."
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
# -------------------------------------------------------------------------
# check programs
cmd="./build.sh configure test"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
#
cmd="make"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
list="
	example/example
	introduction/exp_apx/exp_apx
	introduction/get_started/get_started
	test_more/test_more
"
for program in $list
do
	cmd="./$program"
	echo "$cmd"
	echo "$cmd" >> $dir/trunk/new_stable.log
	if ! $cmd   >> $dir/trunk/new_stable.log
	then
		echo "Error detected"
		exit 1
	fi
done
list="
	adolc
	cppad
	double
	example
	fadbad
	profile
	sacado
"
seed="123"
for name in $list
do
	# Note that example does not use command line arguments,
	# but it does not currently care about their presence.
	cmd="./speed/$name/$name correct  $seed"
	echo "$cmd"
	echo "$cmd" >> $dir/trunk/new_stable.log
	if ! $cmd   >> $dir/trunk/new_stable.log
	then
		echo "Error detected"
		exit 1
	fi
done
cmd="openmp/run.sh"
echo "$cmd"
echo "$cmd" >> $dir/trunk/new_stable.log
if ! $cmd   >> $dir/trunk/new_stable.log
then
	echo "Error detected"
	exit 1
fi
echo "-------------------------------------------------------"
echo "All tests passed; see new_stable.log in this directory."
