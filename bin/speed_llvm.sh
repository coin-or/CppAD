#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
if [  "$0" != 'bin/speed_llvm.sh' ]
then
    echo "bin/speed_llvm.sh: must be executed from its parent directory"
    exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
#
# local copy of cppadcg
bin/get_cppadcg.sh
#
# run_cmake.sh
bin/run_cmake.sh \
    --c++14 \
    --no_adolc \
    --no_colpack \
    --no_eigen \
    --no_ipopt \
    --no_fadbad \
    --no_sacado \
    --no_documentation \
    --std_vector \
    --debug_none
#
# make check_speed_cppadcg check_speed_llvm
echo_eval cd build
echo_eval make check_speed_cppadcg check_speed_llvm
#
echo 'compare cppadcg and llvm with re-taping'
speed/cppadcg/speed_cppadcg det_minor 1234
speed/llvm/speed_llvm det_minor 1234
#
echo 'compare cppadcg and llvm with one tape'
speed/cppadcg/speed_cppadcg det_minor 1234 onetape
speed/llvm/speed_llvm det_minor 1234 onetape
#
echo 'speed_llvm.sh: OK'
exit 0
