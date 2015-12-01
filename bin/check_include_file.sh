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
list=`bin/list_files.sh | sed -n \
	-e '/\.cpp$/p' \
	-e '/\.hpp$/p'`
for file in $list
do
	sed -n -e '/^# *include *<cppad\//p' $file \
		>> bin/check_include_file.1.$$
done
#
cat bin/check_include_file.1.$$ | \
	sed -e 's%[^<]*<%%'  -e 's%>.*$%%' | \
	sort -u > bin/check_include_file.2.$$
#
# The following files should never be included:
#	cppad/local/prototype_op.hpp
#	cppad/example/eigen_plugin.hpp
# All other files should.
# The file cppad/configure.hpp may not yet be created.
bin/list_files.sh | sed -n -e '/cppad\/.*\.hpp$/p' | \
	sed \
		-e '1,1s|^|cppad/configure.hpp\n|' \
		-e '/cppad\/local\/prototype_op.hpp/d' \
		-e '/cppad\/example\/eigen_plugin.hpp/d'  \
		-e '/cppad\/deprecated\//d' | \
	sort -u > bin/check_include_file.3.$$
#
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
