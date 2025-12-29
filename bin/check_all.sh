#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
# ----------------------------------------------------------------------------
#
if [ "$0" != 'bin/check_all.sh' ]
then
   echo "bin/check_all.sh: must be executed from its parent directory"
   exit 1
fi
if [ $# == 1 ]
then
   if [ "$1" == --help ]
   then
cat << EOF
bin/check_all.sh flags
possible flags
--mixed                    mix debug and release compiles
--debug                    only compile for debugging
--release                  only compile for release
--verbose_make             generate verbose makefiles
--skip_external_links      do not check documentation external links
--skip_check_copy          do not check copyright messages
--suppress_spell_warnings  do not check for documentation spelling errors
EOF
      exit 0
   fi
fi
#
# build_type, verbose_make, skip_external_links, suppress_spell_warnings
build_type='mixed'
verbose_make='no'
skip_external_links='no'
skip_check_copy='no'
suppress_spell_warnings='no'
while [ $# != 0 ]
do
   case "$1" in

      --mixed)
      build_type=mixed
      ;;

      --debug)
      build_type=debug
      ;;

      --release)
      build_type=release
      ;;

      --verbose_make)
      verbose_make='yes'
      ;;

      --skip_external_links)
      skip_external_links='yes'
      ;;

      --skip_check_copy)
      skip_check_copy='yes'
      ;;

      --suppress_spell_warnings)
      suppress_spell_warnings='yes'
      ;;

      *)
      echo "bin/check_all.sh: command line argument "$1" is not valid"
      exit 1
      ;;

   esac
   #
   shift
done
#
# grep and sed
source bin/grep_and_sed.sh
#
# LD_LIBRARY_PATH
# all linking of dynamic libraries should use rpath
export LD_LIBRARY_PATH=''
#
# top_srcdir
top_srcdir=`pwd`
echo "top_srcdir = $top_srcdir"
#
# echo_eval
echo_eval() {
   echo $*
   eval $*
}
#
# echo_log_eval
echo_log_eval() {
   echo "$* >& check_all.tmp"
   echo "$*" > $top_srcdir/check_all.tmp
   if ! $* >& $top_srcdir/check_all.tmp
   then
      tail $top_srcdir/check_all.tmp
      echo 'Error: see check_all.tmp'
      exit 1
   fi
   # 1.  If we don't have c++17 and mkstemp, then temp_file is not thread safe.
   #
   # 2.  If using g++ -O3 -DNDEBUG -Wall,
   #     an improper compile time warning is generated at:
   #     forward.hpp:187, reverse.hpp:151, independent.hpp:100-109,
   #     base_alloc.hpp:143, abs_min_quad.hpp:424 .
   #
   # warning
   warning='no'
   if [ "$compiler" == '--clang' ]
   then
      if $sed $top_srcdir/check_all.tmp \
         -e '/temp_file.cpp:.*warning.*tmpnam/d' \
         | $grep ': *warning *:'
      then
         warning='yes'
      fi
   else
      if $sed $top_srcdir/check_all.tmp \
         -e '/temp_file.cpp:.*warning.*tmpnam/d' \
         -e '/forward.hpp:187:.*warning.*outside array bounds/d' \
         -e '/reverse.hpp:151:.*warning.*outside array bounds/d' \
         -e '/independent.hpp:10[0-9]:.*warning.*outside array bounds/d' \
         -e '/base_alloc.hpp:143:.*warning.*may be used uninitialized/d' \
         -e '/abs_min_quad.hpp:424:.*bound.*exceeds maximum/d' \
         | $grep ': *warning *:'
      then
         warning='yes'
      fi
   fi
   if [ "$warning" == 'yes' ]
   then
      echo "The warnings above happened during the command: $*"
      echo "see the file $top_srcdir/check_all.tmp"
      exit 1
   fi
   #
   # check_all.log
   echo '        cat check_all.tmp >> check_all.log'
   cat $top_srcdir/check_all.tmp >> $top_srcdir/check_all.log
}
#
# random_01
random_01() {
   set +e
   eval random_01_$1="`expr $RANDOM % 2`"
   eval echo "random_01_$1=\$random_01_$1"
   set -e
}
#
#  check_all.log
# start new check_all.log
echo "date > check_all.log"
date | $sed -e 's|^|date: |' > check_all.log
#
# $HOME/prefix/cppad
if [ -e "$HOME/prefix/cppad" ]
then
   echo_log_eval rm -r $HOME/prefix/cppad
fi
#
# version
version=$(
   $sed -n -e '/^SET( *cppad_version *"[0-9.]*")/p' CMakeLists.txt | \
      $sed -e 's|.*"\([^"]*\)".*|\1|'
)
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
      msg="build_type = $build_type not debug release or mixed"
      echo "bin/check_all.sh $msg"
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
#
# debug_which
if [ "$use_configure" == 'yes' ]
then
   debug_which='--debug_none'
fi
cat << EOF
tarball         = cppad-$version.tgz
compiler        = $compiler
standard        = $standard
debug_which     = $debug_which
package_vector  = $package_vector
use_configure   = $use_configure
verbose_make    = $verbose_make
EOF
cat << EOF >> $top_srcdir/check_all.log
tarball         = cppad-$version.tgz
compiler        = $compiler
standard        = $standard
debug_which     = $debug_which
package_vector  = $package_vector
use_configure   = $use_configure
verbose_make    = $verbose_make
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
   exclude_package='--no_sacado'
fi
if [ "$(uname)" == 'Darwin' ]
then
   exclude_package+=' --no_colpack'
fi
#
# prefix
# absolute prefix where optional packages are installed
eval `$grep '^prefix=' bin/get_optional.sh`
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
#
# check_version
if echo $version | $grep '[0-9]\{4\}0000[.]' > /dev/null
then
   # special interactive case for stable versions.
   echo_eval bin/check_version.sh
else
   echo_log_eval bin/check_version.sh
fi
#
# bin/check_*.sh
# Run automated checks for the form bin/check_*.sh with a few exceptions.
list=$(
   ls bin/check_* | $sed \
   -e '/check_all.sh/d' \
   -e '/check_doxygen.sh/d' \
   -e '/check_install.sh/d' \
   -e '/check_copy.sh/d' \
   -e '/check_invisible/d'
)
#
echo_eval bin/check_invisible.sh
if [ "$skip_check_copy" == 'no' ]
then
   echo_eval bin/check_copy.sh
fi
for check in $list
do
   echo_log_eval $check
done
#
# run_xrst.sh
flags=''
if [ "$skip_external_links" == 'no' ]
then
   flags+=' --external_links'
fi
if [ "$suppress_spell_warnings" == 'yes' ]
then
   flags+=' --suppress_spell_warnings'
fi
bin/run_xrst.sh $flags
#
# build/cppad-$version.tgz
echo_log_eval bin/package.sh
#
# build/cppad-$version
echo_log_eval cd build
echo_log_eval tar -xzf cppad-$version.tgz
echo_log_eval cd cppad-$version
#
# build/cppad-$version/bin/get_optional.sh
$sed -i bin/get_optional.sh -e "s|^prefix=.*|prefix=$prefix|"
#
# builder
if [ "$use_configure" == 'yes' ]
then
   builder='make'
elif [ "$verbose_make" == 'yes' ]
then
   builder='make'
else
   builder='ninja'
fi
#
# verbose_flag
if [ "$verbose_make" == 'yes' ]
then
   verbose_flag='--verbose_make'
else
   verbose_flag=''
fi
#
# configure or cmake
if [ "$use_configure" == 'yes' ]
then
   echo_log_eval bin/run_configure.sh \
      $verbose_flag \
      $compiler \
      $standard \
      $package_vector
else
   echo_log_eval bin/run_cmake.sh \
      $verbose_flag \
      $compiler \
      $standard \
      $debug_which \
      $exclude_package \
      $package_vector
fi
echo_log_eval cd build
#
# n_job
if which nproc >& /dev/null
then
   n_job=$(nproc)
else
   n_job=$(sysctl -n hw.ncpu)
fi
#
# build: check
echo_log_eval $builder -j $n_job check
#
# speed/cppad/speed_cppad
for option in onetape colpack optimize atomic memory boolsparsity
do
   echo_eval speed/cppad/speed_cppad correct 432 $option
done
#
# speed/adolc/speed_adolc
echo_eval speed/adolc/speed_adolc correct         432 onetape
echo_eval speed/adolc/speed_adolc sparse_jacobian 432 onetape colpack
echo_eval speed/adolc/speed_adolc sparse_hessian  432 onetape colpack
#
# bin/test_multi_thread.sh
cd ..
bin/test_multi_thread.sh $builder
cd build
#
# print_for test
# redo this build in case it is commented out above
program='example/print_for/example_print_for'
echo_log_eval $builder -j $n_job example_print_for
echo_log_eval $program
$program | $sed -e '/^Test passes/,$d' > temp.1.$$
$program | $sed -e '1,/^Test passes/d' > temp.2.$$
if diff temp.1.$$ temp.2.$$
then
   rm temp.1.$$ temp.2.$$
   echo_log_eval echo "print_for: OK"
else
   echo_log_eval echo "print_for: Error"
   exit 1
fi
#
# bin/test_install.sh
echo_log_eval cd ..
if [ "$standard" == '' ]
then
   echo_log_eval bin/test_install.sh $builder --c++17
else
   echo_log_eval bin/test_install.sh $builder $standard
fi
#
#
echo "date >> check_all.log"
date  | $sed -e 's|^|date: |' >> $top_srcdir/check_all.log
# ----------------------------------------------------------------------------
echo "$0: OK" >> $top_srcdir/check_all.log
echo "$0: OK"
exit 0
