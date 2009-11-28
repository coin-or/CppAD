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
# CppAD uses preprocessor '# if 0' comment blocks for temporary changes
# that will to be removed before testing for check in.
echo "Checking for '# if 0' comments blocks in source code"
echo "-------------------------------------------------------" 
ok="yes"
for dir in \
	example \
	test_more \
	cppad \
	cppad/speed \
	cppad/local \
	speed \
	speed/adolc \
	speed/cppad \
	speed/example \
	speed/profile \
	speed/sacado
do
	list=`ls $dir/*.cpp $dir/*.hpp 2> /dev/null`
	for file in $list
	do
		if grep '^# *if *0 *$' $file > /dev/null
		then
			echo "$file has an '# if 0' comment block"
			ok="no"
		fi
	done
done
echo "-------------------------------------------------------" 
if [ "$ok" = "no" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
