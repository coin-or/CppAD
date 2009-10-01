#! /bin/bash
#
if [ "$2" == "" ]
then
	echo "usage: not_available.sh package_name speed_test_name"
	exit 1
fi
package_name="$1"
speed_test_name="$2"
file="speed/cppad/$speed_test_name.cpp"
if [ "$package_name" == cppad ]
then
	echo "not_available.sh: cannot automatically create cppad version."
	exit 1
fi
if [ ! -e $file ]
then
	echo "not_available.sh: must first create $file"
	exit 1
fi
function_prototype=`sed -n -e '/^bool link/,/{/p' $file`
file="speed/$package_name/$speed_test_name.cpp"
if [ -e $file ]
then
	echo "not_available.sh: $file already exists"
	exit 1
fi
echo "creating $file"
cat << EOF > $file
/* \$Id$speed_test_name.cpp */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cstring>
# include <cppad/vector.hpp>

/*
\$begin ${package_name}_${speed_test_name}.cpp\$\$
\$spell
        bool
        CppAD
        ${package_name}
	${speed_test_name}
\$\$

\$section ${package_name} Speed: ${speed_test_name}\$\$

\$codep */
// The ${package_name} version of this test is not yet available
${function_prototype}
	return false;
}
/* \$\$
\$end
*/
EOF
