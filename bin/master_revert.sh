#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/master_revert.sh' ]
then
    echo "bin/master_revert.sh: must be executed from its parent directory"
    exit 1
fi
list=`git diff --name-status master | sed -e 's|^M\t*||'`
for file in $list
do
    git show master:$file > $file
done
echo 'master_revert.sh: OK'
exit 0
