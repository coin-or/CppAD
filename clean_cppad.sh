# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
list=`find .                             \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name 'aclocal.m4'     \) -or \
	\( -name 'autom4te.cache' \) -or \
	\( -name 'config.status'  \) -or \
	\( -name 'cppad-*'        \) -or \
	\( -name 'cygwin_package' \) -or \
	\( -name 'Debug'          \) -or \
	\( -name 'Release'        \) -or \
	\( -name '.deps'          \) -or \
	\( -name 'dev'            \) -or \
	\( -name 'Doc'            \) -or \
	\( -name 'doc'            \) -or \
	\( -name 'htm'            \) -or \
	\( -name 'junk'           \) -or \
	\( -name 'junk?'          \) -or \
	\( -name 'junk.*'         \) -or \
	\( -name 'makefile'       \) -or \
	\( -name 'Release'        \) -or \
	\( -name 'stamp-h1'       \) -or \
	\( -name 'temp'           \) -or \
	\( -name 'temp?'          \) -or \
	\( -name 'temp.*'         \) -or \
	\( -name 'test_one.cpp'   \) -or \
	\( -name '*.a'            \) -or \
	\( -name '*.exe'          \) -or \
	\( -name '*.log'          \) -or \
	\( -name '*.ncb'          \) -or \
	\( -name '*.new'          \) -or \
	\( -name '*.o'            \) -or \
	\( -name '*.obj'          \) -or \
	\( -name '*.old'          \) -or \
	\( -name '*.out'          \) -or \
	\( -name '*.stackdump'    \) -or \
	\( -name '*.suo'          \) -or \
	\( -name '.*.swp'         \) -or \
	\( -name '*.tgz'          \) -or \
	\( -name '*.tmp'          \) -or \
	\( -name '*.zip'          \) -or \
	\( -name '*.[0-9]'                  \) -or \
	\( -name '*.[0-9][0-9]'             \) -or \
	\( -name '*.[0-9][0-9][0-9]'        \) -or \
	\( -name '*.[0-9][0-9][0-9][0-9]'   \) 
`
# remove entries that have /.svn/ in them
list=`echo $list | sed -e 's| [^ ]*/.svn/[^ ]*||g'`
#
if [ "$1" != "-remove" ]
then
 	echo $list | sed -e 's|  *|\n|g'
	echo "use: ./clean_cppad.sh -remove" to remove the files listed above
else
	rm -r -f $list
fi
