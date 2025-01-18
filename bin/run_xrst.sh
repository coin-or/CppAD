#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-25 Bradley M. Bell
# ----------------------------------------------------------------------------
# bin/run_xrst.sh flags
# possible flags
# --help                     print the run_xrst.sh help message
# --target_tex               create tex (instead of html) files
# --exclude_dev              exclude developer documentation
# --suppress_spell_warnings  do not check for documentaiton spelling errors
# --rst_line_numbers         sphinx errors and warnings use rst line numbers
# --replace_spell_commands   replace xrst_spell commands assuming no errors
# --external_links           check documentation external links
# xrst.toml
# The group_list argument will be automatically extracted from xrst.toml
#
# .readthedocs
# The index_page_name will be automatically extracted from .readthedocs.yaml
# ----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
#
# grep, sed
source bin/grep_and_sed.sh
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
--help                     print the run_xrst.sh help message
--target_tex               create tex (instead of html) files
--exclude_dev              exclude developer documentation
--suppress_spell_warnings  do not check for documentaiton spelling errors
--rst_line_numbers         sphinx errors and warnings use rst line numbers
--replace_spell_commands   replace xrst_spell commands assuming no errors
--external_links           check documentation external links
EOF
      exit 0
   fi
fi
#
# exclude_dev, extra_flags
target_tex='no'
exclude_dev='no'
extra_flags=''
while [ $# != 0 ]
do
   case "$1" in

      --target_tex)
      target_tex='yes'
      ;;

      --exclude_dev)
      exclude_dev='yes'
      ;;

      --suppress_spell_warnings)
      extra_flags+=" $1"
      ;;

      --external_links)
      extra_flags+=" $1"
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
# extra_flags
if [ "$target_tex" == 'yes' ]
then
   extra_flags+=' --target tex'
else
   extra_flags+=' --target html'
fi
#
# build/html
if [ -e build/html ]
then
   rm -r build/html
fi
#
# group_list
cat << EOF > temp.sed
/^\\[root_file\\]/ ! b end
: loop
N
/\\n\\[/! b loop
s|^\\[root_file\\]| |
s|\\n *#[^\\n]*||g
s|\\n\\([A-Za-z0-9_.]*\\) *=[^\\n]*|\\1 |g
s|\\n[^\\n]*\$||
#
p
#
: end
EOF
if [ "$exclude_dev" == 'yes' ]
then
   group_list=$( $sed -n -f temp.sed xrst.toml | sed -e 's| dev | |' )
else
   group_list=$( $sed -n -f temp.sed xrst.toml)
fi
#
# index_page_name
if ! grep '^ *--index_page_name' .readthedocs.yaml > /dev/null
then
   echo 'bin/run_xrst.sh expects --index_page_name page_name'
   echo 'to be a line in the file .readthedocs.yaml; see xrst --help'
   exit 1
fi
index_page_name=$(\
   sed -n -e '/^ *--index_page_name*/p' .readthedocs.yaml | \
   sed -e 's|^ *--index_page_name *||' \
)
echo "bin/run_xrst.sh: .readthedocs.yaml: index_page_name = '$index_page_name'"
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
   --html_theme sphinx_rtd_theme \
   --index_page_name $index_page_name \
   --group_list $group_list \
   --number_jobs $n_job \
   $extra_flags
# -----------------------------------------------------------------------------
echo 'run_xrst.sh: OK'
exit 0
