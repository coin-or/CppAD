# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
if [ -e cppad-[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9] ]
then
	dir=`ls -d cppad-[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]`
	echo "check_include_file.sh: must first remove the $dir directory"
	exit 1
fi
echo "Checking difference between C++ include directives and file names."
echo "-------------------------------------------------------------------"
grep '^# *include *<CppAD/' \
	CppAD/*.h \
	CppAD/local/*.hpp \
	example/*.cpp \
	example/*.hpp \
	adolc/*.cpp \
	fadbad/*.cpp \
	test_more/*.cpp \
	speed_cppad/*.cpp |\
sed -e 's%[^<]*<%%' \
    -e 's%>.*$%%' | \
sort -u > junk.1
ls	CppAD/*.h \
	CppAD/local/*.hpp | \
sort -u > junk.2
diff junk.1 junk.2
#
grep '^# *include *"' \
	example/*.cpp | \
sed -e 's%^[^"]*"%example/%' \
    -e 's%".*$%%' | \
sort -u > junk.1
ls	example/*.hpp |\
sort -u > junk.2
diff junk.1 junk.2
#
grep '^# *include *"\.\./example' \
	adolc/*.cpp    \
	fadbad/*.cpp   \
	test_more/*.cpp \
	speed_cppad/*.cpp    | \
sed -e 's%^[^"]*"\.\./%%' \
    -e 's%".*$%%' | \
sort -u > junk.1
ls	example/*.hpp |\
sort -u > junk.2
cat junk.1 junk.2 | sort -u > junk.3
diff -b junk.2 junk.3
#
echo "-------------------------------------------------------------------"
echo "Nothing should be between the two dashed lines above"
echo "If CppAD/config.h appears, run Build all and try again"
