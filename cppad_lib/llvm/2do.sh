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
op_list='
    abs_graph_op
    acos_graph_op
    acosh_graph_op
    add_graph_op
    asin_graph_op
    asinh_graph_op
    atan_graph_op
    atanh_graph_op
    atom_graph_op
    azmul_graph_op
    cexp_eq_graph_op
    cexp_le_graph_op
    cexp_lt_graph_op
    comp_eq_graph_op
    comp_le_graph_op
    comp_lt_graph_op
    comp_ne_graph_op
    cos_graph_op
    cosh_graph_op
    discrete_graph_op
    div_graph_op
    erf_graph_op
    erfc_graph_op
    exp_graph_op
    expm1_graph_op
    log1p_graph_op
    log_graph_op
    neg_graph_op
    mul_graph_op
    pow_graph_op
    print_graph_op
    sign_graph_op
    sin_graph_op
    sinh_graph_op
    sqrt_graph_op
    sub_graph_op
    sum_graph_op
    tan_graph_op
    tanh_graph_op
'
for op in $op_list
do
    file_list=''
    for file in ir_from_graph.cpp ir_to_graph.cpp
    do
        if ! grep "graph::$op[^a-z]" $file > /dev/null
        then
            if [ "$file_list" == '' ]
            then
                file_list="$file"
            else
                file_list="$file_list + $file"
            fi
        fi
    done
    if [ "$file_list" != '' ]
    then
        echo "$file_list: does not contain graph::$op"
    fi
done
