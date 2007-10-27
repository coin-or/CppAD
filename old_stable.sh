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
trunk_revision="523"
yyyymmdd="20060913"
yyyy_mm_dd="2006-09-13"
stable_version="1.0"
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
 "cd stable/$stable_version"
if ! cd stable/$stable_version
then
	echo "Cannot create $dir/stable/$stable_version"
	exit 1
fi
#
# sed edit certain files --------------------------------------------------
#
list="
	AUTHORS
	configure.ac
	Doc.omh
	omh/InstallUnix.omh
	omh/InstallWindows.omh
"
#
# change version for files in list
#
for name in $list
do
	sed $name > $name.tmp \
	-e "s/ [0-9][0-9]-[0-9][0-9]-[0-9][0-9]\"/ $yyyymmdd\"/" \
	-e "s/\"[0-9][0-9]-[0-9][0-9]-[0-9][0-9]\"/\"$yyyymmdd\"/" \
	-e "s/(CppAD, [0-9][0-9]-[0-9][0-9]-[0-9][0-9] *,/(CppAD, $yyyymmdd,/" \
	-e "s/CppAD-[0-9][0-9]-[0-9][0-9]-[0-9][0-9]/cppad-$yyyymmdd/" \
	-e "s/ [0-9][0-9]-[0-9][0-9]-[0-9][0-9]\\\$\\\$/ $yyyymmdd\$\$/" \
	-e "s/, [0-9][0-9]-[0-9][0-9]-[0-9][0-9] *,/, $yyyy_mm_dd,/"
	echo "diff $name $name.tmp"
	diff $name $name.tmp
	mv   $name.tmp $name
done
#
# change version in Build
#
sed < Build > Build.tmp \
	-e "s/Today=.*/Today=\"$yyyymmdd\"/" 
#
# change set of files that svn_status.sh ignores
#
sed < SvnStatus > SvnStatus.tmp \
	-e '/install-sh/d' \
	-e '/decpomp/d' \
	-e '/missing/d' \
	-e '/configure/d' \
	-e '/AUTHORS/d' \
	-e '/configure.ac/d' \
	-e '/Doc.omh/d' \
	-e '/InstallUnix.omh/d' \
	-e '/InstallWindow.omh/d' \
	-e '/diff_today.sh/d'
#
list="$list Build SvnStatus"
for name in $list
do
	echo "diff $name $name.tmp"
	diff $name $name.tmp
	echo "mv   $name.tmp $name"
	mv   $name.tmp $name
done
#
# Determine which built sources are not yet in repository -------------------
#
list_name=`sed configure.ac \
	-n \
	-e '/END AC_CONFIG_FILES/,$d' \
	-e '1,/AC_CONFIG_FILES/d' \
	-e 's/Makefile/&.in/' \
	-e 's/^[ \t]*//' \
	-e '/Makefile/p'`
list_name="
	$list_name 
	cppad/config.h.in
	configure 
	depcomp
	install-sh 
	missing
"
list_add=""
for name in $list_name
do
	if [ ! -e $name ]
	then
		list_add="$list_add $name"
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
# Add build sources to repository ------------------------------------
#
for name in $list_add
do
	if [ -h "$name" ]
	then
		echo "convert $name from symbolic link to regular file"
		echo "cp $name $name.tmp"
		if ! cp $name $name.tmp
		then
			echo "cannot convert $name to regular file"
			exit 1
		fi
		echo "mv $name.tmp $name"
		if ! mv $name.tmp $name
		then
			echo "cannot convert $name to regular file"
			exit 1
		fi
	fi
	echo "svn add $name"
	if ! svn add $name
	then
		echo "cannot add $name to repository"
		exit 1
	fi
done
echo "--------------------------------------------------------------------"
echo "Step 1: cd ../stable/$stable_version"
echo "Step 2: Check differences between repository and local copy."
echo "Step 3: If needs changing, edit CppAD/trunk/new_stable.sh and repeat."
echo "Step 4: svn commit -m \\"
echo "\"stable/$stable_version: set version to $yyyymmdd, add built sources\""
echo "Step 5: Checkout a fresh copy of stable/$stable_version"
echo "Step 6: Test that configure works (without using autoconf or automake)"
echo "Step 8: If problems occur, edit CppAD/trunk/new_stable.sh and repeat."
