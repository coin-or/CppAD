#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
if [  "$0" != 'bin/check_nominmax.sh' ]
then
    echo "bin/check_nominmax.sh: must be executed from its parent directory"
    exit 1
fi
# -----------------------------------------------------------------------------
list=$(git grep -n '^# *include *<windows.h>'  | sed -e 's|\(:[0-9]*:\).*|\1|')
for match in $list
do
    file=$(echo $match | sed -e 's|\([^:]*\):\([0-9]*\):|\1|')
    line=$(echo $match | sed -e 's|\([^:]*\):\([0-9]*\):|\2|')
    let "line = $line - 2"
    if ! sed -n "${line}p" $file | grep '^# *define *NOMINMAX' > /dev/null
    then
cat << EOF
In $file
    # ifndef NOMINMAX
    # define NOMINMAX
    # endif
Must be the three lines before:
    # include <windows.h>
EOF
    fi
done
#
echo 'check_nominmax.sh: OK'
exit 0
