#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/check_xrst.sh' ]
then
   echo 'bin/check_xrst.sh must be run from its parent directory.'
   exit 1
fi
if [ -e build/html ]
then
   rm -r build/html
fi
if [ "$1" != '+dev' ] && [ "$1" != '-dev' ]
then
   echo 'usage: bin/check_xrst.sh (+dev|-dev)'
   exit 1
fi
if [ "$1" == '+dev' ]
then
   group_list='default app dev'
else
   group_list='default app'
fi
# -----------------------------------------------------------------------------
# index_page_name
index_page_name=$(\
   sed -n -e '/^ *--index_page_name*/p' .readthedocs.yaml | \
   sed -e 's|^ *--index_page_name *||' \
)
# -----------------------------------------------------------------------------
# cmd
cmd="xrst \
--local_toc \
--target html \
--html_theme sphinx_rtd_theme \
--index_page_name $index_page_name \
--group_list $group_list \
"
echo "$cmd"
if ! $cmd >& >( tee check_xrst.$$ )
then
   echo 'check_xrst.sh: aboring due to xrst errors above'
   rm check_xrst.$$
   exit 1
fi
if grep '^warning:' check_xrst.$$ > /dev/null
then
   echo 'check_xrst.sh: aboring due to xrst warnings above'
   rm check_xrst.$$
   exit 1
fi
# -----------------------------------------------------------------------------
rm check_xrst.$$
echo 'check_xrst.sh: OK'
exit 0
