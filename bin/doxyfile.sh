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
if [ ! -e "bin/doxyfile.sh" ]
then
	echo "bin/doxyfile.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
if [ "$3" == "" ]
then
	echo "usage: bin/doxyfile.sh version error_file output_directory"
	echo "creates the doxygen configuration file ./doxyfile"
	exit 1
fi
version="$1"
error_file="$2"
output_directory="$3"
echo "create bin/doxyfile.$$"
cat << EOF > bin/doxyfile.$$
ALWAYS_DETAILED_SEC     = YES
BUILTIN_STL_SUPPORT     = YES
EXTRACT_ALL             = YES
EXTRACT_LOCAL_CLASSES   = NO
EXTRACT_PRIVATE         = YES
EXTRACT_STATIC          = YES
FILE_PATTERNS           =  *.hpp  *.cpp
FULL_PATH_NAMES         = NO
GENERATE_LATEX          = NO
GENERATE_TREEVIEW       = YES
INHERIT_DOCS            = NO
INLINE_INHERITED_MEMB   = YES
INPUT                   = ./cppad  ./cppad/local ./cppad_ipopt/src ./speed/src
MULTILINE_CPP_IS_BRIEF  = YES
OUTPUT_DIRECTORY        = $output_directory
PROJECT_NAME            = "CppAD: A C++ Algorithmic Differentiation Package"
PROJECT_NUMBER          = $version
QT_AUTOBRIEF            = YES
REFERENCED_BY_RELATION  = YES
REFERENCES_LINK_SOURCE  = NO
SEPARATE_MEMBER_PAGES   = YES
SHOW_DIRECTORIES        = YES
SHOW_INCLUDE_FILES      = NO
SORT_GROUP_NAMES        = YES
SORT_MEMBER_DOCS        = NO
SOURCE_BROWSER          = YES
STRIP_CODE_COMMENTS     = NO
SUBGROUPING             = NO
TAB_SIZE                = 5
VERBATIM_HEADERS        = NO
WARN_LOGFILE            = $error_file
WARN_NO_PARAMDOC        = YES
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
