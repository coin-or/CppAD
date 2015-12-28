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
if [ ! -e "bin/new_test.sh" ]
then
	echo "bin/new_test.sh: must be executed from its parent directory"
	exit 1
fi
change_list=""
file_path="$1"
# vim bash highlighting has an error so cannot use
# ext="${file_path: -4}"
ext=`echo $file_path | sed -e 's|.*\(....\)$|\1|'`
if [ "$ext" != ".cpp" ] || [ ! -e $file_path ]
then
	echo "usage: bin/new_test.sh file_path"
	echo "where file_path is the new test which must have .cpp extension"
	exit 1
fi
# extract the directory and file name
dir=`echo $file_path | sed -e 's|/[^/]*||'`
file=`echo $file_path | sed -e 's|.*/||'`
#
echo "cd $dir"
cd $dir
#
# extract the program that runs all tests and name of this new test
program=`sed -n -e '/_SOURCES/p' makefile.am |
	sed -e 's|^\t*\(.*\)_SOURCE.*|\1|'`
name=`grep '^bool *[a-zA-Z0-9_]*( *void *)' $file | tail -1 | \
	sed -e 's|^bool *\([a-zA-Z0-9_]*\)( *void *)|\1|'`
# ---------------------------------------------------------------------------
echo "add $file to $program.cpp"
change_list="$program.cpp"
svn revert $program.cpp
comment="// prototype external compiled tests"
if ! grep "$comment" $program.cpp > /dev/null
then
	echo "$comment missing in $program.cpp"
	exit 1
fi
sed -i -e "s|$comment.*|&\nextern bool $name(void);|" $program.cpp
comment="// run external compiled tests"
if ! grep "$comment" $program.cpp > /dev/null
then
	echo "$comment missing in $program.cpp"
	exit 1
fi
sed -i -e "s|$comment.*|&\n\tok \&= Run( $name, \"$name\" );|" $program.cpp
#
# ---------------------------------------------------------------------------
echo "add $file to $dir/makefile.am"
change_list="$change_list makefile.am"
svn revert makefile.am
sed -i -e "/_SOURCES/s|.*|&\n\t$file|" -e '/_SOURCES/s|$| \\|' makefile.am
# ---------------------------------------------------------------------------
# only MS project file is left to do
if ! ls *.vcproj >& /dev/null
then
	# no MS project file in this directory
	echo
	echo "Should hand edit to sort new entries in following files in $dir:"
	echo "$change_list"
	exit 0
fi
project=`ls *.vcproj`
echo "add $file to $dir/$project"
change_list="$change_list $project"
cat << EOF > new_test.sed
/Name="Source Files"/b join
b end
:join
N
/			<File/b add
b join
:add
s|			<File|			<File\r\n\
				RelativePath=".\\\\$file"\r\n\
				>\r\n\
			</File>\r\n\
&|
:end
EOF
svn revert $project
sed -i -f new_test.sed $project
#
echo
echo "Should hand edit to sort new entries in following files:"
echo "$change_list"
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
