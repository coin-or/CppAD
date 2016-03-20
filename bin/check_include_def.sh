#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/check_include_def.sh" ]
then
	echo "bin/check_include_def.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Differences between include file names and ifndef at top directives."
echo "Also make sure same ifndef not used by two different files."
echo "-------------------------------------------------------------------"
list_cppad=`bin/ls_files.sh |  \
	sed -n -e '/^cppad\/deprecated\// d' \
	-e '/^cppad\//! d' \
	-e '/^cppad\/base_require.hpp$/ d' \
	-e '/^cppad\/cppad.hpp$/ d' \
	-e '/^cppad\/utility.hpp$/ d' \
	-e '/\.hpp$/p'`
list_other=`bin/ls_files.sh | \
	sed -n \
	-e '/^cppad\/base_require.hpp$/ p' \
	-e '/^cppad\/cppad.hpp$/ p' \
	-e '/^cppad\/utility.hpp$/ p' \
	-e '/^cppad\// d' \
	-e '/\.hpp$/p'`
#
grep '^# *ifndef *CPPAD_[0-9A-Z_]*_HPP$' $list_other \
	| sed -e 's|.*# *ifndef *CPPAD_\([0-9A-Z_]*\)_HPP$|\1.HPP|' \
	| tr [a-zA-Z] [A-Za-z] \
	> check_include_def.1.$$
#
grep '^# *ifndef *CPPAD_[0-9A-Z_]*_HPP$' $list_cppad \
| sed -e 's|.*# *ifndef *CPPAD_\([^_]*\)_\([0-9A-Z_]*\)_HPP$|CPPAD/\1/\2.HPP|' \
| tr [a-zA-Z] [A-Za-z] \
>> check_include_def.1.$$
#
#
echo "$list_other" | sed -e 's|\([^ ]*\)/||g' > check_include_def.2.$$
echo "$list_cppad" >> check_include_def.2.$$
#
sort check_include_def.1.$$ > check_include_def.3.$$
sort check_include_def.2.$$ > check_include_def.4.$$
#
if diff check_include_def.3.$$ check_include_def.4.$$
then
	different="no"
else
	different="yes"
fi
rm check_include_def.[1-4].$$
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
