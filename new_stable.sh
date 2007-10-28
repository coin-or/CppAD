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
trunk_revision="996"
yyyymmdd="20071016"
yyyy_mm_dd="2007-10-16"
stable_version="2.0"
# -----------------------------------------------------------------------------
repository="http://www.coin-or.org/svn/CppAD"
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
# retrieve stable verison ------------------------------------------
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
# sed edit certain files -----------------------------------------------------
#
list="
	AUTHORS
	cppad/config.h
	configure.ac
	doc.omh
	omh/install_unix.omh
	omh/install_windows.omh
"
#
# change version for entries in list
#
for name in $list
do
	sed $name -f svn_commit.sed | sed > $name.tmp \
		-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/" \
		-e "s/ [0-9]\{8\}\"/ $yyyymmdd\"/" \
		-e "s/\"[0-9]\{8\}\"/\"$yyyymmdd\"/" \
		-e "s/(CppAD, [0-9]\{8\} *,/(CppAD, $yyyymmdd,/" \
		-e "s/cppad-[0-9]\{8\}/cppad-$yyyymmdd/g" \
		-e "s/ [0-9]\{8\}\\\$\\\$/ $yyyymmdd\$\$/"
done
#
# change version for build.sh 
#
sed < build.sh > build.sh.tmp \
	-e "s/yyyymmdd=.*/yyyymmdd=\"$yyyymmdd\"/" \
	-e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/" 
#
# change set of files that svn_status.sh ignores
#
sed < svn_status.sh > svn_status.sh.tmp \
	-e '/AUTHORS/d' \
	-e '/configure.ac/d' \
	-e '/doc.omh/d' \
	-e '/install_unix.omh/d' \
	-e '/install_windows.omh/d' \
	-e '/diff_today.sh/d'
#
list="$list build.sh svn_status.sh"
for name in $list
do
	echo "diff $name $name.tmp"
	diff $name $name.tmp
	echo "mv   $name.tmp $name"
	mv   $name.tmp $name
done
#
# make sure all autoconf and automake output is in repository ----------------
#
list_name=`sed configure.ac \
	-n \
	-e '/END AC_CONFIG_FILES/,$d' \
	-e '1,/AC_CONFIG_FILES/d' \
	-e 's/makefile/&.in/' \
	-e 's/^[ \t]*//' \
	-e '/makefile/p'`
list_name="
	$list_name 
	cppad/config.h.in
	configure 
	depcomp
	install-sh 
	missing
"
for name in $list_name
do
	if [ ! -e $name ]
	then
		echo "$name is not in repository"
		exit 1
	fi
done
#
# Build all sources --------------------------------------------------------- 
#
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
echo "Step 1: cd ../stable/$stable_version"
echo "Step 2: Check differences between repository and local copy."
echo "Step 3: If needs changing, edit CppAD/trunk/new_stable.sh and repeat."
echo "Step 4: svn commit -m \\"
echo "\"stable/$stable_version: set version to $yyyymmdd, add built sources\""
echo "Step 5: Checkout a fresh copy of stable/$stable_version"
echo "Step 6: Test that configure works (without using autoconf or automake)"
echo "Step 8: If problems occur, edit CppAD/trunk/new_stable.sh and repeat."
