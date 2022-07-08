#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
if [ $0 != "bin/appveyor.sh" ]
then
    echo 'bin/appveyor.sh: must be executed from its parent directory'
    exit 1
fi
if [ "$1" != 'make' ] && [ "$1" != 'test_one' ]
then
    echo 'usage: bin/appveyor.sh (make|test_one) target1 target2 ...'
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
if [ -e build ]
then
    echo_eval rm -r build
fi
echo_eval mkdir build
# -----------------------------------------------------------------------------
#
# bin/temp.sh
# version of get_eigen.sh that specifies GNU compiler
cat << EOF > temp.sed
s|^echo_eval cmake|& -D CMAKE_C_COMPILER=gcc|
s|^echo_eval cmake|& -D CMAKE_CXX_COMPILER=g++|
s|bin/get_\$package.sh|bin/temp.sh|
EOF
sed -f temp.sed bin/get_eigen.sh > bin/temp.sh
chmod +x bin/temp.sh
#
# install eigen in build/prefix
bin/temp.sh
#
# set PKG_CONFIG_PATH so CppAD can find eigen3.pc
export PKG_CONFIG_PATH="$(pwd)/build/prefix/share/pkgconfig"
# -----------------------------------------------------------------------------
echo_eval cd build
echo_eval cmake \
    -G '"Unix Makefiles"' \
    -D cppad_prefix=$(pwd)/prefix \
    -D CMAKE_C_COMPILER=gcc \
    -D CMAKE_CXX_COMPILER=g++ \
    -D include_eigen=true \
    ..
# -----------------------------------------------------------------------------
# Microsoft DLLs must be in current directory or execution path
export PATH="$PATH:$(pwd)/cppad_lib"
# -----------------------------------------------------------------------------
# build target1, target2, ...
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
echo 'bin/appveyor.sh: OK'
exit 0
