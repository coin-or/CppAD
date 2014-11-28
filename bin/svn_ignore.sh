#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/svn_ignore.sh" ]
then
	echo "bin/svn_ignore.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
cat << EOF > bin/svn_ignore.$$
aclocal.m4
autom4te.cache
build
check_all.err
commit.sh.old
configure.hpp
doc
doxydoc
doxyfile
doxyfile.*
doxygen.err
junk
junk.*
*.log
new
temp
temp.sh
test_one.exe
test_one.sh
test_one.cpp
*.tmp
EOF
svn propset svn:ignore --recursive -F bin/svn_ignore.$$ .
rm bin/svn_ignore.$$
#
exit 0 
