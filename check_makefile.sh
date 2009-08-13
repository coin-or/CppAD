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
echo "Checking include files listed in makefile.am"
echo "-------------------------------------------------------" 
ls cppad/*.h cppad/*.hpp cppad/local/*.hpp cppad/speed/*.hpp  | \
sort > check_makefile1.$$
#
sed < makefile.am -n -e \
	'/^nobase_myinclude_HEADERS *=/,/^# End nobase_myinclude_HEADERS/p' |
sed -e '/nobase_myinclude_HEADERS/d' -e 's/^\t//' -e 's/ *\\$//' |
sort > check_makefile2.$$
#
if diff check_makefile1.$$ check_makefile2.$$
then
	ok="yes"
else
	ok="no"
fi
rm check_makefile1.$$
rm check_makefile2.$$
echo "-------------------------------------------------------" 
if [ "$ok" = "no" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Checking speed tests set for optimization and not debugging."
echo "-------------------------------------------------------" 
ok="yes"
for dir in adolc cppad double fadbad sacado
do
	file="speed/$dir/makefile.am"
	if ! grep '^[^#]*-DNDEBUG' $file > /dev/null
	then
		echo "Optimization flag is not defined in $file"
		ok="no"
	fi
	if ! grep '^#.*-g' $file > /dev/null
	then
		echo "Debug flag is not commented out in $file"
		ok="no"
	fi
done
echo "-------------------------------------------------------" 
if [ "$ok" = "yes" ]
then
	echo "Ok: nothing is between the two dashed lines above"
        exit 0
else
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
fi
