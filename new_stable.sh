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
# change version number in files -----------------------------------------------
#
# AUTHORS
sed AUTHORS > AUTHORS.tmp \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/"
echo "diff AUTHORS AUTHORS.tmp"
diff AUTHORS AUTHORS.tmp
mv   AUTHORS.tmp AUTHORS 
#
# configure.ac
sed configure.ac > configure.ac.tmp \
	-e "s/(CppAD, [0-9]\{8\} *,/(CppAD, $yyyymmdd,/"
echo "diff configure.ac configure.ac.tmp"
diff configure.ac configure.ac.tmp
mv   configure.ac.tmp configure.ac
#
# For installs without configure, update cppad/config.h
# (gets overwritten when configure runs).
sed cppad/config.h > cppad/config.tmp \
	-e "s/\"[0-9]\{8\}\"/\"$yyyymmdd\"/" \
	-e "s/ [0-9]\{8\}\"/ $yyyymmdd\"/"
echo "diff cppad/config.h cppad/config.tmp"
diff cppad/config.h cppad/config.tmp
mv   cppad/config.tmp cppad/config.h
#
for name in doc.omh omh/install_unix.omh omh/install_windows.omh
do
	sed $name > $name.tmp \
		-e "s/cppad-[0-9]\{8\}/cppad-$yyyymmdd/g" \
		-e "s/ [0-9]\{8\}\\\$\\\$/ $yyyymmdd\$\$/"
	echo "diff $name $name.tmp"
	diff $name $name.tmp
	mv   $name.tmp $name
done
#
# in build.sh 
sed < build.sh > build.sh.tmp \
	-e "s/yyyymmdd=.*/yyyymmdd=\"$yyyymmdd\"/" \
	-e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/" 
echo "diff build.sh build.sh.tmp"
diff build.sh build.sh.tmp
mv build.sh.tmp build.sh
#
# Create configure script ----------------------------------------------------
#
if [ -e configure ]
then
	configure_before="yes"
else
	configure_before="no"
fi
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
if [ "$configure_before" == "no" ]
then
	if ! svn add configure
	then
		echo "cannot add configure to repository"
		exit 1
	fi
fi
# ---------------------------------------------------------------------------
echo "Change into ../stable/$stable_version and then check the differences."
echo "If you are happy with the result, then execute the command"
echo "	svn commit -m \"stable/$stable_version: set date and create configure\""
echo "Otherwise, modify the new_stable.sh script and re-execute it."
