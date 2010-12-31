#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/svn_merge.sh" ]
then
	echo "bin/svn_merge.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
#
# Merge the changes that occurred in from_branch into the current directory
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
# Name of the directory where the changes have been committed
from_branch=branches/multiple_result
#
# Version of the repository corresponding to from_branch just before changes
Start=1583
# 
# Version of the repository corresponding to from_branch after the changes
End=1587
#
# the svn merge command
cmd="svn merge $DryRun -r $Start:$End"
cmd="$cmd https://projects.coin-or.org/svn/CppAD/$from_branch"
echo "$cmd"
$cmd
