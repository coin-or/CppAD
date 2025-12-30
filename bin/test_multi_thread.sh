#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
set -e -u
# ----------------------------------------------------------------------------
# This script is 'test_*' instead of 'check_*' because it requires
# that cmake has already created build/bukld.ninja or Makefile.
program='bin/test_multi_thread.sh'
if [ "$0" != "$program" ]
then
   echo "$program: must be executed from its parent directory"
   exit 1
fi
if [ "$#" != 1 ]
then
   echo "usage: $program builder"
   echo 'where builder is ninja or make'
   exit 1
fi
builder="$1"
if [ "$1" != 'ninja' ] && [ "$1" != 'make' ]
then
   echo "usage:usage:  $program builder"
   echo 'where builder is ninja or make'
   exit 1
fi
if [ "$builder" == 'ninja' ] && [ ! -e build/build.ninja ]
then
   echo "$program: builder is ninja and cannot find build/build.ninja"
   exit 1
fi
if [ "$builder" == 'make' ] && [ ! -e build/Makefile ]
then
   echo "$program: builder is make and cannot file build/Makefile"
   exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# ---------------------------------------------------------------------------
# circular shift program list and set program to first entry in list
next_program() {
   program_list=`echo "$program_list" | sed -e 's| *\([^ ]*\) *\(.*\)|\2 \1|'`
   program=`echo "$program_list" | sed -e 's| *\([^ ]*\).*|\1|'`
}
# ----------------------------------------------------------------------------
#
# build
cd build
$builder check_example_multi_thread
#
# program_list, skip
program_list=''
skip=''
for threading in bthread openmp pthread sthread
do
   dir="example/multi_thread/$threading"
   if [ ! -e "$dir" ]
   then
      skip+=" $threading"
   else
      program="$dir/example_multi_thread_${threading}"
      program_list+=" $program"
      #
      # all programs check the fast cases
      echo_eval $program a11c
      echo_eval $program get_started
      echo_eval $program team_example
   fi
done
if [ "$program_list" != '' ]
then
   # test_time=1,max_thread=4,mega_sum=1
   next_program
   echo_eval $program harmonic 1 4 1
   #
   # test_time=1,max_thread=4,num_solve=100
   next_program
   echo_eval $program atomic_two 1 4 100
   next_program
   echo_eval $program atomic_three 1 4 100
   next_program
   echo_eval $program chkpoint_one 1 4 100
   next_program
   echo_eval $program chkpoint_two 1 4 100
   #
   # test_time=2,max_thread=4,num_zero=20,num_sub=30,num_sum=50,use_ad=true
   next_program
   echo_eval $program multi_newton 2 4 20 30 50 true
fi
#
if [ "$skip" != '' ]
then
   echo "Skipped following multi_treading tests: $skip"
fi
echo 'test_multi_thread.sh: OK'
exit 0
