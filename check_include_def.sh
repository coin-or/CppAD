# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
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
grep '^# *ifndef *CPPAD_[a-zA-Z_]*_INCLUDED$' \
	cppad/*.hpp cppad/local/*.hpp speed/*.hpp example/*.hpp \
	| sed -e '/error_handler.hpp:.*CPPAD_CPPAD_INCLUDED/d' \
	| sort -u > junk.1
 
sed -e 's|\([^.]*\)\.hpp:.*|\1|' -e 's|^.*/||' < junk.1 \
	| tr [a-zA-Z] [A-Za-z] > junk.2
sed -e 's|.*# *ifndef *CPPAD_\([a-zA-Z_]*\)_INCLUDED$|\1|' < junk.1 > junk.3
diff junk.2 junk.3
echo "-----------------------------------------------------"
echo "Nothing should be between the two dashed lines above"
