#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/check_all.sh' ]
then
    echo "bin/check_all.sh: must be executed from its parent directory"
    exit 1
fi
debug_all='no'
if [ "$1" != '' ]
then
    debug_all='yes'
    if [ "$1" != 'debug_all' ]
    then
        echo 'usage: bin/check_all.sh [debug_all]'
        exit 1
    fi
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
check_all_warn() {
cat << EOF > check_all.$$
# Lines that describe where error is
/^In file included from/d
/: note:/d
#
# Ipopt has sign conversion warnings
/\/coin\/.*-Wsign-conversion/d
#
# Adolc has multiple types of conversion warnings
/\/adolc\/.*-W[a-z-]*conversion/d
/\/adolc\/.*-Wshorten-64-to-32/d
#
# Lines describing the error begin with space
/^ /d
#
# Lines summarizing results
/^[0-9]* warnings generated/d
EOF
    sed $top_srcdir/check_all.err -f check_all.$$ > $top_srcdir/check_all.warn
    rm check_all.$$
}
# -----------------------------------------------------------------------------
echo_log_eval() {
    echo $*
    echo $* >> $top_srcdir/check_all.log
    if ! eval $* >> $top_srcdir/check_all.log 2> $top_srcdir/check_all.err
    then
        tail $top_srcdir/check_all.err
        echo 'Error: see check_all.err, check_all.log'
        exit 1
    fi
    check_all_warn
    count=`wc -l $top_srcdir/check_all.warn | sed -e 's|^\([0-9]*\) .*|\1|'`
    if [ "$count" != '0' ]
    then
        head "$top_srcdir/check_all.warn"
        echo 'Warning: see check_all.warn, check_all.log'
        exit 1
    fi
    rm $top_srcdir/check_all.warn $top_srcdir/check_all.err
}
echo_log() {
    echo $*
    echo $* >> $top_srcdir/check_all.log
}
random_01() {
    set +e
    eval random_01_$1="`expr $RANDOM % 2`"
    set -e
}
# -----------------------------------------------------------------------------
# start new check_all.log
echo "date > check_all.log"
date > check_all.log
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
version=`version.sh get`
tarball="cppad-$version.tgz"
# ---------------------------------------------------------------------------
random_01 compiler
if [ "$random_01_compiler" == '0' ]
then
    compiler='default'
else
    compiler='--clang'
fi
random_01 standard
if [ "$random_01_standard" == '0' ]
then
    standard='--c++98 --no_adolc --no_sacado'
else
    standard='--c++11'
fi
#
if [ "$debug_all" == 'yes' ]
then
    package_vector='--cppad_vector'
    debug_which='--debug_all'
else
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
        package_vector='--eigen_vector'
    fi
fi
cat << EOF
tarball         = $tarball
compiler        = $compiler
standard        = $standard
debug_which     = $debug_which
package_vector  = $package_vector
EOF
cat << EOF >> $top_srcdir/check_all.log
tarball         = $tarball
compiler        = $compiler
standard        = $standard
debug_which     = $debug_which
package_vector  = $package_vector
EOF
if [ "$compiler" == 'default' ]
then
    compiler=''
fi
if [ "$standard" == '--c++11' ]
then
    standard='' # default for run_cmake.sh
fi
# ---------------------------------------------------------------------------
# Run automated checks for the form bin/check_*.sh with a few exceptions.
list=`ls bin/check_* | sed \
    -e '/check_all.sh/d' \
    -e '/check_jenkins.sh/d' \
    -e '/check_doxygen.sh/d'`
# ~/devel/check_copyright.sh not included in batch_edit branch
for check in $list
do
    echo_log_eval $check
done
# ---------------------------------------------------------------------------
# Create package to run test in
echo_log_eval bin/package.sh
# -----------------------------------------------------------------------------
# choose which tarball to use for testing
echo_log_eval cd build
echo_log_eval rm -rf cppad-$version
echo_log_eval tar -xzf $tarball
echo_log_eval cd cppad-$version
# -----------------------------------------------------------------------------
echo_log_eval bin/run_cmake.sh $compiler $standard $debug_which $package_vector
echo_log_eval cd build
# -----------------------------------------------------------------------------
# can comment out this make check to if only running tests below it
n_job=`nproc`
echo_log_eval make -j $n_job check
# -----------------------------------------------------------------------------
skip=''
for package in adolc eigen ipopt fadbad sacado
do
    dir=$HOME/prefix/$package
    if [ ! -d "$dir" ]
    then
        skip="$skip $package"
    elif echo $standard | grep "no_$package" > /dev/null
    then
        skip="$skip $package"
    fi
done
# ----------------------------------------------------------------------------
# extra speed tests not run with option specified
#
# make speed_cppad incase make check above is commented out
echo_log_eval make -j $n_job speed_cppad
for option in onetape colpack optimize atomic memory boolsparsity
do
    #
    echo_eval speed/cppad/speed_cppad correct 432 $option
done
if ! echo "$skip" | grep 'adolc' > /dev/null
then
    # make speed_adolc incase make check above is commented out
    echo_log_eval make -j $n_job speed_adolc
    #
    echo_eval speed/adolc/speed_adolc correct         432 onetape
    echo_eval speed/adolc/speed_adolc sparse_jacobian 432 onetape colpack
    echo_eval speed/adolc/speed_adolc sparse_hessian  432 onetape colpack
fi
#
# ----------------------------------------------------------------------------
# extra multi_thread tests
program_list=''
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
        # make program incase make check above is commented out
        echo_log_eval make -j $n_job example_multi_thread_${threading}
        #
        # all programs check the fast cases
        echo_log_eval $program a11c
        echo_log_eval $program simple_ad
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
# make program incase make check above is commented out
echo_log_eval make -j $n_job example_print_for
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
echo_log_eval make install
#
echo "date >> check_all.log"
date >> $top_srcdir/check_all.log
if [ "$skip" != '' ]
then
    echo_log_eval echo "check_all.sh: skip = $skip"
    exit 1
fi
# ----------------------------------------------------------------------------
echo "$0: OK" >> $top_srcdir/check_all.log
echo "$0: OK"
exit 0
