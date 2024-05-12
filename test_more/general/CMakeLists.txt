# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the test_more/general tests
#
# adolc_sources
IF( cppad_has_adolc )
   SET(adolc_sources base_adolc.cpp)
ELSE( cppad_has_adolc )
   SET(adolc_sources "")
ENDIF( cppad_has_adolc )
#
# eigen_sources
IF( cppad_has_eigen )
   SET(eigen_sources cppad_eigen.cpp eigen_mat_inv.cpp)
ELSE( cppad_has_eigen )
   SET(eigen_sources "")
ENDIF( cppad_has_eigen )
#
# ipopt_sourced
IF( cppad_has_ipopt )
   SET(ipopt_sources ipopt_solve.cpp)
ELSE( cppad_has_ipopt )
   SET(ipopt_sources "")
ENDIF( cppad_has_ipopt )
#
# BEGIN_SORT_THIS_LINE_PLUS_5
SET(source_list
   ${adolc_sources}
   ${eigen_sources}
   ${ipopt_sources}
   abs_normal.cpp
   acos.cpp
   acosh.cpp
   add.cpp
   add_eq.cpp
   add_zero.cpp
   adfun.cpp
   asin.cpp
   asinh.cpp
   assign.cpp
   atan.cpp
   atan2.cpp
   atanh.cpp
   atomic_four.cpp
   atomic_three.cpp
   azmul.cpp
   base2ad.cpp
   base_alloc.cpp
   base_complex.cpp
   bool_sparsity.cpp
   check_simple_vector.cpp
   chkpoint_one.cpp
   chkpoint_two.cpp
   compare.cpp
   compare_change.cpp
   cond_exp.cpp
   cond_exp_ad.cpp
   cond_exp_rev.cpp
   copy.cpp
   cos.cpp
   cosh.cpp
   cpp_graph.cpp
   cppad_vector.cpp
   dbl_epsilon.cpp
   dependency.cpp
   div.cpp
   div_eq.cpp
   div_zero_one.cpp
   erf.cpp
   exp.cpp
   expm1.cpp
   extern_value.cpp
   fabs.cpp
   for_hess.cpp
   for_sparse_hes.cpp
   for_sparse_jac.cpp
   forward.cpp
   forward_dir.cpp
   forward_order.cpp
   from_base.cpp
   fun_check.cpp
   general.cpp
   hes_sparsity.cpp
   jacobian.cpp
   json_graph.cpp
   local/is_pod.cpp
   local/json_lexer.cpp
   local/json_parser.cpp
   local/temp_file.cpp
   local/vector_set.cpp
   log.cpp
   log10.cpp
   log1p.cpp
   mul.cpp
   mul_cond_rev.cpp
   mul_cskip.cpp
   mul_eq.cpp
   mul_level.cpp
   mul_zdouble.cpp
   mul_zero_one.cpp
   near_equal_ext.cpp
   neg.cpp
   new_dynamic.cpp
   num_limits.cpp
   ode_err_control.cpp
   optimize.cpp
   parameter.cpp
   poly.cpp
   pow.cpp
   pow_int.cpp
   print_for.cpp
   rev_sparse_jac.cpp
   rev_two.cpp
   reverse.cpp
   romberg_one.cpp
   rosen_34.cpp
   runge_45.cpp
   simple_vector.cpp
   sin.cpp
   sin_cos.cpp
   sinh.cpp
   sparse_hessian.cpp
   sparse_jac_work.cpp
   sparse_jacobian.cpp
   sparse_sub_hes.cpp
   sparse_vec_ad.cpp
   sqrt.cpp
   std_math.cpp
   sub.cpp
   sub_eq.cpp
   sub_zero.cpp
   subgraph_1.cpp
   subgraph_2.cpp
   subgraph_hes2jac.cpp
   tan.cpp
   to_csrc.cpp
   to_string.cpp
   value.cpp
   vec_ad.cpp
   vec_ad_par.cpp
   vec_unary.cpp
)
# END_SORT_THIS_LINE_MINUS_2
set_compile_flags( test_more_general "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(test_more_general EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_general
   ${cppad_lib}
   ${adolc_LINK_LIBRARIES}
   ${ipopt_LINK_LIBRARIES}
   ${colpack_libs}
)
#
# check_test_more_general
add_check_executable(check_test_more general)
