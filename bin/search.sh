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
if [ ! -e "bin/search.sh" ]
then
	echo "bin/search.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo 'usage: bin/search.sh pattern'
	echo '       Search for pattern in the cppad source code'
	exit 1
fi
pattern="$1"
# -----------------------------------------------------------------------------
list=`bin/ls_files.sh | sed \
	-e '/^configure$/d' \
	-e '/^makefile.in$/d' \
	-e '/\/makefile.in$/d' \
	-e '/^compile/d'`
grep -l "$pattern" $list
# ----------------------------------------------------------------------------
exit 0
