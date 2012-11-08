#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/check_verbatim.sh" ]
then
	echo "bin/check_verbatim.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# Make sure that OMhelp verbatim commands referr to same file as command
echo "Checking that OMhelp verbatim commands include from file they appear in." 
echo "----------------------------------------------------------------------"
# use 2> to avoid warnings about greping directories, should fix this 
# some other way
grep '$verbatim%' * */* */*/* 2> /dev/null | sed \
	-e '/^bin\//d' \
	-e 's|:.*$verbatim%| |' \
	-e 's|%.*||' \
	-e '/^junk /d' \
	-e '/^doxydoc\//d' \
	-e '/^work\//d' \
	-e '/^build\//d' \
	-e '/^doc\//d' \
	-e '/\/test_one\.cpp /d' \
	-e '/\/speed_[a-z]*\.omh /d' \
	-e '/omh\/license.omh /d' \
	-e '/cppad\/local\/cond_exp.hpp /d' \
	> bin/check_verbatim.1.$$
different="no"
file=""
for this in `cat bin/check_verbatim.1.$$`
do
	
	if [ "$file" == "" ]
	then
		file="$this"
	else
		file_root=`echo $file | sed \
			-e 's|.*/||' -e 's|_hpp\.omh||' -e 's|\.[^.]*$||'`
		this_root=`echo $this | sed \
			-e 's|.*/||' -e 's|\.[^.]*$||'`
		if [ "$file_root" != "$this_root" ]
		then
			echo "in $file references $this"
			different="yes"
		fi
		file=""
	fi
done
for index in 1
do
	rm bin/check_verbatim.$index.$$
done
echo "-------------------------------------------------------------------"
if [ $different = "yes" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
