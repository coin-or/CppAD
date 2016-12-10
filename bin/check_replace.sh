#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/check_replace.sh" ]
then
	echo "bin/check_replace.sh: must be executed from its parent directory"
	exit 1
fi
check_replace() {
	define_file="cppad/local/$1"
	replace_file="cppad/local/$2"
	new_file="cppad/local/$2.$$"
	bin/replace_html.py $define_file $replace_file $new_file
	if ! diff $replace_file $new_file > /dev/null
	then
		cat << EOF
check_replace.sh: Error:
The replacement text in $replace_file
does not match its definition in $define_file.
Execute the following command to fix this:
	mv $new_file $replace_file
EOF
		exit 1
	fi
	rm $new_file
}
# -----------------------------------------------------------------------------
# files with definitions and replacemnet in same file
check_replace cond_op.hpp                        cond_op.hpp
check_replace load_op.hpp                        load_op.hpp
check_replace store_op.hpp                       store_op.hpp
check_replace forward0sweep.hpp                  forward1sweep.hpp
# -----------------------------------------------------------------------------
echo "replace_html.sh: OK"
exit 0
