#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/check_svn_dist.sh" ]
then
	echo "bin/check_svn_dist.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
log_dir=`pwd`
script="bin/check_svn_dist.sh"
repository="https://projects.coin-or.org/svn/CppAD"
branch=`pwd | sed -e 's|.*/CppAD/||' -e 's|.*/cppad/||'`
if [ -e "svn_dist" ]
then
	echo "rm -rf svn_dist"
	rm -rf svn_dist
fi
# ----------------------------------------------------------------------------
echo "svn checkout $repository/$branch svn_dist"
svn checkout $repository/$branch svn_dist
# ----------------------------------------------------------------------------
# Things to do in the svn_dist directory
# ----------------------------------------------------------------------
echo "cd svn_dist"
cd svn_dist
#
echo "bin/autotools.sh configure"
if ! bin/autotools.sh configure
then
	echo "$script: error durring bin/autotools.sh configure in ./svn_dist"
	exit 1
fi
#
# ----------------------------------------------------------------------
# Things to do in the svn_dist/work directory
# ----------------------------------------------------------------------
echo "cd work"
cd work
#
echo "$script: make test >& svn_dist.log"
echo "The following will give details of progress of command above"
echo "	tail -f svn_dist.log"
make test >&  $log_dir/svn_dist.log
#
if grep 'warning:' $log_dir/svn_dist.log
then
	echo "$script: Stopping because there are warnings in svn_dist.log"
	exit 1
fi
#
echo "cat test.log >> svn_dist.log"
cat test.log >> $log_dir/svn_dist.log
#
echo "OK: $script, results are in svn_dist.log"
