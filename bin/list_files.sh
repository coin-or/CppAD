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
if [ $0 != "bin/list_files.sh" ]
then
	echo "bin/list_files.sh: must be executed from its parent directory"
	exit 1
fi
#
if [ "$1" == '' ]
then
cat << EOF
usage: list_files.sh ext_1 [ ext_2 ... ]

lists files with the specified extensions excluding those of the form:
*/new/*, build/*, doc/*, doxydoc/*, */test_one.cpp, /junk\$ext, */temp\$ext

where \$ext takes the values ext_1, ext_2, ...
File and directory names do not begin with './'.
EOF
fi
for ext in $*
do
	# should change '.' to '[.]' before executing sed comand below
	find . -name "*$ext" | sed \
		-e '/[/]new[/]/d' \
		-e '/build[/]/d' \
		-e '/doc[/]/d' \
		-e '/doxydoc[/]/d' \
		-e '/[/]test_one.cpp$/d' \
		-e "/[/]junk$ext"'$/d' \
		-e "/[/]temp$ext"'$/d' \
		-e 's|^[.]/||'
done
