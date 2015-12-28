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
if [ $0 != "bin/trace.sh" ]
then
	echo "bin/trace.sh: must be executed from its parent directory"
	exit 1
fi
name="$1"
option="$2"
file="cppad/local/$name.hpp"
#
ok='yes'
if [ "$option" != '0' ] && [ "$option" != '1' ]
then
	ok='no'
fi
echo "grep '_TRACE [01]' $file"
if ! grep '_TRACE [01]' $file > /dev/null
then
	ok='no'
fi
if [ "$ok" == 'no' ]
then
	echo 'usage: bin/trace.sh name (0|1)'
	echo '	where the file cppad/local/name.hpp has a defined TRACE option'
	exit 1
fi
old=`grep '_TRACE [01]' $file`
sed -e "s|TRACE [01]|TRACE $option|" -i $file
new=`grep '_TRACE [01]' $file`
#
echo "old: $old"
echo "new: $new"
#
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
