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
if [ $0 != "bin/list_files.sh" ]
then
	echo "bin/list_files.sh: must be executed from its parent directory"
	exit 1
fi
if [ -e .git ]
then
	git ls-files
elif [ -e .svn ]
then
	svn list --recursive | sed -e '/\/$/d'
else
	echo 'cannot find ./.git or ./.svn'
fi
