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
#
echo "Checking difference between C++ include directives and file names."
echo "-------------------------------------------------------------------"
grep '^# *include *<cppad/' \
	cppad/*.hpp \
	cppad/local/*.hpp \
	cppad/speed/*.hpp \
	example/*.cpp \
	example/*.hpp \
	speed/*.cpp \
	speed/adolc/*.cpp \
	speed/cppad/*.cpp \
	speed/fadbad/*.cpp \
	test_more/*.cpp \
	speed_cppad/*.cpp  > junk.1
#
cat junk.1 | sed -e 's%[^<]*<%%'  -e 's%>.*$%%' | sort -u > junk.2
ls	cppad/config.h \
	cppad/*.hpp \
	cppad/local/*.hpp \
	cppad/speed/*.hpp \
	| sort -u > junk.3 
diff junk.2 junk.3
#
echo "-------------------------------------------------------------------"
echo "Nothing should be between the two dashed lines above"
