#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ $0 != "bin/travis.sh" ]
then
   echo 'bin/travis.sh: must be executed from its parent directory'
   exit 1
fi
if [ "$1" != 'make' ] && [ "$1" != 'test_one' ]
then
   echo 'usage: bin/travis.sh (make|test_one) target1 target2 ...'
   echo 'target: if make specified, is one of the available make commands'
   echo          if test_one, specified, is the path to a test file.
   exit 1
fi
cmd="$1"
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ -e 'build' ]
then
   echo_eval rm -r build
fi
echo_eval mkdir build
echo_eval cd build
echo_eval cmake \
   -D cppad_prefix=$(pwd)/prefix \
   -D cppad_cxx_flags='-std=c++11' \
   ..
# -----------------------------------------------------------------------------
if [ "$cmd" == 'make' ]
then
   shift
   while [ "$1" != '' ]
   do
      echo_eval make "$1"
      shift
   done
else
   echo_eval cd ..
   shift
   while [ "$1" != '' ]
   do
      echo_eval bin/test_one.sh "$1"
      shift
   done
fi
# -----------------------------------------------------------------------------
echo 'bin/travis.sh: OK'
exit 0
