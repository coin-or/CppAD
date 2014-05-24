#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/check_svn_id.sh" ]
then
	echo "bin/check_svn_id.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Checking for \$Id:.*\$ in beginning of source code"
echo "-------------------------------------------------------" 
ok="yes"
list=`find . \
	\( -name \*.hpp \) -or \
	\( -name \*.cpp \) -or \
	\( -name \*.omh \) -or \
	\( -name \*.sh \) -or \
	\( -name \*.in \) -or \
	\( -name makefile.am \) -or \
	\( -name CMakeLists.txt \) |
	sed \
		-e '/\/build\//d'  \
		-e '/config\.h\.in/d' \
		-e '/\/junk\./d' \
		-e '/makefile\.in/d'  \
		-e '/\/new\//d'  \
		-e '/\/temp\./d'
`
for file in $list
do
	if ! head -2 $file | grep '$Id:.*\$' > /dev/null
	then
		echo "$file does not have '\$Id:.*\$' in first two lines"
		ok="no"
	fi
done
echo "-------------------------------------------------------" 
if [ "$ok" = "no" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
