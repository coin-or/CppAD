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
if [ ! -e "bin/check_include_file.sh" ]
then
	msg="must be executed from its parent directory"
	echo "bin/check_include_file.sh: $msg"
	exit 1
fi
# -----------------------------------------------------------------------------
#
echo "Checking difference between C++ include directives and file names."
echo "-------------------------------------------------------------------"
if [ -e check_include_file.1.$$ ]
then
	echo "bin/check_include_file.sh: unexpected bin/check_include_file.1.$$"
	exit 1
fi
for ext in .cpp .hpp
do
	dir_list=`find . -name "*$ext" | sed -e '/junk\.[^.]*$/d' \
		-e 's|^\./||' -e '/^work/d' -e 's|/[^/]*$||' | sort -u`  
	for dir in $dir_list 
	do
		list=`ls $dir/*$ext`
		for file in $list
		do
			sed -n -e '/^# *include *<cppad\//p' $file \
				>> bin/check_include_file.1.$$
		done
	done
done
#
cat bin/check_include_file.1.$$ | \
	sed -e 's%[^<]*<%%'  -e 's%>.*$%%' | \
	sort -u > bin/check_include_file.2.$$
# The file cppad/local/prototype_op.hpp should never be included. 
# All other files should.
ls	cppad/*.hpp \
	cppad/example/*.hpp \
	cppad/ipopt/*.hpp \
	cppad/local/*.hpp \
	cppad/speed/*.hpp | sed \
		-e '/cppad\/local\/prototype_op.hpp/d' \
		-e '/cppad\/example\/eigen_plugin.hpp/d' | \
		sort > bin/check_include_file.3.$$ 
if diff bin/check_include_file.2.$$ bin/check_include_file.3.$$
then
	different="no"
else
	different="yes"
fi
for index in 1 2 3
do
	rm bin/check_include_file.$index.$$
done
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
