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
if [ $0 != "bin/check_user_def.sh" ]
then
	echo "bin/check_user_def.sh: must be executed from its parent directory"
	exit 1
fi
# ---------------------------------------------------------------------------
echo 'Check user API preprocessor define symbols'
echo '-----------------------------------------------------------------------'
file_list=`bin/search.sh 'head CPPAD' | sed -e '/bin\/check_user_def.sh/d'`
symbol_list=''
for file in $file_list
do
	symbol=`sed -n -e '/$head CPPAD/p' -e '/$subhead CPPAD/p' $file | sed \
		-e 's/^.*head \(CPPAD[a-zA-Z0-9_]*\).*/\1/'`
	symbol_list="$symbol_list $symbol:$file"
done
for symbol_file in $symbol_list
do
	symbol=`echo $symbol_file | sed -e 's|:.*||'`
	file=`echo $symbol_file | sed -e 's|.*:||'`
	if ! grep $symbol omh/preprocessor.omh > /dev/null
	then
		echo "The symbol $symbol"
		echo "appears in $file omhelp documentation"
		echo 'but does not appear in omh/preprocessor.omh'
		exit 1
	fi
done
for symbol_file in $symbol_list
do
	symbol=`echo $symbol_file | sed -e 's|:.*||'`
	file=`echo $symbol_file | sed -e 's|.*:||'`
	ok='false'
	if grep "$symbol *in user api" cppad/core/undef.hpp > /dev/null
	then
		ok='true'
	fi
	if grep "$symbol *in deprecated api" cppad/core/undef.hpp > /dev/null
	then
		ok='true'
	fi
	if [ "$ok" == 'false' ]
	then
		echo "The symbol $symbol"
		echo "appears in $file omhelp documentation"
		echo "but is not listed as in user api in cppad/core/undef.hpp"
		exit 1
	fi
done
echo '-----------------------------------------------------------------------'
echo "check_user_def.sh: OK"

