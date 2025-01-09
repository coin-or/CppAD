#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
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
if [ $# == 1 ]
then
   if [ "$1" == --help ]
   then
cat << EOF
bin/run_xrst.sh flags
possible flags
--include_dev              include_developer documentation
--external_links           check documentation external links
--suppress_spell_warnings  do not check for documentaiton spelling errors
EOF
      exit 0
   fi
fi
#
# inculude_dev, extra_flags
include_dev='no'
extra_flags=''
while [ $# != 0 ]
do
   case "$1" in

      --include_dev)
      include_dev='yes'
      ;;

      --external_links)
      extra_falgs+=" $1"
      ;;

      --suppress_spell_warnings)
      extra_falgs+=" $1"
      ;;

      *)
      echo "bin/run_xrst.sh: command line argument "$1" is not valid"
      exit 1
      ;;

   esac
   #
   shift
done
#
# build/html
if [ -e build/html ]
then
   rm -r build/html
fi
#
# group_list
if [ "$include_dev" == 'yes' ]
then
   group_list='default app dev'
else
   group_list='default app'
fi
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
#
# xrst
echo_eval xrst \
--local_toc \
--target html \
--html_theme sphinx_rtd_theme \
--index_page_name $index_page_name \
--group_list $group_list \
--number_jobs $n_job \
$extra_flags
# -----------------------------------------------------------------------------
echo 'run_xrst.sh: OK'
exit 0
