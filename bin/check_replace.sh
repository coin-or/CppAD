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
if [ $0 != "bin/check_replace.sh" ]
then
    echo "bin/check_replace.sh: must be executed from its parent directory"
    exit 1
fi
check_replace() {
    define_file="include/cppad/local/$1"
    replace_file="include/cppad/local/$2"
    new_file="include/cppad/local/$2.$$"
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
check_replace store_op.hpp                       store_op.hpp
check_replace sweep/forward0.hpp                 sweep/forward1.hpp
# -----------------------------------------------------------------------------
echo "replace_html.sh: OK"
exit 0
