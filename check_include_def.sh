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
if [ -e cppad-[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9] ]
then
	dir=`ls -d cppad-[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]`
	echo "check_include_def.sh: must first remove the $dir directory"
	exit 1
fi
echo "Differences between include file names and directives"
echo "-----------------------------------------------------"
#
grep '^# *ifndef *CppAD[a-zA-Z]*Included$' \
	CppAD/*.h \
	example/*.hpp \
	| sed -e '/ErrorHandler.h:.*# ifndef CppADCppADIncluded/d' \
	| sort -u > junk.1
sed -e 's|\([^.]*\)\.h:.*|\1|' -e 's|^.*/||' < junk.1 > junk.2
sed -e 's|.*# *ifndef *CppAD\([a-zA-Z]*\)Included$|\1|' < junk.1 > junk.3
diff junk.2 junk.3
#
grep '^# *define *CppAD[a-zA-Z]*Included$' \
	CppAD/*.h \
	example/*.hpp \
	| sort -u > junk.1
sed -e 's|\([^.]*\)\.h:.*|\1|' -e 's|^.*/||' < junk.1 > junk.2
sed -e 's|.*# *define *CppAD\([a-zA-Z]*\)Included$|\1|' < junk.1 > junk.3
diff junk.2 junk.3
# --------------------------------------------------------------------------
grep '^# *ifndef *CPPAD_[a-zA-Z_]*_INCLUDED$' \
	CppAD/local/*.hpp \
	| sort -u > junk.1
 
sed -e 's|\([^.]*\)\.hpp:.*|\1|' -e 's|^.*/||' < junk.1 \
	| tr [a-zA-Z] [A-Za-z] > junk.2
sed -e 's|.*# *ifndef *CPPAD_\([a-zA-Z_]*\)_INCLUDED$|\1|' < junk.1 > junk.3
diff junk.2 junk.3
#
grep '^# *define *CPPAD_[a-zA-Z_]*_INCLUDED$' \
 	CppAD/local/*.hpp \
 	| sort -u > junk.1
sed -e 's|\([^.]*\)\.hpp:.*|\1|' -e 's|^.*/||' < junk.1 \
	| tr [a-zA-Z] [A-Za-z] > junk.2
sed -e 's|.*# *define *CPPAD_\([a-zA-Z_]*\)_INCLUDED$|\1|' < junk.1 > junk.3
diff junk.2 junk.3
echo "-----------------------------------------------------"
echo "Nothing should be between the two dashed lines above"
