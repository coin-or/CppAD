#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ ! -e "bin/run_xrst.sh" ]
then
   echo "bin/run_xrst.sh: must be executed from its parent directory"
   exit 1
fi
echo_eval xrst \
   --local_toc \
   --html_theme sphinx_rtd_theme \
   --group_list default app dev
#
echo 'run_xrst.sh: OK'
exit 0
