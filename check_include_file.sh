# ! /bin/bash 
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
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
	ipopt_cppad/*/*.hpp \
	ipopt_cppad/*/*.cpp \
	cppad/*.hpp \
	cppad/local/*.hpp \
	cppad/speed/*.hpp \
	example/*.cpp \
	example/*.hpp \
	speed/*.cpp \
	speed/adolc/*.cpp \
	speed/cppad/*.cpp \
	speed/fadbad/*.cpp \
	speed/example/*.cpp \
	test_more/*.cpp > junk.1
#
cat junk.1 | sed -e 's%[^<]*<%%'  -e 's%>.*$%%' | sort -u > junk.2
# The file cppad/local/prototype_op should never be included. 
# All other files should.
ls	cppad/config.h \
	cppad/*.hpp \
	cppad/local/*.hpp \
	cppad/speed/*.hpp \
	| sed -e '/cppad\/local\/prototype_op.hpp/d' | sort > junk.3 
if diff junk.2 junk.3
then
	different="no"
else
	different="yes"
fi
#
echo "-------------------------------------------------------------------"
if [ $different = "yes" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
