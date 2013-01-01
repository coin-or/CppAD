#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/new_stable.sh" ]
then
	echo "bin/new_stable.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
copy_from_trunk='keep'     # do (frist time), keep (use current), redo
trunk_revision='2700'      # trunk revision number that stable corresponds to
yyyy_mm_dd='2013-00-00'    # Date corresponding to this trunk revision
# -----------------------------------------------------------------------------
echo "copy_from_trunk=$copy_from_trunk"
echo "trunk_revision=$trunk_revision"
echo "yyyy_mm_dd=$yyyy_mm_dd"
echo '------------------------------------------------------------------------'
stable_version=`echo $yyyy_mm_dd | sed -e 's/-//g'`
release_version="$stable_version.0"
repository="https://projects.coin-or.org/svn/CppAD"
rep_trunk="$repository/trunk"
rep_stable="$repository/stable/$stable_version"
# -----------------------------------------------------------------------------
# check initial working directory
dir=`pwd | sed -e 's|.*/||'`
if [ "$dir" != "trunk" ]
then
	echo bin/"new_stable.sh: must execute this script in the trunk"
	exit 1
fi
# -----------------------------------------------------------------------------
# check copyright date in bin/commit.sh is current
cat << EOF > bin/new_stable.1.$$
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
EOF
yy=`echo $yyyy_mm_dd | sed -e 's|..\(..\).*|\1|'`
cat << EOF > bin/new_stable.2.$$
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-$yy Bradley M. Bell
EOF
sed -i -f bin/commit.sed bin/new_stable.1.$$
if ! diff bin/new_stable.1.$$ bin/new_stable.2.$$ 
then
	echo 'new_stable.sh: Copyright year in bin/commit.sed is out of date.'
	echo 'Fix it and then rerun bin/new_stable.sh.'
	rm bin/new_stable.*.$$ 
	exit 1
fi
rm bin/new_stable.*.$$ 
# -----------------------------------------------------------------------------
echo "cd .."
cd ..
#
if [ "$copy_from_trunk" = "redo" ] 
then
	# delete old copy of branch
	msg="Replacing old stable/$stable_version."
	echo "svn delete $rep_stable -m \"$msg\""
	      svn delete $rep_stable -m "$msg"
fi
if [ "$copy_from_trunk" = "do" ] || [ "$copy_from_trunk" = "redo" ]
then
	#
	# create the new stable version
	temp_1="Create stable/$stable_version"
	temp_2="from trunk at revision $trunk_revision."
	msg="$temp_1 $temp_2"
	echo "svn copy -r $trunk_revision $rep_trunk $rep_stable -m \"$msg\""
	      svn copy -r $trunk_revision $rep_trunk $rep_stable -m "$msg"
	if [ ! -d stable ]
	then
		echo "mkdir stable"
		      mkdir stable
	fi
fi
if [ -e "stable/$stable_version" ]
then
	echo "rm -rf stable/$stable_version"
	      rm -rf stable/$stable_version
fi
#
# retrieve stable version from repository ------------------------------------
#
echo "svn checkout -q $rep_stable stable/$stable_version"
      svn checkout -q $rep_stable stable/$stable_version
#
# make sure that bin/new_stable.sh corresponds to this version 
# (may not be same as version in repository that was copied).
echo "cp trunk/bin/new_stable.sh stable/$stable_version/bin/new_stable.sh"
      cp trunk/bin/new_stable.sh stable/$stable_version/bin/new_stable.sh
#
echo "cd stable/$stable_version"
      cd stable/$stable_version
#
# set the version number in root CMakeLists.txt to $stable_version
echo "bin/version.sh set $release_version"
      bin/version.sh set $release_version
#
# copy version number to other files
echo "bin/version.sh copy"
      bin/version.sh copy 
#
# set the value of stable version in corresponding new_release.sh
echo "automatic editing: $stable_version/bin/new_release.sh"
sed -i bin/new_release.sh \
	-e "s/stable_version=.*/stable_version=\"$stable_version\"/"
#
# Inform build.sh that this is a stable version
echo "automatic editing: $stable_version/build.sh"
sed -i build.sh -e 's/^version_type=.*/version_type="stable"/'
#
# Set download documentation to use web version of corresponding release 
echo "automatic editing of omh/install/download.omh"
dir="http://www.coin-or.org/download/source/CppAD"
sed -i omh/install/download.omh \
	-e "s|cppad-$release_version.[eg]pl.tgz|\n$dir/&%\n&|" 
#
# Instructions --------------------------------------------------------------
cat << EOF
1: In the directory stable/$stable_version, review differences using
       bin/commit.sh list
   All changed files should be present. Review the differences.
2: If you find problems, fix trunk/bin/new_stable.sh, re-run it, and goto 1.
3: In stable/$stable_version run the following commands:
	bin/check_all.sh
      ./build.sh all test
4: If errors occur, fix trunk/bin/new_stable.sh, re-run it, and goto 1.
5: Commit changes to trunk/bin/new_stable.sh 
6: In stable/$stable_version commit changes using
	bin/commit.sh edit
   then edit bin/commit.sh to change the comments and then run
      bin/commit.sh run
7: Make sure commited all necessary changes in stable/$stable_version
      bin/commit.sh list
8: In stable/$stable_version check first, then run the script
      bin/new_release.sh	
EOF
exit 0
