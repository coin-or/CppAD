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
copy_from_trunk="keep"     # do (frist time), keep (use current), redo
trunk_revision="1258"      # trunk revision number that stable corresponds to
yyyy_mm_dd="2008-08-26"    # Date corresponding to this trunk revision
# -----------------------------------------------------------------------------
echo "copy_from_trunk=$copy_from_trunk"
echo "trunk_revision=$trunk_revision"
echo "yyyy_mm_dd=$yyyy_mm_dd"
echo
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
	echo "must execute this script in the trunk"
	exit 1
fi
echo "cd .."
cd ..
dir=`pwd`
if [ "$copy_from_trunk" = "redo" ] 
then
	# delete old copy of branch
	msg="Replacing old stable/$stable_version."
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
	temp_1="Create stable/$stable_version"
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
if ! svn checkout --quiet $rep_stable stable/$stable_version
then
	echo "cannot checkout stable/$stable_version"
	exit 1
fi
#
echo "cd stable/$stable_version"
if ! cd stable/$stable_version
then
	echo "Cannot create $dir/stable/$stable_version"
	exit 1
fi
#
# Automatic editing ------------------------------------------------ 
#
sed < build.sh > build.sh.$$ \
	-e "s/yyyymmdd=.*/yyyymmdd=\"$release_version\"/" \
	-e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/" 
#
sed < svn_status.sh > svn_status.sh.$$ \
	-e '/^yyyymmdd=/,$d'
#
dir="http://www.coin-or.org/download/source/CppAD"
sed < omh/install_windows.omh.in > omh/install_windows.omh.in.$$ \
	-e "s|cppad-@VERSION@.[cg]pl.tgz|\n$dir/&%\n&|" 
#
dir="http://www.coin-or.org/download/source/CppAD"
sed < omh/install_unix.omh.in > omh/install_unix.omh.in.$$ \
	-e "s|cppad-@VERSION@.[cg]pl.tgz|\n$dir/&%\n&|" 
list="
	build.sh
	svn_status.sh
	omh/install_windows.omh.in
	omh/install_unix.omh.in
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
# Built sources ------------------------------------------------ 
echo "./build.sh version"
if ! ./build.sh version
then
	echo "Error during ./build.sh version"
	exit 1
fi
#
echo "./build.sh automake"
if ! ./build.sh automake
then
	echo "Error during ./build.sh automake"
	exit 1
fi
#
echo "./build.sh configure"
if ! ./build.sh configure
then
	echo "Error during ./build.sh configure"
	exit 1
fi
#
# Instructions --------------------------------------------------------------
#
echo "1: Review differences using ./svn_status.sh in stable/$stable_version."
echo "   (All the source changes should be present at this time.)"
echo "2: If not correct, fix trunk/new_stable.sh and re-run it and goto 1."
echo "3: In stable/$stable_version run the following commands:"
echo "      ./build.sh omhelp"
echo "      ./build.sh dist"
echo "      ./build.sh test"
echo "4: If errors occur, fix trunk/new_stable.sh and goto 1."
echo "5: Commit changes in stable/$stable_version."
echo "6: Check did all necessary commits in stable/$version with command"
echo "      ./svn_status.sh"
echo
echo "7: In stable/$stable_version run the command \"./build.sh all test\""
echo "8: If errors occur, fix stable/$stable_version and goto 5."
echo
echo "9: Commit the changes to trunk/new_stable.sh."
echo
exit 0
