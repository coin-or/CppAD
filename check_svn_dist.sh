#! /bin/bash
# $Id:$
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
#
script="check_svn_dist.sh"
repository="https://projects.coin-or.org/svn/CppAD"
branch=`pwd | sed -e 's|.*/CppAD/||' -e 's|.*/cppad/||'`
#! /bin/bash
if [ -e "svn_dist" ]
then
	echo "rm -rf svn_dist"
	if ! rm -rf svn_dist
	then
		echo "$script: cannot remove old svn_dist directory"
		exit 1
	fi
fi
# ----------------------------------------------------------------------------
echo "svn checkout $repository/$branch svn_dist"
svn checkout $repository/$branch svn_dist
# ----------------------------------------------------------------------------
# Things to do in the svn_dist directory
# ----------------------------------------------------------------------
echo "cd svn_dist"
if ! cd svn_dist
then
	echo "$script: cannot change into svn_dist directory"
	exit 1
fi
#
echo "./build.sh configure"
if ! ./build.sh configure
then
	echo "$script: error durring ./build.sh configure in ./svn_dist"
	exit 1
fi
#
echo "openmp/run.sh"
openmp/run.sh 
# ----------------------------------------------------------------------
# Things to do in the svn_dist/work directory
# ----------------------------------------------------------------------
log_dir=`pwd`
echo "cd work"
if ! cd work
then
	echo "build.sh $1: Cannot change into svn_dist/work directory"
	exit 1
fi
#
echo "make test >& svn_dist/make.log"
echo "The following will give details of progress of command above"
echo "	tail -f svn_dist/make.log"
#
if ! make test >&  $log_dir/make.log
then
	echo "There are errors in svn_dist/make.log"
	exit 1
fi
if grep 'warning:' $log_dir/make.log
then
	tmp=`pwd`
	echo "Stopping because there are warnings in svn_dist/make.log"
	exit 1
fi
#
if ! mv test.log $log_dir
then
	tmp=`pwd`
	echo "Cannot $tmp/test.log to $log_dir/test.log"
	exit 1
fi
echo "OK: make test, results are in svn_dist/test.log" 
