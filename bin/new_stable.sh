#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
echo 'Need to change this from using git-svn to using push_git2svn.sh'
echo 'See bin/new_release.sh.'
exit 1
if [ "$0" != "bin/new_stable.sh" ]
then
	echo "bin/new_stable.sh: must be executed from its parent directory"
	exit 1
fi
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
copy_from_trunk='keep'     # do (frist time), keep (use current)
trunk_revision='3507'      # trunk revision number that stable corresponds to
yyyy_mm_dd='2015-00-00'    # Date corresponding to this trunk revision
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
if [ "$copy_from_trunk" = "redo" ]
then
	# delete old stable copy
	msg="Replacing old stable/$stable_version."
	echo "svn delete $rep_stable -m \"$msg\""
	svn delete $rep_stable -m "$msg"
fi
# -----------------------------------------------------------------------------
if [ "$copy_from_trunk" = "do" ] || [ "$copy_from_trunk" = "redo" ]
then
	#
	# create the new stable copy
	temp_1="Create stable/$stable_version"
	temp_2="from trunk at revision $trunk_revision."
	msg="$temp_1 $temp_2"
	echo "svn copy -r $trunk_revision $rep_trunk $rep_stable -m \"$msg\""
	svn copy -r $trunk_revision $rep_trunk $rep_stable -m "$msg"
fi
# -----------------------------------------------------------------------------
echo 'Use git-svn to fetch the new stable version'
fetch="stable/$stable_version:refs/remotes/svn/stable/$stable_version"
if ! grep "fetch *= *$fetch" .git/config > /dev/null
then
	sed -e "s|^\turl *=.*|&\n\tfetch = $fetch|" -i .git/config
fi
#
# fetch the branch
echo_eval git svn fetch
# -----------------------------------------------------------------------------
# checkout the new stable version
if git branch | grep "$stable_version"
then
	git checkout $stable_version
else
	echo_eval git checkout -b $stable_version svn/stable/$stable_version
fi
# make sure that bin/new_stable.sh corresponds to current master version
# (may not be same as version in repository that was copied).
echo_eval git show master:bin/new_stable.sh > bin/new_stable.sh
#
# set the version number in root CMakeLists.txt to $stable_version
echo_eval bin/version.sh set $release_version
#
# copy version number to other files
echo_eval bin/version.sh copy
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
1: Review differences using git. If you find problems, fix both
   master and $new_stable versions of bin/new_stable.sh.
3: Run the following commands:
	bin/check_all.sh
4: If errors occur, fix both master and $new_stable version of
   bin/new_stable.sh and goto 3.
5: Run the script
      bin/new_release.sh
EOF
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
