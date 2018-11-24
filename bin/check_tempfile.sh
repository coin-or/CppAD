#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#            GNU General Public License, Version 3.0.
# -----------------------------------------------------------------------------
if [ ! -e "bin/check_tempfile.sh" ]
then
	echo "bin/check_tempfile.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
list=`ls | sed -n '/\.[0-9]*$/p'`
if [ "$list" != '' ]
then
    ls | sed -n '/\.[0-9]*$/p'
	echo 'check_tempfile.sh: Error'
	exit 1
fi
echo 'check_tempfile.sh: OK'
exit 0
