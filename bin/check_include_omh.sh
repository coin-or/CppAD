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
find . \( -name '*.c'   \) -or \
       \( -name '*.cpp' \) -or \
       \( -name '*.hpp' \) -or \
       \( -name '*.omh' \) -or \
       \( -name '*.txt' \) -or \
       \( -name '*.am' \) | sed \
		-e '/.\/work\//d' \
		-e '/.\/build\//d' \
		-e '/.\/new\//d' \
		-e '/.\/doc\//d' \
		-e 's|./||' \
		> bin/check_include_omh.1.$$
for file in `cat bin/check_include_omh.1.$$`
do
	# assume $childtable, ... , $verbatim use % for delimiter
	# also assume verbatim commands use one line (would be nice to allow
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
	-e 's/^[ 	]*//' \
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
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
