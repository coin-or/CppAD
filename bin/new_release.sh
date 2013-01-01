#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/new_release.sh" ]
then
	echo "bin/new_release.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
repository="https://projects.coin-or.org/svn/CppAD"
stable_version="20130000"
release="0"
release_version="$stable_version.$release"
msg="Creating releases/$release_version"
# -----------------------------------------------------------------------------
# check initial working directory
dir=`pwd | sed -e 's|.*/[Cc][Pp][Pp][Aa][Dd]/||'`
check="stable/$stable_version"
if [ "$dir" != "$check" ]
then
	echo bin/"new_release.sh: must execute this script in $check"
	exit 1
fi
# -----------------------------------------------------------------------------
# Check release version
if svn list $repository/releases | grep "$release_version" > /dev/null
then
	echo bin/"new_release.sh: Release number $release_version already exists."
	echo "You must first change the assigment"
	echo "	release=$release"
	echo "in file bin/new_release.sh to a higher release number."
	exit 1
fi
echo "svn revert configure.ac"
      svn revert configure.ac
if ! grep "AC_INIT(cppad.*, $release_version" configure.ac > /dev/null
then
	echo bin/"new_release.sh: Change version number in configure.ac to be"
	echo "$release_version, then execute"
	echo "	./build.sh version automake configure"
	echo "then commit the changes."
	exit 1
fi
# -----------------------------------------------------------------------------
rep_stable="$repository/stable/$stable_version"
rep_release="$repository/releases/$release_version"
echo "svn copy $rep_stable $rep_release -m \"$msg\""
      svn copy $rep_stable $rep_release -m "$msg"
# -----------------------------------------------------------------------------
echo "cd ../.."
cd ../..
#
if [ -e conf ]
then
	echo "rm -rf conf"
	      rm -rf conf
fi
echo "svn checkout $repository/conf conf"
      svn checkout $repository/conf conf
#
echo "cd conf"
      cd conf
#
msg="Update stable and release numbers in projDesc.xml"
echo bin/"new_release.sh: $msg"
sed -i projDesc.xml \
	-e "/^ *<stable/,/^ *<\/stable/s/[0-9]\{8\}/$stable_version/" \
	-e "/^ *<release/,/^ *<\/release/s/[0-9]\{8\}\.[0-9]*/$release_version/"
#
echo "Use the command the following command to finish the process"
echo "	svn commit -m \"$msg\" ../../conf/projDesc.xml"
