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
copy_done="yes"
trunk_revision="1087"
yyyymmdd="20071124"
yyyy_mm_dd="2007-11-24"
stable_version="2.1"
# -----------------------------------------------------------------------------
repository="https://projects.coin-or.org/svn/CppAD"
rep_trunk="$repository/trunk"
rep_stable="$repository/stable/$stable_version"
#
# check initial working directory
wd=`pwd | sed -e 's|.*/||'`
if [ "$wd" != "trunk" ]
then
	echo "must start this script in the trunk"
	exit 1
fi
echo "cd .."
cd ..
dir=`pwd`
if [ "$copy_done" == "no" ]
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
echo "svn checkout $rep_stable stable/$stable_version"
svn checkout $rep_stable stable/$stable_version
#
echo "cd stable/$stable_version"
if ! cd stable/$stable_version
then
	echo "Cannot create $dir/stable/$stable_version"
	exit 1
fi
#
# set the proper version -----------------------------------------------------
sed < AUTHORS > AUTHORS.$$ \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/"
sed < configure.ac > configure.ac.$$\
	-e "s/(CppAD, [0-9]\{8\} *,/(CppAD, $yyyymmdd,/" 
sed < omh/download.omh > omh/download.omh.$$ \
	-e "s/cppad-[0-9]\{8\}/cppad-$yyyymmdd/g"
sed < build.sh > build.sh.$$ \
	-e "s/yyyymmdd=.*/yyyymmdd=\"$yyyymmdd\"/" \
	-e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/" 
sed < svn_status.sh > svn_status.sh.$$ \
	-e "s/yyyymmdd=.*/yyyymmdd=\"$yyyymmdd\"/" \
	-e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/"  \
	-e '/^[\t ]*cppad\/config.h$/d'
#
list="
	AUTHORS
	configure.ac
	omh/download.omh
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
# Build all sources --------------------------------------------------------- 
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
echo "--------------------------------------------------------------------"
echo "Step 1: change directory: cd ../stable/$stable_version"
echo "Step 2: review differences: ./svn_status.sh"
echo "Step 3: If not correct, edit CppAD/trunk/new_stable.sh and repeat."
echo "Step 4: use ./svn_commit.sh to commit changes." 
echo "Step 5: Checkout a fresh copy of stable/$stable_version"
echo "Step 6: Test that configure works (without using autoconf or automake)"
echo "Step 8: If problems occur, edit CppAD/trunk/new_stable.sh and repeat."
