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
#
# Merge the changes that occurred in FromDirectory into the ToDirectory
#
DryRun="--dry-run"
if [ "$1" == "not-dry-run" ]
then
	DryRun=""
fi
echo "DryRun = $DryRun"
#
# script to help in execution of svn merge command
#
# Name of the directory that will receive the changes
ToDirectory=trunk
#
# Name of the directory where the changes have been committed
FromDirectory=branches/speed
#
# Version of the repository corresponding to FromDirectory just before changes
Start=1197
# 
# Version of the repository corresponding to FromDirecrtory after the changes
End=HEAD
#
# Change into the directory corresponding to the entire repository; i.e.,
# https://www.coin-or.org/svn/CppAD/ 
echo "cd .."
cd ..
#
# the svn merge command
args="merge $DryRun -r $Start:$End"
args="$args https://projects.coin-or.org/svn/CppAD/$FromDirectory"
args="$args $ToDirectory"
echo "svn $args"
svn $args
#
# create commands to go in svn_commit
#
echo "create files for use by svn_commit.sh"
echo "$ToDirectory/svn_merge.old"
grep '^D    ' < $ToDirectory/svn_merge.out \
	| sed -e 's/^D */\t/' -e "s|$ToDirectory/||" \
		> $ToDirectory/svn_merge.old
echo "$ToDirectory/svn_merge.change_list"
grep '^[UA]    ' < $ToDirectory/svn_merge.out \
	| sed -e 's/^[UA] */\t/'  -e "s|$ToDirectory/||" \
		> $ToDirectory/svn_merge.change_list
