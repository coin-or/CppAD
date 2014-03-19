#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/svn_merge.sh" ]
then
	echo "bin/svn_merge.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
#
# Merge the changes that occurred in from_branch into the current directory
#
# dry_run='--dry-run'
dry_run=''
#
# accept='-accept theirs-full'
accept=''
#
# script to help in execution of svn merge command
# 
# Location of the repository
repository='https://projects.coin-or.org/svn/CppAD'
#
# Name of the directory where the changes have been committed
from_branch='branches/mul_dir'
#
# Version of the repository corresponding to from_branch just before changes
Start=3175
# 
# Version of the repository corresponding to from_branch after the changes
End=3213
#
# the svn merge command
cmd="svn merge $accept $dry_run -r $Start:$End $repository/$from_branch"
echo "$cmd"
$cmd
