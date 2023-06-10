#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
program="bin/speed_new.sh"
if [ "$0" != "$program" ]
then
   echo "$program: must be executed from its parent directory"
   exit 1
fi
if [ "$1" == '' ]
then
cat << EOF
usage:
$program test_name [option_1] [option_2] ...

possible test are:
all, det_lu, det_minor, mat_mul ode, poly, sparse_hessian, sparse_jacobian

possible options are:
atomic, boolsparsity, colpack, memory, onetape, optimize, revsparsity
EOF
   exit 1
fi
if [ ! -d new ]
then
   echo "$program: the directory ./new does not exist."
   echo 'It should contain the new source code.'
   exit 1
fi
test_name="$1"
shift
option_list="$test_name"
for option in $*
do
   option_list="${option_list}_$option"
done
if [ "$test_name" == 'all' ]
then
   test_name='speed'
fi
# ----------------------------------------------------------------------------
target_dir='build/speed/cppad'
if [ ! -e $target_dir ]
then
   echo_eval mkdir -p $target_dir
fi
# ----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# ----------------------------------------------------------------------------
#
for name in cur new
do
   if [ "$name" == 'cur' ]
   then
      # revert cppad source code to the current version
      echo_eval git reset --hard --quiet
   else
      echo_eval git_new.sh from
   fi
   echo "bin/run_cmake.sh --debug_none > /dev/null"
   bin/run_cmake.sh --debug_none > /dev/null
   #
   out_file="$name.$option_list.out"
   if [ -e "$target_dir/$out_file" ]
   then
      echo "Using existing $target_dir/$out_file"
   else
      # compile the speed test
      echo "ninja -C build check_speed_cppad > $target_dir/$name.log"
      ninja -C build check_speed_cppad > $target_dir/$name.log
      #
      # run speed test for the current version
      echo "$target_dir/speed_cppad $test_name 123 $* > $target_dir/$out_file"
      $target_dir/speed_cppad $test_name 123 $* > $target_dir/$out_file
      #
   fi
done
# compare the results
echo "    one=cur, two=new"
bin/speed_diff.sh \
   $target_dir/cur.$option_list.out $target_dir/new.$option_list.out
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
