#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
set -e -u
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/run_xrst.sh' ]
then
   echo 'bin/run_xrst.sh must be run from its parent directory.'
   exit 1
fi
if [ -e build/html ]
then
   rm -r build/html
fi
if [ "$#" != 1 ]
then
   echo 'usage: bin/run_xrst.sh (+dev|-dev)'
   exit 1
fi
if [ "$1" != '+dev' ] && [ "$1" != '-dev' ]
then
   # If you change this, you will also need to change the instructions on
   # https://cppad.readthedocs.io/latest/download.html
   echo 'usage: bin/run_xrst.sh (+dev|-dev)'
   exit 1
fi
if [ "$1" == '+dev' ]
then
   group_list='default app dev'
else
   group_list='default app'
fi
# -----------------------------------------------------------------------------
#
# index_page_name
index_page_name=$(\
   sed -n -e '/^ *--index_page_name*/p' .readthedocs.yaml | \
   sed -e 's|^ *--index_page_name *||' \
)
#
# n_job
if which nproc >& /dev/null
then
   n_job=$(nproc)
else
   n_job=$(sysctl -n hw.ncpu)
fi
# -----------------------------------------------------------------------------
# xrst
echo_eval nice -10 xrst \
--local_toc \
--target html \
--html_theme sphinx_rtd_theme \
--index_page_name $index_page_name \
--group_list $group_list \
--number_jobs $n_job
# -----------------------------------------------------------------------------
echo 'run_xrst.sh: OK'
exit 0
