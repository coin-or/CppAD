#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/doxyfile.sh" ]
then
	echo "bin/doxyfile.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
if [ "$1" == "" ]
then
	echo "usage: bin/doxyfile.sh version"
	echo "creates the doxygen configuration file ./doxyfile"
	exit 1
fi
version="$1"
echo "create bin/doxyfile.$$"
cat << EOF > bin/doxyfile.$$
PROJECT_NAME            = "CppAD: A C++ Algorithmic Differentiation Package"
PROJECT_NUMBER          = $version
OUTPUT_DIRECTORY        = doxydoc
ALWAYS_DETAILED_SEC     = YES
INLINE_INHERITED_MEMB   = YES
FULL_PATH_NAMES         = NO
QT_AUTOBRIEF            = YES
MULTILINE_CPP_IS_BRIEF  = YES
INHERIT_DOCS            = NO
SEPARATE_MEMBER_PAGES   = YES
TAB_SIZE                = 5
BUILTIN_STL_SUPPORT     = YES
SUBGROUPING             = NO
EXTRACT_ALL             = YES
EXTRACT_PRIVATE         = YES
EXTRACT_STATIC          = YES
EXTRACT_LOCAL_CLASSES   = NO
SHOW_INCLUDE_FILES      = NO
SORT_MEMBER_DOCS        = NO
SHOW_DIRECTORIES        = YES
WARN_NO_PARAMDOC        = YES
WARN_LOGFILE            = doxygen.err
INPUT                   = ./cppad  ./cppad/local ./cppad_ipopt/src
FILE_PATTERNS           =  *.hpp  *.cpp
SOURCE_BROWSER          = YES
STRIP_CODE_COMMENTS     = NO
REFERENCED_BY_RELATION  = YES
REFERENCES_LINK_SOURCE  = NO
VERBATIM_HEADERS        = NO
GENERATE_TREEVIEW       = YES
EOF
sed \
	-e 's/^/s|^\\(/' \
	-e 's/ *=/ *=\\).*|\\1/' \
	-e 's/$/|/' \
	-i bin/doxyfile.$$
#
echo "doxygen -g doxyfile > /dev/null"
doxygen -g doxyfile > /dev/null
#
echo "sed -f bin/doxyfile.$$ -i doxyfile"
sed -f bin/doxyfile.$$ -i doxyfile
#
echo "rm bin/doxyfile.$$"
rm bin/doxyfile.$$
