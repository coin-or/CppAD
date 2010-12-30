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
if [ $0 != "bin/check_include_def.sh" ]
then
	echo "bin/check_include_def.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Differences between include file names and ifndef at top directives."
echo "Also make sure same ifndef not used by two different files."
echo "-------------------------------------------------------------------"
# note CPPAD_CPPAD_INCLUDED is a special case, see cppad/error_handler.hpp
grep '^# *ifndef *CPPAD_[0-9a-zA-Z_]*_INCLUDED$' \
	cppad_ipopt/*/*.hpp \
	cppad/*.hpp \
	cppad/local/*.hpp \
	cppad/speed/*.hpp \
	example/*.hpp \
	openmp/*.hpp \
	| sed \
	-e 's|.*# *ifndef *CPPAD_\([0-9a-zA-Z_]*\)_INCLUDED$|\1.HPP|' \
	| tr [a-zA-Z] [A-Za-z] \
	| sort \
	> bin/check_include_def.1.$$
 
ls \
	cppad_ipopt/*/*.hpp \
	cppad/*.hpp \
	cppad/local/*.hpp \
	cppad/speed/*.hpp \
	example/*.hpp \
	openmp/*.hpp \
	| sed -e 's|.*/||' -e 's|^cppad_||' \
	| sort -u \
	> bin/check_include_def.2.$$
if diff bin/check_include_def.1.$$ bin/check_include_def.2.$$
then
	different="no"
else
	different="yes"
fi
rm bin/check_include_def.1.$$
rm bin/check_include_def.2.$$
#
echo "-------------------------------------------------------------------"
if [ $different = "yes" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
