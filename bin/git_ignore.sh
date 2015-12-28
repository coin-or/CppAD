#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/git_ignore.sh" ]
then
	echo "bin/git_ignore.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# use @ for * so does not get replaced during echo command
dir_list='
	.
	./@
	./@/@
'
top_dir_list='
git_commit.sh
aclocal.m4
ar-lib
autom4te.cache
build
commit.sh.old
doc
doxydoc
cppad/configure.hpp
'
all_dir_list='
@.err
@.log
@.swp
@.tmp

build
new
test_one.exe
test_one.sh
test_one.cpp
'
root_name_list='
doxyfile
junk
temp
'
# -----------------------------------------------------------------------------
if [ -e .gitignore ]
then
	rm .gitignore
fi
echo '# top directory only' >> .gitignore
for name in $top_dir_list
do
	echo "$name" | sed -e 's|^./||' -e 's|@|*|g' >> .gitignore
done
echo '# all directories' >> .gitignore
for name in $all_dir_list
do
	for dir in $dir_list
	do
		echo "$dir/$name" | sed -e 's|^./||' -e 's|@|*|g' >> .gitignore
	done
done
echo '# all directions with and without extension' >> .gitignore
for name in $root_name_list
do
	for dir in $dir_list
	do
		echo "$dir/$name"   | sed -e 's|^./||' -e 's|@|*|g' >> .gitignore
		echo "$dir/$name.*" | sed -e 's|^./||' -e 's|@|*|g' >> .gitignore
	done
done
# ----------------------------------------------------------------------------
echo 'git_ignore.sh: OK'
exit 0
