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
if [  "$0" != 'bin/check_addon.sh' ]
then
	echo "bin/check_addon: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
addon_list='
	CG
	PY
	TMB
	MIXED
'
grep_cmd=''
for addon in $addon_list
do
	if [ "$grep_cmd" == '' ]
	then
		grep_cmd="CPPAD_${addon}_"
	else
		grep_cmd="$grep_cmd|CPPAD_${addon}_"
	fi
done
#
echo "Checking soruce code for names reserved for addon packages"
echo "-------------------------------------------------------"
ok="yes"
file_list=`bin/ls_files.sh`
for file in $file_list
do
	if grep -E $grep_cmd $file > /dev/null
	then
		echo "$file containts $grep_cmd"
		ok="no"
	fi
done
echo "-------------------------------------------------------"
if [ "$ok" = "no" ]
then
	echo 'bin/check_if.sh: Error'
	exit 1
else
	echo 'bin/check_if.sh: OK'
	exit 0
fi
