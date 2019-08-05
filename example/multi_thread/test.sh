#! /bin/sh -e
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
# script used by */makefile.am to run a default case for all the the tests
# --------------------------------------------------------------------------
# setup
next_program() {
    i_program=`expr $i_program + 1`
    if [ $i_program -ge $n_program ]
    then
        i_program='0'
    fi
    case $i_program in
        0)
        program=`echo "$program_list" | sed -e 's| *\([^ ]*\).*|\1|'`
        ;;

        1)
        program=`echo "$program_list" | sed -e 's| *[^ ]* *\([^ ]*\).*|\1|'`
        ;;

        2)
        program=`echo "$program_list" | sed -e 's| *[^ ]* [^ ]* *||'`
        ;;
    esac
}
n_program='0'
program_list=''
for program in openmp_test pthread_test bthread_test
do
    if [ -e "$program" ]
    then
        program_list="$program $program_list"
        n_program=`expr $n_program + 1`
    fi
done
echo "program_list = $program_list"
echo "n_program = $n_program"
if [ "$n_program" = '0' ]
then
    echo "example/multi_thread/test.sh: nothing to test"
    exit 0
fi
i_program='0'
next_program
# --------------------------------------------------------------------------
# test_time=1 max_thread=4, mega_sum=1
./$program harmonic 1 4 1
next_program
echo
# test_time=1 max_thread=4, num_solve=100
./$program multi_atomic_two 1 4 100
next_program
echo
# test_time= 2 max_thread=4, num_zero=20, num_sub=30, num_sum=500, use_ad=true
./$program multi_newton 2 4 20 30 500 true
next_program
echo
# fast cases, do all programs
for program in openmp_test pthread_test bthread_test
do
    if [ -e "$program" ]
    then
        ./$program a11c
        echo
        ./$program simple_ad
        echo
        ./$program team_example
        echo
    fi
done
