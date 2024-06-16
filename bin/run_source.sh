#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
set -e -u
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
echo "$#"
if [ "$#" != 1 ] || [ "$0" != "bin/run_source.sh" ]
then
   echo "usage: bin/run_source.sh source_file"
   exit 1
fi
#
# source_file
source_file="$1"
if [ ! -f $source_file ]
then
   echo "run_source.sh: $source_file is not a file"
   exit 1
fi
if ! echo $source_file | grep '[.]cpp$' > /dev/null
then
   echo "run_source.sh: $source_file does not end with .cpp"
   exit 1
fi
# -----------------------------------------------------------------------------
# build/run_source
if [ ! -e build/run_source ]
then
   mkdir -p build/run_source
fi
cd build/run_source
cmake -S ../.. -B .
make cppad_lib
cd ../..
#
# LD_LIBRARY_PATH
cppad_lib_dir=$(
   find build/run_source -name 'libcppad_lib.*' | \
   head -1 | \
   sed -e 's|/[^/]*$||' \
)
export LD_LIBRARY_PATH="$cppad_lib_dir"
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
#
# executable
echo_eval g++ $source_file \
   -g \
   -O0 \
   -I include \
   -L $LD_LIBRARY_PATH \
   -lcppad_lib \
   -o build/run_source/executable
echo_eval build/run_source/executable
#
# ------------------------------------------------------------------------------
echo "bug/run_source.sh: OK"
exit 0
