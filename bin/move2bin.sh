#! /bin/bash -e
#
if [ $0 != "bin/move2bin.sh" ]
then
	echo "bin/move2bin.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ] 
then
	echo "usage: bin/move2bin.sh file"
	exit 1
fi
file="$1"
cat << EOF > bin/move2bin.$$
#! /bin/bash -e
# \$Id:\$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
EOF
echo "svn revert $file"
svn revert $file
#
if [ -e bin/$file ]
then 
	echo "svn delete --force trunk/bin/$file"
	svn delete --force trunk/bin/$file
fi
#
echo "svn copy $file bin/$file"
svn copy $file bin/$file
#
cat bin/move2bin.$$ > bin/$file
cat << EOF  >> bin/$file
if [ \$0 != "bin/$file" ]
then
	echo "bin/$file: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
EOF
name=`echo $file | sed -e 's|\.sh||'`
cat << EOF > bin/move2bin.$$
1,12d
s|\./$file|bin/$file|g
s|\([^/]\)$file|bin/\1$file|g
s|$name.\\$\\$|bin/$name.\\$\\$|g
EOF
echo "sed -f bin/move2bin.$$  $file >> bin/$file"
sed -f bin/move2bin.$$ $file        >> bin/$file
#
rm bin/move2bin.$$
svn delete $file
