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
copy_from_stable="keep"    # do or keep
repository="https://projects.coin-or.org/svn/CppAD"
release_version="0"
# -----------------------------------------------------------------------------
stable_version=`pwd | sed -e 's|.*/||'`
release_version="$stable_version.$release_version"
rep_stable="$repository/stable/$stable_version"
rep_release="$repository/releases/$release_version"
#
if [ "$copy_from_stable" != "do" ] && [ "$copy_from_stable" != "keep" ] 
then
	echo "copy_from_stable must be either \"do\" or \"keep\""
	exit 1
fi

#
# check initial working directory
check=`pwd | sed -e 's|/[^/]*$||' -e 's|.*/||'`
if [ "$check" != "stable" ]
then
        echo "must execute this script from a CppAD/stable/* directory"
        exit 1
fi
echo "cd ../.."
cd ../..
dir=`pwd`
check=`echo $dir | sed -e 's|.*/||'`
if [ "$check" != "CppAD" ]
then
        echo "must execute this script from a CppAD/stable/* directory"
        exit 1
fi
if [ "$copy_from_stable" = "do" ] 
then
	# create the new release verison
	msg="Creating releases/$release_version"
	echo "svn copy $rep_stable $rep_release -m \"$msg\""
	if ! svn copy $rep_stable $rep_release -m "$msg"
	then
		"Cannot create $rep_release"
		exit 1
	fi
	if [ ! -d releases ]
	then
		if ! mkdir releases
		then
			echo "Cannot create the directory $dir/releases"
			exit 1
		fi
	fi
fi
if [ -e releases/$release_version ]
then
	echo "rm -rf releases/$release_version"
	if ! rm -rf releases/$release_version
	then
		echo "cannot remove old copy of $dir/releases/$release_version"
		exit 1
	fi
fi
#
echo "svn checkout --quiet $rep_release releases/$release_version"
svn checkout --quiet $rep_release releases/$release_version
#
echo "cd releases/$release_version"
if ! cd releases/$release_version
then
	echo "Cannot create $dir/releases/$release_version"
	exit 1
fi
sed -i build.sh \
	-e "s/yymmdd=.*/yymmdd=\"$release_version\"/" build.sh 
sed -i configure.ac \
	-e "s/AC_INIT(CppAD,[^,]*,/AC_INIT(CppAD, $release_version,/"
#
./build.sh automake
./build.sh configure
#
echo "Check changes in CppAD/releases/$release_version to the files"
echo "build.sh, configure.ac, configure, and cppad/config.h"
echo "If OK, commit these changes. Then checkout the new version of"
echo "CppAD/releases/$release_version and execute the tests"
echo "	./build.sh configure"
echo "	./build.sh omhelp"
echo "	./build.sh dist"
echo "	./build.sh test"
echo "	./build.sh gpl"
echo "	./build.sh move"
