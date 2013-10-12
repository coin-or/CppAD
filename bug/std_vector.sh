#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Using g++ 4.8.1 results in the following error message:
#
# std_vector.cpp:8:7: error: no match for ‘operator|=’ (operand types are 
# ‘std::vector<bool>::reference {aka std::_Bit_reference}’ and ‘bool’)
#  y[1] |= true;
#       ^
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
cat << EOF > $name.cpp
# include <vector>
int main(void)
{	int N = 1;
	std::vector<bool> y(N);
	for(int i = 0; i < N; i++ )
		y[i] = false;
	y[0]  = y[0] | true;
	y[1] |= true;
	return 0;
}
EOF
echo "g++ -g $name.cpp -o $name"
g++ -g $name.cpp -o $name
#
echo "./$name"
./$name
#
echo "rm $name $name.cpp"
rm $name $name.cpp
