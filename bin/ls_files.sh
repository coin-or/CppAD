#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#            GNU General Public License, Version 3.0.
# -----------------------------------------------------------------------------
# list files in repository (not deleted)
# -----------------------------------------------------------------------------
if [ $0 != "bin/ls_files.sh" ]
then
	echo "bin/ls_files.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
list=`git ls-files`
git ls-files -d > list_files.$$
for file in $list
do
	if ! grep "$file" list_files.$$ > /dev/null
	then
		echo $file
	fi
done
rm list_files.$$
# ----------------------------------------------------------------------------
exit 0
