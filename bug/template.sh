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
cat << EOF
Description
EOF
cat << EOF > bug.$$
# include <iostream>
int main(void)
{	// C++ source code 
	using std::cout;

	cout << "1. svn copy template.sh <name>.sh\n";
	cout << "2. Edit <name>.sh replacing description and C++ source code\n"; 
	cout << "3. Run ./<name>.sh\n";
	cout << "where <name> is a name that describes the bug\n";
	
	return 0;
}
EOF
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
mv ../bug.$$ $name.cpp
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ./$name
then
	echo "OK"
else
	echo "Error"
fi
