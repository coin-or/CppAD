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
list="
	abs_op.hpp
	configure.hpp
	define.hpp
	op_code.hpp
	player.hpp
	unary_op.hpp
"
for name in $list
do
	if grep -i "$name.*warning" doxygen.log
	then
		echo "Unexpected doxygen error or warning for $name."
		exit 1
	fi
done
