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
if [ ! -e "bin/check_op_code.sh" ]
then
	echo "bin/check_op_code.sh: must be executed from its parent directory"
	exit 1
fi
echo "bin/check_op_code.sh: checking that op codes are in alphabetical order:"
# ---------------------------------------------------------------------------
# check enum list of codes are in alphabetical order
sed -n -e '/^enum/,/^\tNumberOp  /p' cppad/local/op_code.hpp | sed \
	-e '/^enum/d' \
	-e '/^\tNumberOp  /d' \
	-e 's/^[	]*//' \
	-e 's/Op[, ].*//' \
	-e '/^\/\//d' > bin/op_code.1.$$
#
sort --ignore-case bin/op_code.1.$$ > bin/op_code.2.$$
if ! diff bin/op_code.1.$$ bin/op_code.2.$$
then
	echo "check_op_code.sh: enum list is not in alphabetical order"
	rm bin/op_code.*.$$
	exit 1
fi
# -----------------------------------------------------------------------------
# check NumArgTable
sed -n -e '/NumArgTable\[\]/,/^[ \t]*};/p' cppad/local/op_code.hpp | \
	sed \
		-e '/NumArgTable\[\]/d' \
		-e '/NumberOp.*not used/d' \
		-e '/^[ \t]*};/d' \
		-e 's|^[ \t]*[0-9],* *// *||' \
		-e 's|Op.*||' \
		> bin/op_code.3.$$
#
if ! diff bin/op_code.1.$$ bin/op_code.3.$$
then
	echo "check_op_code.sh: NumArgTable list is not in alphabetical order"
	rm bin/op_code.*.$$
	exit 1
fi
# -----------------------------------------------------------------------------
# check NumResTable (last line of NumResTable is not used)
sed -n -e '/NumResTable\[\]/,/^[ \t]*};/p' cppad/local/op_code.hpp | \
	sed \
		-e '/NumResTable\[\]/d' \
		-e '/^[ \t]*};/d' \
		-e '/NumberOp.*not used/d' \
		-e 's|^[ \t]*[0-9],* *// *||' \
		-e 's|Op.*||' \
		> bin/op_code.4.$$
#
if ! diff bin/op_code.1.$$ bin/op_code.4.$$
then
	echo "check_op_code.sh: NumResTable list is not in alphabetical order"
	echo "(or missing last line)"
	rm bin/op_code.*.$$
	exit 1
fi
# -----------------------------------------------------------------------------
# check OpNameTable
sed -n -e '/const char \*OpNameTable\[\]/,/^[ \t]*};/p' cppad/local/op_code.hpp | \
	sed \
		-e '/OpNameTable\[\]/d' \
		-e '/"Number".*not used/d' \
		-e '/^[ \t]*};/d' \
		-e 's|^[ \t]*"||' \
		-e 's|".*||' \
		> bin/op_code.5.$$
#
if ! diff bin/op_code.1.$$ bin/op_code.5.$$
then
	echo "check_op_code.sh: OpName list is not in alphabetical order"
	rm bin/op_code.*.$$
	exit 1
fi
# -----------------------------------------------------------------------------
# clean up
rm bin/op_code.*.$$
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
