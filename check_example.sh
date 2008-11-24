# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
echo "Checking that all examples are in omh/example_list.omh"
echo "-------------------------------------------------------" 
file_list="example/*.cpp example/*.hpp ipopt_cppad/*.cpp ipopt_cppad/*.hpp"
sed < omh/example_list.omh > check_example.$$ \
	-n -e '/\$begin ListAllExamples\$\$/,/\$end/p' 
ok="yes"
for file in $file_list
do
	name=`grep '$begin' $file | sed -e 's/.*$begin *//' -e 's/ *$$.*//'`
	if [ "$name" != "" ]
	then
		if ! grep "$name" check_example.$$ > /dev/null
		then
			echo "$name is missing from omh/example_list.omh"
			ok="no"
		fi
	fi
done
rm check_example.$$
echo "-------------------------------------------------------" 
if [ "$ok" = "yes" ]
then
	echo "Ok: nothing is between the two dashed lines above"
        exit 0
else
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
fi
