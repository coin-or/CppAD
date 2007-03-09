# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
echo "Differences between include file names and directives"
echo "-----------------------------------------------------"
grep '^# *ifndef *CPPAD_[0-9a-zA-Z_]*_INCLUDED$' \
cppad/*.hpp cppad/local/*.hpp cppad/speed/*.hpp example/*.hpp openmp/*.hpp \
	| sed -e 's|.*# *ifndef *CPPAD_\([0-9a-zA-Z_]*\)_INCLUDED$|\1|' \
	| tr [a-zA-Z] [A-Za-z] \
	| sort -u \
	> junk.1
 
ls cppad/*.hpp cppad/local/*.hpp cppad/speed/*.hpp example/*.hpp openmp/*.hpp \
	| sed -e 's|.*/||' -e 's|\.hpp||' \
	| sort -u \
	> junk.2
diff junk.1 junk.2
echo "-----------------------------------------------------"
echo "Nothing should be between the two dashed lines above"
