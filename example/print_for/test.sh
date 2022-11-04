#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
./print_for | tee test.log
sed -e '/^Test passes/,\$$d' < test.log > test.1
sed -e '1,/^Test passes/d'   < test.log > test.2
if ! diff test.1 test.2 ; then exit 1 ; fi
