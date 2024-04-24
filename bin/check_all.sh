#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ "$0" != 'bin/check_all.sh' ]
then
   echo "bin/check_all.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$1" != 'mixed' ] && [ "$1" != 'debug' ] && [ "$1" != 'release' ]
then
   echo 'bin/check_all.sh: (mixed|debug|release) [verbose_make]'
   exit 1
fi
if [ "$2" != '' ] && [ "$2" != 'verbose_make' ]
then
   echo 'bin/check_all.sh: (mixed|debug|release) [verbose_make]'
   exit 1
fi
build_type="$1"
if [ "$2" == 'verbose_make' ]
then
   verbose_make='--verbose_make'
else
   verbose_make=''
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
echo_log_eval() {
   echo "$* >& check_all.tmp"
   echo "$*" > $top_srcdir/check_all.tmp
   if ! $* >& $top_srcdir/check_all.tmp
   then
      tail $top_srcdir/check_all.tmp
      echo 'Error: see check_all.tmp'
      exit 1
   fi
   if grep ': *warning *:"' $top_srcdir/check_all.tmp
   then
      echo 'Warning: see check_all.tmp'
      exit 1
   fi
   echo '        cat check_all.tmp >> check_all.log'
   cat $top_srcdir/check_all.tmp >> $top_srcdir/check_all.log
}
echo_log() {
   echo $*
   echo $* >> $top_srcdir/check_all.log
}
random_01() {
   set +e
   eval random_01_$1="`expr $RANDOM % 2`"
   eval echo "random_01_$1=\$random_01_$1"
   set -e
}
# -----------------------------------------------------------------------------
# start new check_all.log
echo "date > check_all.log"
date | sed -e 's|^|date: |' > check_all.log
top_srcdir=`pwd`
echo "top_srcdir = $top_srcdir"
# ---------------------------------------------------------------------------
# circular shift program list and set program to first entry in list
next_program() {
   program_list=`echo "$program_list" | sed -e 's| *\([^ ]*\) *\(.*\)|\2 \1|'`
   program=`echo "$program_list" | sed -e 's| *\([^ ]*\).*|\1|'`
}
# ---------------------------------------------------------------------------
if [ -e "$HOME/prefix/cppad" ]
then
   echo_log_eval rm -r $HOME/prefix/cppad
fi
# ---------------------------------------------------------------------------
# version
version=$(
   sed -n -e '/^SET( *cppad_version *"[0-9.]*")/p' CMakeLists.txt | \
      sed -e 's|.*"\([^"]*\)".*|\1|'
)
# ---------------------------------------------------------------------------
#
# compiler
random_01 compiler
if [ "$random_01_compiler" == '0' ]
then
   compiler='default'
else
   compiler='--clang'
fi
#
# standard
random_01 standard
if [ "$random_01_standard" == '0' ]
then
   random_01 standard
   if [ "$random_01_standard" == '0' ]
   then
      standard='--c++11'
   else
      standard='--c++17'
   fi
else
   standard='--c++17'
fi
#
# use_configure
random_01 use_configure
if [ "$random_01_use_configure" == '0' ]
then
   random_01 use_configure
   if [ "$random_01_use_configure" == '0' ]
   then
      use_configure='yes'
   else
      use_configure='no'
   fi
else
   use_configure='no'
fi
#
# package_vector, debug_which
if [ "$build_type" == 'debug' ]
then
   package_vector='--cppad_vector'
   debug_which='--debug_all'
elif [ "$build_type" == 'release' ]
then
   package_vector='--cppad_vector'
   debug_which='--debug_none'
else
   if [ "$build_type" != 'mixed' ]
   then
      echo 'bin/check_all.sh: build_type not debug release or mixed'
      exit 1
   fi
   random_01 debug_which
   if [ "$random_01_debug_which" == '0' ]
   then
      debug_which='--debug_even'
   else
      debug_which='--debug_odd'
   fi
   #
   random_01 package_vector
   if [ "$random_01_package_vector" == '0' ]
   then
      package_vector='--boost_vector'
   else
      if [ "$standard" == '--c++17' ]
      then
         package_vector='--eigen_vector'
      else
         package_vector='--std_vector'
      fi
   fi
fi
cat << EOF
tarball         = $cppad-$version.tgz
compiler        = $compiler
standard        = $standard
debug_which     = $debug_which
package_vector  = $package_vector
use_configure   = $use_configure
EOF
cat << EOF >> $top_srcdir/check_all.log
tarball         = $cppad-$version.tgz
compiler        = $compiler
standard        = $standard
debug_which     = $debug_which
package_vector  = $package_vector
use_configure   = $use_configure
EOF
#
# compiler
if [ "$compiler" == 'default' ]
then
   compiler=''
fi
#
# standard, exclude_package
if [ "$standard" == '--c++17' ]
then
   standard='' # default for run_cmake.sh and configure
   exclude_package=''
else
   exclude_package='--no_eigen --no_sacado'
fi
# ---------------------------------------------------------------------------
# absoute prefix where optional packages are installed
eval `grep '^prefix=' bin/get_optional.sh`
if [[ "$prefix" =~ ^[^/] ]]
then
   prefix="$(pwd)/$prefix"
fi
if [ ! -d $prefix/include/cppad/cg ]
then
   echo "Cannot find $prefix/include/cppad/cg"
   echo 'Probably need to run bin/get_optional.sh'
   exit 1
fi
export LD_LIBRARY_PATH="$prefix/lib:$prefix/lib64"
# ---------------------------------------------------------------------------
# check_version
if echo $version | grep '[0-9]\{4\}0000[.]' > /dev/null
then
   # special interactive case for stable versions.
   echo_eval bin/check_version.sh
else
   echo_log_eval bin/check_version.sh
fi
# ---------------------------------------------------------------------------
# Run automated checks for the form bin/check_*.sh with a few exceptions.
list=$(
   ls bin/check_* | sed \
   -e '/check_all.sh/d' \
   -e '/check_doxygen.sh/d' \
   -e '/version.sh/d' \
   -e '/check_install.sh/d' \
   -e '/check_invisible/d'
)
#
echo_eval bin/check_invisible.sh
for check in $list
do
   echo_log_eval $check
done
#
# run_xrst.sh
bin/run_xrst.sh +dev
# ---------------------------------------------------------------------------
# build/cppad-$version.tgz
echo_log_eval bin/package.sh
#
# build/cppad-$version
echo_log_eval cd build
echo_log_eval tar -xzf cppad-$version.tgz
echo_log_eval cd cppad-$version
#
# build/cppad-$version/build/prefix
if [ ! -e build ]
then
   mkdir build
fi
echo_log_eval cp -r ../prefix build/prefix
#
# configure or cmake
if [ "$use_configure" == 'yes' ]
then
   builder='make'
   if [ "$compiler" == 'clang' ]
   then
      echo_log_eval bin/run_configure.sh $standard --with_clang
   else
      echo_log_eval bin/run_configure.sh $standard
   fi
else
   if [ "$verbose_name" != '' ]
   then
      builder='make'
   else
      builder='ninja'
   fi
   echo_log_eval bin/run_cmake.sh \
      $verbose_make \
      --profile_speed \
      $compiler \
      $standard \
      $debug_which \
      $exclude_package \
      $package_vector
fi
echo_log_eval cd build
#
# n_job
n_job=$(nproc)
# -----------------------------------------------------------------------------
#
# build: example_jit
# avoid *.so not found on some vitual systems (during parallelll build)
cmd="nice -10 $builder -j $n_job"
echo_log_eval $cmd example_jit
#
# build: check
echo_log_eval $cmd check
# ----------------------------------------------------------------------------
# extra speed tests not run with different options
#
for option in onetape colpack optimize atomic memory boolsparsity
do
   echo_eval speed/cppad/speed_cppad correct 432 $option
done
#
echo_eval speed/adolc/speed_adolc correct         432 onetape
echo_eval speed/adolc/speed_adolc sparse_jacobian 432 onetape colpack
echo_eval speed/adolc/speed_adolc sparse_hessian  432 onetape colpack
#
# ----------------------------------------------------------------------------
# extra multi_thread tests
program_list=''
skip=''
for threading in bthread openmp pthread
do
   dir="example/multi_thread/$threading"
   if [ ! -e "$dir" ]
   then
      skip="$skip example_multi_thread_${threading}"
   else
      program="$dir/example_multi_thread_${threading}"
      program_list="$program_list $program"
      #
      # redo this build in case it is commented out above
      echo_log_eval $builder -j $n_job example_multi_thread_${threading}
      #
      # all programs check the fast cases
      echo_log_eval $program a11c
      echo_log_eval $program get_started
      echo_log_eval $program team_example
   fi
done
if [ "$program_list" != '' ]
then
   # test_time=1,max_thread=4,mega_sum=1
   next_program
   echo_log_eval $program harmonic 1 4 1
   #
   # test_time=1,max_thread=4,num_solve=100
   next_program
   echo_log_eval $program atomic_two 1 4 100
   next_program
   echo_log_eval $program atomic_three 1 4 100
   next_program
   echo_log_eval $program chkpoint_one 1 4 100
   next_program
   echo_log_eval $program chkpoint_two 1 4 100
   #
   # test_time=2,max_thread=4,num_zero=20,num_sub=30,num_sum=50,use_ad=true
   next_program
   echo_log_eval $program multi_newton 2 4 20 30 50 true
fi
#
# print_for test
program='example/print_for/example_print_for'
#
# redo this build in case it is commented out above
echo_log_eval $builder -j $n_job example_print_for
#
echo_log_eval $program
$program | sed -e '/^Test passes/,$d' > junk.1.$$
$program | sed -e '1,/^Test passes/d' > junk.2.$$
if diff junk.1.$$ junk.2.$$
then
   rm junk.1.$$ junk.2.$$
   echo_log_eval echo "print_for: OK"
else
   echo_log_eval echo "print_for: Error"
   exit 1
fi
#
# ---------------------------------------------------------------------------
# check install
echo_log_eval $builder install
echo_log_eval cd ..
echo_log_eval bin/check_install.sh
# ---------------------------------------------------------------------------
#
echo "date >> check_all.log"
date  | sed -e 's|^|date: |' >> $top_srcdir/check_all.log
if [ "$skip" != '' ]
then
   echo_log_eval echo "check_all.sh: skip = $skip"
   exit 1
fi
# ----------------------------------------------------------------------------
echo "$0: OK" >> $top_srcdir/check_all.log
echo "$0: OK"
exit 0
