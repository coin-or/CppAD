# ! /bin/bash 
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# classes that have been completely documented
class_list="
	player
	recorder
	sparse_pack
	sparse_set
"
#
# member functions names that begin with the following have been documented
# (but the corresponding class has not yet been completely documented).
member_list="
	SparseJacobian
"
# files that have been completely documented
file_list="
	abs_op.hpp
	add_op.hpp
	acos_op.hpp
	asin_op.hpp
	cond_op.hpp
	configure.hpp
	cos_op.hpp
	cosh_op.hpp
	cppad_assert.hpp
	define.hpp
	discrete_op.hpp
	div_op.hpp
	forward0sweep.hpp
	forward_sweep.hpp
	for_jac_sweep.hpp
	for_sparse_jac.hpp
	fun_construct.hpp
	hash_code.hpp
	load_op.hpp
	log_op.hpp
	mul_op.hpp
	op_code.hpp
	optimize.hpp
	player.hpp
	print_op.hpp
	pow_op.hpp
	prototype_op.hpp
	recorder.hpp
	reverse_sweep.hpp
	rev_hes_sweep.hpp
	rev_jac_sweep.hpp
	rev_sparse_hes.hpp
	rev_sparse_jac.hpp
	sin_op.hpp
	sinh_op.hpp
	sparse_binary_op.hpp
	sparse_unary_op.hpp
	sqrt_op.hpp
	store_op.hpp
	sub_op.hpp
	sparse_jacobian.hpp
	sparse_hessian.hpp
	sparse_pack.hpp
	sparse_set.hpp
"
# --------------------------------------------------------------------------
for class in $class_list
do
	if grep -i "warning:.*$class::" doxygen.log
	then
		echo "Unexpected doxygen error or warning for $file."
		exit 1
	fi
done
# --------------------------------------------------------------------------
for member in $member_list
do
	if grep -i "warning:.*member.*$member" doxygen.log
	then
		echo "Unexpected doxygen error or warning for $file."
		exit 1
	fi
done
# --------------------------------------------------------------------------
for file in $file_list
do
	if [ ! -e "cppad/local/$file" ] && [ ! -e "cppad/$file" ]
	then
		echo "check_doxygen.sh: cannot find file $file"
		exit 1
	fi
	if grep -i "$file.*warning" doxygen.log
	then
		echo "Unexpected doxygen error or warning for $file."
		exit 1
	fi
done
