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
if [ "$0" != "bin/svn_merge.sh" ]
then
	echo "bin/svn_merge.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# Merge the changes that occurred in from_branch into the current directory
#
# backup this file (incase it gets overwritten by merge)
cp bin/svn_merge.sh bin/svn_merge.sh.$$
#
# make sure current direction is up to date
svn update
#
# dry_run='--dry-run'
# dry_run=''
dry_run=''
#
# accept='--accept theirs-full'
# accept=''
accept='--accept theirs-full'
#
# Location of the repository
repository='https://projects.coin-or.org/svn/CppAD'
#
# Name of the directory where the changes have been committed
from_branch='branches/forward_dir'
#
# Version of the repository corresponding to from_branch just before changes
Start=3219
#
# Version of the repository corresponding to from_branch after the changes
End=3300
#
# the svn merge command
cmd="svn merge $accept $dry_run -r $Start:$End $repository/$from_branch"
echo "$cmd"
eval $cmd
#
# restore original version of this file
mv bin/svn_merge.sh.$$ bin/svn_merge.sh
#
# get the current status
echo "svn status > svn_merge.log"
svn status > svn_merge.log
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
