#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
possible_tests='
   all
   det_lu
   det_minor
   mat_mul ode
   poly
   sparse_hessian
   sparse_jacobian
'
possible_options='
   atomic
   boolsparsity
   colpack
   memory
   onetape
   optimize
   revsparsity
   subgraph
'
# ----------------------------------------------------------------------------
program="bin/speed_branch.sh"
if [ "$0" != "$program" ]
then
   echo "$program: must be executed from its parent directory"
   exit 1
fi
if ! git branch | grep '^\* master' > /dev/null
then
   echo 'bin/speed_branch.sh: must start execution from master branch'
   exit 1
fi
if [ "$3" == '' ]
then
tests=`echo $possible_tests | sed -e 's|\n||' -e 's|\t| |'`
options=`echo $possible_options | sed -e 's|\n||' -e 's|\t| |'`
cat << EOF
usage:
$program branch_one branch_two test_name [option_1] [option_2] ...

possible tests are:
$tests

possible options are:
$options
EOF
   exit 1
fi
#
branch_one="$1"
shift
branch_two="$1"
shift
test_name="$1"
if ! echo "$possible_tests" | grep "$test_name" > /dev/null
then
   echo "speed_branch.sh: $test_name is not a valid test name"
   exit 1
fi
shift
option_list="$test_name"
for option in $*
do
   if ! echo $possible_options | grep "$option" > /dev/null
   then
      echo "speed_branch.sh: $option is not a valid option"
      exit 1
   fi
   option_list="${option_list}_$option"
done
if [ "$test_name" == 'all' ]
then
   test_name='speed'
fi
# ----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ ! -d '.git' ]
then
   echo "$program: only implemented for git repository"
   exit 1
fi
# ----------------------------------------------------------------------------
target_dir='build/speed/cppad'
if [ ! -e $target_dir ]
then
   echo_eval mkdir -p $target_dir
fi
# -----------------------------------------------------------------------------
for branch in $branch_one $branch_two
do
   # for stable branches, remove stable/ from output file name
   out_file=`echo $branch.$option_list.out | sed -e 's|stable/||'`
   log_file=`echo $branch.log | sed -e 's|stable/||'`
   #
   if [ -e "$target_dir/$out_file" ]
   then
      echo "Using existing $target_dir/$out_file"
   else
      # use --quiet to supress detached HEAD message
      echo_eval git checkout --quiet $branch
      #
      day=`git log -1 --date=iso | grep '^Date:' | \
         sed -e 's|Date: *||' -e 's|-||g' -e 's| .*||'`
      if [ "$test_name" == 'speed' ] || [ "$test_name" == 'sparse_hessian' ]
      then
         if [ "$day" -le '20150130' ]
         then
            echo "test_name is all or sparse_hessian"
            echo "and branch $branch came on or before 20150130"
            echo "when bug was fixed in the sparse_hessian speed test."
            exit 1
         fi
      fi
      #
      # versions of CppAD before 20170625 did not have --debug_none option
      echo "bin/run_cmake.sh --debug_none >& $target_dir/$log_file"
      if ! bin/run_cmake.sh --debug_none >& $target_dir/$log_file
      then
         echo "bin/run_cmake.sh >& $target_dir/$log_file"
         bin/run_cmake.sh >& $target_dir/$log_file
      fi
      #
      echo "ninja check_speed_cppad >>& $target_dir/$log_file"
      ninja -C build check_speed_cppad >& speed_branch.log.$$
      cat speed_branch.log.$$ >> $target_dir/$log_file
      rm speed_branch.log.$$
      #
      echo "$target_dir/speed_cppad $test_name 123 $* > $target_dir/$out_file"
      $target_dir/speed_cppad $test_name 123 $* > $target_dir/$out_file
      #
   fi
done
# return to master (branch where we started)
echo_eval git checkout --quiet master
#
# compare the results
echo "    one=$branch_one , two=$branch_two"
out_file_one=`echo $branch_one.$option_list.out | sed -e 's|stable/||'`
out_file_two=`echo $branch_two.$option_list.out | sed -e 's|stable/||'`
bin/speed_diff.sh $target_dir/$out_file_one $target_dir/$out_file_two
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
