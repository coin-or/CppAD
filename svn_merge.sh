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
# Merge the changes that occurred in FromDirectory into the ToDirectory
#
DryRun="--dry-run"
if [ "$1" == "not--dry-run" ]
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
FromDirectory=branches/intro
#
# Version of the repository corresponding to FromDirectory just before changes
Start=907
# 
# Version of the repository corresponding to FromDirecrtory after the changes
End=923
#
# echo the svn merge command
echo "cd  .."
echo "svn merge $DryRun -r $Start:$End                    \\"
echo "	  http://www.coin-or.org/svn/CppAD/$FromDirectory \\"
echo "	  $ToDirectory                                    \\"
echo "| tee $ToDirectory/svn_merge.out "
#
# Change into the directory corresponding to the entire repository; i.e.,
# http://www.coin-or.org/svn/CppAD/ 
cd ..
#
# execute the svn merge command
svn merge $DryRun -r $Start:$End                         \
    http://www.coin-or.org/svn/CppAD/$FromDirectory      \
    $ToDirectory                                         \
| tee $ToDirectory/svn_merge.out
#
# create commands to go in svn_commit
#
echo "create files for use by svn_commit.sh"
echo "$ToDirectory/svn_merge.move_list"
grep '^D    ' < $ToDirectory/svn_merge.out \
	| sed -e 's/^D */\t/' -e "s|$ToDirectory/||" \
		> $ToDirectory/svn_merge.move_list
echo "$ToDirectory/svn_merge.change_list"
grep '^[UA]    ' < $ToDirectory/svn_merge.out \
	| sed -e 's/^[UA] */\t/'  -e "s|$ToDirectory/||" \
		> $ToDirectory/svn_merge.change_list
