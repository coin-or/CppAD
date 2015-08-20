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
if [ ! -e "bin/check_include_omh.sh" ]
then
	echo "bin/check_include_omh.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# Make sure omhelp, under cygwin, has not matched file names with wrong case.
echo "Checking difference between OMhelp include directives and file names."
echo "----------------------------------------------------------------------"
# super list of file names that are referenced by omhelp commands
bin/list_files.sh | sed -n \
	-e '/\.c$/p' \
	-e '/\.cpp$/p' \
	-e '/\.hpp$/p' \
	-e '/\.omh$/p' \
	-e '/\.txt$/p' \
	-e '/\.am$/p' \
	> bin/check_include_omh.1.$$
#
# add *.sh files with omhelp documentation
ls bin/get_*.sh >> bin/check_include_omh.1.$$
#
for file in `cat bin/check_include_omh.1.$$`
do
	# 1. assume $childtable, ... , $verbatim use % for delimiter
	# 2. assume verbatim commands use one line.
	# 3. assume $childtable, $children, $contents use multiple lines.
	# multiple line verbatim commands).
	sed -n < $file >> bin/check_include_omh.2.$$ \
		-e 's/^#[ \t][ \t]*//' \
		-e '/$childtable%/,/%$\$/p' \
		-e '/$children%/,/%$\$/p' \
		-e '/$contents%/,/%$\$/p' \
		-e '/$verbatim%/p'
done
sed < bin/check_include_omh.2.$$ > bin/check_include_omh.3.$$ \
	-e 's/$childtable//' \
	-e 's/$children//' \
	-e 's/$contents//' \
	-e 's/$verbatim%\([^%]*\)%.*/\1/' \
	-e 's/%//' \
	-e 's/$\$//' \
	-e '/^ *$/d' \
	-e 's/^[	]*//' \
	-e 's|\\|/|g'
#
different="no"
for file in `cat bin/check_include_omh.3.$$`
do
	if [ "`grep $file bin/check_include_omh.1.$$`" == "" ]
	then
		echo $file
		different="yes"
	fi
done
for index in 1 2 3
do
	rm bin/check_include_omh.$index.$$
done
#
echo "-------------------------------------------------------------------"
if [ $different = "yes" ]
then
	echo "Error: nothing should be between the two dashed lines above."
	echo 'Perhaps this file is not and has not yet been added to repository ?'
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
