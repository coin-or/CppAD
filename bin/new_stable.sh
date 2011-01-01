#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/new_stable.sh" ]
then
	echo "bin/new_stable.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
copy_from_trunk="keep"     # do (frist time), keep (use current), redo
trunk_revision="1773"      # trunk revision number that stable corresponds to
yyyy_mm_dd="2011-01-01"    # Date corresponding to this trunk revision
# -----------------------------------------------------------------------------
echo "copy_from_trunk=$copy_from_trunk"
echo "trunk_revision=$trunk_revision"
echo "yyyy_mm_dd=$yyyy_mm_dd"
echo
stable_revision=`expr $trunk_revision + 1`
stable_version=`echo $yyyy_mm_dd | sed -e 's/-//g'`
release_version="$stable_version.0"
repository="https://projects.coin-or.org/svn/CppAD"
rep_trunk="$repository/trunk"
rep_stable="$repository/stable/$stable_version"
#
# check initial working directory
dir=`pwd | sed -e 's|.*/||'`
if [ "$dir" != "trunk" ]
then
	echo bin/"new_stable.sh: must execute this script in the trunk"
	exit 1
fi
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
echo "svn checkout -q -r $stable_revision $rep_stable stable/$stable_version"
      svn checkout -q -r $stable_revision $rep_stable stable/$stable_version
#
# make sure that bin/new_stable.sh corresponds to this version 
# (may not be same as verion in repository that was copied).
echo "cp trunk/bin/new_stable.sh stable/$stable_version/bin/new_stable.sh"
      cp trunk/bin/new_stable.sh stable/$stable_version/bin/new_stable.sh
#
echo "cd stable/$stable_version"
      cd stable/$stable_version
#
# set the value of stable version in corresponding new_release.sh
echo "automatic editing: $stable_version/bin/new_release.sh"
sed -i bin/new_release.sh \
	-e "s/stable_version=.*/stable_version=\"$stable_version\"/"
#
# Inform build.sh that this is a stable version
echo "automatic editing: $stable_version/bin/build.sh"
sed -i bin/build.sh -e 's/^version_type=.*/version_type="stable"/'
#
# Set web for download of corresponding release version
echo "automatic editing: $stable_version/omh/install_windows.omh.in"
dir="http://www.coin-or.org/download/source/CppAD"
sed -i omh/install_windows.omh.in \
	-e "s|cppad-@VERSION@.[cg]pl.tgz|\n$dir/&%\n&|" 
#
# Set web for download of corresponding release version
echo "automatic editing: $stable_version/omh/install_unix.omh.in"
sed -i omh/install_unix.omh.in \
	-e "s|cppad-@VERSION@.[cg]pl.tgz|\n$dir/&%\n&|" 
#
# Instructions --------------------------------------------------------------
cat << EOF
1: In the directory stable/$stable_version, review differences using
       bin/commit.sh list
   All changed files should be present. Review the differences.
2: If you find problems, fix trunk/bin/new_stable.sh, re-run it, and goto 1.
3: In stable/$stable_version run the following command:
      bin/build.sh all
4: If errors occur, fix trunk/bin/new_stable.sh, re-run it, and goto 1.
5: Commit changes to trunk/bin/new_stable.sh.
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
