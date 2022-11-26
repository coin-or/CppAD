#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ ! -e "bin/check_op_code.sh" ]
then
   echo "bin/check_op_code.sh: must be executed from its parent directory"
   exit 1
fi
file='include/cppad/local/op_code_var.hpp'
# ---------------------------------------------------------------------------
# order in enum list
sed -n -e '/^enum/,/NumberOp/p' $file | sed \
   -e '/^enum/d' \
   -e '/NumberOp/d' \
   -e 's/^[ ]*//' \
   -e 's/,.*//' > temp.1
# -----------------------------------------------------------------------------
# check NumArgTable
sed -n -e '/NumArgTable\[\]/,/NumberOp/p' $file | \
   sed \
      -e '/NumArgTable\[\]/d' \
      -e '/NumberOp/d' \
      -e 's|^ */[*] ||' \
      -e 's| *[*]/.*||' > temp.2
#
if ! diff temp.1 temp.2
then
   echo 'check_op_code.sh: NumArgTable list is different from enum list'
   exit 1
fi
# -----------------------------------------------------------------------------
# check NumResTable
sed -n -e '/NumResTable\[\]/,/NumberOp/p' $file | \
   sed \
      -e '/NumResTable\[\]/d' \
      -e '/NumberOp/d' \
      -e 's|^ */[*] ||' \
      -e 's| *[*]/.*||' > temp.2
#
if ! diff temp.1 temp.2
then
   echo 'check_op_code.sh: NumResTable list is different from enum list'
   exit 1
fi
# -----------------------------------------------------------------------------
# check OpNameTable
sed -n -e '/const char \*OpNameTable\[\]/,/"Number"/p' $file | \
   sed \
      -e '/OpNameTable\[\]/d' \
      -e '/"Number"/d' \
      -e 's|^ *"||' \
      -e 's|".*||' \
      > temp.2
#
if ! diff temp.1 temp.2
then
   echo 'check_op_code.sh: OpNameTable list is different from enum list'
   exit 1
fi
# -----------------------------------------------------------------------------
# clean up
rm temp.1 temp.2
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
