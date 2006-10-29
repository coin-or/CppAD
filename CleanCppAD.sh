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
if [ "$1" = "-remove" ]
then
	rm -r -f `find .               \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '*_tape.[0-9]'   \) -or \
	\( -name '*_tape[0-9]'    \) -or \
	\( -name 'aclocal.m4'     \) -or \
	\( -name 'autom4te.cache' \) -or \
	\( -name 'configure'      \) -or \
	\( -name 'config.h.in'    \) -or \
	\( -name 'config.status'  \) -or \
	\( -name 'cppad-*'        \) -or \
        \( -name 'Debug'          \) -or \
        \( -name 'Release'        \) -or \
        \( -name '.deps'          \) -or \
	\( -name 'Dev'            \) -or \
	\( -name 'Doc'            \) -or \
	\( -name 'htm'            \) -or \
	\( -name 'junk'           \) -or \
	\( -name 'junk?'          \) -or \
	\( -name 'junk.*'         \) -or \
	\( -name 'Makefile'       \) -or \
	\( -name 'Makefile.in'    \) -or \
        \( -name 'Release'        \) -or \
        \( -name 'stamp-h1'       \) -or \
	\( -name 'temp'           \) -or \
	\( -name 'temp?'          \) -or \
	\( -name 'temp.*'         \) -or \
	\( -name 'TestOne.cpp'    \) -or \
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
	\( -name '*.zip'          \)`
else
	find .                         \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '_adolc-*'       \) -or \
	\( -name '*_tape.[0-9]'   \) -or \
	\( -name '*_tape[0-9]'    \) -or \
	\( -name 'aclocal.m4'     \) -or \
	\( -name 'autom4te.cache' \) -or \
	\( -name 'configure'      \) -or \
	\( -name 'config.h.in'    \) -or \
	\( -name 'config.status'  \) -or \
	\( -name 'cppad-*'        \) -or \
        \( -name 'Debug'          \) -or \
        \( -name '.deps'          \) -or \
	\( -name 'Dev'            \) -or \
	\( -name 'Doc'            \) -or \
	\( -name 'htm'            \) -or \
	\( -name 'junk'           \) -or \
	\( -name 'junk?'          \) -or \
	\( -name 'junk.*'         \) -or \
	\( -name 'Makefile'       \) -or \
	\( -name 'Makefile.in'    \) -or \
        \( -name 'Release'        \) -or \
        \( -name 'stamp-h1'       \) -or \
	\( -name 'temp'           \) -or \
	\( -name 'temp?'          \) -or \
	\( -name 'temp.*'         \) -or \
	\( -name 'TestOne.cpp'    \) -or \
	\( -name '*.a'            \) -or \
	\( -name '*.exe'          \) -or \
	\( -name '*.gz'           \) -or \
	\( -name '*.log'          \) -or \
	\( -name '*.ncb'          \) -or \
	\( -name '*.new'          \) -or \
	\( -name '*.o'            \) -or \
	\( -name '*.obj'          \) -or \
	\( -name '*.obj'          \) -or \
	\( -name '*.old'          \) -or \
	\( -name '*.stackdump'    \) -or \
	\( -name '*.suo'          \) -or \
	\( -name '.*.swp'         \) -or \
	\( -name '*.tmp'          \) -or \
	\( -name '*.zip'          \)
	echo "To remove files listed above use: CleanCppAD -remove"
fi
