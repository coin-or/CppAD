#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ ! -e "bin/check_doxygen.sh" ]
then
   echo "bin/check_doxygen.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
if ! bin/run_doxygen.sh
then
   echo 'check_doxygen.sh: Error'
   exit 1
fi
echo 'check_doxygen.sh: OK'
exit 0
